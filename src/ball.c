//-----------------------------------------------------------------------------
// BALL.C - contains definition of BALL_TASK and all its controlling functions
// as collision with other balls, with borders or holes, and through the
// PHYSICS library modifies periodically its params and updates its position
//-----------------------------------------------------------------------------
#include "init.h"
#include "physics.h"

//-----------------------------------------------------------------------------
//RESET_COLLISION_PARAMETERS FUNCTION - reset parameters each period 
//before checking collisions with other balls
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
    point   p3;                                 //projection point
    double  dist;                               //distance between two balls
    double  c_angle, b_angle, r_angle;          //used to manage collisions
    int     collision_counter = 0;              //stores number of collisions
    int     j;                                  //ball index

    for (j=0; j<N_BALLS; j++) {
        if (ball[j].alive && j != i) {          //check only for ball on table
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
                //set flag if hit ball was still, so it can update coordinates
                if (ball[j].still)
                    ball[j].still = false;
            }
            ball[i].pd[j] = dist;               //update prev distance
        }
    }
    return collision_counter;
}
//-----------------------------------------------------------------------------
// SYNC_BALL_TASK FUNCTION - all ball task are synchronized at this point,
// so they can modify their speed and angle after each ball has read them
// for checking collisions
//-----------------------------------------------------------------------------
void    sync_ball_task(int i) {
    int     j;                                  //ball index
    //tasks synchronization through mutex and private semaphores
    sem_wait(&mutex);                           //start critical section
    for (j=0; j<N_BALLS; j++) {
        if (j != i)
            sem_post(&ball_sem[j]);
    }
    sem_post(&mutex);                           //end critical section

    for (j=0; j<N_BALLS-1; j++)
        sem_wait(&ball_sem[i]);
}
//-----------------------------------------------------------------------------
// UPDATE_BALL_PARAMETERS FUNCTION - calculate ball speed and angle adding up
// all vectors and determing result vector, where its module corresponds to
// ball speed and its direction corresponds to ball angle
//-----------------------------------------------------------------------------
void    update_ball_parameters(int i) {
    ball[i].gd.x += (ball[i].ld.x * ball[i].sl);
    ball[i].gd.y += (ball[i].ld.y * ball[i].sl);
    ball[i].speed = sqrt(pow(ball[i].gd.x, 2) + pow(ball[i].gd.y, 2));
    ball[i].angle = get_vector_angle(ball[i].gd,ball[i].speed);
}
//-----------------------------------------------------------------------------
// UPDATE_SCORE FUNCTION - increments score of player who has put a ball
// in a hole (never called for white ball), then put the ball on the
// player's panel
//-----------------------------------------------------------------------------
void    update_score(int i) {
    sem_wait(&mutex);                           //start critical section
    if (user.player == 1) {
        set_ball_parameters(347 + (34 * user.p1_score), 8, &ball[i]);
        user.p1_score ++;
    }
    else {
        set_ball_parameters(347 + (34 * user.p2_score), 762, &ball[i]);
        user.p2_score ++;
    }
    ball[i].alive = false;                      //disable ball controls
    sem_post(&mutex);                           //end critical section
}
//-----------------------------------------------------------------------------
// C_HOLE FUNCTION - check if coordinate of a ball are in the range of
// the specified hole, if so update score of current player
//-----------------------------------------------------------------------------
void    c_hole(int sx, int dx, int up, int down, int i) {
    if (ball[i].c.x >= sx && ball[i].c.x <= dx) {
        if (ball[i].c.y >= up && ball[i].c.y <= down) {
            //if white ball, don't update score and put it out of the table,
            //until all balls are still (then it will be replaced on table)
            if(i == 0){
                set_ball_parameters(3000,3000,&ball[0]);
                ball[i].alive = false;
            }
            else
                update_score(i);
        }
    }
}
