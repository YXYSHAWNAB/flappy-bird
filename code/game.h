#ifndef GAME_H
#define GAME_H
#include<conio.h>
#include<graphics.h>
#include<vector>
#include<time.h>
using namespace std;
const int SCREEN_WIDTH=800;
const int SCREEN_HEIGHT=600;
const int GROUND_HEIGHT=100;
const int BIRD_SIZE=30;//???
const int PIPE_WIDTH=80;
const int PIPE_GAP=200;
const int PIPE_SPEED=5;
const int GRAVITY=1;
const int FLAP_STRENGTH=-10;
const int INITIAL_LIVES=3;
const int INVINCIBILITY_FRAMES=90;
const COLORREF LIFE_ORB_GREEN=RGB(50,205,50);
// ?????????????????????????? PIPE_GAP/LIFE_ORB_RADIUS_DIV???��?????
const int LIFE_ORB_SPAWN_PERCENT=22;
const int LIFE_ORB_RADIUS_DIV=6;
const COLORREF SKY_BLUE=RGB(133,206,235);
const COLORREF GROUND_BROWN=RGB(222,184,135);
const COLORREF GRASS_GREEN=RGB(124,252,0);
const COLORREF DARK_YELLOW=RGB(255,200,0);
const COLORREF BIRD_ORANGE=RGB(255,165,0);
const COLORREF LIGHT_GREEN=RGB(144,238,144);
//��????
class Bird
{
private:
	int x,y;
	int velocity;//??????
	bool alive;//?????
	
public:
	Bird();//??????
	void flap();//?????
	void update();//??????
	void draw();
	int getx() const;
	int gety() const;
	int getradius() const;//��???????????????
	bool isalive() const;
	void kill();
	void respawn();
	bool is_on_ground() const;
};
class Pipe{
private:
	int x;
	int pipeoy;
	int topheight;
	bool pass;
	bool orb_taken;
	bool has_life_orb;
	bool pipe_damage_applied;
public:
	Pipe(int ax,int pos);
	int getx() ;
	bool is_pass(const Bird&bird);
	void update();
	void draw();
	bool isoffscreen() const;
	bool is_x_x(const Bird&bird) const;
	bool try_collect_orb(const Bird& bird);
	bool try_hit_pipe(const Bird& bird);
};
class Game{
	private:
		Bird bird;
		vector<Pipe> pipes;
		int score;
		int maxscore;
		int lives;
		int invincibility_frames;
		bool gameover;
		bool gamestart;
		clock_t lastpipe;
		void take_hit();
	public:
		Game();
		void in();
		void update_all();
		void draw();
		void reset();
		void ifgameover();
		bool ifokici=0;
		bool get_ifokici();
	private:
		void drawcloud();
		void drawground();
		void drawscore();
		void drawmessage(); 
};
#endif
