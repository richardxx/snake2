/*
  Anyone can't live without food , so does the snake .
  There's two kind of food : animal and fruit . The fruit has fixed position on
  the screen , while the animal not . The animal perhaps run when you approach it , 
  and it will disappear in 3 or 4 rounds. 
  You can also add your favourite food , just write a little code .
	
  Copyright 2005,2006 by Richard Leon
*/

#ifndef FOOD_H
#define FOOD_H

const int IAMFRUIT = 311;
const int IAMANIMAL = 224;

class snd_player;
struct point;

class food
{
public:
  virtual void put_food () = 0;
  virtual void erase_food ();	// common routine
  virtual int run () = 0;
  virtual int get_type () const = 0;

public:
    food ();
    virtual ~ food ();

protected:
    bool has_sound;
  point girlfriend;		// food is my girlfriend ??
  snd_player *sound_appear;
};

class fruit:public food
{
public:
  void put_food ();
  int run ();
  int get_type () const;

public:
    fruit ();
   ~fruit ();
};

class animal:public food
{
public:
  void put_food ();
  int run ();
  int get_type () const;

public:
    animal ();
   ~animal ();
};

#endif
