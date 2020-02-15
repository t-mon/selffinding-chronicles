#include "gamemusic.h"

GameMusic::GameMusic(QObject *parent) : QObject(parent)
{

}

GameMusic::~GameMusic()
{
    cleanUp();
}

QString GameMusic::source() const
{
    return m_source;
}

void GameMusic::setSource(const QString &source)
{
    if (m_source == source)
        return;

    m_source = source;
    cleanUp();
    qCDebug(dcSoundEngine()) << "GameMusic source" << m_source;
    m_stream = Game::instance()->soundEngine()->registerMusicStream(m_source);
}

qreal GameMusic::volume() const
{
    return m_volume;
}

void GameMusic::setVolume(qreal volume)
{
    if (qFuzzyCompare(m_volume, volume))
        return;

    m_volume = volume;
    if (m_stream) m_stream->setVolume(m_volume);
    emit volumeChanged(m_volume);
}

void GameMusic::cleanUp()
{
    if (m_stream) {
        Game::instance()->soundEngine()->unregisterMusicStream(m_stream);
        m_stream->deleteLater();
    }
}

void GameMusic::play()
{
    if (!m_stream)
        return;

    m_stream->play();
}

void GameMusic::pause()
{
    if (!m_stream)
        return;

    m_stream->pause();
}

void GameMusic::resume()
{
    if (!m_stream)
        return;

    m_stream->resume();
}

void GameMusic::rewind()
{
    if (!m_stream)
        return;

    m_stream->rewind();
}

void GameMusic::stop()
{
    if (!m_stream)
        return;

    m_stream->stop();
}
