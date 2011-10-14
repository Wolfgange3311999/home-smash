#include <nds.h> //BLAH
#include "../ndsvideo.h"
#include "block.h" //Include the block graphics
#include "../blockID.h"
u16* stonegfx;
void STONE_render(int x,int y){
	if (x>-32 && x<288 && y>-32 && y<224)createsprite32x32(x,y,stonegfx,false,0);	
}
void STONE_setup(){
	stonegfx=oamAllocateGfx(&oamMain,SpriteSize_32x32,SpriteColorFormat_256Color);
	char* blocktiles;
	blocktiles=(char*)&blockTiles;
	blocktiles+=(32*32)*STONE;
	dmaCopy(blocktiles,stonegfx,32*32);
}
