//-----------------------------------------------------------------------------
// INIT.H - header file of INIT.C
// Contains costants, global data structures and task declarations.
//-----------------------------------------------------------------------------
#if !defined INIT_H
#define INIT_H

//-----------------------------------------------------------------------------
// INCLUDE LIBRARIES
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <allegro.h>
#include <stdbool.h>
#include <math.h>
#include <semaphore.h>

#include "ptask.h"
#include "pmutex.h"
//-----------------------------------------------------------------------------
// BACKGROUND CONSTANTS
//-----------------------------------------------------------------------------
#define		RES_X			1200		// window x resolution
#define 	RES_Y			800			// window y resolution
#define 	PANEL1_X 		325			// x coordinate of top ball panel
#define 	PANEL1_Y 		-20			// y coordinate of top ball panel
#define 	PANEL2_X 		325			// x coordinate of bottom ball panel
#define 	PANEL2_Y 		730			// y coordinate of bottom ball panel
//-----------------------------------------------------------------------------
// POOL TABLE CONSTANTS
//-----------------------------------------------------------------------------
#define 	TABLE_X 		200			// x coordinate of pool table
#define 	TABLE_Y 		180			// y coordinate of pool table
#define 	N_HOLES 		6			// number of holes of pool table
#define 	HOLE_X 			230			// x coordinate of top left hole
#define 	HOLE_Y 			215			// y coordinate of top left hole
#define 	HOLE_DISTANCE 	371			// absolute distance between holes
#define 	TOP_Y 			215			// y coordinate of top table border
#define 	BOT_Y 		    586			// y coordinate of bot table border
#define 	LEFT_X 			230			// x coordinate of left table border
#define 	RIGHT_X 		972			// x coordinate of right table border
//-----------------------------------------------------------------------------
// BALL CONSTANTS
//-----------------------------------------------------------------------------
#define     BALLSIZE        30          //size of ball bitmap
#define     BALL_DISTANCE   21          //distance between white ball and cue
#define     N_BALLS         16          //total number of balls including white
#define     WHITE_X         387         //x coordinate of white ball
#define     WHITE_Y         385         //y coordinate of white ball
#define     BALL1_X         700         //x coordinate of ball 1
#define     BALL1_Y         385         //y coordinate of ball 1
#define     SPEED           20          //max speed of a ball
//-----------------------------------------------------------------------------
// USER CONSTANTS
//-----------------------------------------------------------------------------
#define     CUE_X           370         //start x pos of cue
#define     CUE_Y           402         //start y pos of cue
#define     AIM             0           //in that states user moves cue
#define     LOAD            1           //here user decides when to shot
#define     RELEASE         2           //here cue moves towards white ball
#define     WAKE_BALL       3           //here user give parameters to white
#define     WAIT            4           //here user wait for all balls to stop
//-----------------------------------------------------------------------------
// SCHEDULER CONSTANTS
//-----------------------------------------------------------------------------
#define     P               28          //period time in milliseconds
#define     RT_R            5           //render_task run time in milliseconds
#define     RT_B            23          //ball_task run time in milliseconds
#define     RT_U            2           //user run time in milliseconds
#define     DL_R            27          //render deadline in milliseconds
#define     DL_B            26          //ball deadline in milliseconds
#define     DL_U            28          //user deadline in milliseconds
//-----------------------------------------------------------------------------
// DATA STRUCTURES
//-----------------------------------------------------------------------------
typedef struct  point   {               //used to store x and y coordinates
    int     x;
    int     y;
}   point;

typedef struct  vector  {               //used to store x and y vector value
	double  x;
	double  y;
}   vector;

typedef struct  ball_struct {           //contains parameter of a ball
    point   p;                          //top-left point of ball bitmap
    point   c;                          //centre coordinate of ball
    vector  d;                          //stores x,y direction of ball
    double  angle;                      //direction angle in range 0-360
    double  speed;                      //current speed of ball
    bool    alive;                      //true if ball is still on the table
    bool    still;                      //true if ball is not moving
    vector  ld;                         //storex x,y left direction of ball
    vector  gd;                         //stores x,y gain direction of ball
    double  sl;                         //speed left of ball after collision
    double  sg;                         //speed gain of ball after collision
    int     cf[N_BALLS];                //resolve compenetration bug
    double  pd[N_BALLS];                //previous distance between balls
}   ball_struct;

typedef struct  user_struct {           //contain user states and cue params
    int     state;                      //current user state
    double  aim_angle;                  //angle between mouse and white ball
    int     cue_angle;                  //how much rotate cue in range 0-256
    point   p;                          //pivot on which rotate cue bitmap
    int     wd;                         //distance between cue and white ball
    double  shot_power;                 //power given to white ball after shot
    int     player;                     //indicates which player is playing
    int     p1_score;                   //current score of player 1
    int     p2_score;                   //current score of player 2
}   user_struct;
//-----------------------------------------------------------------------------
// BITMAP OBJECT DECLARATIONS
//-----------------------------------------------------------------------------
extern  BITMAP*     parquet_bmp;        //bitmap for the background
extern  BITMAP*     pool_table_bmp;     //bitmap for the pool table
extern  BITMAP*     ball_panel_bmp;     //bitmap for top and bot ball panel
extern  BITMAP*     ball_bmp[N_BALLS];  //16 bitmaps for 16 balls
extern  BITMAP*     cue_bmp;            //bitmap for user cue
extern  BITMAP*     buffer_bmp;         //buffer bitmap
//-----------------------------------------------------------------------------
// GLOBAL VARIABLE DECLARATIONS
//-----------------------------------------------------------------------------
extern  point       hole[N_HOLES];      //contains coordinates of holes
extern  ball_struct ball[N_BALLS];      //contains parameters of balls
extern  user_struct user;               //contain user states and cue params
//-----------------------------------------------------------------------------
// SEMAPHORE DECLARATIONS
//-----------------------------------------------------------------------------
extern  sem_t       mutex;              //for mutual exclusion
extern  sem_t       ball_sem[N_BALLS];  //for synchronize ball tasks
//-----------------------------------------------------------------------------
// FUNCTION DECLARATIONS
//-----------------------------------------------------------------------------
extern  void        init_game(void);    //init game used in INIT.C
//-----------------------------------------------------------------------------
// TASK FUNCTION DECLARATIONS
//-----------------------------------------------------------------------------
extern  void        render_task(void);  //periodically draws on screen bitmaps
extern  void        ball_task(void);    //periodically updates ball parameters
extern  void        user_task(void);    //periodically updates user parameters

#endif
