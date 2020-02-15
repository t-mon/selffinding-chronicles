#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <QObject>
#include <QSoundEffect>

#include "audiomixer.h"

class SoundEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal soundEffectVolume READ soundEffectVolume WRITE setSoundEffectVolume NOTIFY soundEffectVolumeChanged)
    Q_PROPERTY(qreal musicVolume READ musicVolume WRITE setMusicVolume NOTIFY musicVolumeChanged)

public:
    explicit SoundEngine(QObject *parent = nullptr);

    qreal soundEffectVolume() const;
    void setSoundEffectVolume(qreal volume);

    qreal musicVolume() const;
    void setMusicVolume(qreal volume);

    // Can be used to register music
    Q_INVOKABLE AudioDecoderStream *registerMusicStream(const QString &source);
    Q_INVOKABLE void unregisterMusicStream(AudioDecoderStream *musicStream);

    // Can be used to add decoded sound effects and play them when required
    Q_INVOKABLE AudioDecoderStream *registerSoundEffect(const QString &source);
    Q_INVOKABLE void unregisterSoundEffect(AudioDecoderStream *soundEffectStream);

private:
    QAudioDeviceInfo m_audioDeviceInfo;
    QAudioFormat m_audioFormat;

    qreal m_soundEffectVolume = 1.0;
    qreal m_musicVolume = 1.0;

    QAudioOutput *m_output = nullptr;
    AudioMixer *m_audioMixer = nullptr;

signals:
    void soundEffectVolumeChanged(qreal volume);
    void musicVolumeChanged(qreal volume);

};

#endif // SOUNDENGINE_H
