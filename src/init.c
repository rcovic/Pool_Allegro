//-----------------------------------------------------------------------------
// INIT.C - provides methods to initialize data parameters
//-----------------------------------------------------------------------------
#include "init.h"

//-----------------------------------------------------------------------------
// BITMAP OBJECTS DEFINITION (already commented in INIT.H)
//-----------------------------------------------------------------------------
BITMAP*  parquet_bmp;
BITMAP*  pool_table_bmp;
BITMAP*  ball_panel_bmp;
BITMAP*  ball_bmp[N_BALLS];  //16 bitmaps for 16 balls
BITMAP*  cue_bmp;            //bitmap for user cue
BITMAP*  buffer_bmp;         //buffer bitmap
//-----------------------------------------------------------------------------
// GLOBAL VARIABLES DEFINITION
//-----------------------------------------------------------------------------
point       hole[N_HOLES];       //contains coordinates of holes
ball_struct ball[N_BALLS];       //contains parameters of balls
user_struct user;                //contain user states and cue params
//-----------------------------------------------------------------------------
// INIT_ENVIRONMENT FUNCTION: initialize allegro environment and ptask library
//-----------------------------------------------------------------------------
void init_environment(void){
    allegro_init();
    set_color_depth(desktop_color_depth());
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, RES_X, RES_Y, 0, 0);
    install_mouse();
    show_mouse(screen);
    install_keyboard();
    ptask_init(SCHED_OTHER, GLOBAL, NO_PROTOCOL);
}
//-----------------------------------------------------------------------------
// INIT_BITMAPS FUNCTION: load bitmaps from img folder to bitmap objects
//-----------------------------------------------------------------------------
void init_bitmaps(void){
    char    ballFilename[255];      //stores i-ball filename
    int     i;                      //ball index
    
    parquet_bmp = load_bitmap("img/parquet.bmp", NULL);
    pool_table_bmp = load_bitmap("img/poolTable.bmp", NULL);
    ball_panel_bmp = load_bitmap("img/statsPanel.bmp", NULL);
    
     for (i=0; i<N_BALLS; ++i) {
        sprintf(ballFilename, "img/ball%d.bmp", i);
        ball_bmp[i] = load_bitmap(ballFilename, NULL);
    }

    
    buffer_bmp = create_bitmap(RES_X, RES_Y);
}

//-----------------------------------------------------------------------------
// INIT_POOL_TABLE FUNCTION: calculate and stores all hole positins
//-----------------------------------------------------------------------------
void    init_pool_table(void)   {
    int i;                          //hole index

	for (i=0; i<N_HOLES; ++i) {
		hole[i].x = (HOLE_X + HOLE_DISTANCE * (i % 3));
		hole[i].y = (HOLE_Y + HOLE_DISTANCE * (i / 3));
	}
}
//-----------------------------------------------------------------------------
// SET_BALL_PARAMETERS FUNCTION: called when game starts or when a ball
// is out of the game
//-----------------------------------------------------------------------------
void    set_ball_parameters(int x, int y, ball_struct* b) {
    b->p.x = x;
	b->p.y = y;
	b->c.x = x +15;
	b->c.y = y +15;
	b->d.x = x;
	b->d.y = y;
	b->speed = 0;
	b->alive = true;
	b->still = true;
}
//-----------------------------------------------------------------------------
// INIT_BALLS FUNCTION: calculate all ball positions and sets theirs parameters
//-----------------------------------------------------------------------------
void    init_balls() {
    //local variables used to calculate next ball position
	int     threshold = 1;
	int     count = 0;
	point   tmp;
	tmp.x = BALL1_X;
	tmp.y = BALL1_Y;
    int     i;                      //ball index
	set_ball_parameters(WHITE_X, WHITE_Y, &ball[0]);
    
    for (i=1; i<N_BALLS; ++i) {
        if (count >= threshold) {
	        threshold++;
			count = 0;
			tmp.x += BALLSIZE;
			tmp.y += BALLSIZE/2;
		}
		if (i == 1)
		    set_ball_parameters(BALL1_X,BALL1_Y,&ball[1]);
		else
			set_ball_parameters(tmp.x, tmp.y - count * BALLSIZE, &ball[i]);
		count++;
	}
}
//-----------------------------------------------------------------------------
// INIT_USER FUNCTION: reset user status and cue parameters after each turn
//-----------------------------------------------------------------------------
void    init_user() {
	user.state = 0;
	user.aim_angle = 0;
	user.cue_angle = 0;
	user.wd = -30;
	user.shot_power = 0;
}
//-----------------------------------------------------------------------------
// INIT_USER_SCORES FUNCTION: set both user scores to 0
//-----------------------------------------------------------------------------
void    init_user_scores() {
	user.p1_score = 0;
	user.p2_score = 0;
}
//-----------------------------------------------------------------------------
// INIT_GAME FUNCTION: calls all the INIT_FUNCTIONS when game starts
//-----------------------------------------------------------------------------
void    init_game(void) {
    init_environment();
    init_bitmaps();
    init_pool_table();
    init_balls();
    init_user();
    init_user_scores();
}

