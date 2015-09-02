#include <algorithm>
#include <vector>

#include "blockUpdater.h"
#include "world.h"
#include "blocks.h"
#include "blockID.h"
#include "nifi.h"
#include "communications.h"
#include "inventory.h"
#include "general.h"

//Include blockUpdaters
#include "blockUpdaters/furnace.h"
#include "blockUpdaters/air.h"
#include "blockUpdaters/grass.h"
#include "blockUpdaters/dirt.h"
#include "blockUpdaters/junglegrass.h"
#include "blockUpdaters/mycelium.h"
#include "blockUpdaters/leaf.h"
#include "blockUpdaters/redwoodLeaf.h"
#include "blockUpdaters/jungleLeaf.h"
#include "blockUpdaters/snowTop.h"
#include "blockUpdaters/snowGrass.h"
#include "blockUpdaters/cactus.h"
#include "blockUpdaters/plants.h"
#include "blockUpdaters/oakSapling.h"
#include "blockUpdaters/spruceSapling.h"
#include "blockUpdaters/jungleSapling.h"
#include "blockUpdaters/ladder.h"
#include "blockUpdaters/door.h"
#include "blockUpdaters/water.h"
#include "mobs/mobFunctions.h"
#include "graphics/3DHandler.h"

//Todo: Profile to find out which of std::vector, std::list or std::forward_list is the quickest.
std::vector<BlockUpdateInfo> UpdaterList;
BlockUpdater* blockUpdaters[NUM_UPDATERS];

BlockUpdater::BlockUpdater() { }

bool BlockUpdater::update(WorldObject* world, int x, int y, bool bg)
{
	return false;
}

void BlockUpdater::chanceUpdate(WorldObject* world, int x, int y, bool bg) { }

void proceduralBlockUpdateInit()
{
	blockUpdaters[0] = new FurnaceUpdater;
	blockUpdaters[1] = new AirUpdater;
	blockUpdaters[2] = new GrassUpdater;
	blockUpdaters[3] = new JungleGrassUpdater;
	blockUpdaters[4] = new MyceliumUpdater;
	blockUpdaters[5] = new DirtUpdater;
	blockUpdaters[6] = new LeafUpdater;
	blockUpdaters[7] = new RedwoodLeafUpdater;
	blockUpdaters[8] = new JungleLeafUpdater;
	blockUpdaters[9] = new SnowTopUpdater;
	blockUpdaters[10] = new SnowGrassUpdater;
	blockUpdaters[11] = new CactusUpdater;
	blockUpdaters[12] = new ShrubUpdater;
	blockUpdaters[13] = new TallGrassUpdater;
	blockUpdaters[14] = new RedFlowerUpdater;
	blockUpdaters[15] = new YellowFlowerUpdater;
	blockUpdaters[16] = new RedMushroomUpdater;
	blockUpdaters[17] = new BrownMushroomUpdater;
	blockUpdaters[18] = new OakSaplingUpdater;
	blockUpdaters[19] = new JungleSaplingUpdater;
	blockUpdaters[20] = new SpruceSaplingUpdater;
	blockUpdaters[21] = new LadderUpdater;
	blockUpdaters[22] = new DoorUpdater;
	blockUpdaters[23] = new DoorTopClosedUpdater;
	blockUpdaters[24] = new DoorTopOpenUpdater;
	blockUpdaters[25] = new DoorBottomClosedUpdater;
	blockUpdaters[26] = new DoorBottomOpenUpdater;
	blockUpdaters[27] = new WaterUpdater;
}

int updaterIndex(int blockID, int index = 0)
{
	switch (blockID)
	{
	case FURNACE: return 0;
	case AIR: return 1;
	case GRASS: return 2;
	case GRASS_JUNGLE: return 3;
	case MYCELIUM: return 4;
	case DIRT: return 5;
	case LEAVES_OAK: return 6;
	case LEAVES_SPRUCE: return 7;
	case LEAVES_JUNGLE: return 8;
	case SNOW_TOP: return 9;
	case GRASS_SNOW: return 10;
	case CACTUS: return 11;
	case SHRUB: return 12;
	case TALL_GRASS: return 13;
	case FLOWER_RED: return 14;
	case FLOWER_YELLOW: return 15;
	case MUSHROOM_RED: return 16;
	case MUSHROOM_BROWN: return 17;
	case SAPLING_OAK: return 18;
	case SAPLING_JUNGLE: return 19;
	case SAPLING_SPRUCE: return 20;
	case LADDER: return 21;
	case DOOR_ITEM: return 22;
	case DOOR_CLOSED_TOP: return 23;
	case DOOR_OPEN_TOP: return 24;
	case DOOR_CLOSED_BOTTOM: return 25;
	case DOOR_OPEN_BOTTOM: return 26;
	case WATER: return 27;
	default:
		return -1;
	}
}

static void recursiveUpdate(WorldObject *world, int x, int y, bool bg);

void updateDir(WorldObject *world, int x, int y, bool bg, int dir)
{
	switch (dir)
	{
	case 0:
		recursiveUpdate(world, x + 1, y, bg);
		break;
	case 1:
		recursiveUpdate(world, x, y - 1, bg);
		break;
	case 2:
		recursiveUpdate(world, x - 1, y, bg);
		break;
	case 3:
		recursiveUpdate(world, x, y + 1, bg);
		break;
	default:
		break;
	}
}

void updateBlocksAround(WorldObject *world, int x, int y, bool bg)
{
	recursiveUpdate(world, x + 1, y, bg);
	recursiveUpdate(world, x, y - 1, bg);
	recursiveUpdate(world, x - 1, y, bg);
	recursiveUpdate(world, x, y + 1, bg);
	recursiveUpdate(world, x, y, bg);
}

void recursiveUpdate(WorldObject *world, int x, int y, bool bg)
{
	int &blockXY = bg ? world->bgblocks[x][y] : world->blocks[x][y];
	int index = updaterIndex(blockXY);
	if (index!=-1)
	{
		UpdaterList.push_back(BlockUpdateInfo{x,y,bg,2,false});
		int chance = blockUpdaters[index]->chance;
		if(chance!=NO_CHANCE)
			UpdaterList.push_back(BlockUpdateInfo{x,y,bg,rand() % chance+2,true});
	}
}


void updateAround(WorldObject *world, int x, int y)
{
	updateBlocksAround(world,x,y,false);
	updateBlocksAround(world,x,y,true);
}

void proceduralBlockUpdate(WorldObject* world)
{
	//Go through the UpdaterList, and update if the TTL is 1, call updateAround if the update requests it.
	//If TTL is not 1, decrement the TTL value.
	std::vector<BlockUpdateInfo>::iterator it;
	for (it = UpdaterList.begin(); it != UpdaterList.end();)
	{
		if (it->TimeToLive<2)
		{
			if (it->x > 0 && it->x < WORLD_WIDTH && it->y > 0 && it->y < WORLD_HEIGHT)
			{
				int &blockXY = it->bg ? world->bgblocks[it->x][it->y] : world->blocks[it->x][it->y];
				int index = updaterIndex(blockXY);
				if (index!=-1)
				{
					if (it->chance)
					{
						blockUpdaters[index]->chanceUpdate(world,it->x,it->y,it->bg);
						int chance = blockUpdaters[index]->chance;
						if(chance!=NO_CHANCE)
							UpdaterList.push_back(BlockUpdateInfo{it->x,it->y,it->bg,rand() % chance+2,true});
					}
					else
					{
						if(blockUpdaters[index]->update(world,it->x,it->y,it->bg))
							updateBlocksAround(world,it->x,it->y,it->bg);
					}
				}
			}
			it = UpdaterList.erase(it);
		}
		else
		{
			it->TimeToLive -= 1;
			++it;
		}
	}

}
