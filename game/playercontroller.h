#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QPointF>
#include <QObject>

class Character;

class PlayerController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Character *player READ player NOTIFY playerChanged)
    Q_PROPERTY(ControlMode controlMode READ controlMode WRITE setControlMode NOTIFY controlModeChanged)
    Q_PROPERTY(QPointF movementVector READ movementVector)

public:
    enum ControlMode {
        ControlModeKeyBoard,
        ControlModeKeyBoardMouse,
        ControlModeTouchscreen
    };
    Q_ENUM(ControlMode)

    explicit PlayerController(QObject *parent = nullptr);

    ControlMode controlMode() const;
    void setControlMode(const ControlMode &controlMode);

    Character *player() const;
    void setPlayer(Character *player);

    bool forwardPressed() const;
    bool backwardPressed() const;
    bool leftPressed() const;
    bool rightPressed() const;

    bool primaryActionPressed() const;
    bool secondaryActionPressed() const;

    void keyPressed(const Qt::Key &key);
    void keyReleased(const Qt::Key &key);

    Q_INVOKABLE void clickPrimaryAction();
    Q_INVOKABLE void clickSecondaryAction();

    QPointF movementVector();

private:
    Character *m_player = nullptr;
    ControlMode m_controlMode = ControlModeKeyBoard;

    QPointF m_velocityVector;

    bool m_forwaredPressed = false;
    bool m_backwardPressed = false;
    bool m_leftPressed = false;
    bool m_rightPressed = false;

    bool m_primaryActionPressed = false;
    bool m_secondaryActionPressed = false;
    bool m_inventoryPressed = false;
    bool m_shootPressed = false;

    QPointF m_joystickVector;
    double m_joystickVelocity = 0;
    double m_joystickAngle = 0;

    void setForwardPressed(bool pressed);
    void setBackwardPressed(bool pressed);
    void setLeftPressed(bool pressed);
    void setRightPressed(bool pressed);

    void setPrimaryActionPressed(bool pressed);
    void setSecondaryActionPressed(bool pressed);
    void setInventoryPressed(bool pressed);
    void setShootPressed(bool pressed);

    QPointF moveKeyBoard();
    QPointF moveKeyBoardMouse();
    QPointF moveTouchscreen();

signals:
    void playerChanged(Character *player);
    void controlModeChanged(ControlMode controlMode);

    void forwaredPressedChanged(bool pressed);
    void backwardsPressedChanged(bool pressed);
    void leftPressedChanged(bool pressed);
    void rightPressedChanged(bool pressed);
    void primaryActionPressedChanged(bool pressed);
    void secondaryActionPressedChanged(bool pressed);

    void forwardClicked();
    void backwardClicked();
    void leftClicked();
    void rightClicked();

    void primaryActionClicked();
    void secondaryActionClicked();

    void weaponPressed();
    void firearmPressed();
    void inventoryPressed();
    void shoot();
    void escape();
    void beam();

public slots:
    Q_INVOKABLE void onJoystickChanged(const QPointF &joystickVector, double velocity, double angle);

};

#endif // PLAYERCONTROLLER_H
