#include <vector>
#include "../general.h"
#include "graphics.h"
#include "particleHandler.h"

std::vector<Particle> particles;

Graphic* waterGraphic;

void initParticles()
{
	waterGraphic = new Graphic;
	loadGraphic(waterGraphic, GRAPHIC_PARTICLE, 1);
}

Graphic* getCloneWaterGraphic()
{
	Graphic* g = new Graphic;
	setCloneGraphic(waterGraphic, g);
	return g;
}

Particle::~Particle()
{
	if (KillGraphic)
		unloadGraphic(&g);
}

Particle::Particle(float x, float y, float vx, float vy, float ax
				   , float ay, int LiveTime, Graphic g, bool KillGraphic)
: x(FixedPoint(true, (int) (x * FixedPoint::SCALER))), y(FixedPoint(true, (int) (y * FixedPoint::SCALER)))
, vx(FixedPoint(true, (int) (vx * FixedPoint::SCALER))), vy(FixedPoint(true, (int) (vy * FixedPoint::SCALER)))
, ax(FixedPoint(true, (int) (ax * FixedPoint::SCALER))), ay(FixedPoint(true, (int) (ay * FixedPoint::SCALER)))
, LiveTime(LiveTime), KillGraphic(KillGraphic), g(g) { }

Particle::Particle(int x, int y, int vx, int vy, int ax, int ay, int LiveTime, Graphic g, bool KillGraphic)
: Particle(FixedPoint(x), FixedPoint(y), FixedPoint(vx), FixedPoint(vy), FixedPoint(ax), FixedPoint(ay), LiveTime, g, KillGraphic) { }

bool Particle::UpdatePhys()
{
	x += vx;
	y += vy;
	vx += ax;
	vy += ay;
	return (bool)--LiveTime;
}

void Particle::Draw(WorldObject& world)
{
	showGraphic(&g, x - world.camX, y - world.camY);
}

void addParticle(Particle p)
{
	particles.push_back(p);
}

void updateParticles(WorldObject& world)
{
	std::vector<Particle>::iterator it;
	for (it = particles.begin(); it != particles.end(); ++it)
	{
		bool ShouldDelete = (*it).UpdatePhys();
		(*it).Draw(world);
		if (!ShouldDelete)
		{
			--it;
			particles.erase(it + 1);
		}
	}
}

void createSplash(const FixedPoint &px, const FixedPoint &py, const FixedPoint &vx, const FixedPoint &vy)
{
	FixedPoint initVY = std::min((FixedPoint) - 3, -vy / 2);
	addParticle(Particle(px + FixedPoint(2), py, vx * FixedPoint(true, 8) + FixedPoint(true, 100), initVY, FixedPoint(0), FixedPoint(true, 49), 20, *getCloneWaterGraphic(), true));
	addParticle(Particle(px + FixedPoint(0), py, vx * FixedPoint(true, 8) + FixedPoint(true, 50), initVY, FixedPoint(0), FixedPoint(true, 49), 20, *getCloneWaterGraphic(), true));
	addParticle(Particle(px - FixedPoint(4), py, vx * FixedPoint(true, 8) + FixedPoint(true, -50), initVY, FixedPoint(0), FixedPoint(true, 49), 20, *getCloneWaterGraphic(), true));
	addParticle(Particle(px - FixedPoint(6), py, vx * FixedPoint(true, 8) + FixedPoint(true, -100), initVY, FixedPoint(0), FixedPoint(true, 49), 20, *getCloneWaterGraphic(), true));
}