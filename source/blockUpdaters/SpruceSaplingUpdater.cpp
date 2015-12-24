#include "../WorldObject.h"
#include "blockUpdater.h"
#include "../blockID.h"
#include "SpruceSaplingUpdater.h"
#include "OakSaplingUpdater.h"

SpruceSaplingUpdater::SpruceSaplingUpdater()
{
	chance = SAPLING_CHANCE_UPDATE;
}

bool SpruceSaplingUpdater::update(WorldObject &world, int x, int y, bool bg)
{
	saplingUpdate(world, x, y, bg);
	return false;
}

void SpruceSaplingUpdater::chanceUpdate(WorldObject &world, int x, int y, bool bg)
{
	saplingChanceUpdate(world, x, y, bg);
}
