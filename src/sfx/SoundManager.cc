#include "SoundManager.h"

#include <AL/alut.h>

namespace Sfx {
  
SoundManager::SoundManager(unsigned int aEffectSources)
{
  ALCenum error;	
  alGetError();
  iDevice = alcOpenDevice(0);
    
  iContext = alcCreateContext(iDevice, 0);
  if (!alcMakeContextCurrent(iContext))
  {
    error = alGetError();
  }

  /* Generate effect sources and buffers */
  iEffectSources.resize(aEffectSources);
  iEffectBuffers.resize(aEffectSources);
  alGenSources((ALuint)aEffectSources, &iEffectSources[0]);
  error = alGetError();
  for (size_t i = 0; i < aEffectSources; ++i)
  {
    alSourcef(iEffectSources[i], AL_PITCH, 1);
    alSourcef(iEffectSources[i], AL_GAIN, 1);
    alSource3f(iEffectSources[i], AL_POSITION, 0, 0, 0);
    alSource3f(iEffectSources[i], AL_VELOCITY, 0, 0, 0);
    alSourcei(iEffectSources[i], AL_LOOPING, AL_FALSE);
    error = alGetError();
  }

  alGenBuffers((ALuint)aEffectSources, &iEffectBuffers[0]);
  error = alGetError();

  alGenSources((ALuint)1, &iMusicSource);
  error = alGetError();
  alSourcef(iMusicSource, AL_PITCH, 1);
  alSourcef(iMusicSource, AL_GAIN, 0.3);
  alSource3f(iMusicSource, AL_POSITION, 0, 0, 0);
  alSource3f(iMusicSource, AL_VELOCITY, 0, 0, 0);
  alSourcei(iMusicSource, AL_LOOPING, AL_TRUE);
  error = alGetError();

  alGenBuffers((ALuint)1, &iMusicBuffer);
  error = alGetError();  
}

SoundManager::~SoundManager()
{
  alDeleteSources((ALuint)iEffectSources.size(), &iEffectSources[0]);
  alDeleteBuffers((ALuint)iEffectBuffers.size(), &iEffectBuffers[0]);
  alDeleteSources(1, &iMusicSource);
  alDeleteBuffers(1, &iMusicBuffer);
  alcMakeContextCurrent(0);
  alcDestroyContext(iContext);
  alcCloseDevice(iDevice);
}

void SoundManager::LoadTrack(const char* aTrack)
{
  ALsizei size, freq;
  ALenum format;
  ALvoid *data;
  ALboolean loop = AL_FALSE;
  ALCenum error;  
  alutLoadWAVFile((signed char*)(aTrack), &format, &data, &size, &freq, &loop);
  alBufferData(iMusicBuffer, format, data, size, freq);
  alSourcei(iMusicSource, AL_BUFFER, iMusicBuffer);
}

void SoundManager::StartMusic()
{
  alSourcePlay(iMusicSource);  
}

void SoundManager::StopMusic()
{
  alSourceStop(iMusicSource);
}
 
void SoundManager::PlayEffect(const char* aEffect)
{
  ALint source_state;
  for(size_t i = 0; i < iEffectSources.size(); ++i)
  {
    alGetSourcei(iEffectSources[i], AL_SOURCE_STATE, &source_state);
    if (source_state == AL_STOPPED || source_state == AL_INITIAL) {
      // Play on this source
      ALsizei size, freq;
      ALenum format;
      ALvoid *data;
      ALboolean loop = AL_FALSE;
      alutLoadWAVFile((signed char*)(aEffect), &format, &data, &size, &freq, &loop);
      alBufferData(iEffectBuffers[i], format, data, size, freq);
      alSourcei(iEffectSources[i], AL_BUFFER, iEffectBuffers[i]);
      alSourcePlay(iEffectSources[i]);
      break;
    }
  }
}

}
