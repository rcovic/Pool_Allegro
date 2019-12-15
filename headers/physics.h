//-----------------------------------------------------------------------------
// PHYSICS.H - header file of PHYSICS.C
//-----------------------------------------------------------------------------
#if !defined PHYSICS_H
#define PHYSICS_H

//-----------------------------------------------------------------------------
// INCLUDE LIBRARIES - Just needs of INIT.H library who contains all
//-----------------------------------------------------------------------------
#include "init.h"

//-----------------------------------------------------------------------------
// MATH COSTANTS
//-----------------------------------------------------------------------------
#define     PI      3.1415          //PI constant to work with circles
//-----------------------------------------------------------------------------
// PHYSICS FUNCTION DECLARATIONS
//-----------------------------------------------------------------------------
//set angle between 0 and 360 degrees
extern  void    adjust_angle(double* angle);
//get distance between 2 points saving x,y coord. of projection along x axis
extern  double  get_dist(point* p3, int x1, int y1, int x2, int y2);
//get angle between 2 points calculated along x axis
extern  double  get_angle(point p3, double distance);
//same as get_angle but uses a vector value instead of 2 points
extern  double  get_vector_angle(vector v, double module);
//calculate how much speed a ball gives to another
extern  double  get_speed_given(int tid, double angle);
//calculate bounce angle of a ball when collides with another ball
extern  double  get_bounce_angle(double a1, double a2);

#endif


