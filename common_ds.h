/*
  This is the file representing the commonly used data structure .
  This at first comes from the snake internal data structures , but now they are expanded to
  many filed , so I put it here .

  Copyright 2006 by Richard Leon
*/

#ifndef COMMON_DS
#define COMMON_DS

struct point
{
  int x;
  int y;
    point ();
    point (int, int);
    point (const point &);
  point operator+ (const point &);
    point & operator+= (const point &);
};

typedef point vector;

class general
{
public:
  static char collide (const point &);
  static void use_color (int);
  static int test_cpu_speed ();	// Simple loop tests
};

#endif
