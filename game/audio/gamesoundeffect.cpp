#include "gamesoundeffect.h"
#include "../debugcategories.h"

GameSoundEffect::GameSoundEffect(QObject *parent) : QObject(parent)
{

}

GameSoundEffect::~GameSoundEffect()
{
    cleanUp();
}

QString GameSoundEffect::source() const
{
    return m_source;
}

void GameSoundEffect::setSource(const QString &source)
{
    if (m_source == source)
        return;

    m_source = source;
    cleanUp();
    qCDebug(dcSoundEngine()) << "GameSoundEffect source" << m_source;
    m_stream = Game::instance()->soundEngine()->registerSoundEffect(m_source);
}

void GameSoundEffect::cleanUp()
{
    if (m_stream) {
        Game::instance()->soundEngine()->unregisterSoundEffect(m_stream);
        m_stream->deleteLater();
    }
}

void GameSoundEffect::play()
{
    if (!m_stream)
        return;

    m_stream->play();
}

void GameSoundEffect::pause()
{
    if (!m_stream)
        return;

    m_stream->pause();
}

void GameSoundEffect::resume()
{
    if (!m_stream)
        return;

    m_stream->resume();
}

void GameSoundEffect::rewind()
{
    if (!m_stream)
        return;

    m_stream->rewind();
}

void GameSoundEffect::stop()
{
    if (!m_stream)
        return;

    m_stream->stop();
}
