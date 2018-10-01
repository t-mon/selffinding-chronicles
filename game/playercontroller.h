#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>

class Player;

class PlayerController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ControlMode controlMode READ controlMode WRITE setControlMode NOTIFY controlModeChanged)

public:
    enum ControlMode {
        ControlModeKeyBoard,
        ControlModeKeyBoardMouse,
        ControlModeTouchscreen
    };
    Q_ENUM(ControlMode)

    explicit PlayerController(Player *player, QObject *parent = nullptr);

    ControlMode controlMode() const;
    void setControlMode(const ControlMode &controlMode);

    bool forwardPressed() const;
    bool backwardPressed() const;
    bool leftPressed() const;
    bool rightPressed() const;
    bool primaryActionPressed() const;
    bool secondaryActionPressed() const;

    void keyPressed(const Qt::Key &key);
    void keyReleased(const Qt::Key &key);

    QPointF delta();

private:
    Player *m_player = nullptr;
    ControlMode m_controlMode = ControlModeKeyBoard;

    bool m_forwaredPressed = false;
    bool m_backwardPressed = false;
    bool m_leftPressed = false;
    bool m_rightPressed = false;
    bool m_primaryActionPressed = false;
    bool m_secondaryActionPressed = false;

    void setForwardPressed(bool forwaredPressed);
    void setBackwardPressed(bool backwardPressed);
    void setLeftPressed(bool leftPressed);
    void setRightPressed(bool rightPressed);

    void setPrimaryActionPressed(bool primaryActionPressed);
    void setSecondaryActionPressed(bool secondaryActionPressed);

    QPointF moveKeyBoard();
    QPointF moveKeyBoardMouse();
    QPointF moveTouchscreen();

signals:
    void controlModeChanged(ControlMode controlMode);

public slots:

};

#endif // PLAYERCONTROLLER_H