#include "soundengine.h"
#include "../debugcategories.h"

SoundEngine::SoundEngine(QObject *parent) :
    QObject(parent),
    m_audioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice()),
    m_audioFormat(m_audioDeviceInfo.preferredFormat())
{
    qCDebug(dcSoundEngine()) << "Picking default output device" << m_audioDeviceInfo.deviceName();
    qCDebug(dcSoundEngine()) << m_audioFormat;

    m_output = new QAudioOutput(m_audioFormat, this);
    m_output->setBufferSize(12000);
    m_audioMixer = new AudioMixer(m_audioFormat, this);
    m_output->start(m_audioMixer);

    qCDebug(dcSoundEngine()) << "Initialized successfully" << m_output->bufferSize();
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

AudioDecoderStream *SoundEngine::registerMusicStream(const QString &source)
{
    AudioDecoderStream *musicStream = new AudioDecoderStream(source, m_audioFormat, this);
    musicStream->setRepeating(true);

    qCDebug(dcSoundEngine()) << "Register music stream" << musicStream;
    connect(musicStream, &AudioDecoderStream::readyChanged, this, [this, musicStream](bool ready){
        if (ready && musicStream->state() == AudioDecoderStream::Playing) {
            qCDebug(dcSoundEngine()) << musicStream << "add to the mixer";
            m_audioMixer->addAudioDecoderStream(musicStream);
        }
    });

    // Start initializing decoder data
    musicStream->initialize();
    return musicStream;
}

void SoundEngine::unregisterMusicStream(AudioDecoderStream *musicStream)
{
    qCDebug(dcSoundEngine()) << "Unregister music stream" << musicStream;
    m_audioMixer->removeAudioDescoderStream(musicStream);
    musicStream->clear();
    musicStream->deleteLater();
}

AudioDecoderStream *SoundEngine::registerSoundEffect(const QString &source)
{
    AudioDecoderStream *soundEffectStream = new AudioDecoderStream(source, m_audioFormat, this);
    soundEffectStream->setRepeating(false);

    qCDebug(dcSoundEngine()) << "Register sound effect stream" << soundEffectStream;
    connect(soundEffectStream, &AudioDecoderStream::stateChanged, this, [this, soundEffectStream](AudioDecoderStream::State state){
        switch (state) {
        case AudioDecoderStream::Playing:
            qCDebug(dcSoundEngine()) << "Start playing sound effect" << soundEffectStream;
            m_audioMixer->addAudioDecoderStream(soundEffectStream);
            break;
        default:
            qCDebug(dcSoundEngine()) << "Stop playing sound effect" << soundEffectStream;
            m_audioMixer->removeAudioDescoderStream(soundEffectStream);
            break;
        }
    }, Qt::DirectConnection);

    // Start initializing decoder data
    soundEffectStream->initialize();
    return soundEffectStream;
}

void SoundEngine::unregisterSoundEffect(AudioDecoderStream *soundEffectStream)
{
    qCDebug(dcSoundEngine()) << "Unregister sound effect stream" << soundEffectStream;
    m_audioMixer->removeAudioDescoderStream(soundEffectStream);
    soundEffectStream->clear();
}
