#include "audiomixer.h"
#include "../debugcategories.h"

AudioMixer::AudioMixer(const QAudioFormat &audioFormat, QObject *parent) :
    QIODevice(parent),
    m_audioFormat(audioFormat)
{
    setOpenMode(QIODevice::ReadOnly);
}

void AudioMixer::addAudioDecoderStream(AudioDecoderStream *stream)
{
    m_streams.append(stream);
}

void AudioMixer::removeAudioDescoderStream(AudioDecoderStream *stream)
{
    m_streams.removeAll(stream);
}

qint64 AudioMixer::readData(char *data, qint64 maxlen)
{
    memset(data, 0, static_cast<unsigned long>(maxlen));

    const qint16 depth = sizeof(qint16);
    const qint16 samples = static_cast<qint16>(maxlen / depth);

    foreach (AudioDecoderStream *stream, m_streams) {
        if (!stream->ready())
            continue;

        qint16 sample;
        qint16 *cursor = reinterpret_cast<qint16 *>(data);
        for (int i = 0; i < samples; i++, cursor++) {
            if (stream->read(reinterpret_cast<char *>(&sample), depth)) {
                *cursor = mixAudioData(*cursor, sample);
            }
        }
    }

    return maxlen;
}

qint64 AudioMixer::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data)
    Q_UNUSED(len)
    return 0;
}

qint16 AudioMixer::mixAudioData(qint32 sampleOne, qint32 sampleTwo)
{
    const qint32 result = sampleOne + sampleTwo;

    if (ValueRange::max() < result) {
        return ValueRange::max();
    }

    if (ValueRange::min() > result) {
        return ValueRange::min();
    }

    return static_cast<qint16>(result);
}
