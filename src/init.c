//-----------------------------------------------------------------------------
// INIT.C - provides methods to initialize allegro environment, ptask library
// semaphores, bitmaps and fonts and global variables used for pool table
// balls and user. Initialization is made through the INIT_GAME function
// who is called from the MAIN when game is executed before creating tasks
// and if we press the enter key on the ending screen to reset all
//-----------------------------------------------------------------------------
#include "init.h"

//-----------------------------------------------------------------------------
// BITMAP OBJECTS DEFINITION AND FONT
//-----------------------------------------------------------------------------
BITMAP*     parquet_bmp;        	//bitmap for the background
BITMAP*     pool_table_bmp;     	//bitmap for the pool table
BITMAP*     ball_panel_bmp;     	//bitmap for top and bot ball panel
BITMAP*     ball_bmp[N_BALLS];  	//16 bitmaps for 16 balls
BITMAP*     cue_bmp;            	//bitmap for user cue
BITMAP*     buffer_bmp;         	//buffer bitmap
FONT*		stats_font;				//font for player stats
FONT*		win_font;				//font for winner player message
FONT*		restart_font;			//font for restart message
//-----------------------------------------------------------------------------
// GLOBAL VARIABLES DEFINITION
//-----------------------------------------------------------------------------
point       hole[N_HOLES];       	//contains coordinates of holes
ball_struct ball[N_BALLS];       	//contains parameters of balls
user_struct user;                	//contain user states and cue params
//-----------------------------------------------------------------------------
// SEMAPHORE DEFINITION
//-----------------------------------------------------------------------------
sem_t       mutex;              	//for mutual exclusion
sem_t       ball_sem[N_BALLS];  	//for synchronize ball tasks
//-----------------------------------------------------------------------------
// INIT_ENVIRONMENT FUNCTION: initialize allegro environment and ptask library
//-----------------------------------------------------------------------------
void    init_environment(void)  {
    allegro_init();
    set_color_depth(desktop_color_depth());
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, RES_X, RES_Y, 0, 0);
    install_mouse();
    show_mouse(screen);
    install_keyboard();
    ptask_init(SCHED_OTHER, GLOBAL, NO_PROTOCOL);
}	
//-----------------------------------------------------------------------------
// INIT_SEMAPHORES FUNCTION: initialize mutex and private semaphores
//-----------------------------------------------------------------------------
void	init_semaphores(void){
	int i;								//ball index
	sem_init(&mutex, 0, 1);				//set mutex semaphore

	for (i=0; i<N_BALLS; i++)
		sem_init(&ball_sem[i], 0, 0);	//set private semaphores
}
//-----------------------------------------------------------------------------
// INIT_BITMAPS FUNCTION: load bitmaps from img folder to bitmap objects
// and font from font folder
//-----------------------------------------------------------------------------
void    init_bitmaps_fonts(void)  {
    char    ballFilename[255];      	//stores i-ball filename
    int     i;                      	//ball index

	//bitmaps initialization
    parquet_bmp = load_bitmap("img/parquet.bmp", NULL);
    pool_table_bmp = load_bitmap("img/poolTable.bmp", NULL);
    ball_panel_bmp = load_bitmap("img/statsPanel.bmp",NULL);
    cue_bmp = load_bitmap("img/cue1.bmp", NULL);

    for (i=0; i<N_BALLS; i++) {
        sprintf(ballFilename, "img/ball%d.bmp", i);
        ball_bmp[i] = load_bitmap(ballFilename, NULL);
    }

    buffer_bmp = create_bitmap(RES_X, RES_Y);
	//fonts initalization
	stats_font = load_font("Georgia.pcx",NULL, NULL);
	win_font = load_font("ArialBlack.pcx",NULL, NULL);
    restart_font = load_font("Arial.pcx",NULL, NULL);
}
//-----------------------------------------------------------------------------
// INIT_POOL_TABLE FUNCTION: calculate and stores all hole positins
//-----------------------------------------------------------------------------
void    init_pool_table(void)   {
    int i;                          	//hole index

	for (i=0; i<N_HOLES; ++i) {
		hole[i].x = (HOLE_X + HOLE_DISTANCE * (i % 3));
		hole[i].y = (HOLE_Y + HOLE_DISTANCE * (i / 3));
	}
}
//-----------------------------------------------------------------------------
// SET_BALL_PARAMETERS FUNCTION: called when game starts or when a ball
// is out of the game to reset it on a new position as still
//-----------------------------------------------------------------------------
void    set_ball_parameters(int x, int y, ball_struct* b) {
	int		i;							//ball index

    b->p.x = x;
	b->p.y = y;
	b->c.x = x +15;
	b->c.y = y +15;
	b->d.x = x;
	b->d.y = y;
	b->speed = 0;
	b->alive = true;
	b->still = true;

	for (i=0; i<N_BALLS; i++)
		b->pd[i] = 1000;				//max previous distance
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
    int     i;                      	//ball index
	set_ball_parameters(WHITE_X, WHITE_Y, &ball[0]);
    
    for (i=1; i<N_BALLS; i++) {
        if (count >= threshold) {
	        threshold++;
			count = 0;
			tmp.x += BALLSIZE;
			tmp.y += BALLSIZE/2;
		}
		if (i == 1)
		    set_ball_parameters(BALL1_X, BALL1_Y, &ball[1]);
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

