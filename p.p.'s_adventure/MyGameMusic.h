#pragma once
#include <SDL_mixer.h>
#include <string>
using std::string;
class MyGameMusic
{
private:
	Mix_Chunk *sample;
public:
	MyGameMusic( string& fileName );
	~MyGameMusic(void);
	void setVolume( float volume );
	static void InitMusicSystem();
	static void ExitMusicSystem();
};

