#pragma once
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>
#include <iostream>
#include <vector>

class GameAudio
{
	public:
		GameAudio(); //Sets audio data. 
		~GameAudio(); //SDL quits.

		void newSoundEffect(const char* path); //Adds a new sound effect.
		void newAudioTrack(const char* path); //Adds a new audio track.
		void playSoundEffect(const int which) const; //Plays the chossen sound effect.
		void playAudioTrack(); //Plays the audio track.

	private:
		Mix_Music* bgMusic;
		std::vector<Mix_Chunk*> mSoundEffectBank;

};
