/*
   	I emulate the behavior of snake in this file .
	To isolate the code in one file , I can do more things than you ever expect ,
	for example , this time I make the snake looks colorful ...
	
	Copyright 2006 by Richard Leon
*/

#ifndef LOVELY_SNAKE_H
#define LOVELY_SNAKE_H

const int D_LEFT = 0;
const int D_UP = 1;
const int D_RIGHT = 2;
const int D_DOWN = 3;

class snake_list;
class snd_player;
struct point;

class snake
{
public:
  int get_score () const;
  int get_speed () const;
  int get_weight () const;

public:
    virtual void give_birth ();
  virtual int translate_keys (int);
  virtual int run_snake ();
  virtual void refresh_display () const;

public:
    snake ();
    virtual ~ snake ();

protected:
  int move_snake ();
  int edge_test (const point &);
  void eat_food (char);
  void refresh_weight () const;
  void refresh_speed () const;

private:
    bool has_sound;
  int cur_dir;			// the direction of the snake head
  int speed_shift_control;
  int body_length;
  int weight;
  int eat_fruit;
  int eat_animal;
  int vertical_interval;
  int horizontal_interval;
  int cpu_speed;
  snake_list *body;
  snd_player *walk_sound;
  vector my_fd;			// an easy way to guide the snake
};

#endif
