/*
   The only data structure I use for the creation of snake is the linked list .
   To simplify the design , I only encapsulate the most used operations .

   Copyright 2005,2006 by Richard Leon
*/

#ifndef STRUCTURE_H
#define STRUCTURE_H

struct point;

class snake_list
{
private:
  typedef struct node
  {
    int x;
    int y;
    node *prev;
      node (int xx, int yy):x (xx), y (yy), prev (0)
    {
    }
  }
  snake_section;

  snake_section *head;
  snake_section *rear;

public:
  snake_list ();
  ~snake_list ();

public:
  void insert_node (const point &);	// longten the snake            
  void get_tail (point &);
  void get_head (point &);
  void delete_tail ();
  void clear ();
};

#endif
