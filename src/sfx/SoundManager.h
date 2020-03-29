#ifndef SFX_SOUNDMANAGER_H
#define SFX_SOUNDMANAGER_H

#include <AL/al.h>
#include <AL/alc.h>

#include <vector>

namespace Sfx {

class SoundManager
{
  public:
    SoundManager(unsigned int aEffectSources = KDefaultEffectSources);
    ~SoundManager();
    void LoadTrack(const char* aTrack);
    void StartMusic();
    void StopMusic();
    void PlayEffect(const char* aEffect);

    static constexpr const unsigned int KDefaultEffectSources = 7;
  private:
    ALCdevice *iDevice;
    ALCcontext *iContext;
    
    ALuint iMusicSource;
    std::vector<ALuint> iEffectSources;

    ALuint iMusicBuffer;
    std::vector<ALuint> iEffectBuffers;
};


}

#endif
