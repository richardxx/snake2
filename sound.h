/*
  To add a audio module is not the task of this plan , but to satisfy the teacher I have
  to do so . I haven't refer this as a homework of mine , that's the reason for me to do
  the improvement .
  It's very simple of this version , but I hope it useful .

  Copyright 2006 by Richard Leon
*/

#ifndef SOUND_H
#define SOUND_H

struct snd_param
{
  // set to -1 when the correspond value is not changed

  int snd_vol;			// Sound volume
  int samp_format;		// Sample format
  int chan_num;			// Channel number
  int samp_rate;		// Sampling rate
};

class snd_player
{
public:
  static void config_sound_card ();
  static void restore_sound_card ();
  static void vol_up ();	// add 10 to each channel
  static void vol_down ();	// substract 10 to each channel

private:
  static void tune_sys_volume ();
  static void restore_sys_volume ();
  static void set_volume (int, int);

public:
    snd_player ();
   ~snd_player ();

public:
  void play ();
  bool set_sound (const char *);

private:
  void unset_sound ();

private:
    bool snd_avl;
  char snd_path[256];
  unsigned int file_len;
  int snd_file_handler;
  char *snd_buffer;

private:
  static snd_param org_snd_param;
  static int snd_volume;
};

#endif
