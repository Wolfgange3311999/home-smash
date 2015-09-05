#pragma once
#include "InvBlock.h"

class Furnace
{
public:
	InvBlock sourceBlock, fuelBlock, resultBlock;
	int fuel;

	Furnace() : sourceBlock(), fuelBlock(), resultBlock(), fuel(0) { }

	Furnace(InvBlock source, InvBlock fuel)
	: sourceBlock(source), fuelBlock(fuel), resultBlock(), fuel(0) { }
};