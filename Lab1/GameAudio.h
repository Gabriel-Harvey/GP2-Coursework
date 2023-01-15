#pragma once
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>
#include <iostream>
#include <vector>

class GameAudio
{
	public:
		GameAudio();
		~GameAudio();

		void newSoundEffect(const char* path);
		void newAudioTrack(const char* path);
		void playSoundEffect(const int which) const;
		void playAudioTrack();

	private:
		Mix_Music* bgMusic;
		std::vector<Mix_Chunk*> mSoundEffectBank;

};
