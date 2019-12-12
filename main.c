//-----------------------------------------------------------------------------
// MAIN.C - Initialize game, start tasks and ends game on user input
//-----------------------------------------------------------------------------
#include "init.h"
//-----------------------------------------------------------------------------
// MAIN function
//-----------------------------------------------------------------------------
int main(void){
	//initialize game
	init_game();

	//create render task							
	ptask_create_edf(render_task,P,RT,DL,NOW);

	//wait for user input to end
	while (!key[KEY_ESC]){};
						
	//close allegro environment
	allegro_exit();

	return 0;
}

