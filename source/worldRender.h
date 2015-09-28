#pragma once
#include "world.h"
#include "block_small.h"
#include "graphics/graphics.h"

void worldRender_Init();
void worldRender_Render(WorldObject &world);
void worldRender_RenderWater(WorldObject &world);
void Calculate_Brightness(WorldObject &world);
extern "C"
{
void brightnessSpread(WorldObject *world, int x, int y, int brightness);
void sunSpread(WorldObject *world, int x, int y, int brightness);
}
void updateBrightnessAround(WorldObject &world, int x, int y);
void setSun(int brightness);
int getBrightness(WorldObject &world, int x, int y);
void clearUnusedBlockSprites();

class BlockSpriteContainer
{
public:
	Graphic sprite;
	int blockID;
	bool hasBeenRendered;
	bool loaded;
	void draw(int x, int y);
	BlockSpriteContainer(int blockID, int paletteID):sprite(),blockID(blockID),hasBeenRendered(false)
	{
		loaded = loadGraphic(&sprite, GRAPHIC_BLOCK, blockID, 16, 32, paletteID);
	}
	~BlockSpriteContainer()
	{
		unloadGraphic(&sprite);
	}
};