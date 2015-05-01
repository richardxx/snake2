/*
   	A small wonderful game to kill your idle time , and anyone can play it for fun !
	When the first release of the code in 2005 , I have recived many suggestions about 
	improvement , and also I have done two large revisions since then .
	This is the third time to modify the code , and this time I want to rearchitect the 
	game and rewrite it totally in pure C++ , or say aimed to clearfy the interface , 
	then you can add new features much easier .
	This methoed is so called object-oreinted programming , and till now I'm still master
	in C programming , the result is you can find many ugly code in the program , but I
	think everything has its own beginning , if I have tried , it will go better .

	Hope you enjoy the code !
	For some bug report or suggestions: frogxx@gmail.com or richardxx@163.com
 
	Copyright 2005,2006 by Richard Leon
	First released at 2005.3.10 , last modification started at 2006.2.23
*/

#include <signal.h>
#include <iostream>
#include <cstdlib>
#include "system.h"

using
  std::exit;
using
  std::cout;
using
  std::endl;

static void
err_handler (int);

const char *
  prog_dir = "/usr/local/GreedySnake/";
const char *
  snd_dir = "audio/";
envr *
  penv = NULL;

int
main ()
{
  // All things have shifted to the system level class
  penv = new envr;

  signal (SIGINT | SIGSEGV, err_handler);
  if (penv->start_game () == -1)
    {
      cout << endl;
      cout <<
	"Your terminal setting may be wrong , you can't play this game ." <<
	endl;
    }

  delete
    penv;
  return 0;
}

static void
err_handler (int sig)
{
  delete
    penv;			// automatically call the destructor
  exit (-1);
}
