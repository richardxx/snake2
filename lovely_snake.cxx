/*
  It's indeedly very interesting to draw a vivid snake on screen , and this time I have
  improve my implementation to make it colorful . It's really wonderful , but I also should
  consider the performace of the program execution , so I just play a trick on that .

  Copyright 2005,2006 by Richard Leon
*/

#include <unistd.h>
#include <ncurses.h>
#include "common_ds.h"
#include "snake_list.h"
#include "lovely_snake.h"
#include "sound.h"
#include "const_dec.h"

//First , I should implement the interface derived

void
snake::give_birth ()
{
  static int color_accul = 0;
  point start_pos;
  const int ypos = 4;

  body->clear ();		// release the memory first
  speed_shift_control = DEFAULT_SPEED;	// reset the parameters
  cur_dir = D_LEFT;
  body_length = BORN_LENGTH;
  weight = 0;
  eat_fruit = 0;
  eat_animal = 0;
  my_fd.x = -1;			// guide west always at start
  my_fd.y = 0;

  /*
  vertical_interval = (cpu_speed + 2) << speed_shift_control;
  horizontal_interval = (cpu_speed + 1) << speed_shift_control;
  */
  horizontal_interval = DEFAULT_SPEED;
  vertical_interval = DEFAULT_SPEED + SPEED_INTERVAL;

  for (int i = 0; i < body_length; i++)
    {
      start_pos.x = BORN_LOCATION + body_length - i;
      start_pos.y = ypos;
      body->insert_node (start_pos);
      general::use_color (color_accul);
      mvaddch (ypos, start_pos.x, snake_graphic);
    }

  color_accul = 6 - ((++color_accul) & 5);
  move (ypos, BORN_LOCATION);	// point to head
  refresh ();
}

int
snake::get_score () const
{
  // the algorithm here is just a reference , you can modify it if you like

  int over_load = (speed_shift_control * body_length) >> 2;
  return (over_load + eat_animal * 2 + eat_fruit) * 5;
}

int inline
snake::get_speed () const
{
  return speed_shift_control;
}

int inline
snake::get_weight () const
{
  return weight;
}

int
snake::translate_keys (int ch)
{
  switch (ch)
    {
      /* Basically , I must omit the two types of error :
         1.Be the same direction;
         2.Be the opposite direction;
         But , the possibility of the first event is very small , and it can't
         destroy the program when occured , so I don't have code to handle it .
       */

    case KEY_RIGHT:
      if (cur_dir != D_LEFT)
	{
	  my_fd.x = 1;
	  my_fd.y = 0;
	  cur_dir = D_RIGHT;
	}
      break;

    case KEY_LEFT:
      if (cur_dir != D_RIGHT)
	{
	  my_fd.x = -1;
	  my_fd.y = 0;
	  cur_dir = D_LEFT;
	}
      break;

    case KEY_UP:
      if (cur_dir != D_DOWN)
	{
	  my_fd.x = 0;
	  my_fd.y = -1;
	  cur_dir = D_UP;
	}
      break;

    case KEY_DOWN:
      if (cur_dir != D_UP)
	{
	  my_fd.x = 0;
	  my_fd.y = 1;
	  cur_dir = D_DOWN;
	}
      break;

    case SPEED_UP:
      {
	vertical_interval -= SPEED_INTERVAL;
	horizontal_interval -= SPEED_INTERVAL;
	refresh_speed ();  
      }
      break;

    case SPEED_DOWN:
      {
	vertical_interval += SPEED_INTERVAL;
	horizontal_interval += SPEED_INTERVAL;
	refresh_speed ();  
      }
      break;
      
    default:
      return INVALID;
    }

  return SUCCESS;
}

/*
  Return type:
	-1=You die;
	1=The snake met a kind of food;
	0=Nothing serious.
*/
int
snake::run_snake ()
{
  static int snake_status;

  //first , make the snake move forward
  //if you can't move the snake , then the game is over
  snake_status = move_snake ();
  if (snake_status != 0)
    return snake_status;

  //because the interval between two rows is lager than two columns
  
  if (cur_dir == D_LEFT || cur_dir == D_RIGHT)
    usleep(horizontal_interval);
  // for (int i = 0; i < horizontal_interval; i++);
  else
    usleep(vertical_interval);
    //for (int i = 0; i < vertical_interval; i++);
    
  return 0;
}

void
snake::refresh_display () const
{
  refresh_weight ();
  refresh_speed ();
}

// Then the constructor and destructor , it has very few works to do

snake::snake ()
{
  cpu_speed = general::test_cpu_speed ();
  body = new snake_list ();
  walk_sound = new snd_player ();
  //has_sound=walk_sound->set_sound("snake.wav");
}

snake::~snake ()
{
  delete body;
  delete walk_sound;
}

// The hardest work is hided in the private interface

/* Return value:
	0=normal;
	-1=can't move,you die;
	1=I grew bigger.
*/
int
snake::move_snake ()
{
  static point tail, head;
  static point new_body;
  static char mask;
  static int color_accul = 2;	// magic number

  body->get_tail (tail);
  body->get_head (head);
  mvaddch (tail.y, tail.x, ' ');

  new_body.x = head.x + my_fd.x;
  new_body.y = head.y + my_fd.y;
  mask = general::collide (new_body);

  int rtest = edge_test (new_body);
  if (rtest == -1)		// no another chance
    return -1;

  else if (rtest == -2)		// wait to escape 
    return 0;

  else
    {
      /*if (has_sound==true)
         walk_sound->play(); */// it's so noisy since I comment it

      if (mask == NOTHING)
	{
	  body->delete_tail ();
	  body->insert_node (new_body);
	  general::use_color (color_accul);
	  mvaddch (new_body.y, new_body.x, snake_graphic);
	}

      else if (mask == HEALTHY)	//I meet food  !!
	{
	  body->insert_node (new_body);
	  color_accul = (color_accul + 7) & 5;
	  general::use_color (6 - color_accul);
	  mvaddch (new_body.y, new_body.x, snake_graphic);
	  eat_food (mask);
	  return 1;
	}

      else if (mask == DIE_DELAY)	//the snake is in danger!
	;

      else if (mask == DIE_IMMEDIATELY)	//What a pity!
	return -1;
    }

  return 0;
}

/* Return value :
	-1=you die;
	-2=you have chance to escape;
	0=you are healthy!
*/
int
snake::edge_test (const point & ckpos)
{
  static int stay_counter = 0;

  if (ckpos.x > VIEW_RIGHT || ckpos.x < VIEW_LEFT)
    {
      if (++stay_counter == 5)
	{
	  stay_counter = 0;
	  return -1;
	}
      return -2;
    }

  if (ckpos.y >= VIEW_BOTTOM || ckpos.y <= VIEW_TOP)
    {
      if (++stay_counter == 5)
	{
	  stay_counter = 0;
	  return -1;
	}
      return -2;
    }

  stay_counter = 0;		//clear the flag

  return 0;
}

void
snake::eat_food (char mask)
{
  if (mask == fruit_graphic)
    {
      ++eat_fruit;
      ++weight;
    }

  else
    {
      /* Great , I catch an animal                    */

      ++eat_animal;
      weight += 2;
    }
  ++body_length;

  refresh_weight ();
}

void inline
snake::refresh_weight () const
{
  general::use_color (1);
  mvaddstr (VIEW_BOTTOM + 1, 37, "Weight:");
  printw ("%d", weight);
}

void inline
snake::refresh_speed () const
{
  general::use_color (1);
  mvaddstr (VIEW_BOTTOM + 1, 52, "Speed:");
  printw ("%d", speed_shift_control - SPEED_LOWBOUND);
}
