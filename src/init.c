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
// INIT_GAME FUNCTION: calls all the INIT_FUNCTIONS when game starts
//-----------------------------------------------------------------------------
void init_game(void){
    init_environment();
    init_bitmaps();
    init_pool_table();
    init_balls();
    init_user();
    init_user_scores();
}
