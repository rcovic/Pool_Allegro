//-----------------------------------------------------------------------------
// USER.C - contains definition of USER_TASK and all its auxiliar functions
//-----------------------------------------------------------------------------
#include "init.h"
#include "physics.h"

//-----------------------------------------------------------------------------
// LOCAL VARIABLES DEFINITION
//-----------------------------------------------------------------------------
int mousePos, xMouse, yMouse;                   //used to calc mouse position
uint8_t prev_mouse_state;                       //used to check mouse click
uint8_t curr_mouse_state;                       //used to check mouse click

bool load_flag;                                 //true if cue is loading

int left_balls = N_BALLS;                       //used to check remaining balls
//-----------------------------------------------------------------------------
// GET_MOUSE_POSITION FUNCTION - calculate x and y mouse coordinates
//-----------------------------------------------------------------------------
void    get_mouse_position() {
    mousePos = mouse_pos;
	xMouse = mousePos >> 16;
	yMouse = mousePos & 0x0000ffff;
}
//-----------------------------------------------------------------------------
// CALCULATE_AIM_ANGLE FUNCTION - calculate aim angle based on mouse position
//-----------------------------------------------------------------------------
double  calculate_aim_angle() {
    point   p3;                                 //stores mouse projection
    double  distance;                           //dist between mouse and cue

    distance = get_distance(&p3, xMouse, yMouse, ball[0].c.x, ball[0].c.y);
    return get_angle(p3, distance);             //return calculated angle
}
//-----------------------------------------------------------------------------
// CALCULATE_CUE_ANGLE FUNCTION - calculate cue angle in range 0-255 and pivot
// in order to rotate cue bitmap along centre of white ball as pivot
//-----------------------------------------------------------------------------
void    calculate_cue_angle() {
    //get cue angle
    user.cue_angle = ((user.aim_angle + 90 ) * 256 / 360);
    //update pivot based on angle
    user.p.x = (ball[0].c.x + cos((user.aim_angle + 270) * PI / 180) * 6);
	user.p.y = (ball[0].c.y + sin((user.aim_angle + 270) * PI / 180) * 6);
}
//-----------------------------------------------------------------------------
// CHECK_MOUSE_CLICK FUNCTION - check if user left click on mouse (toggle mode)
//-----------------------------------------------------------------------------
void    check_mouse_click() {
	if(mouse_b & 1)
        curr_mouse_state = 1;
    if(curr_mouse_state && ! prev_mouse_state)
        user.state += 1;
    prev_mouse_state = curr_mouse_state;
    curr_mouse_state = 0;
}
//-----------------------------------------------------------------------------
// USER_TASK FUNCTION- implementation of user task who periodically performs
// action based on user status and user input
//-----------------------------------------------------------------------------
void user_task(void) {

    //Code executed periodically
    while(1) {
        switch  (user.state){                   //check on user state
            case AIM:                           //aiming phase                          
				get_mouse_position();
				user.aim_angle = calculate_aim_angle();
				calculate_cue_angle();
				check_mouse_click();
				break;
			default:
				break;
		}
        ptask_wait_for_period();                //synchronize task with period
    }
}
