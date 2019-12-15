//-----------------------------------------------------------------------------
// BALL.C - contains definition of BALL_TASK and all its controlling functions
//-----------------------------------------------------------------------------
#include "init.h"
#include "physics.h"

//-----------------------------------------------------------------------------
//RESET_COLLISION_PARAMETERS FUNCTION - reset parameters before checking
// for collisions with other balls
//-----------------------------------------------------------------------------
void    reset_collision_parameters(int i) {
  ball[i].ld.x = 0;
  ball[i].ld.y = 0;
  ball[i].gd.x = 0;
  ball[i].gd.y = 0;
  ball[i].sl = ball[i].speed;
  ball[i].sg = 0;
}
//-----------------------------------------------------------------------------
// CHECK_BALL_COLLISION FUNCTION - calculate left speed in a direction and gain
// speed in other direction based on collision with other balls, then return
// how many balls a ball collides with
//-----------------------------------------------------------------------------
int     check_ball_collision(int i) {
    point  p3;                                  //projection point
    double dist;                                //distance between two balls
    double c_angle, b_angle, r_angle;           //collision,bounce,rev angle
    int collision_counter = 0;                  //stores how many collision
    int j;                                      //ball index

    for (j=0; j<N_BALLS; j++) {
        if (ball[j].alive && j != i) {
            dist = get_dist(&p3, ball[j].c.x, ball[j].c.y, ball[i].c.x, ball[i].c.y);
            //collision if distance between 2 ball centers is 2*ball_radius
            if (ball[i].pd[j] > dist && ball[i].cf[j] < 0 && dist < 30.0f) {
                collision_counter++;
                ball[i].cf[j] = 2;              //resolve compenetration bug
                //calculate collsion, bounce and reverse angle
                c_angle = get_angle(p3, dist);
                b_angle = get_bounce_angle(ball[i].angle, c_angle);
                r_angle = ((int)(c_angle + 180)) % 360;
                adjust_angle(&b_angle);
                //calculate how much speed is left and in which direction
                ball[i].sl -= get_speed_given(i, c_angle);
                ball[i].ld.x += cos((b_angle) * PI / 180);
                ball[i].ld.y += sin((b_angle) * PI / 180);
                if (ball[i].sl < 0)             //resolve compenetration bug
                    ball[i].sl = 0;
                //calculate how much speed is gained and in which direction
                ball[i].sg = get_speed_given(j, r_angle);
                ball[i].gd.x += cos(r_angle * PI / 180) * ball[i].sg;
                ball[i].gd.y += sin(r_angle * PI / 180) * ball[i].sg;
                //set still flag if ball was still, so it can update coordinate
                if (ball[i].still)
                    ball[i].still = false;
            }
            ball[i].pd[j] = dist;               //update prev distance
        }
    }
    return collision_counter;
}
