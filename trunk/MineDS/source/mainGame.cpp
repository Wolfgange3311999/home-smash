#include <nds.h>
#include <stdio.h>
#include "world.h"
#include "worldgen.h"
#include "message.h"
#include "mining.h"
#include "blockID.h"
#include "blocks.h"
#include "worldRender.h"
#include "graphics/graphics.h"
#include "graphics/subBgHandler.h"
#include "mobs/mobHandler.h"
#include "deathScreen.h"

#include "daynight.h"
bool up;
Graphic graphics[10];
void mobHandlerUpdate(worldObject* world);
worldObject* mainGame(int Mode,worldObject* CurrentWorld)
{
	mobsReset();
	consoleClear();
	lcdMainOnBottom();
	iprintf("\x1b[7;0HGenerating world...\n");
	touchPosition touch;
	int i,j;
	if (Mode==0) // Generate new world!
	{
		if (CurrentWorld==NULL) CurrentWorld = (worldObject *) calloc(1, sizeof(worldObject));
		iprintf("\x1b[8;0HC");
		if (CurrentWorld==NULL)
		{
			return NULL;
		}
		generateWorld(CurrentWorld);
		iprintf("\x1b[8;0HA");
	}	
	CurrentWorld->CamX=0;
	CurrentWorld->CamY=0;
	CurrentWorld->timeInWorld=0;
	//loadGraphicSub(&graphics[9],2,8);
	consoleClear();	
	while(1){
		scanKeys();
		touchRead(&touch);
		miningUpdate(CurrentWorld,CurrentWorld->CamX,CurrentWorld->CamY,touch,keysDown());
		mobHandlerUpdate(CurrentWorld);
		update_message();
		if (keysDown() & KEY_START)
			break;
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		graphicFrame();
		if (deathScreenUpdate())
			break;
		timeUpdate(CurrentWorld);
		worldRender_Render(CurrentWorld,CurrentWorld->CamX,CurrentWorld->CamY);
	}
	return CurrentWorld;
}
