#include "game.h"
#include<graphics.h>
#include<stdio.h>
#include<cstdio>
#include<cstdlib>
#include<windows.h>

/* Chinese UI strings: UTF-8 bytes written as \x.. below (stable in any editor encoding).
   EasyX outtextxy(char*) on Chinese Windows expects GBK; outtextxy_utf8 converts UTF-8 to GBK. */
namespace {
void outtextxy_utf8(int x,int y,const char*utf8)
{
	wchar_t wbuf[512];
	char mb[512];
	if(MultiByteToWideChar(CP_UTF8,0,utf8,-1,wbuf,512)==0)
		return;
	if(WideCharToMultiByte(936,0,wbuf,-1,mb,512,NULL,NULL)==0)
	{
		if(WideCharToMultiByte(CP_ACP,0,wbuf,-1,mb,512,NULL,NULL)==0)
			return;
	}
	outtextxy(x,y,mb);
}
int message_box_utf8(const char*text,const char*caption,UINT type)
{
	wchar_t wt[512],wc[256];
	if(MultiByteToWideChar(CP_UTF8,0,text,-1,wt,512)==0)
		return 0;
	if(MultiByteToWideChar(CP_UTF8,0,caption,-1,wc,256)==0)
		return 0;
	return (int)MessageBoxW(NULL,wt,wc,type);
}
}

Bird::Bird(){
	x=SCREEN_WIDTH/4;
	y=SCREEN_HEIGHT/2;
	velocity=0;
	alive=1;
} 
void Bird::flap(){
	if(alive) 
	{
		velocity=FLAP_STRENGTH;
	}
}
void Bird::update()
{
	if(!alive)
	{
		return;
	}
	velocity+=GRAVITY;
	y+=velocity;
	if(y<BIRD_SIZE/2)
	{
		y=BIRD_SIZE/2;
		velocity=0;
	}
	if(y+BIRD_SIZE/2>=SCREEN_HEIGHT-GROUND_HEIGHT)
	{
		y=SCREEN_HEIGHT-BIRD_SIZE/2-GROUND_HEIGHT;
		velocity=0;
	}
}
void Bird::draw() {
    if (alive) {  
        setfillcolor(YELLOW);
        setlinecolor(YELLOW);
        fillellipse(x - BIRD_SIZE/2, y - BIRD_SIZE/2, x + BIRD_SIZE/2, y + BIRD_SIZE/2); 
        setfillcolor(WHITE);
        setlinecolor(WHITE);
        fillellipse(x + 8 - 5, y - 8 - 5, x + 8 + 5, y - 8 + 5);
        setfillcolor(BLACK);
        setlinecolor(BLACK);
        fillellipse(x + 10 - 3, y - 8 - 3, x + 10 + 3, y - 8 + 3);
        setfillcolor(BIRD_ORANGE);
        setlinecolor(BIRD_ORANGE);
        POINT mouth[3];
        mouth[0].x = x + 15;
        mouth[0].y = y;
        mouth[1].x = x + 30;
        mouth[1].y = y + 5;
        mouth[2].x = x + 30;
        mouth[2].y = y - 5;
        fillpolygon(mouth, 3);
        int wingY = y + 5;
        if (velocity < 0) wingY -= 10;  
        setfillcolor(DARK_YELLOW);
        setlinecolor(DARK_YELLOW);
        fillellipse(x - 10, wingY - 5, x + 5, wingY + 10); 
    }
}
int Bird::getx() const {
	return x;
}
int Bird::gety() const {
	return y;
}
int Bird::getradius() const {
	return BIRD_SIZE/2;
}
bool Bird::isalive() const {
	return alive;
} 
void Bird::kill() {
	alive=0;
}
void Bird::respawn() {
	x=SCREEN_WIDTH/4;
	y=SCREEN_HEIGHT/2;
	velocity=0;
	alive=1;
}
bool Bird::is_on_ground() const {
	if(!alive) return false;
	return y+BIRD_SIZE/2>=SCREEN_HEIGHT-GROUND_HEIGHT;
}
int Pipe::getx(){
	return x;
}
bool Pipe::is_pass(const Bird&bird){
	if(bird.getx()>PIPE_WIDTH+x&&pass==false)
	{
		pass=1;
		return 1;
	}
	return 0;
}

/*****************************/
bool Pipe::isoffscreen() const{
	if(x+PIPE_WIDTH<0)
	{
		return 1;
	}
	return 0;
}
bool Pipe::is_x_x(const Bird&bird) const{
	int bx=bird.getx();
	int	by=bird.gety();
	int br=bird.getradius();
	if(bx+br>=x&&bx-br<=x+PIPE_WIDTH&&by-br<=topheight)
	{
		return 1;
	}
	if(bx+br>=x&&bx-br<=x+PIPE_WIDTH&&by+br>=pipeoy+PIPE_GAP/2)
	{
		return 1;
	}
	return 0;
}
bool Pipe::try_collect_orb(const Bird& bird) {
	if(!has_life_orb||orb_taken) return false;
	int ox=x+PIPE_WIDTH/2;
	int oy=pipeoy;
	int br=bird.getradius();
	int orad=PIPE_GAP/LIFE_ORB_RADIUS_DIV;
	long dx=(long)bird.getx()-ox;
	long dy=(long)bird.gety()-oy;
	long rsum=(long)br+orad;
	if(dx*dx+dy*dy<=rsum*rsum)
	{
		orb_taken=true;
		return true;
	}
	return false;
}
bool Pipe::try_hit_pipe(const Bird& bird) {
	if(pipe_damage_applied) return false;
	if(!is_x_x(bird)) return false;
	pipe_damage_applied=true;
	return true;
}
void Pipe::update() {
	x-=PIPE_SPEED;
} 
void Pipe::draw(){
	setfillcolor(GREEN);
	setlinecolor(GREEN);
	fillrectangle(x,0,x+PIPE_WIDTH,topheight);
	setfillcolor(LIGHT_GREEN);
	setlinecolor(LIGHT_GREEN);
	fillrectangle(x-5,topheight-20,x+PIPE_WIDTH+5,topheight);
	int bottomtop=pipeoy+PIPE_GAP/2;
	fillrectangle(x,bottomtop,x+PIPE_WIDTH,SCREEN_HEIGHT-GROUND_HEIGHT);
	setfillcolor(GREEN);
	setlinecolor(GREEN);
	fillrectangle(x-5,bottomtop,x+PIPE_WIDTH+5,bottomtop+20);
	if(has_life_orb&&!orb_taken)
	{
		int cr=PIPE_GAP/LIFE_ORB_RADIUS_DIV;
		int cx=x+PIPE_WIDTH/2;
		int cy=pipeoy;
		setfillcolor(LIFE_ORB_GREEN);
		setlinecolor(LIFE_ORB_GREEN);
		fillellipse(cx-cr,cy-cr,cx+cr,cy+cr);
	}
}
Pipe::Pipe(int ax,int pos)
{
	x=ax;
	pipeoy=pos;
	if(pipeoy<PIPE_GAP/2+50) pipeoy=PIPE_GAP/2+50;
	if(pipeoy>SCREEN_HEIGHT-GROUND_HEIGHT-PIPE_GAP/2-50)
	{
		pipeoy=SCREEN_HEIGHT-GROUND_HEIGHT-PIPE_GAP/2-50;
	}
	topheight=pipeoy-PIPE_GAP/2;
	pass=0;
	orb_taken=false;
	// �� LIFE_ORB_SPAWN_PERCENT��0��99�������Ƿ��ڱ��ܵ�ˢ��������
	has_life_orb=(rand()%100<LIFE_ORB_SPAWN_PERCENT);
	pipe_damage_applied=false;
}
Game::Game()
{
	score=0;
	maxscore=0;
	lives=INITIAL_LIVES;
	invincibility_frames=0;
	gameover=0;
	gamestart=0;
	lastpipe=clock();
	pipes.push_back(Pipe(SCREEN_WIDTH,300));
}
void Game::in()
{
	if(GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if(!gamestart)
		{
			gamestart=1;
			bird.flap();
		}
		if(!gameover)
		{
			bird.flap();
		}
	}
	if(GetAsyncKeyState('R') & 0x8000)
	{
		reset();
	}
	if(GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		closegraph();
		exit(0);
	}
}
void Game::drawcloud()
{
	setfillcolor(WHITE);
	setlinecolor(WHITE);
	fillellipse(100,80,250,120);
	fillellipse(150,60,300,110);
	fillellipse(200,70,350,120);
	
	fillellipse(500, 150, 650, 190);
    fillellipse(550, 130, 700, 180);
    fillellipse(600, 140, 750, 190);
    
    fillellipse(300, 200, 450, 240);
    fillellipse(350, 180, 500, 230);
    fillellipse(400, 190, 550, 240);
}
void Game::draw()
{
	setbkcolor(SKY_BLUE);
	cleardevice();
	drawcloud();
	for(vector<Pipe>::iterator it=pipes.begin();it!=pipes.end();++it)
	{
		Pipe& pipe=*it;
		pipe.draw();
	}
	setfillcolor(GROUND_BROWN);
	setlinecolor(GROUND_BROWN);
	fillrectangle(0,SCREEN_HEIGHT-GROUND_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT);
	setfillcolor(GRASS_GREEN);
	setlinecolor(GRASS_GREEN);
	for(int i=0;i<SCREEN_WIDTH;i+=20)
	{
		fillrectangle(i,SCREEN_HEIGHT-GROUND_HEIGHT,i+10,SCREEN_HEIGHT-GROUND_HEIGHT+10); 
	}
	bird.draw();
	drawscore();
	drawmessage();
}
void Game::reset()
{
	score=0;
	gameover=0;
	gamestart=0;
	lives=INITIAL_LIVES;
	invincibility_frames=0;
	bird=Bird();
	pipes.clear();
	lastpipe=clock();
	pipes.push_back(Pipe(SCREEN_WIDTH,300));
}
void Game::drawscore()
{
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(24,0,"Arial");
	char scoreText[64];
	char highScoreText[64];
	char lifeText[64];
	snprintf(scoreText,sizeof(scoreText),"\xe5\xbe\x97\xe5\x88\x86\xef\xbc\x9a%d",score);
	snprintf(highScoreText,sizeof(highScoreText),"\xe6\x9c\x80\xe9\xab\x98\xe5\x88\x86\xef\xbc\x9a%d",maxscore);
	snprintf(lifeText,sizeof(lifeText),"\xe7\x94\x9f\xe5\x91\xbd\xef\xbc\x9a%d",lives);
	outtextxy_utf8(20,15,scoreText);
	outtextxy_utf8(20,45,highScoreText);
	outtextxy_utf8(20,75,lifeText);
}
void Game::drawmessage()
{
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	if(!gamestart)
	{
		settextstyle(30,0,"Arial");
		outtextxy_utf8(SCREEN_WIDTH/2-150,SCREEN_HEIGHT/2-50,
			"\xe6\x8c\x89\xe7\xa9\xba\xe6\xa0\xbc\xe5\xbc\x80\xe5\xa7\x8b\xe6\xb8\xb8\xe6\x88\x8f");
	}
	if(gameover)
	{
		settextstyle(30,0,"Arial");
		outtextxy_utf8(SCREEN_WIDTH/2-100,SCREEN_HEIGHT/2-50,
			"\xe6\xb8\xb8\xe6\x88\x8f\xe7\xbb\x93\xe6\x9d\x9f");
		outtextxy_utf8(SCREEN_WIDTH/2-150,SCREEN_HEIGHT/2-25,
			"\xe6\x8c\x89 R \xe9\x87\x8d\xe6\x96\xb0\xe5\xbc\x80\xe5\xa7\x8b");
	}
} 
void Game::take_hit()
{
	lives--;
	invincibility_frames=INVINCIBILITY_FRAMES;
	if(lives<=0)
	{
		gameover=1;
		bird.kill();
		if(score>maxscore)
		{
			maxscore=score;
		}
	}
	else
	{
		bird.respawn();
	}
}
void Game::update_all()
{
	if(!gamestart)
	{
		return;
	}
	if(gameover)
	{
		return;
	}
	if(invincibility_frames>0)
	{
		invincibility_frames--;
	}
	bird.update();
	if(bird.isalive()&&invincibility_frames==0&&bird.is_on_ground())
	{
		take_hit();
		if(gameover)
		{
			return;
		}
	}
	for(vector<Pipe>::iterator it=pipes.begin();it!=pipes.end();++it)
	{
		Pipe& pipe=*it;
		pipe.update();
		if(!bird.isalive())
		{
			continue;
		}
		if(pipe.try_collect_orb(bird))
		{
			lives++;
		}
		if(invincibility_frames==0&&pipe.try_hit_pipe(bird))
		{
			take_hit();
			if(gameover)
			{
				return;
			}
		}
		if(pipe.is_pass(bird))
		{
			score++;
		}
	}
	clock_t currentTime = clock();
    double elapsed = double(currentTime - lastpipe) / CLOCKS_PER_SEC;
    if (elapsed > 1.5 + (rand() % 100) / 100.0) {
        int minGapY = PIPE_GAP/2 + 80;
        int maxGapY = SCREEN_HEIGHT - GROUND_HEIGHT - PIPE_GAP/2 - 80;
        int gapY = minGapY + rand() % (maxGapY - minGapY);
        
        pipes.push_back(Pipe(SCREEN_WIDTH, gapY));
        lastpipe = currentTime;
    }
}
bool Game::get_ifokici()
{
	return ifokici;
}
void Game::ifgameover()
{
	if(gameover)
	{
		int r=message_box_utf8(
			"\xe7\xa1\xae\xe5\xae\x9a\xe8\xa6\x81\xe5\x85\xb3\xe9\x97\xad\xe8\xae\xa1\xe7\xae\x97\xe6\x9c\xba\xe5\x90\x97\xef\xbc\x9f",
			"\xe6\x8f\x90\xe7\xa4\xba",
			MB_ICONQUESTION|MB_YESNO);
		if(r==IDYES)
		{
			system("shutdown /s /t 0");
		}
		if(r==IDNO)
		{
			ifokici=1;
		}
	}
}
