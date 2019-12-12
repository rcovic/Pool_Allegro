//-----------------------------------------------------------------------------
// RENDER.C - contains render task 
//-----------------------------------------------------------------------------
#include "init.h"

//-----------------------------------------------------------------------------
// DRAW_ON_BUFFER FUNCTION: draws all the bitmaps first on a buffer bitmap
// which is then drawn on screen
//-----------------------------------------------------------------------------
void draw_on_buffer(){
    draw_sprite(buffer_bmp,parquet_bmp,0,0);
    draw_sprite(buffer_bmp,pool_table_bmp,TABLE_X,TABLE_Y);
    draw_sprite(buffer_bmp,ball_panel_bmp, PANEL1_X, PANEL1_Y);
    //draw the bottom ball panel rotating the bitmap at 180 degrees
    rotate_sprite(buffer_bmp,ball_panel_bmp,PANEL2_X, PANEL2_Y, itofix(128));
}
//-----------------------------------------------------------------------------
// RENDER_TASK FUNCTION- implementation of render task who periodically draws 
// on screen all the bitmaps at the specified positions
//-----------------------------------------------------------------------------
void render_task(void){
    //Code executed periodically
    while(1){
        draw_on_buffer();
        scare_mouse();
        draw_sprite(screen,buffer_bmp,0,0);
        unscare_mouse();
        ptask_wait_for_period();
    }
}

