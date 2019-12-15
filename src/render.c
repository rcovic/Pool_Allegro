//-----------------------------------------------------------------------------
// RENDER.C - contains render task 
//-----------------------------------------------------------------------------
#include "init.h"

//-----------------------------------------------------------------------------
// DRAW_ON_BUFFER FUNCTION: draws all the bitmaps first on a buffer bitmap
// which is then drawn on screen
//-----------------------------------------------------------------------------
void    draw_on_buffer() {
    int i = 0;                                      //ball index

    draw_sprite(buffer_bmp, parquet_bmp, 0, 0);
    draw_sprite(buffer_bmp, pool_table_bmp, TABLE_X, TABLE_Y);
    draw_sprite(buffer_bmp, ball_panel_bmp, PANEL1_X, PANEL1_Y);
    //draw the bottom ball panel rotating the bitmap at 180 degrees
    rotate_sprite(buffer_bmp, ball_panel_bmp, PANEL2_X, PANEL2_Y, itofix(128));
     
    for (i=0; i<N_BALLS; ++i)                       //draw the 16 balls
        draw_sprite(buffer_bmp, ball_bmp[i], ball[i].p.x, ball[i].p.y);

    if (user.state < WAKE_BALL)                     //draw cue if user has to shot
          pivot_sprite(buffer_bmp, cue_bmp, user.p.x, user.p.y, 0, user.wd, itofix(user.cue_angle));

}
//-----------------------------------------------------------------------------
// RENDER_TASK FUNCTION- implementation of render task who periodically draws 
// on screen all the bitmaps at the specified positions
//-----------------------------------------------------------------------------
void render_task(void){
    //Code executed periodically
    while(1){
        draw_on_buffer();
        scare_mouse();                      //avoid mouse interfere with render
        draw_sprite(screen,buffer_bmp,0,0); //bitmaps are rendered on screen
        unscare_mouse();
        ptask_wait_for_period();            //synchronize task with period
    }
}

