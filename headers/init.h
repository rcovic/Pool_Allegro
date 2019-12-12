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
#define 	HOLE_DIST 		371			// absolute distance between holes
#define 	TOP_Y 			215			// y coordinate of top table border
#define 	BOT_Y 		    586			// y coordinate of bot table border
#define 	LEFT_X 			230			// x coordinate of left table border
#define 	RIGHT_X 		972			// x coordinate of right table border
//-----------------------------------------------------------------------------
// SCHEDULER CONSTANTS
//-----------------------------------------------------------------------------
#define     P               20          //period time in milliseconds
#define     RT              18          //run time in milliseconds
#define     DL              18          //deadline in milliseconds
//-----------------------------------------------------------------------------
// BITMAP OBJECTS DECLARATION
//-----------------------------------------------------------------------------
extern BITMAP*   parquet_bmp;	        //bitmap for the background
extern BITMAP*   pool_table_bmp;         //bitmap for the pool table
extern BITMAP*   ball_panel_bmp;         //bitmap for top and bot ball panel
extern BITMAP*  buffer_bmp;              //buffer bitmap
//-----------------------------------------------------------------------------
// FUNCTION DECLARATIONS
//-----------------------------------------------------------------------------
extern  void init_game(void);
//-----------------------------------------------------------------------------
// TASK FUNCTION DECLARATIONS
//-----------------------------------------------------------------------------
extern  void render_task(void);          //periodically draws on screen bitmaps

#endif
