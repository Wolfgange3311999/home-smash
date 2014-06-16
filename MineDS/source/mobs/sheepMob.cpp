#include <stdio.h>
#include "../world.h"
#include "../blocks.h"
#include "hurt.h"
#include "../graphics/graphics.h"
#include "../debugflag.h"
#include "sheepMob.h"
#include "mobHandler.h"
#include "../blockID.h"
#include "../general.h"
#include "../sounds.h"
#include "../collision.h"
#include "../inventory.h"
#include "../general.h"
Graphic sheepMobGraphic[2];

sheepMob::sheepMob()
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
	smallmob=true;
}
sheepMob::sheepMob(int a,int b)
{	
	jump=0;
	gravity=3;
	gravityValue=3;
	sx=10;
	sy=16;
	x=a;
	y=b;
	vy=0;
	vx=0;
	alive=false;
	onground=false;
	facing=false;
	mobtype=5;
	health=10;
	ping=0;
	animation=0;
	notarget=0;
	timeTillWifiUpdate=rand()%4+4;
	smallmob=true;
}
void sheepMob::updateMob(worldObject* world)
{
	if (rand()%2 == 1 && mov == 1)
		dir = true;
	else if (mov == 1)
		dir = false;

	if (animation==0) showGraphic(&sheepMobGraphic[0],x-world->CamX,y-world->CamY,facing ? true:false);
	else if (animation==1) 	showGraphic(&sheepMobGraphic[1],x-world->CamX,y-world->CamY,facing  ? true:false);

	if (host==true)
	{
		target = mobHandlerFindMob(256,2,x,y);

		if (target->x < x && target->mobtype==2 && scared) facing = false; //Face away from the player when scared
		else if (target->mobtype==2 && scared) facing = true;
		jump++;
		if (collisions[0]==false) y+=vy;
		else vy=0;
		if (collisions[3])
		{
			vy=0;
			y+=3;
		}
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
			if (rand()%50 == 17 || mov < 60) //2% chance that the sheep will move from 60 pixels. The 26 can be any number between 0 and 99.
			{
				if (dir)
					facing = true;
				else
					facing = false;
				if (!collisions[1] && !dir && jump>1)
				{
					x += dir ? -1 : 1;
					jump=0;
				}
				if (!collisions[2] && dir && jump>1)
				{
					x += dir ? -1 : 1;
					jump=0;
				}
				else if ((collisions[1] || collisions[2]) && collisions[0]  && !collisions[3])
				{
					vy=-2;
					y+=vy;
				}
				if (mov > 59)
					mov = 0;
				mov++;
			}
		}

		else if (!collisions[1] && !facing && jump)
		{
			x+= facing ? -1 : 1;
			scaredtimer++;
			jump=0;
		}
		else if (!collisions[2] && facing && jump)
		{
			x+= facing ? -1 : 1;
			scaredtimer++;
			jump=0;
		}
		else if ((collisions[1] || collisions[2]) && collisions[0] && !collisions[3])
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
		//if (notarget > 1800) killMob();
		if (animationclearframes==0) animation=0;
		else animationclearframes--;
	}
}
void sheepMob::sendWifiUpdate()
{
}
void sheepMob::saveToFile(FILE* pFile)
{
}
void sheepMob::loadFromFile(FILE* pFile)
{
}
bool canSheepMobSpawnHere(worldObject* world,int x,int y)
{
	y++;
	if ((world->blocks[x][y+1]==GRASS || world->blocks[x][y+1]==SNOW_GRASS) && isBlockWalkThrough(world->blocks[x][y]) && world->blocks[x][y]!=CACTUS && world->blocks[x][y+1]!=CACTUS) return true;
	return false;
}
void sheepMobInit()
{
	loadGraphic(&sheepMobGraphic[0],true,14,16,16);
	loadGraphic(&sheepMobGraphic[1],true,15,16,16);
}
void sheepMob::hurt(int amount,int type)
{
	
	if (animation==1)
		return;
	if (type!=VOID_HURT)
		vy-=2;
	y+=vy;
	health-=amount;
	playSound(SHEEP_H);
	if (type == PLAYER_HURT)
		scared = true;
	if (health<=0 && type == PLAYER_HURT)
	{
		addInventory(WHITE_WOOL,rand()%4);
	}
	animation=1;
	animationclearframes=20;
}
bool sheepMob::isMyPlayer()
{
	return false;
}
