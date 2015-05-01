/*
   Nearly there's no code , but may be one day it will be filled with functions .
   These functions are all most often used by some objects , and to define them here were
   not so elegant , but I don't have better way now .

   Copyright 2006 by Richard Leon
*/

#include <ctime>
#include <ncurses.h>
#include "const_dec.h"
#include "common_ds.h"

using
  std::difftime;
using
  std::time;

// The structure of point representation , very easy to archive

point::point ()
{
  x = y = 0;
}

point::point (int xx, int yy)
{
  x = xx;
  y = yy;
}

point::point (const point & p)
{
  this->x = p.x;
  this->y = p.y;
}

point
point::operator+ (const point & ap)
{
  point p (*this);
  p += ap;
  return p;
}

point & point::operator+= (const point & ap)
{
  this->x += ap.x;
  this->y += ap.y;
  return *this;
}

/* Return type:
	DIE_DELAY:you touch yourself or obstacles;
	NOTHING:normally,nothing to do else;
	HEALTHY:you meet something can eat;
*/
char
general::collide (const point & object)
{
  static int d_counter = 0;
  static char p_collection[] = { empty_graphic, snake_graphic, fruit_graphic,
    animal_graphic, u_obstacle, d_obstacle
  };
  chtype original = mvinch (object.y, object.x);
  char mask = original & A_CHARTEXT;
  int i;

  for (i = 0; i < different_icons; i++)
    if (mask == p_collection[i])
      break;
  switch (i)
    {
    case 0:
      d_counter = 0;
      return NOTHING;
      break;

    case 1:
    case 4:
    case 5:
      if (++d_counter <= 3)
	return DIE_DELAY;
      else
	return DIE_IMMEDIATELY;
      break;

    case 2:
    case 3:
      d_counter = 0;
      return HEALTHY;

    default:
      // Never reach here
      break;
    }

  return mask;
}

//      0:magenta
//      1:red
//      2:blue
//      3:yellow
//      4:green 
//      5:white
//      6:interface (view port)

void
general::use_color (int icolor)
{
  attron (A_NORMAL | COLOR_PAIR ((icolor)));
}

//calculate a appromixiate value to represent the cpu speed 

int
general::test_cpu_speed ()
{
  time_t t1;
  time_t t2;

  t1 = time (NULL);
  for (register int i = 0; i < 5000000; i++);
  t2 = time (NULL);
  double d_time = difftime (t2, t1);

  if (d_time <= 0.00001)
    return 7;
  else if (d_time > 0.00001 && d_time <= 0.0001)
    return 5;
  else
    return 3;
}
