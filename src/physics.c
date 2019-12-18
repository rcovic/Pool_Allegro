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
//-----------------------------------------------------------------------------
// GET_SPEED_GIVEN FUNCTION: calculates how much speed a ball gives to another
// inversely proportional to the difference between collision and move angle
//-----------------------------------------------------------------------------
double  get_speed_given(int i, double angle){
    double  diff;                                   //diff between angles
    double  speed_given;                            //return value

    //calculate difference on a 0-360 degrees circumference
    if (ball[i].angle > 270 && angle <90)
        diff = abs(((int)(ball[i].angle - (angle + 360)) % 90));
    else if (ball[i].angle < 90 && angle > 270)
        diff = abs(((int)((ball[i].angle + 360) - angle) % 90));
    else
        diff = abs(((int)(ball[i].angle - (angle)) % 90));
    
    //Calculate speed given holding back a 10% value to avoid too static game
    speed_given = (ball[i].speed * (1 - (diff / 90.0f)));
    return speed_given * 0.9f;
}
//-----------------------------------------------------------------------------
// GET_BOUNCE_ANGLE FUNCTION: calculate bounce angle of a ball when collides
// with another ball based on ball direction angle and collision angle
// generally calculating a 90 degrees back or forth as bounce
//-----------------------------------------------------------------------------
double  get_bounce_angle(double a1, double a2){
    double bounce_angle;                            //return value

    if ((a1 < a2 && !(a1 <= 90 && a2 >= 270)) || (a1 >= 270 && a2 <= 90))
        bounce_angle = a2 - 90;
    else if (a1 > a2 || (a1 <= 90 && a2 >= 270))
        bounce_angle = a2 + 90;
	//ball is pushed behind instead of perp. if hits a ball with
	//collision angle equal to direction angle
    else if (a1 == a2)								                        
        bounce_angle = a2 + 180;
    return bounce_angle;
}
//-----------------------------------------------------------------------------
// DEGREE_TO_M FUNCTION: get the slope value given an angle
//-----------------------------------------------------------------------------
double degree_to_m(double angle){
	double rad=angle*PI/180;
	double m=tan(rad);
	return m;
}
//-----------------------------------------------------------------------------
// SET_TOP - set x end point given y TOP coordinate for a line with given m
//-----------------------------------------------------------------------------
void set_top(point *b, int x, int y, double m){
		b->y=TOP_Y+5;
		b->x = (((b->y - y)/m) + x) ;
}
//-----------------------------------------------------------------------------
// SET_BOT - set x end point given y BOT coordinate for a line with given m
//-----------------------------------------------------------------------------
void set_bot(point *b, int x, int y, double m){   
		b->y=BOT_Y-5;
		b->x = (((b->y - y)/m) + x) ;
}
//-----------------------------------------------------------------------------
// SET_LEFT - set y end point given x LEFT coordinate for a line with given m
//-----------------------------------------------------------------------------
void set_left(point *b, int x, int y, double m){   
		b->x=LEFT_X+2;
		b->y = (((b->x - x)*m) + y) ;
}
//-----------------------------------------------------------------------------
// SET_RIGHT - set y end point given x RIGHT coordinate for a line with given m
//-----------------------------------------------------------------------------
void set_right(point *b, int x, int y, double m){   
		b->x=RIGHT_X-2;
		b->y = (((b->x - x)*m) + y) ;
}
//-----------------------------------------------------------------------------
// LINE_BORDER_COLLISION: calculate on which border line has to stop, based
// on the current line angle, updating end x,y coordinates for render function
//-----------------------------------------------------------------------------
void line_border_coll(point* b, int x, int y, double d, double m){
    double e;                                 //angle between hole and start
    point tmp;                                //tmp point to get distance
    double dist;                              //dist between hole and start

    if(d >= 270 && d <= 360){                        
        dist = get_dist(&tmp, hole[2].x, hole[2].y, x, y);
        e = get_angle(tmp, dist);             
        if(d < e || e == 0)
            set_top(b,x, y,m);
        else
            set_right(b,x, y,m);

    }
    else if(d >= 90 && d < 180){                        
        dist = get_dist(&tmp, hole[3].x, hole[3].y, x, y);
        e = get_angle(tmp, dist);             
        if(d < e)
            set_bot(b,x, y,m);
        else
            set_left(b,x, y,m);
    }
    else if(d < 90){                        
        dist = get_dist(&tmp, hole[5].x, hole[5].y, x, y);
        e = get_angle(tmp, dist);             
        if(d < e)
            set_right(b,x, y,m);
        else
            set_bot(b,x, y,m);
    }
    else{                        
        dist = get_dist(&tmp, hole[0].x, hole[0].y, x, y);
        e = get_angle(tmp, dist);             
        if(d < e)
            set_left(b,x, y,m);
        else
            set_top(b,x, y,m);
    }
}
