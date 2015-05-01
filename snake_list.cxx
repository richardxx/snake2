/*
  A light weight linked list , do it as quickly as I can , this could improve the performance .
  But since the third version , I modify the internal implementaion , so the compatiable 
  problem comes out , and the result is that the architecture is more powerful .

  Copyright 2005,2006 by Richard Leon
*/

#include "common_ds.h"
#include "snake_list.h"

snake_list::snake_list ()
{
  head = 0;
  rear = 0;
}

snake_list::~snake_list ()
{
  clear ();
}

void
snake_list::insert_node (const point & gpos)
{
  // New node is added at the head

  snake_section *new_pos = new snake_section (gpos.x, gpos.y);

  if (head != 0)
    head->prev = new_pos;
  if (rear == 0)
    rear = new_pos;

  head = new_pos;
}

void
snake_list::get_tail (point & rpos)
{
  rpos.x = rear->x;
  rpos.y = rear->y;
}

void
snake_list::get_head (point & rpos)
{
  rpos.x = head->x;
  rpos.y = head->y;
}

void
snake_list::delete_tail ()
{
  snake_section *t;

  if (rear != 0)
    {
      t = rear->prev;
      delete rear;
      rear = t;
    }
}

void
snake_list::clear ()
{
  if (rear != 0)
    {
      snake_section *t;
      while (rear != 0)
	{
	  t = rear->prev;
	  delete rear;
	  rear = t;
	}
    }

  head = 0;
  rear = 0;
}
