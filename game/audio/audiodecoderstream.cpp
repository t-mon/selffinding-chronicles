#include "audiodecoderstream.h"
#include "../debugcategories.h"

#include <QFileInfo>

AudioDecoderStream::AudioDecoderStream(const QAudioFormat &audioFormat, QObject *parent) :
    QIODevice(parent),
    m_audioFormat(audioFormat),
    m_input(&m_data),
    m_output(&m_data)
{
    init();
}

AudioDecoderStream::AudioDecoderStream(const QString &source, const QAudioFormat &audioFormat, QObject *parent) :
    QIODevice(parent),
    m_source(source),
    m_audioFormat(audioFormat),
    m_input(&m_data),
    m_output(&m_data)
{
    init();
}

QString AudioDecoderStream::source() const
{
    return m_source;
}

void AudioDecoderStream::setSource(const QString &source)
{
    if (m_source == source)
        return;

    m_source = source;
}

bool AudioDecoderStream::repeating() const
{
    return m_repeating;
}

void AudioDecoderStream::setRepeating(bool repeating)
{
    if (m_repeating == repeating)
        return;

    m_repeating = repeating;
    emit repeatingChanged(m_repeating);
}

bool AudioDecoderStream::ready() const
{
    return m_ready;
}

AudioDecoderStream::State AudioDecoderStream::state() const
{
    return m_state;
}

int AudioDecoderStream::position() const
{
    return static_cast<int>(m_output.pos() / (m_audioFormat.sampleSize() / 8) / (m_audioFormat.sampleRate() / 1000) / (m_audioFormat.channelCount()));
}

void AudioDecoderStream::setPosition(int position)
{
    const int target = position * (m_audioFormat.sampleSize() / 8) * (m_audioFormat.sampleRate() / 1000) * (m_audioFormat.channelCount());
    m_output.seek(target);
}

int AudioDecoderStream::length() const
{
    return static_cast<int>(m_output.size() / (m_audioFormat.sampleSize() / 8) / (m_audioFormat.sampleRate() / 1000) / (m_audioFormat.channelCount()));
}

bool AudioDecoderStream::atEnd() const
{
    return m_output.size() && m_output.atEnd() && m_ready;
}

void AudioDecoderStream::clear()
{
    m_decoder.stop();
    m_data.clear();
    m_file.close();

    m_ready = false;
    emit readyChanged(m_ready);
}

qint64 AudioDecoderStream::readData(char *data, qint64 maxlen)
{
    memset(data, 0, static_cast<unsigned long>(maxlen));

    if (m_state == State::Playing) {
        m_output.read(data, maxlen);
        if (maxlen > 0) {
            QByteArray buff(data, static_cast<int>(maxlen));
            emit newData(buff);
        }

        // Todo: check repeating
        if (atEnd() && !m_repeating) {
            stop();
        }
    }

    return maxlen;
}

qint64 AudioDecoderStream::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data)
    Q_UNUSED(len)
    return 0;
}

void AudioDecoderStream::init()
{
    setOpenMode(QIODevice::ReadOnly);

    // Initialize decoder
    m_decoder.setNotifyInterval(10);
    m_decoder.setAudioFormat(m_audioFormat);
    connect(&m_decoder, &QAudioDecoder::bufferReady, this, &AudioDecoderStream::onBufferReady);
    connect(&m_decoder, &QAudioDecoder::finished, this, &AudioDecoderStream::onFinished);

    // Initialize buffers
    if (!m_input.open(QIODevice::WriteOnly) || !m_output.open(QIODevice::ReadOnly)) {
        qCWarning(dcSoundEngine()) << "Could not open buffer of" << this;
        return;
    }
}

void AudioDecoderStream::onBufferReady()
{
    const QAudioBuffer &buffer = m_decoder.read();
    const int length = buffer.byteCount();
    const char *data = buffer.constData<char>();
    m_input.write(data, length);
}

void AudioDecoderStream::onFinished()
{
    qCDebug(dcSoundEngine()) << this << "deconding finished";
    m_ready = true;
    emit readyChanged(m_ready);
}

void AudioDecoderStream::play()
{
    qCDebug(dcSoundEngine()) << this << "start playing";

    // Clean up
    clear();

    // Open the file
    m_file.setFileName(m_source);
    if (!m_file.open(QIODevice::ReadOnly)) {
        qCWarning(dcSoundEngine()) << "Could not open audio file of" << this;
        return;
    }

    // Start decoding
    m_decoder.setSourceDevice(&m_file);
    m_decoder.start();

    m_state = State::Playing;
    emit stateChanged(m_state);
}

void AudioDecoderStream::pause()
{
    qCDebug(dcSoundEngine()) << this << "paused";
    m_state = State::Paused;
    emit stateChanged(m_state);
}

void AudioDecoderStream::resume()
{
    qCDebug(dcSoundEngine()) << this << "resume";
    m_state = State::Playing;
    emit stateChanged(m_state);
}

void AudioDecoderStream::stop()
{
    clear();
    m_state = State::Stopped;
    emit stateChanged(m_state);
    qCDebug(dcSoundEngine()) << this << "stopped";
}

QDebug operator<<(QDebug debug, AudioDecoderStream *stream)
{
    debug.nospace() << "AudioDecoderStream(" << QFileInfo(stream->source()).fileName() << ")";
    return debug.space();
}
