/***************************************************************************
 *            			const_dec.h
 *
 *  Sat Aug 13 16:45:15 2005
 *  Copyright  2005  Richard Leon
 *  Email richardxx@163.com
 ****************************************************************************/
/*
   Perhaps use a lot of constants is better custom in C++ , so I decide to do that .
   I have tried hard to accustom usage of constant , I was failed many times , and at last
   I gave it up , so the macros are at last added .
*/

#ifndef CONST_DEC_H
#define CONST_DEC_H

#define SUCCESS				0
#define FAILED 				-1
#define INVALID				-1987	// magic

const int KEY_SPACE = static_cast < int >(' ');
const int QUIT = static_cast < int >('q');
const int PAUSE = static_cast < int >('p');
const int SPEED_UP = static_cast < int >(',');
const int SPEED_DOWN = static_cast < int >('.');
const int VOLUME_UP = static_cast < int >('=');
const int VOLUME_DOWN = static_cast < int >('-');

// No program want the user to press a upper case character
const char DIE_DELAY = 'A';
const char DIE_IMMEDIATELY = 'B';
const char HEALTHY = 'C';
const char NOTHING = 'D';

const int VIEW_TOP = 1;
const int VIEW_BOTTOM = 24 - 2;
const int VIEW_LEFT = 0;
const int VIEW_RIGHT = 79;
const int VIEW_LINES = 24 - 2;
const int VIEW_COLS = 79;

const int DRAW_TIME_X = 60;
const int DRAW_TIME_Y = 24 - 2;
const int DRAW_FOOD_X = 20;
const int DRAW_FOOD_Y = 24 - 1;
const int DRAW_WEIGHT_X = 30;
const int DRAW_WEIGHT_Y = 24;

const int BORN_LOCATION = 57;
const int BORN_LENGTH = 4;

const int DEFAULT_SPEED = 100000;
const int SPEED_INTERVAL = 6000;
const int SPEED_UPBOUND = 300000;
const int SPEED_LOWBOUND = 50000;

const char empty_graphic = ' ';
const char snake_graphic = '*';
const char fruit_graphic = '$';
const char animal_graphic = 'M';
const char u_obstacle = '^';
const char d_obstacle = 'V';
const int different_icons = 6;

#endif
