#include "soundengine.h"
#include "../debugcategories.h"

SoundEngine::SoundEngine(QObject *parent) : QObject(parent)
{
    QAudioDeviceInfo device = QAudioDeviceInfo::defaultOutputDevice();
    qCDebug(dcSoundEngine()) << "Picking default output device for sound";
    qCDebug(dcSoundEngine()) << device.deviceName();
    qCDebug(dcSoundEngine()) << device.preferredFormat();
    QAudioFormat audioFormat = device.preferredFormat();

    m_output = new QAudioOutput(audioFormat, this);
    m_audioMixer = new AudioMixer(audioFormat, this);
    m_output->start(m_audioMixer);

    AudioDecoderStream *ambientSound = new AudioDecoderStream(audioFormat);
    ambientSound->setSource("/home/timon/development/selffinding-chronicles/selffinding-chronicles/resources/sounds/ambient/spring-birds-loop.ogg");
    m_audioMixer->addAudioDecoderStream(ambientSound);

    qCDebug(dcSoundEngine()) << "Initialized successfully";
}

qreal SoundEngine::soundEffectVolume() const
{
    return m_soundEffectVolume;
}

void SoundEngine::setSoundEffectVolume(qreal volume)
{
    if (qFuzzyCompare(m_soundEffectVolume, volume))
        return;

    qCDebug(dcSoundEngine()) << "Sound effect volume changed" << volume;
    m_soundEffectVolume = volume;
    emit soundEffectVolumeChanged(m_soundEffectVolume);
}

qreal SoundEngine::musicVolume() const
{
    return m_musicVolume;
}

void SoundEngine::setMusicVolume(qreal volume)
{
    if (qFuzzyCompare(m_musicVolume, volume))
        return;

    qCDebug(dcSoundEngine()) << "Music volume changed" << volume;
    m_musicVolume = volume;
    emit musicVolumeChanged(m_musicVolume);
}

void SoundEngine::playInventoryOpen()
{

}
