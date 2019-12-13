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
