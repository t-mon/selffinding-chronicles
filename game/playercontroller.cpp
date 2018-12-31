#include "playercontroller.h"
#include "debugcategories.h"
#include "items/character.h"

#include <QtMath>

PlayerController::PlayerController(Character *player, QObject *parent) :
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
    case Qt::Key_E:
        setPrimaryActionPressed(true);
        break;
    case Qt::Key_Space:
        setSecondaryActionPressed(true);
        break;
    case Qt::Key_Q:
        qCDebug(dcPlayerController()) << "Inventory pressed";
        setInventoryPressed(true);
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
    case Qt::Key_E:
        setPrimaryActionPressed(false);
        break;
    case Qt::Key_Space:
        setSecondaryActionPressed(false);
        break;
    case Qt::Key_Q:
        setInventoryPressed(false);
        break;
    default:
        break;
    }
}

QPointF PlayerController::velocityVector()
{
    return delta();
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

    return deltaOffset;
}

void PlayerController::setForwardPressed(bool pressed)
{
    if (m_forwaredPressed == pressed)
        return;

    qCDebug(dcPlayerController()) << "Forwared" << (pressed ? "pressed" : "released");
    m_forwaredPressed = pressed;
    emit forwaredPressedChanged(m_forwaredPressed);

    if (m_forwaredPressed) emit forwardClicked();
}

void PlayerController::setBackwardPressed(bool pressed)
{
    if (m_backwardPressed == pressed)
        return;

    qCDebug(dcPlayerController()) << "Backwards" << (pressed ? "pressed" : "released");
    m_backwardPressed = pressed;
    emit backwardsPressedChanged(m_backwardPressed);
    if (m_backwardPressed) emit backwardClicked();
}

void PlayerController::setLeftPressed(bool pressed)
{
    if (m_leftPressed == pressed)
        return;

    qCDebug(dcPlayerController()) << "Left" << (pressed ? "pressed" : "released");
    m_leftPressed = pressed;
    emit leftPressedChanged(m_backwardPressed);
    if (m_leftPressed) emit leftClicked();
}

void PlayerController::setRightPressed(bool pressed)
{
    if (m_rightPressed == pressed)
        return;

    qCDebug(dcPlayerController()) << "Right" << (pressed ? "pressed" : "released");
    m_rightPressed = pressed;
    emit rightPressedChanged(m_rightPressed);
    if (m_rightPressed) emit rightClicked();
}

void PlayerController::setPrimaryActionPressed(bool pressed)
{
    if (m_primaryActionPressed == pressed)
        return;

    qCDebug(dcPlayerController()) << "Primary action" << (pressed ? "pressed" : "released");
    m_primaryActionPressed = pressed;
    emit primaryActionPressedChanged(m_primaryActionPressed);
    if (m_primaryActionPressed)
        emit primaryActionPressed();
}

void PlayerController::setSecondaryActionPressed(bool pressed)
{
    if (m_secondaryActionPressed == pressed)
        return;

    qCDebug(dcPlayerController()) << "Secondary action" << (pressed ? "pressed" : "released");
    m_secondaryActionPressed = pressed;
    emit secondaryActionPressedChanged(m_secondaryActionPressed);
    if (m_secondaryActionPressed)
        emit secondaryActionPressed();
}

void PlayerController::setInventoryPressed(bool pressed)
{
    if (m_inventoryPressed == pressed)
        return;

    m_inventoryPressed = pressed;
    if (m_inventoryPressed)
        emit inventoryPressed();
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

    return QPointF(deltaX, deltaY);
}

QPointF PlayerController::moveTouchscreen()
{
    return QPointF();
}
