#include "game.h" 
#include<cstdlib>

int main(){
	initgraph(SCREEN_WIDTH,SCREEN_HEIGHT);
	srand(static_cast<unsigned> (time(NULL)));
	Game play;
	while(1)
	{
		play.in();
		play.update_all();
		play.draw();
		if(play.get_ifokici()==0)
		{
			play.ifgameover();
		}
		Sleep(30);
	}
	closegraph();
	return 0;
} 
