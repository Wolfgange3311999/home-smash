#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "hurt.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "pigMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "../message.h"
#include "../colision.h"
bool scared=false; //Whether pig is scared and will run away from the player
int scaredtimer; //Limits how long the pig is scared
bool dir; //Direction of pig movement
int mov; //Loop counter for pig movement 
Graphic pigMobGraphic[2];

pigMob::pigMob()
{
	x=0;
	y=0;
	vy=0;
	vx=0;
	ping=0;
	alive=false;
	onground=false;
	health=10;
	mobtype=0;
	animationclearframes=0;
	notarget=0;
}
pigMob::pigMob(int a,int b)
{	
	jump=0;
	gravity=3;
	gravityValue=3;
	sx=6;
	sy=32;
	x=a;
	y=b;
	vy=0;
	vx=0;
	alive=false;
	onground=false;
	facing=false;
	mobtype=4;
	health=10;
	ping=0;
	animation=0;
	notarget=0;
	timeTillWifiUpdate=rand()%4+4;
}
void pigMob::updateMob(worldObject* world)
{

	if (rand()%2 == 1 && mov == 1)
		dir = true;
	else if (mov == 1)
		dir = false;

	if (animation==0) showGraphic(&pigMobGraphic[0],x-world->CamX - (facing ? 10:0),y-world->CamY,facing ? false:true);
	else if (animation==1) 	showGraphic(&pigMobGraphic[1],x-world->CamX - (facing ? 10:0),y-world->CamY,facing  ? false:true);

	if (host==true)
	{
		target = mobHandlerFindMob(128,2,x,y);

		if (target->x < x && target->mobtype==2 && scared) facing = true;
		else if (target->mobtype==2 && scared) facing = false;
		jump++;
		if (colisions[0]==false) y+=vy;
		else vy=0;

		if (scaredtimer > 125)
		{
			scared = false;
			scaredtimer = 0;
		}

		if (target->mobtype!=2)
		{
			scared = false;
			notarget++;
			jump=0;
		}

		else if (!scared)
		{
			if (rand()%50 == 17 || mov < 60) //2% chance that the pig will move from 60 pixels. The 26 can be any number between 0 and 99.
			{
				if (dir)
					facing = true;
				else
					facing = false;
				if (!colisions[1] && !dir && !colisions[3] && jump>1)
				{
					x += dir ? 1 : -1;
					jump=0;
				}
				if (!colisions[2] && dir && !colisions[3] && jump>1)
				{
					x += dir ? 1 : -1;
					jump=0;
				}
				else if ((colisions[1] || colisions[2]) && colisions[0])
				{
					vy=-2;
					y+=vy;
				}
				if (mov > 59)
					mov = 0;
				mov++;
			}
		}

		else if (!colisions[1] && !facing && !colisions[3] && jump>1)
		{
			x+= facing ? 1 : -1;
			scaredtimer++;
			jump=0;
		}
		else if (!colisions[2] && facing && !colisions[3] && jump>1)
		{
			x+= facing ? 1 : -1;
			scaredtimer++;
			jump=0;
		}
		else if ((colisions[1] || colisions[2]) && colisions[0])
		{
			scaredtimer++;
			vy=-2;
			y+=vy;
		}
		if (mobtype==2) notarget=0;
		ping=0;
		if (health<=0)
		{
			killMob();
		}
		if (notarget > 1800) killMob();
		if (animationclearframes==0) animation=0;
		else animationclearframes--;
		//iprintf("colisions = %d\n",colisions[0]);
	}
}
void pigMob::sendWifiUpdate()
{
}
void pigMob::saveToFile(FILE* pFile)
{
}
void pigMob::loadFromFile(FILE* pFile)
{
}
bool canPigMobSpawnHere(worldObject* world,int x,int y)
{
	y++;
	if (!isBlockWalkThrough(world->blocks[x][y+1]) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y]!=CACTUS && world->blocks[x][y+1]!=CACTUS) return true;
	return false;
}
void pigMobInit()
{
	loadGraphic(&pigMobGraphic[0],true,5);
	loadGraphic(&pigMobGraphic[1],true,6);
}
void pigMob::hurt(int amount,int type)
{
	
	if (animation==1)
		return;
	if (type!=VOID_HURT)
		vy-=2;
	y+=vy;
		health-=amount;
	if (type == PLAYER_HURT)
		scared = true;
	animation=1;
	animationclearframes=20;
}
bool pigMob::isMyPlayer()
{
	return false;
}
