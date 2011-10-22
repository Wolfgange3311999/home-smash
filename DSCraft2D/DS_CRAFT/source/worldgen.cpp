#include "world.h"
#include "blockID.h"
#include <stdio.h>
#include <nds.h>	
#define COAL_START WORLD_HEIGHT/3+1
#define COAL_RARENESS 20
#define IRON_START 24
#define IRON_RARENESS 26
void fixgrass(worldObject* world){
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++)
			if (world->blocks[i][j]==GRASS) world->blocks[i][j]=DIRT; //Change all the GRASS blocks into dirt blocks
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++){
			if(world->blocks[i][j]==DIRT){
				world->blocks[i][j]=GRASS; //Then if a block should be grass make it grass
				j=WORLD_HEIGHT+1;  			   //And Exit this X
			}
			else if (world->blocks[i][j]!=AIR){ //And If we have not encountered dirt and we are at a different block
				j=WORLD_HEIGHT+1;			    //Exit this X
			}
		}
}
void rockwall(worldObject* world,int x,int y){
	int i;
	for (i=y;i<=WORLD_HEIGHT;i++){
		world->blocks[x][i]=STONE;
	}
}
void addore(worldObject* world){
	int y=COAL_START;
	int i;
	int id=COAL_ORE;
	int rareness=COAL_RARENESS;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==COAL_ORE) world->blocks[i][y]=id;
			}		
		}
		if (y%2) rareness--;
		y++;
	}
	y=IRON_START;
	id=IRON_ORE;
	rareness=IRON_RARENESS;
	while(y<WORLD_HEIGHT){
		for (i=0;i<=WORLD_WIDTH;i++){
			if (rand() % rareness ==0){
				//Place a block
				if (world->blocks[i][y]==STONE) world->blocks[i][y]=id;
				else if (world->blocks[i][y]==COAL_ORE) world->blocks[i][y]=id;
			}		
		}
		if (y%2) rareness--;
		y++;
	}
}

void addrock(worldObject* world){
	int i,j;
	for (i=0;i<=WORLD_WIDTH;i++)
		for (j=0;j<=WORLD_HEIGHT;j++){
			if(world->blocks[i][j]==DIRT){
				rockwall(world,i,j+rand()%(3)+3);
				j=WORLD_HEIGHT+1;  			   //And Exit this X
			}
		}
}
void generateWorld(worldObject* world){
	int x=0;
	int y=rand() % WORLD_HEIGHT/4 + WORLD_HEIGHT/8;
	int i;
	for(i=0;i<=WORLD_WIDTH;i++)
		for (x=0;x<=WORLD_HEIGHT;x++) world->blocks[i][x]=AIR;
	for (x=0;x<=WORLD_WIDTH;x++){
		y+=rand() % 5-2;
		if (y<5) y=5;
		if (y>WORLD_HEIGHT/3) y=WORLD_HEIGHT/3;
		int i;
		printf("%d\n",y);
		for (i=y;i<=WORLD_HEIGHT;i++) world->blocks[x][i]=DIRT;
	}

	addrock(world);
	addore(world);
	fixgrass(world);
	for (x=0;x<=WORLD_WIDTH;x++){
		world->blocks[x][WORLD_HEIGHT]=BEDROCK;	
	}
}
void randGenerate(worldObject* world){
	int i,j;
	for(i=0;i<=WORLD_WIDTH;i++){
		for(j=0;j<=WORLD_HEIGHT;j++){
			world->blocks[WORLD_WIDTH][j]=rand()%23;
		}
	}
}
