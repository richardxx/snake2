/*
  This file contains the implementation of the sound core functions .
  I was just started to study sound programming , so the code isn't so robust .
  Now I only support raw wav format sound , and better to make the size less than 10kb , 
  because I copy them to the memory .

  Copyright 2006 by Richard Leon
*/

#include <unistd.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <cstring>
#include "sound.h"
#include "const_dec.h"

using
  std::strcpy;
using
  std::strcat;

int
  snd_player::snd_volume = (70 << 8) | 70;	// proper on my machine
snd_param
  snd_player::org_snd_param;

extern const char *
  snd_dir;			// predefined path
extern const char *
  prog_dir;

// ===================General Purpose Procedures==========================

void
snd_player::set_volume (int l, int r) 
{
  int vol_handler;

  snd_volume = (l << 8) | (r & 0xFF);
  if ((vol_handler = open ("/dev/mixer", O_WRONLY)) == -1)
     return;

  if (ioctl (vol_handler, MIXER_WRITE (SOUND_MIXER_VOLUME), &snd_volume) ==
      -1)
    
    {
      close (vol_handler);
      return;
    }

  close (vol_handler);
  return;
}

void
snd_player::tune_sys_volume ()
{
  int vol;
  int vol_handler;

  if ((vol_handler = open ("/dev/mixer", O_RDWR)) == -1)
     return;
  if (ioctl (vol_handler, MIXER_READ (SOUND_MIXER_VOLUME), &vol) == -1)
    
    {
      close (vol_handler);
      return;
    }
  else
    org_snd_param.snd_vol = vol;

  if (ioctl (vol_handler, MIXER_WRITE (SOUND_MIXER_VOLUME),
	     &snd_volume) == -1)
    
    {
      // No any side effects
    }

  close (vol_handler);
  return;
}

void
snd_player::restore_sys_volume ()
{
  int vol_handler;

  if (ioctl (vol_handler, MIXER_WRITE (SOUND_MIXER_VOLUME),
	     &org_snd_param.snd_vol) == -1)
    {
      // Try to tune back yourself , V_V
    }

  close (vol_handler);
}

/*
  The process of sound card configuration is no right or wrong , it just to tune the system
  to fit for the application . I don't care about the error here , but it won't occur in
  most modern machine really .
*/
void
snd_player::config_sound_card ()
{
  int format;
  int audio_fd;

  if ((audio_fd = open ("/dev/dsp", O_WRONLY)) == -1)
    return;

  org_snd_param.samp_format = -1;	// I only use him in this version

  if (ioctl (audio_fd, AFMT_QUERY, &format) == -1);
  // I can't get the original sampling format , so I don't 
  // try to change them
  else
    {
      org_snd_param.samp_format = format;
      if (ioctl (audio_fd, SNDCTL_DSP_GETFMTS, &format) == -1)
	format = AFMT_U8;
      else
	{
	  if (format & AFMT_S16_LE)
	    format = AFMT_S16_LE;
	  else
	    format = AFMT_U8;
	}
      // I don't care if your card support the specified format
      if (ioctl (audio_fd, SNDCTL_DSP_SETFMT, &format) == -1)
	org_snd_param.samp_format = -1;
    }

  format = 2;			/*  1=mono,  2=stereo  */
  if (ioctl (audio_fd, SNDCTL_DSP_CHANNELS, &format) == -1);
  
    /* 
       I'm not planed to change the sampling speed yet
       format=22050;
       if (ioctl(audio_fd,SNDCTL_DSP_SPEED,&format)==-1) 
       {
       perror("SNDCTL_DSP_SPEED");
       exit(Error  code);
       }
     */ 
    tune_sys_volume ();		// make it portable

  close (audio_fd);
  return;
}

void
snd_player::restore_sound_card ()
{
  int audio_fd;

  if ((audio_fd = open ("/dev/dsp", O_WRONLY)) == -1)
    return;

  if (org_snd_param.samp_format != -1)
    {
      if (ioctl (audio_fd, SNDCTL_DSP_SETFMT, &org_snd_param.samp_format) ==
	  -1);
      // Do it yourself
    }

  restore_sys_volume ();

  close (audio_fd);
}

void
snd_player::vol_up ()
{
  int l = (snd_volume >> 8) + 10;
  int r = (snd_volume & 0xff) + 10;

  if (l > 100 || r > 100);
  else
    set_volume (l, r);
}

void
snd_player::vol_down ()
{
  int l = (snd_volume >> 8) - 10;
  int r = (snd_volume & 0xff) - 10;

  if (l < 0 || r < 0);
  else
    set_volume (l, r);
}

//==============================Class independent procedures====================

/*
  The usage of this snd_player class :
	1.Every objects who defined its own sound resource should get an instance of 
	  snd_player class , then every time you should just play the sound ;
	2.When the program terminated , restore the previous status .
*/

snd_player::snd_player ()
{
  snd_avl = false;
}

snd_player::~snd_player ()
{
  unset_sound ();
}

/*
  I played a trick on playing sound . I think mmap the file into the user space , then
  copy the data to sound card is the fastest way , so the code is a bit difficult here .
  To completely understand the code , please learn the Linux system calls .
*/
bool
snd_player::set_sound (const char *snd_res)
{
  strcpy (snd_path, snd_dir);
  strcat (snd_path, snd_res);

  if (snd_avl == true)		// if there already a sound file opened
    unset_sound ();

  if ((snd_file_handler = open (snd_path, O_RDONLY)) == -1)
    return false;

  /* 
     Not in every machine , it has enough CPU resource to handle the sound 
     in ranged time , so I decide to take only part of them

     file_len=lseek(snd_file_handler,0,SEEK_END);
     lseek(snd_file_handler,0,SEEK_SET);  // move the cursor back to beginning
   */

  file_len = 2048;		// the file limits is 3KB
  if ((snd_buffer = (char *) mmap (0, file_len, PROT_READ,
				   MAP_PRIVATE, snd_file_handler,
				   0)) == (char *) MAP_FAILED)
    {
      close (snd_file_handler);
      snd_avl = false;
      return false;
    }

  snd_avl = true;
  return true;
}

void
snd_player::unset_sound ()
{
  if (snd_avl == true)
    {
      munmap (snd_buffer, file_len);
      close (snd_file_handler);
      snd_avl = false;
    }
}

/*
  After the prepration , the rest work is very easy to handle .
*/
void
snd_player::play ()
{
  int dev_handler;

  if ((dev_handler = open ("/dev/dsp", O_WRONLY)) == -1)
    return;

  if (snd_avl == true)
    write (dev_handler, snd_buffer, file_len);

  close (dev_handler);
}
