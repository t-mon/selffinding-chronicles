#ifndef GAMEMUSIC_H
#define GAMEMUSIC_H

#include <QObject>
#include "../game.h"


class GameMusic : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit GameMusic(QObject *parent = nullptr);
    ~GameMusic();

    QString source() const;
    void setSource(const QString &source);

    qreal volume() const;
    void setVolume(qreal volume);

private:
    QString m_source;
    qreal m_volume = 1.0;
    AudioDecoderStream *m_stream = nullptr;

    void cleanUp();

signals:
    void sourceChanged(const QString &source);
    void volumeChanged(qreal volume);

public slots:
    void play();
    void pause();
    void resume();
    void rewind();
    void stop();

};

#endif // GAMEMUSIC_H
