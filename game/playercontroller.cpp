#include "playercontroller.h"
#include "debugcategories.h"
#include "player.h"

#include <QtMath>

PlayerController::PlayerController(Player *player, QObject *parent) :
    QObject(parent),
    m_player(player)
{

}

PlayerController::ControlMode PlayerController::controlMode() const
{
    return m_controlMode;
}

void PlayerController::setControlMode(const PlayerController::ControlMode &controlMode)
{
    if (m_controlMode == controlMode)
        return;

    m_controlMode = controlMode;
    emit controlModeChanged(m_controlMode);
}

bool PlayerController::forwardPressed() const
{
    return m_forwaredPressed;
}

bool PlayerController::backwardPressed() const
{
    return m_backwardPressed;
}

bool PlayerController::leftPressed() const
{
    return m_leftPressed;
}

bool PlayerController::rightPressed() const
{
    return m_rightPressed;
}

bool PlayerController::primaryActionPressed() const
{
    return m_primaryActionPressed;
}

bool PlayerController::secondaryActionPressed() const
{
    return m_secondaryActionPressed;
}

void PlayerController::keyPressed(const Qt::Key &key)
{
    // TODO: load key map
    switch (key) {
    case Qt::Key_W:
        setForwardPressed(true);
        break;
    case Qt::Key_S:
        setBackwardPressed(true);
        break;
    case Qt::Key_A:
        setLeftPressed(true);
        break;
    case Qt::Key_D:
        setRightPressed(true);
        break;
    case Qt::Key_Space:
        setPrimaryActionPressed(true);
        break;
    case Qt::Key_E:
        setSecondaryActionPressed(true);
        break;
    default:
        break;
    }
}

void PlayerController::keyReleased(const Qt::Key &key)
{
    // TODO: load key map
    switch (key) {
    case Qt::Key_W:
        setForwardPressed(false);
        break;
    case Qt::Key_S:
        setBackwardPressed(false);
        break;
    case Qt::Key_A:
        setLeftPressed(false);
        break;
    case Qt::Key_D:
        setRightPressed(false);
        break;
    case Qt::Key_Space:
        setPrimaryActionPressed(false);
        break;
    case Qt::Key_E:
        setSecondaryActionPressed(false);
        break;
    default:
        break;
    }
}

QPointF PlayerController::delta()
{
    QPointF deltaOffset;

    if (!m_player->movable())
        return deltaOffset;

    switch (m_controlMode) {
    case ControlModeKeyBoard:
        deltaOffset = moveKeyBoard();
        break;
    case ControlModeKeyBoardMouse:
        deltaOffset = moveKeyBoardMouse();
        break;
    case ControlModeTouchscreen:
        deltaOffset = moveTouchscreen();
        break;
    }

    if (!deltaOffset.isNull())
        qCDebug(dcPlayerController()) << m_controlMode << "delta" << deltaOffset;

    return deltaOffset;
}

void PlayerController::setForwardPressed(bool forwaredPressed)
{
    qCDebug(dcPlayerController()) << "Forwared" << (forwaredPressed ? "pressed" : "released");
    m_forwaredPressed = forwaredPressed;
}

void PlayerController::setBackwardPressed(bool backwardPressed)
{
    qCDebug(dcPlayerController()) << "Backwards" << (backwardPressed ? "pressed" : "released");
    m_backwardPressed = backwardPressed;
}

void PlayerController::setLeftPressed(bool leftPressed)
{
    qCDebug(dcPlayerController()) << "Left" << (leftPressed ? "pressed" : "released");
    m_leftPressed = leftPressed;
}

void PlayerController::setRightPressed(bool rightPressed)
{
    qCDebug(dcPlayerController()) << "Right" << (rightPressed ? "pressed" : "released");
    m_rightPressed = rightPressed;
}

void PlayerController::setPrimaryActionPressed(bool primaryActionPressed)
{
    qCDebug(dcPlayerController()) << "Primary action" << (primaryActionPressed ? "pressed" : "released");
    m_primaryActionPressed = primaryActionPressed;
}

void PlayerController::setSecondaryActionPressed(bool secondaryActionPressed)
{
    qCDebug(dcPlayerController()) << "Secondary action" << (secondaryActionPressed ? "pressed" : "released");
    m_secondaryActionPressed = secondaryActionPressed;
}

QPointF PlayerController::moveKeyBoard()
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

    if (!m_forwaredPressed && !m_backwardPressed && !m_leftPressed && !m_rightPressed) {
        m_player->setMoving(false);
        return QPointF();
    }

    qreal deltaX = qRound(m_player->speed() * qCos(angle) * 10000.0) / 10000.0;
    qreal deltaY = qRound(m_player->speed() * qSin(angle) * 10000.0) / 10000.0;

    m_player->setAngle(angle);
    return QPointF(deltaX, deltaY);
}

QPointF PlayerController::moveKeyBoardMouse()
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

    if (!m_forwaredPressed && !m_backwardPressed && !m_leftPressed && !m_rightPressed) {
        m_player->setMoving(false);
        return QPointF();
    }

    double deltaX = qRound(m_player->speed() * qCos(angle) * 10000.0) / 10000.0;
    double deltaY = qRound(m_player->speed() * qSin(angle) * 10000.0) / 10000.0;

    m_player->setAngle(angle);
    return QPointF(deltaX, deltaY);
}

QPointF PlayerController::moveTouchscreen()
{
    return QPointF();
}
