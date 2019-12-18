//-----------------------------------------------------------------------------
// PHYSICS.C Implementation of physics methods used to calculate physics
// parameters, like distances, angles and coordinates to draw aiming line
//-----------------------------------------------------------------------------
#include "init.h"
#include "physics.h"

//-----------------------------------------------------------------------------
// ADJUST_ANGLE FUNCTION: set angle between 0 and 360 degrees
//-----------------------------------------------------------------------------
void    adjust_angle(double* angle) {
    if (*angle < 0)
        *angle += 360;
    else if (*angle >= 360)
        *angle -= 360;
}
//-----------------------------------------------------------------------------
// GET_DISTANCE FUNCTION: get distance between 2 points saving x,y 
// coordinates of projection of a point along x axis
//-----------------------------------------------------------------------------
double  get_dist(point* p3, int x1, int y1, int x2, int y2) {
    double    distance;                             //stores return value

    p3->x = x1 - x2;                                //get x projection
    p3->y = y1 - y2;                                //get y projection
    distance = sqrt(pow(p3->x, 2) + pow(p3->y, 2)); //calculate distance
    return distance;
}
//-----------------------------------------------------------------------------
// GET_ANGLE FUNCTION: get angle between 2 points calculated along x axis
//-----------------------------------------------------------------------------
double  get_angle(point p3, double distance) {
    double  angle;                                  //stores return value

    angle = acos(p3.x / distance) * 180 / PI;       //calculate angle

    if (p3.y < 0)                                   //adjust angle
        angle = (360 - abs(angle)) % 360;
    return  angle;
}
//-----------------------------------------------------------------------------
// GET_VECTOR_ANGLE FUNCTION: uses a vector value instead of 2 points
//-----------------------------------------------------------------------------
double  get_vector_angle(vector v, double module) {
    double  angle;                                  //stores return value

    //calculate angle using a 0.001 padding to avoid division on 0
    angle = acos(((v.x) + 0.001) / (module + 0.001)) * 180 / PI;

    if (v.y < 0)                                    //adjust angle
        angle = (360 - abs(angle)) % 360;
    return  angle;
}
