#include "world.h"
#include "game.h"

#include <QtMath>
#include <QDebug>

World::World(QObject *parent) :
    QObject(parent),
    m_size(QSize(50, 50)),
    m_sceneSize(QSize(30, 20)),
    m_cellSize(30)
{
    m_player = new Player(this);
    connect(m_player, &Player::positionChanged, this, &World::onPlayerPositionChanged);

    // Init occupied fields
    m_occupiedFields = new Fields(this);
    m_occupiedFields->addField(new Field(QPoint(6, 6), m_occupiedFields));

    setboundingPosition(QPoint(0, 0));
    setBoundingSize(QSize(10, 10));

    // Init current field
    m_player->setPosition(QPointF(5, 5));
}

QSize World::size() const
{
    return m_size;
}

void World::setSize(const QSize &size)
{
    if (m_size == size)
        return;

    qDebug() << "World: size changed" << size;
    m_size = size;
    emit sizeChanged(m_size);
}

QSize World::sceneSize() const
{
    return m_sceneSize;
}

void World::setSceneSize(const QSize &sceneSize)
{
    if (m_sceneSize == sceneSize)
        return;

    qDebug() << "World: scene size changed" << sceneSize;
    m_sceneSize = sceneSize;
    emit sceneSizeChanged(m_sceneSize);
}

QSize World::boundingSize() const
{
    return m_boundingSize;
}

void World::setBoundingSize(const QSize &boundingSize)
{
    if (m_boundingSize == boundingSize)
        return;

    qDebug() << "World: bounding size changed" << boundingSize;
    m_boundingSize = boundingSize;
    emit boundingSizeChanged(m_boundingSize);
}

int World::cellSize() const
{
    return m_cellSize;
}

void World::setCellSize(const int &cellSize)
{
    if (m_cellSize == cellSize)
        return;

    qDebug() << "World: cell size changed" << cellSize;
    m_cellSize = cellSize;
    emit cellSizeChanged(m_cellSize);
}

Player *World::player()
{
    return m_player;
}

Fields *World::occupiedFields()
{
    return m_occupiedFields;
}

QPoint World::currentPlayerField() const
{
    return m_currentPlayerField;
}

QPointF World::boundingPosition() const
{
    return m_boundingPosition;
}

void World::setForwardPressed(bool forwaredPressed)
{
    m_forwaredPressed = forwaredPressed;
}

void World::setBackwardPressed(bool backwardPressed)
{
    m_backwardPressed = backwardPressed;
}

void World::setLeftPressed(bool leftPressed)
{
    m_leftPressed = leftPressed;
}

void World::setRightPressed(bool rightPressed)
{
    m_rightPressed = rightPressed;
}

void World::setCurrentPlayerField(const QPoint &currentField)
{
    if (m_currentPlayerField == currentField)
        return;

    qDebug() << "World: current filed" << currentField;
    m_currentPlayerField = currentField;
    emit currentPlayerFieldChanged(m_currentPlayerField);
}

void World::setboundingPosition(const QPointF &boundingPosition)
{
    if (m_boundingPosition == boundingPosition)
        return;

    qDebug() << "World: current screen position" << boundingPosition;
    m_boundingPosition = boundingPosition;
    emit boundingPositionChanged(m_boundingPosition);
}

void World::moveKeyBoard()
{
    qreal angle = 0;

    // Forward
    if (m_forwaredPressed && !m_backwardPressed && !m_leftPressed && !m_rightPressed)
        angle = -M_PI_2;

    // Forward / Left
    if (m_forwaredPressed && !m_backwardPressed && m_leftPressed && !m_rightPressed)
        angle = -M_PI_2 - M_PI_4;

    // Forward / Right
    if (m_forwaredPressed && !m_backwardPressed && !m_leftPressed && m_rightPressed)
        angle = -M_PI_4;

    // Backward
    if (!m_forwaredPressed && m_backwardPressed && !m_leftPressed && !m_rightPressed)
        angle = M_PI_2;

    // Backward / Left
    if (!m_forwaredPressed && m_backwardPressed && m_leftPressed && !m_rightPressed)
        angle = M_PI_4 + M_PI_2;

    // Backward / Right
    if (!m_forwaredPressed && m_backwardPressed && !m_leftPressed && m_rightPressed)
        angle = M_PI_4;

    // Left
    if (!m_forwaredPressed && !m_backwardPressed && m_leftPressed && !m_rightPressed)
        angle = -M_PI;

    // Right
    if (!m_forwaredPressed && !m_backwardPressed && !m_leftPressed && m_rightPressed)
        angle = 0;

    //double angleDegree = angle * 180 / M_PI;
    //qDebug() << angle << angleDegree;

    if (!m_forwaredPressed && !m_backwardPressed && !m_leftPressed && !m_rightPressed) {
        m_player->setMoving(false);
        return;
    }

    qreal deltaX = qRound(m_player->speed() * qCos(angle) * 10000.0) / 10000.0;
    qreal deltaY = qRound(m_player->speed() * qSin(angle) * 10000.0) / 10000.0;

    m_player->setAngle(angle);

    movePlayer(QPointF(m_player->position().x() + deltaX, m_player->position().y() + deltaY));
}

void World::moveKeyBoardMouse()
{
    qreal angle = m_player->angle();

    // Forward
    if (m_forwaredPressed && !m_backwardPressed && !m_leftPressed && !m_rightPressed)
        angle = m_player->angle();

    // Forward / Left
    if (m_forwaredPressed && !m_backwardPressed && m_leftPressed && !m_rightPressed)
        angle = m_player->angle() - M_PI_4;

    // Forward / Right
    if (m_forwaredPressed && !m_backwardPressed && !m_leftPressed && m_rightPressed)
        angle = m_player->angle() + M_PI_4;

    // Backward
    if (!m_forwaredPressed && m_backwardPressed && !m_leftPressed && !m_rightPressed)
        angle = m_player->angle() + M_PI;

    // Backward / Left
    if (!m_forwaredPressed && m_backwardPressed && m_leftPressed && !m_rightPressed)
        angle = m_player->angle() - M_PI_4 - M_PI_2;

    // Backward / Right
    if (!m_forwaredPressed && m_backwardPressed && !m_leftPressed && m_rightPressed)
        angle = m_player->angle() + M_PI_4 + M_PI_2;

    // Left
    if (!m_forwaredPressed && !m_backwardPressed && m_leftPressed && !m_rightPressed)
        angle = m_player->angle() - M_PI_2;

    // Right
    if (!m_forwaredPressed && !m_backwardPressed && !m_leftPressed && m_rightPressed)
        angle = m_player->angle() + M_PI_2;

    //double angleDegree = angle * 180 / M_PI;
    //qDebug() << angle << angleDegree;

    if (!m_forwaredPressed && !m_backwardPressed && !m_leftPressed && !m_rightPressed) {
        m_player->setMoving(false);
        return;
    }

    double deltaX = qRound(m_player->speed() * qCos(angle) * 10000.0) / 10000.0;
    double deltaY = qRound(m_player->speed() * qSin(angle) * 10000.0) / 10000.0;

    movePlayer(QPointF(m_player->position().x() + deltaX, m_player->position().y() + deltaY));

    //qDebug() << deltaX << deltaY << qAbs(qPow(deltaX, 2) + qPow(deltaY, 2));
}

void World::moveTouchscreen()
{

}

void World::movePlayer(const QPointF newPosition)
{
    QPointF resultPosition = newPosition;

    // Check world end north
    if (newPosition.y() < 0.5) {
        resultPosition = QPointF(resultPosition.x(), 0.5);
    }

    // Check world end east
    if (newPosition.x() > m_size.width() - 0.5) {
        resultPosition = QPointF(m_size.width() - 0.5, resultPosition.y());
    }

    // Check world end south
    if (newPosition.y() > m_size.height() - 0.5) {
        resultPosition = QPointF(resultPosition.x(), m_size.height() - 0.5);
    }

    // Check world end west
    if (newPosition.x() < 0.5) {
        resultPosition = QPointF(0.5, resultPosition.y());
    }

    // Check if the field is movable
    m_player->setPosition(resultPosition);
}

void World::onPlayerPositionChanged()
{
    // Calculate the current field on the map
    int x = static_cast<int>(m_player->position().x());
    int y = static_cast<int>(m_player->position().y());
    setCurrentPlayerField(QPoint(x, y));
}

void World::tick()
{
    if (!m_player->movable())
        return;

    switch (Game::instance()->controlMode()) {
    case Game::ControlModeKeyBoard:
        moveKeyBoard();
        break;
    case Game::ControlModeKeyBoardMouse:
        moveKeyBoardMouse();
        break;
    case Game::ControlModeTouchscreen:
        moveTouchscreen();
        break;
    default:
        break;
    }

    // Evaluate players field



    // Evaluate environment



}
