#include "StdAfx.h"
#include "OggPlayer.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include <string>
#include <exception>
using std::runtime_error;
using std::string;
OggPlayer::OggPlayer(void)
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
	//sample = Mix_LoadWAV( "YSO_001.ogg" );
	sample = Mix_LoadWAV( "YSO_001.ogg" );
	if(!sample) {
		printf("Mix_LoadWAV: %s\n", Mix_GetError());
    // handle error
	}

	Mix_PlayChannel(-1, sample, 1);
}


OggPlayer::~OggPlayer(void)
{
}
