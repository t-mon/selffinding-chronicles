#ifndef GAMESOUNDEFFECT_H
#define GAMESOUNDEFFECT_H

#include <QObject>
#include "../game.h"

class GameSoundEffect : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit GameSoundEffect(QObject *parent = nullptr);
    ~GameSoundEffect();

    QString source() const;
    void setSource(const QString &source);

private:
    QString m_source;
    AudioDecoderStream *m_stream = nullptr;

    void cleanUp();

signals:
    void sourceChanged(const QString &source);

public slots:
    void play();
    void pause();
    void resume();
    void rewind();
    void stop();

};

#endif // GAMESOUNDEFFECT_H
