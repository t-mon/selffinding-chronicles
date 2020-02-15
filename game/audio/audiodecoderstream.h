#ifndef AUSIODECODERSTREAM_H
#define AUSIODECODERSTREAM_H

#include <QFile>
#include <QBuffer>
#include <QIODevice>
#include <QAudioBuffer>
#include <QAudioDecoder>

class AudioDecoderStream : public QIODevice
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool repeating READ repeating WRITE setRepeating NOTIFY repeatingChanged)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)

public:
    enum State {
        Playing,
        Paused,
        Stopped
    };
    Q_ENUM(State)

    explicit AudioDecoderStream(const QAudioFormat &audioFormat, QObject *parent = nullptr);
    explicit AudioDecoderStream(const QString &source, const QAudioFormat &audioFormat, QObject *parent = nullptr);

    QString source() const;
    void setSource(const QString &source);

    qreal volume() const;
    void setVolume(qreal volume);

    bool repeating() const;
    void setRepeating(bool repeating);

    bool isReady() const;
    State state() const;

    int position() const;
    void setPosition(int position);
    int length() const;

    bool atEnd() const override;

    void initialize();
    void clear();

protected:
    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char* data, qint64 len) override;

private:
    QString m_source;
    qreal m_volume = 1.0;
    QAudioFormat m_audioFormat;

    bool m_repeating = false;
    bool m_ready = false;
    State m_state = Stopped;

    QFile m_file;
    QBuffer m_input;
    QBuffer m_output;
    QByteArray m_data;
    QAudioDecoder m_decoder;

    void init();

signals:
    void sourceChanged(const QString &source);
    void volumeChanged(qreal volume);
    void repeatingChanged(bool repeating);
    void readyChanged(bool ready);
    void stateChanged(AudioDecoderStream::State state);
    void newData(const QByteArray& data);

private slots:
    void onBufferReady();
    void onFinished();

public slots:
    void play();
    void pause();
    void resume();
    void rewind();
    void stop();
};

QDebug operator<<(QDebug debug, AudioDecoderStream *stream);

#endif // AUSIODECODERSTREAM_H
