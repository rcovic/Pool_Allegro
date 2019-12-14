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
//get distance between 2 points saving x,y coord. of projection along x axis
extern  double  get_distance(point* p3, int x1, int y1, int x2, int y2);
//get angle between 2 points calculated along x axis
extern  double  get_angle(point p3, double distance);
//same as get_angle but uses a vector value instead of 2 points
extern  double  get_vector_angle(vector v, double module);

#endif

