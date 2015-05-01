/*
  This file derives from the io.cxx in old days , but I didn't want to reserve that file
  since last modification , because the chaotics of that file . 
  I divided that file into three parts : snake , food and this system operating . Some 
  useful utilies also be added directly to the main.cxx file , because I think that's a
  thoughtful place to store them . 	

  Copyright 2005,2006 by Richard Leon
  Originally write at 2005.3.10 , modified at 2006.2.23
*/

#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>
#include "common_ds.h"
#include "lovely_snake.h"
#include "food.h"
#include "compet.h"
#include "system.h"
#include "sound.h"
#include "const_dec.h"

using
  std::srand;
using
  std::rand;
using
  std::time;
using
  std::sprintf;
using
  std::log10;

envr::envr ()
{
  exit_game = false;
  snd_player::config_sound_card ();	// now nothing to do
}

envr::~envr ()
{
  for (int i = 0; i < FOOD_TYPE; ++i)
    delete all_food[i];
  delete first_player;
  snd_player::restore_sound_card ();
  endwin ();
}

void inline
envr::reset ()
{
  exit_game = false;
  my_food = all_food[FOOD_FRUIT_INDEX];
}

void
envr::set_food ()
{
  if (rand () <= (RAND_MAX >> 2))
    my_food = all_food[FOOD_ANIMAL_INDEX];
  else
    my_food = all_food[FOOD_FRUIT_INDEX];

  my_food->put_food ();
}

/*
  Return type :
  	0=The game is exited properly;
	-1=Something wrong occured;
	-2=User choose to exit game.
*/
int
envr::start_game ()
{
  // First , do the ncurses configuration

  initscr ();			// do screen initialization
  keypad (stdscr, TRUE);	// direct to screen
  noecho ();			// do not echo back
  nl ();			// no new line key
  nodelay (stdscr, TRUE);	// cancel the infinit loop 
  curs_set (0);			// hide the cursor
  color_startup ();		// initialize the color configuration
  clear ();			// clear the screen first

  init_rand ();			// initialize the random seed and game starting time

  // Add each objects , this is a temporary way to do that

  try
  {
    all_food[FOOD_FRUIT_INDEX] = new fruit ();
    all_food[FOOD_ANIMAL_INDEX] = new animal ();
    my_food = all_food[FOOD_FRUIT_INDEX];	// default is fruit
    first_player = new snake ();

    int r;
    while ((r = title ()) != 0)	// draw the menu selection screen
      {
	if (r == -1)
	  return -2;
      }
    do_main_loop ();		// never return until game over
  }
  catch ( ...)
  {
    // This time I have nothing to do
    return -1;
  }

  return 0;
}

int
envr::end_game ()
{
  bool choice = false;
  int menu_at = 1;
  int ch;
  char score[16];		// that's enough
  WINDOW *msgbox;
  const int pos[] = { 0, 9, 21 };
  const int disp_row = 6;
  const int disp_col_base = 5;

  // First draw a board on screen

  msgbox = newwin (9, 40, 6, 20);
  box (msgbox, '|', '-');
  mvwaddstr (msgbox, 2, 9, "Your final score is :");
  sprintf (score, "%d", first_player->get_score ());
  mvwaddstr (msgbox, 4, 9, score);
  waddstr (msgbox, "  Snake Currency");

  mvwaddstr (msgbox, disp_row, disp_col_base, "> Replay ");
  waddstr (msgbox, "  Main Menu ");
  waddstr (msgbox, "  Exit");

  while (true)
    {
      if (choice == true)
	break;

      wrefresh (msgbox);
      ch = getch ();
      switch (ch)
	{
	case KEY_LEFT:
	  if (menu_at == 1)
	    break;
	  else
	    {
	      mvwaddch (msgbox, disp_row, disp_col_base + pos[menu_at - 1],
			' ');
	      menu_at--;
	      mvwaddch (msgbox, disp_row, disp_col_base + pos[menu_at - 1],
			'>');
	    }
	  break;

	case KEY_RIGHT:
	  if (menu_at == 3)
	    break;
	  else
	    {
	      mvwaddch (msgbox, disp_row, disp_col_base + pos[menu_at - 1],
			' ');
	      menu_at++;
	      mvwaddch (msgbox, disp_row, disp_col_base + pos[menu_at - 1],
			'>');
	    }
	  break;

	case KEY_SPACE:
	case KEY_ENTER:
	  choice = true;
	  break;
	}

      draw_estimated_time ();
    }

  delwin (msgbox);		// forgive it 

  switch (menu_at)
    {
    case 1:
      game_restart ();
      break;

    case 2:
      reset ();
      int r;
      while ((r = title ()) != 0)
	{
	  if (r == -1)
	    return -1;
	}
      break;

    case 3:
      return -1;
    }

  return 0;
}

void
envr::draw_stage (int ltx, int lty, int brx, int bry)
{
  general::use_color (6);

  for (int i = ltx; i <= brx; i++)
    mvaddch (lty, i, '-');

  for (int i = ltx; i <= brx; i++)
    mvaddch (bry, i, '-');

  mvaddch (lty, ltx, '+');
  mvaddch (lty, brx, '+');
  mvaddch (bry, ltx, '+');
  mvaddch (bry, brx, '+');
  refresh ();
}

void
envr::draw_version ()
{
  const int disp_row = VIEW_TOP - 1;
  const int disp_col = (VIEW_COLS >> 1) - 20;

  general::use_color (3);
  mvaddstr (disp_row, disp_col, "Contribute yourself to the Free Software !");
}

void
envr::draw_estimated_time ()
{
  static time_t now;

  general::use_color (1);
  now = time (NULL);
  mvaddstr (VIEW_BOTTOM + 1, 19, "Time(sec):");
  printw ("%d", now - time_start);
}

void
envr::show_help ()
{
  clear ();
  general::use_color (1);

  mvaddstr (3, 25, "Short Tutorial For Greedy Snake Game");
  mvaddstr (5, 10, "1.Use up,down,left,right keys to control the snake;");
  mvaddstr (7, 10, "2.Press <,> to accelerate and decelerate;");
  mvaddstr (9, 10, "3.Press p to pause the game;");
  mvaddstr (11, 10,
	    "4.When the game paused , Press =,- to turn up and down the volume.");
  mvaddstr (13, 10,
	    "5.Press q to exit game immediately at any time except then paused.");
  mvaddstr (20, 20, "Press any key to return .....");

  while (getch () == ERR);
}

int
envr::itoa (int n, char *out)
{
  char base = '0';
  int step = static_cast < int >(log10 (n) + 1);	// how many digits
  register int tq;

  out += step;
  *(out--) = '\0';
  for (int i = 0; i < step; ++i)
    {
      tq = n & 9;
      *(out--) = (base + tq);
      n /= 10;
    }

  return step;
}

//a substitution version of the basic random routine , it allows you to specify the range 
int
envr::random (int range)
{
  if (range >= RAND_MAX)
    return -1;
  return rand () & (range - 1);
}

//initialize the random seed 
void
envr::init_rand ()
{
  time_t tp;

  tp = time (NULL);
  srand (tp);
  time_start = tp;		// steal the chance
}

void inline
envr::color_startup ()
{
  start_color ();
  init_pair (0, COLOR_MAGENTA, COLOR_BLACK);
  init_pair (1, COLOR_RED, COLOR_BLACK);
  init_pair (2, COLOR_BLUE, COLOR_BLACK);
  init_pair (3, COLOR_YELLOW, COLOR_BLACK);
  init_pair (4, COLOR_GREEN, COLOR_BLACK);
  init_pair (5, COLOR_WHITE, COLOR_BLACK);
  init_pair (6, COLOR_WHITE, COLOR_BLUE);
}

void inline
envr::game_restart ()
{
  reset ();
  snatch_menu (stage_choose);
}

int
envr::snatch_menu (int menu)
{
  general::use_color (1);
  clear ();

  switch (menu)
    {
    case 1:
      break;

    case 2:
      install_sword ();
      break;

    case 4:
      return -1;
    }

  draw_stage (VIEW_LEFT, VIEW_TOP, VIEW_RIGHT, VIEW_BOTTOM);
  draw_version ();
  draw_estimated_time ();
  first_player->give_birth ();
  my_food->put_food ();
  first_player->refresh_display ();

  return 0;
}

/*
   Return type:
	0=Resume game;
	-1=Exit game;
	1=Redo;
*/
int
envr::title ()
{
  int menu_at = 1;
  bool choice = false;
  const int menu_num = 4;
  const int disp_rows[] = { 13, 15, 17, 19 };
  const int disp_col = 27;
  const int disp_base = 11;

  clear ();
  general::use_color (4);

  mvaddstr (3, 5,
	    "  ********     *       *           *           *      *       ********  ");
  mvaddstr (4, 5,
	    "  *            * *     *                       *    *         *         ");
  mvaddstr (5, 5,
	    "  *            *  *    *         *   *         *  *           *         ");
  mvaddstr (6, 5,
	    "  ********     *   *   *                       **             ********  ");
  mvaddstr (7, 5,
	    "         *     *    *  *       *********       *  *           *         ");
  mvaddstr (8, 5,
	    "         *     *     * *                       *    *         *         ");
  mvaddstr (9, 5,
	    "  ********     *       *     *           *     *      *       ********  ");

  general::use_color (3);

  mvaddstr (disp_rows[0], disp_col, ">   N O R M A L    M O D E");
  mvaddstr (disp_rows[1], disp_col, "    F R E E    C O M P I T I O N");
  mvaddstr (disp_rows[2], disp_col, "    D O C U M E N T A T I O N");
  mvaddstr (disp_rows[3], disp_col, "    E X I T    G A M E");

  refresh ();

  while (choice != true)
    {
      int ch = getch ();
      switch (ch)
	{
	case KEY_DOWN:
	  if (menu_at < menu_num)
	    {
	      menu_at++;
	      mvaddch (disp_base + 2 * (menu_at - 1), disp_col, ' ');
	      mvaddch (disp_base + 2 * menu_at, disp_col, '>');
	    }
	  else
	    {
	      menu_at = 1;
	      mvaddch (disp_rows[menu_num - 1], disp_col, ' ');
	      mvaddch (disp_base + 2 * menu_at, disp_col, '>');
	    }

	  break;

	case KEY_UP:
	  if (menu_at > 1)
	    {
	      menu_at--;
	      mvaddch (disp_base + 2 * (menu_at + 1), disp_col, ' ');
	      mvaddch (disp_base + 2 * menu_at, disp_col, '>');
	    }
	  else
	    {
	      menu_at = menu_num;
	      mvaddch (disp_rows[0], disp_col, ' ');
	      mvaddch (disp_base + 2 * menu_at, disp_col, '>');
	    }

	  break;

	case KEY_SPACE:
	  choice = true;
	  break;
	}
      refresh ();
    }

  if (menu_at == 3)
    {
      show_help ();
      return 1;
    }

  stage_choose = menu_at;
  return snatch_menu (menu_at);
}

void
envr::do_main_loop ()
{
  int ch;
  int gstatus;

  while (1)
    {
      while (exit_game == false)
	{
	  ch = getch ();
	  switch (ch)
	    {
	    case ERR:
	      break;		// handle it locally

	    case PAUSE:
	      while ((ch = getch () != PAUSE))
		{
		  if (ch == VOLUME_UP)
		    snd_player::vol_up ();
		  else if (ch == VOLUME_DOWN)
		    snd_player::vol_down ();
		  draw_estimated_time ();
		}
	      break;

	    case QUIT:
	      exit_game = true;
	      break;

	    default:
	      if (first_player->translate_keys (ch) == INVALID)
		;
	      break;
	    }

	  if (exit_game == false)
	    {
	      if ((gstatus = first_player->run_snake ()) == -1)
		{
		  exit_game = true;
		  break;
		}

	      else if (gstatus == 1)
		set_food ();

	      if (stage_choose == 2)
		sword_control ();

	      if (my_food->run () == 1)
		set_food ();

	      draw_estimated_time ();
	    }

	  else
	    break;
	}

      int rd = end_game ();
      if (rd == 0)
	{
	  exit_game = false;
	  continue;
	}
      else
	break;
    }

  return;
}
