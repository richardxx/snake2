/*
  This file contains the code of the obstacle manipulation interface , I think it's better
  to obsolate the class implementaion and the controlling interface .
 
  Copyright 2005,2006 by Richard leon 
*/

#include <ncurses.h>
#include "const_dec.h"
#include "common_ds.h"
#include "compet.h"

// All these variables can't be seen by outer space

static ground_sword gs[2];
static sky_sword ss;
static int gs1_len = 0;
static int gs2_len = 0;
static int ss_len = 0;

// Isn't it a bit harder ?

const int ground_sword_height = 9;
const int sky_sword_height = 13;

void
install_sword ()
{
  gs[0].set_position (VIEW_BOTTOM - 1, 30);
  gs[1].set_position (VIEW_BOTTOM - 1, 47);
  ss.set_position (VIEW_TOP + 1, 40);

  gs[0].install ();
  gs[1].install ();
  ss.install ();

  gs1_len = 0;
  gs2_len = 0;
  ss_len = 0;
}

void
sword_control ()
{
  static int gs1_dir = -1;	//down
  static int gs2_dir = -1;	//down
  static int ss_dir = 1;	//up

  general::use_color (4);

  if (gs1_dir == -1)
    {
      if (--gs1_len < 0)
	{
	  gs1_dir = 1;
	  gs1_len = 0;
	}
      else
	gs[0].goes_down ();
    }

  else
    {
      if (++gs1_len > ground_sword_height)
	{
	  gs1_dir = -1;
	  gs1_len = ground_sword_height;
	}
      else
	gs[0].goes_up ();
    }

  if (gs2_dir == -1)
    {
      if (--gs2_len < 0)
	{
	  gs2_dir = 1;
	  gs2_len = 0;
	}
      else
	gs[1].goes_down ();
    }

  else
    {
      if (++gs2_len > ground_sword_height)
	{
	  gs2_dir = -1;
	  gs2_len = ground_sword_height;
	}
      else
	gs[1].goes_up ();
    }

  if (ss_dir == -1)
    {
      if (--ss_len < 0)
	{
	  ss_dir = 1;
	  ss_len = 0;
	}
      else
	ss.goes_down ();
    }

  else
    {
      if (++ss_len > sky_sword_height)
	{
	  ss_dir = -1;
	  ss_len = sky_sword_height;
	}
      else
	ss.goes_up ();
    }

}
