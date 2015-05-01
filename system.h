/*
   This file contains the code of the general purpose functions , such as initialize the 
   data and print out the cover .
   Originally these procedures look like C functions , that's right , I thought this could 
   help me to simplify the design , but not elegant .
   
   Change log:
	1.Fix the bug included in the menu selector ;
	2.Move all the separated functions into a single class , I think this could help to
 	  hide the details . 
   Copyright 2005,2006 by Richard Leon
*/

#ifndef __SYSTEM_H
#define __SYSTEM_H

const int FOOD_TYPE = 2;
const int FOOD_FRUIT_INDEX = 0;
const int FOOD_ANIMAL_INDEX = 1;

class food;
class snake;

class envr
{
public:
  envr ();
  ~envr ();

public:
  void reset ();
  void set_food ();
  int start_game ();		//from now on , you can move the snake

public:
    bool get_game_status () const;
  void set_game_status (bool);
  int get_cpu_speed () const;

private:
  int end_game ();		//print some messages
  int title ();			//the startup screen for selecting item
  int snatch_menu (int);	//handle the menu chosen
  void draw_stage (int, int, int, int);
  void draw_version ();
  void draw_estimated_time ();
  void show_help ();
  int itoa (int, char *);
  int random (int);		//get a number less than...
  void init_rand ();
  void color_startup ();
  void game_restart ();

private:
  void do_main_loop ();		//handle keyboard input and other affairs

private:
    bool exit_game;		// flag for game status
  bool pause;
  int time_start;
  int stage_choose;
  food *all_food[FOOD_TYPE];	// store instances of all types of food
  food *my_food;
  snake *first_player;		// point to the player's player
};

#endif
