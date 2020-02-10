#ifndef AUDIOMIXER_H
#define AUDIOMIXER_H

#include <QIODevice>
#include <QAudioOutput>

#include "audiodecoderstream.h"

typedef std::numeric_limits<qint16> ValueRange;

class AudioMixer : public QIODevice
{
    Q_OBJECT

public:
    explicit AudioMixer(const QAudioFormat &audioFormat, QObject *parent = nullptr);

    void addAudioDecoderStream(AudioDecoderStream *stream);
    void removeAudioDescoderStream(AudioDecoderStream *stream);

private:
    QAudioFormat m_audioFormat;
    QList<AudioDecoderStream *> m_streams;

protected:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

private:
    qint16 mixAudioData(qint32 sampleOne, qint32 sampleTwo);


signals:

};

#endif // AUDIOMIXER_H
