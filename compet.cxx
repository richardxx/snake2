/*
  This module is added later during the second revision , that day I had just read over the
  the Bjarne Stroustup's classic C++ book , it simulated me to do some practice , so the
  the creative idea came to me.

  Copyright 2005,2006 by Richard Leon
  First writen at 2005.10.20
*/

#include <ncurses.h>
#include "const_dec.h"
#include "common_ds.h"
#include "compet.h"

void
sword::reset_left_corner (int y, int x)
{
  left_corner.y = y;
  left_corner.x = x;
}

void
sword::reset_right_corner (int y, int x)
{
  right_corner.y = y;
  right_corner.x = x;
}

const point &
sword::get_left_corner () const
{
  return left_corner;
}

const point &
sword::get_right_corner () const
{
  return right_corner;
}

void
sword::atom_fill (char type)
{
  int y1 = left_corner.y;
  int y2 = right_corner.y;
  int x1 = left_corner.x;
  int x2 = right_corner.x;

  for (int i = y1; i <= y2; i++)
    for (int j = x1; j <= x2; j++)
      mvaddch (i, j, type);
}

void
sword::clear_filled ()
{
  atom_fill (' ');
}

void
sky_sword::set_position (int y, int x)
{
  reset_left_corner (y, x);
}

void
sky_sword::fill_blank ()
{
  atom_fill (u_obstacle);
}

void
sky_sword::goes_down ()
{
  const point & tp = get_right_corner ();

  clear_filled ();
  reset_right_corner (tp.y - 1, tp.x);
  fill_blank ();

}

void
sky_sword::goes_up ()
{
  const point & tp = get_right_corner ();

  clear_filled ();
  reset_right_corner (tp.y + 1, tp.x);
  fill_blank ();
}

void
sky_sword::install ()
{
  const point & tp = get_left_corner ();

  reset_right_corner (tp.y, tp.x + 3);
}



void
ground_sword::set_position (int y, int x)
{
  reset_right_corner (y, x + 3);
}

void
ground_sword::fill_blank ()
{
  atom_fill (d_obstacle);
}

void
ground_sword::goes_down ()
{
  const point & tp = get_left_corner ();

  clear_filled ();
  reset_left_corner (tp.y + 1, tp.x);
  fill_blank ();
}

void
ground_sword::goes_up ()
{
  const point & tp = get_left_corner ();

  clear_filled ();
  reset_left_corner (tp.y - 1, tp.x);
  fill_blank ();
}

void
ground_sword::install ()
{
  const point & tp = get_right_corner ();

  reset_left_corner (tp.y, tp.x - 3);
}
