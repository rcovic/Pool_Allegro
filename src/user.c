//-----------------------------------------------------------------------------
// USER.C - contains definition of USER_TASK and all its auxiliar functions
//-----------------------------------------------------------------------------
#include "init.h"
#include "physics.h"

//-----------------------------------------------------------------------------
// LOCAL VARIABLES DEFINITION
//-----------------------------------------------------------------------------
int mousePos;                   //used to calc mouse position
uint8_t prev_mouse_state;                       //used to check mouse click
uint8_t curr_mouse_state;                       //used to check mouse click
uint8_t prev_key_state;							//used to check key press
uint8_t curr_key_state;							//used to check key press
bool load_flag;                                 //true if cue is loading
int left_balls = N_BALLS - 1;					//used to check remaining balls
//-----------------------------------------------------------------------------
// GET_MOUSE_POSITION FUNCTION - calculate x and y mouse coordinates
//-----------------------------------------------------------------------------
void    get_mouse_position() {
    mousePos = mouse_pos;
	user.m.x = mousePos >> 16;
	user.m.y = mousePos & 0x0000ffff;
}
//-----------------------------------------------------------------------------
// CALCULATE_AIM_ANGLE FUNCTION - calculate aim angle based on mouse position
//-----------------------------------------------------------------------------
double  calculate_aim_angle() {
    point   p3;                                 //stores mouse projection
    double  distance;                           //dist between mouse and cue

    distance = get_dist(&p3, user.m.x, user.m.y, ball[0].c.x, ball[0].c.y);
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
	if (mouse_b & 1)
        curr_mouse_state = 1;
    if (curr_mouse_state && ! prev_mouse_state)
        user.state += 1;
    prev_mouse_state = curr_mouse_state;
    curr_mouse_state = 0;
}
//-----------------------------------------------------------------------------
// CHECK_KEYBOARD FUNCTION - check if user press the A key to enable aim mode
//-----------------------------------------------------------------------------
void	check_keyboard(void) {
	if (key[KEY_A])
        curr_key_state = 1;
    if (curr_key_state && !prev_key_state)
        user.aim_key = !user.aim_key;
    prev_key_state = curr_key_state;
    curr_key_state = 0;
}
//-----------------------------------------------------------------------------
// LOAD_RELEASE_CUE FUNCTION - moves cue back and forth white ball, calculating
// the shot power inversely proportional to the distance 
//-----------------------------------------------------------------------------
void    load_release_cue() {
	if (!load_flag)								//moves back
		user.wd -= SPEED / 4;
	else										//moves forth
		user.wd += SPEED / 4;
    //boundaries to invert movement
	if (user.wd < -160 || user.wd > -20)
		load_flag = !load_flag;
    //calculate shot power
	user.shot_power = ((double) abs(user.wd) / 160.0f) * SPEED;
}
//-----------------------------------------------------------------------------
// RELEASE_CUE FUNCTION - moves cue towards white ball, telling user task
// to move it once the cue reaches the ball 
//-----------------------------------------------------------------------------
void    release_cue() {
	if (user.wd < -15)
		user.wd += SPEED * 2;
	else
		user.state = WAKE_BALL;
}
//-----------------------------------------------------------------------------
// WAKE_WHITE FUNCTION - sets white ball angle and speed and tells user task
// to wait to the next shot 
//-----------------------------------------------------------------------------
void    wake_white() {
	ball[0].angle = user.aim_angle;
	ball[0].speed = user.shot_power;
	ball[0].still = false;
	user.state = WAIT;                         //change user state
}
//-----------------------------------------------------------------------------
// CHECK_Y FUNCTION - called when white ball finished in a hole and sets
// appropriately along y axis in order to avoid compenetration with other ball
// finished in white ball start position. Function called recursively if need
//-----------------------------------------------------------------------------
int		check_y(int y) {
	int	i;										//ball index
	int	newY = y;								//start white y coordinate									

	for (i=0; i<N_BALLS; i++) {
		if (ball[i].c.x > 390 && ball[i].c.x < 430)
			if (ball[i].c.y > (y - 15) && ball[i].c.y < (y + 15))
				//recursive call to use another position if current is busy
        		newY=check_y(y + 15);
  }
	return newY;
}
//-----------------------------------------------------------------------------
// WAIT_USER FUNCTION - if all remainig balls are still, it prepares user for
// the next shot (based on if a user has scored a point)
//-----------------------------------------------------------------------------
void    wait_user() {
    int still_balls_counter = 0;               //balls in movement
    int left_balls_counter = 0;                //remaining balls
    int i;                                     //balls counter

	//count how many ball are not in movement and how many are still on table	
	for (i=0; i<N_BALLS; i++){
		if (ball[i].still)
			still_balls_counter++;
        if (ball[i].alive && i != 0)
            left_balls_counter++;
	}
	//end game if no balls are left (except the white one)
	if (left_balls_counter == 0)
		user.state = END;
	//next player if all balls are still
	else if (still_balls_counter == N_BALLS) {
		//change player if no ball in hole or if white ball is in hole
        if (left_balls_counter == left_balls || ball[0].alive == false)
            user.player = (user.player % 2) + 1;
		//before next turn set appropriately white ball if it is in hole
		if (ball[0].alive == false)
			set_ball_parameters(WHITE_X, check_y(387), &ball[0]);
        left_balls = left_balls_counter;
		init_user();							//reset user state
	}
}
//-----------------------------------------------------------------------------
// CHECK_ENTER FUNCTION - called when user state is in END and check for
// ENTER key press, if so, restart the game
//-----------------------------------------------------------------------------
void	check_enter() {
	if (key[KEY_ENTER])
		init_game(true);
}
//-----------------------------------------------------------------------------
// USER_TASK FUNCTION- implementation of user task who periodically performs
// action based on user status and user input
//-----------------------------------------------------------------------------
void    user_task(void) {
	user.player = 1;							//player 1 starts
    //Code executed periodically
    while(1) {
        switch  (user.state){                   //check on user state
            case AIM:                           //aiming phase                          
				get_mouse_position();
				user.aim_angle = calculate_aim_angle();
				calculate_cue_angle();
				check_mouse_click();
				check_keyboard();
				break;
            case LOAD:                          //loading phase
			    load_release_cue();
				check_mouse_click();
				break;
			case RELEASE:                       //release phase
			    release_cue();
				break;
			case WAKE_BALL:                     //wake white phase
			    wake_white();
				break;
			case WAIT:                          //wait phase
			    wait_user();
			    break;
			case END:							//end phase
				check_enter();
			default:
				break;
		}
        ptask_wait_for_period();                //synchronize task with period
    }
}
