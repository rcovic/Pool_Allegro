//-----------------------------------------------------------------------------
// MAIN.C - Initialize game, start tasks and ends game on user input
//-----------------------------------------------------------------------------
#include "init.h"
//-----------------------------------------------------------------------------
// MAIN function
//-----------------------------------------------------------------------------
int main(void){
	int	i = 0;				//ball index

	//initialize game
	init_game();

	//create render task							
	ptask_create_edf(render_task, P, RT_R, DL_R, NOW);
	
	//create 16 ball tasks
	for (i=0; i<N_BALLS; ++i)
		ptask_create_edf(ball_task, P, RT_B, DL_B, NOW);

	//create user task
	ptask_create_edf(user_task, P, RT_U, DL_U, NOW);

	//wait for user input to end
	while (!key[KEY_ESC]){};
						
	//close allegro environment
	allegro_exit();

	return 0;
}
