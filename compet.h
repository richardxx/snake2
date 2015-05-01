/*
   A long I have planed to add some interactive features to my program since the first'
   edition , but till now I just find the way to solve the problem . This is module is
   just a testing , if the performace fits me , then I will consider to expand the whole
   program .

   Copyright 2005,2006 by Richard Leon
   First writed at 05.7.27
*/

#ifndef	STAGE_HEADER
#define STAGE_HEADER

struct point;

class sword
{
private:
  mutable point left_corner;
  mutable point right_corner;

protected:
  void atom_fill (char);

protected:
  void reset_left_corner (int, int);
  void reset_right_corner (int, int);

protected:
  const point & get_left_corner () const;
  const point & get_right_corner () const;

protected:
  void clear_filled ();

protected:
    virtual void set_position (int, int) = 0;
  virtual void fill_blank () = 0;
  virtual void goes_down () = 0;
  virtual void goes_up () = 0;
  virtual void install () = 0;

};

class sky_sword:private sword
{

public:
  void set_position (int, int);
  void fill_blank ();
  void goes_down ();
  void goes_up ();
  void install ();
};

class ground_sword:private sword
{

public:
  void set_position (int, int);
  void fill_blank ();
  void goes_down ();
  void goes_up ();
  void install ();
};

// A highly recommended method is hide the interface into wrapped function

extern void install_sword ();
extern void sword_control ();

#endif
