#include "GameAudio.h"

//Contructor,
GameAudio::GameAudio()
{

    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialise audio: %s", Mix_GetError());
        exit(-1);
    }
}

//Destructor.
GameAudio::~GameAudio()
{
    SDL_Quit();
}

//Adds a new sound effect.
void GameAudio::newSoundEffect(const char* path)
{
    Mix_Chunk* tempChunk = Mix_LoadWAV(path);

    if (tempChunk != nullptr)
    {
        mSoundEffectBank.push_back(tempChunk);
        std::cout << (mSoundEffectBank.size() - 1) << "The Sound is Ready, path: " << path << '\n';
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize audio: %s", Mix_GetError()); //Through an error if it cant initilize the audio.  
    }
}

//Adds a new audio track.
void GameAudio::newAudioTrack(const char* path)
{
    bgMusic = Mix_LoadMUS(path);

    if (bgMusic == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }
}

//Plays the chossen sound effect.
void GameAudio::playSoundEffect(const int which) const
{
    //Returns an error message if chossen sound effect is out of range.
    if (which > mSoundEffectBank.size() - 1)
    {
        std::cout << "Sound out of range.\n";
        return;
    }

    //Plays the sound effect.
    Mix_PlayChannel(-1, mSoundEffectBank[which], 0);

    std::cout << "Played Sound: " << which << '\n';
}

//Plays the audio track.
void GameAudio::playAudioTrack()
{
    if (Mix_PlayingMusic() == 0)
    {
        Mix_PlayMusic(bgMusic, -1);//Plays the background music
    }
}