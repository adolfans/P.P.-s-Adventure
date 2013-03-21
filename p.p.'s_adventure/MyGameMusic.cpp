#include "StdAfx.h"
#include "MyGameMusic.h"
#include <SDL.h>
#include <exception>
using std::runtime_error;

MyGameMusic::MyGameMusic( string& fileName )
	:defaultVolume( 0.125 )
{
	sample = Mix_LoadWAV( fileName.c_str() );
	if(!sample) {
		printf("Mix_LoadWAV: %s\n", Mix_GetError());
    // handle error
	}

	Mix_PlayChannel(-1, sample, 1);

	Mix_VolumeChunk( sample, static_cast<int>(defaultVolume*MIX_MAX_VOLUME));
}


MyGameMusic::~MyGameMusic(void)
{
	Mix_FreeChunk( sample );
}

void MyGameMusic::setVolume( float volume )
{
	Mix_VolumeChunk(sample, static_cast<int>(volume*MIX_MAX_VOLUME));
}

void MyGameMusic::InitMusicSystem()
{
	if(SDL_Init(SDL_INIT_AUDIO)==-1) {
		throw runtime_error( string("SDL_Init: \n") + SDL_GetError() );
	}
	int flags=MIX_INIT_OGG|MIX_INIT_MOD;
	int initted=Mix_Init(flags);
	if(initted&flags != flags) {
		throw runtime_error( string("Mix_Init: Failed to init required ogg and mod support! \n")
								+ Mix_GetError() );
	}
	Mix_Chunk *sample;
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
		throw runtime_error( string("Mix_OpenAudio: \n") +Mix_GetError() );
	}
}

void MyGameMusic::ExitMusicSystem()
{
	 Mix_CloseAudio();
	 Mix_Quit();
	 SDL_Quit();
}