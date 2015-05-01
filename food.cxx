/*
  This filed is created during the third revision , in the past they are stay together with
  the system and snake manipulation functions , they were combined called io functions . This
  time I put them in a separate file , it makes us easy to understand the code .

  Copyright 2006 by Richard Leon
*/

#include <ncurses.h>
#include <cstdlib>
#include "common_ds.h"
#include "food.h"
#include "sound.h"
#include "const_dec.h"

using
  std::rand;

void inline
food::erase_food ()
{
  mvaddch (girlfriend.y, girlfriend.x, ' ');
  return;
}

food::food ()
{
  sound_appear = new snd_player ();
}

food::~food ()
{
  delete sound_appear;
}

//=======================================Fruit====================================

fruit::fruit ()
{
  has_sound = sound_appear->set_sound ("fruit.wav");
}

fruit::~fruit ()
{
  // nothing to do
}

void
fruit::put_food ()
{
  int rx, ry;

  general::use_color (3);
  while (1)
    {
      rx = rand () & (VIEW_COLS - 1);
      ry = rand () & (VIEW_LINES - 1);
      if (ry <= VIEW_TOP)
	continue;

      girlfriend.x = rx;
      girlfriend.y = ry;
      if (general::collide (girlfriend) != NOTHING)
	continue;
      break;
    }

  mvaddch (ry, rx, fruit_graphic);
  if (has_sound == true)
    sound_appear->play ();

  refresh ();
}

int inline
fruit::run ()
{
  // I don't have legs , 5555555555555.....
  return 0;
}

int
fruit::get_type () const
{
  return IAMFRUIT;
}

//=========================================Animal====================================
animal::animal ()
{
  has_sound = sound_appear->set_sound ("frog.wav");
}

animal::~animal ()
{
  // nothing to do
}

void
animal::put_food ()
{
  int rx, ry;

  general::use_color (2);
  while (1)
    {
      rx = rand () & (VIEW_COLS - 1);
      ry = rand () & (VIEW_LINES - 1);
      if (ry <= VIEW_TOP)
	continue;

      girlfriend.x = rx;
      girlfriend.y = ry;
      if (general::collide (girlfriend) != NOTHING)
	continue;
      break;
    }

  mvaddch (ry, rx, animal_graphic);
  if (has_sound == true)
    sound_appear->play ();

  refresh ();
}

/*
  Return type:
	0=have no idea about danger;
	1=safe now;
	-1=start to escape;
*/
int
animal::run ()
{
  static int run = 0;		//you must catch me in 3 rounds or it escapes
  static int n_step = 0;
  const int rounds = 3;
  const int counter = 80;

  if (n_step < counter)
    n_step++;
  else
    {
      n_step = 0;
      erase_food ();

      if (run++ < rounds)
	{
	  put_food ();
	  return -1;
	}
      else
	{
	  run = 0;
	  return 1;
	}
    }

  return 0;
}

int
animal::get_type () const
{
  return IAMANIMAL;
}
