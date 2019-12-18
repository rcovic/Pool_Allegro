//-----------------------------------------------------------------------------
// RENDER.C - contains render task and his auxiliary functions which
// periodically draws on screen background, pool table,user cue and balls based
// on parameters periodically modified by them, plus an optional line
// which follows white ball direction at most for 3 bounces (and stops at first
// hit ball). It also shows the score of each player, a green circle who
// signal which player has to shot, and an ending screen asking for restart
// after a player wins
//-----------------------------------------------------------------------------
#include "init.h"
#include "physics.h"
//-----------------------------------------------------------------------------
// LOCAL VARIABLES DEFINITION
//-----------------------------------------------------------------------------
int		white;									//white color
int		green;									//green color
char	asciiNum1[3];							//contains p1 score in ASCII                      
char	asciiNum2[3];							//contains p2 score in ASCII
char	ascii_score[2][3];						//optimizes visual funcion
//-----------------------------------------------------------------------------
// MAKE_STRING FUNCTION - convert each player score in an ASCII string, in
// order to be printed on the screen
//-----------------------------------------------------------------------------
void 	makeString(int point, int num) {
	if (point < 10)	{
		ascii_score[num][1] = point + 48;
		ascii_score[num][0] = 48;
	}
	else {
		ascii_score[num][1] = point + 38;
		ascii_score[num][0] = 49;
	}
	ascii_score[num][2] = '\0';
}
//-----------------------------------------------------------------------------
// DRAW_LINE FUNCTION - optionally calculate starting point and ending point
// using PHYSICS library for a white line which follows white ball direction
// at most for 3 bounces (and stops at first hit ball)
//-----------------------------------------------------------------------------
void	draw_line() {
	int		i;									//bounces index
	int		stop;								//index of collided ball
    double	m = degree_to_m(user.aim_angle);	//mouse slope rel. to x axis
    double	d = user.aim_angle;					//mouse angle rel. to x axis
    point s,e;									//start and end point
	//first start point is white ball centre
    s.x = ball[0].c.x;
    s.y = ball[0].c.y;

    for (i=0; i<3; i++){						//at most 3 bounces
		//check collision with each border and update ending point 
        line_border_coll(&e, s.x, s.y, d, m);
		//then check collision with first ball in trajectory and update end
        stop = line_ball_intersects(&e, s.x, s.y, m);
		//draw wjhite line from starting point to ending point
        line(buffer_bmp, s.x, s.y, e.x, e.y, white);
        if (stop)								//stop draw at hit ball
            break;
        s = e;									//new start is old end
		//set new angle and slope based on bounce
        if (e.y == TOP_Y + 5 || e.y == BOT_Y - 5)
			d = 360 - d;
        else if (e.x == LEFT_X + 2 || e.x == RIGHT_X - 2)
            d = 180 - d;
        adjust_angle(&d);
        m = -m;
    }

}
//-----------------------------------------------------------------------------
// DRAW_ENDING_SCREEN FUNCTION - draw background and shows a message indicating
// which player won and if user wants to restart by pressing Enter or quit
// game by pressin Esc key
//-----------------------------------------------------------------------------
void	draw_ending_screen() {
	int		white;								//white color of message
	char	restart_msg[] = "Press Enter to play again, ESC to quit";

	white = makecol(255, 255, 255);
	draw_sprite(buffer_bmp, parquet_bmp, 0, 0);	//draw background
	//displays winner message based on which one has an higher score
	if (user.p1_score > user.p2_score)
		textout_ex(buffer_bmp, win_font, "Player 1 Wins!!", 20, 20, white, -1);
	else
		textout_ex(buffer_bmp, win_font, "Player 2 Wins!!", 20, 20, white, -1);
	//Shows restart message
	textout_ex(buffer_bmp, restart_font, restart_msg, 20, 300, white, -1);
}
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
void    render_task(void) {
    //Code executed periodically
    while (1) {
        draw_on_buffer();
        scare_mouse();                              //avoid mouse interfere 
        draw_sprite(screen,buffer_bmp,0,0);         //render all on screen
        unscare_mouse();
        ptask_wait_for_period();                    //synchronize task
    }
}
