#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
#include "../world.h"
#include "../player.h"
u16* REDSTONE_OREgfx;
void REDSTONE_ORE_render(int x,int y){
	createsprite32x32(x,y,REDSTONE_OREgfx,false,0);	
}
void REDSTONE_ORE_setup(){
	REDSTONE_OREgfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*REDSTONE_ORE;
	dmaCopy(blocktiles,REDSTONE_OREgfx,32*32);
}
void REDSTONE_ORE_colision(playerActor* player,worldObject* world,int bx,int by,int result){
	if (result==0 || result==2){
		player->y=by*32-63; //64 == playerheight
		player->vy=0;
		player->onblock=true;
	}
	if (result==3){//player colides on right
		player->x-=2; //Move him back one so he is not coliding anymore :)
	}
	if (result==1){//colides on left
		player->x+=2;
	}
	if (result==4){
		//colision upwards
		player->vy=0;
		player->y+=2;	
	}
}
