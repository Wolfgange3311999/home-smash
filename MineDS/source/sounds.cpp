#include <nds.h>
#include <maxmod9.h>    // Maxmod definitions for ARM9
#include "soundbank.h"  // Soundbank definitions
#include "soundbank_bin.h"
#include "sounds.h"
#include "nifi.h"
#include <stdio.h>
#include <dswifi9.h>
#include <string.h>
bool maxModInit=false;
musicStruct music;
void initSounds()
{
  	mmInitDefaultMem( (mm_addr)soundbank_bin );
	int i;
	for (i=0;i<MSL_NSAMPS;i++) mmLoadEffect(i); //loads all sfx
	for (i=0;i<MSL_NSONGS;i++) mmLoad(i); //loads all songs
	//music.musictype=rand()%MSL_NSONGS;//randomly choose a song
	//music.volume=0;
	//music.playing=false;
	//music.frames=0;
	//music.volumechanging=0; //0 for no changing, 1 for fade in, 2 for fade out..
	maxModInit=true;
}
void playSound(int sound)
{
	if (isWifi())
	{
		unsigned short buffer[100];
		int server_id = getServerID();			
		sprintf((char *)buffer,"[SND: %d %d", server_id, sound);
		Wifi_RawTxFrame(strlen((char *)buffer) + 1, 0x0014, buffer);	
	}
	if (maxModInit==false) 
		initSounds();
	switch(sound)
	{
		case PIG_H: mmEffect(SFX_PIG); break;
		case PLAYER_H: mmEffect(SFX_PLAYERHIT); break;
		case ZOMBIE_H: mmEffect(SFX_ZOMBIEHIT); break;
		case COW_H: mmEffect(SFX_COWHIT); break;
	}
}
void playSoundNiFi(int sound)
{
	if (maxModInit==false) 
		initSounds();
	switch(sound)
	{
		case PIG_H: mmEffect(SFX_PIG); break;
		case PLAYER_H: mmEffect(SFX_PLAYERHIT); break;
		case ZOMBIE_H: mmEffect(SFX_ZOMBIEHIT); break;
		case COW_H: mmEffect(SFX_COWHIT); break;
	}
}

void playMusic(int music)
{
	if (maxModInit==false)
		initSounds();
	switch(music)
	{
		case CALM : mmStart(MOD_CALM,MM_PLAY_ONCE); break;
		case HAL2 : mmStart(MOD_HAL2,MM_PLAY_LOOP); break;
	}
}

void stopMusic()
{
     mmStop();	
}
/*void soundUpdate(){
	music.frames++;
	if (music.frames==30){
		if(rand()%256==0 ){
			//iprintf("BLARG");
			if (music.playing==false){
				music.playing=true;
				//iprintf("Start");
				music.volumechanging=1;
				music.musictype=rand() %3;
				music.volume=0;
				if (music.musictype==0) mmStart(MOD_CALM,MM_PLAY_ONCE);
				else if (music.musictype==1) mmStart(MOD_LIVINGMICE,MM_PLAY_ONCE);
				else if (music.musictype==2) mmStart(MOD_WETHANDS,MM_PLAY_ONCE);
				mmSetModuleVolume(music.volume);
			}
			else if (music.playing==true && rand() % 2){
				//iprintf("Stop");
				music.playing=false;
				music.volumechanging=2;
				music.volume=1024;
				mmSetModuleVolume(music.volume);
			}

		}
		music.frames=0;
	}
	if (music.volumechanging==1){
		music.volume++;
		mmSetModuleVolume(music.volume);
		if (music.volume==1024) music.volumechanging=0;
	}
	if (music.volumechanging==2){
		music.volume--;
		mmSetModuleVolume(music.volume);
		if (music.volume==0){
			music.volumechanging=0;
			mmStop();
		}
	}
	if (mmActive()==0) music.playing=false;
}*/
