#ifndef WORLD_H
#define WORLD_H

#include <QSize>
#include <QPoint>
#include <QObject>

#include "fields.h"
#include "player.h"

class World : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QSize sceneSize READ sceneSize WRITE setSceneSize NOTIFY sceneSizeChanged)
    Q_PROPERTY(QSize boundingSize READ boundingSize WRITE setBoundingSize NOTIFY boundingSizeChanged)
    Q_PROPERTY(QPointF boundingPosition READ boundingPosition NOTIFY boundingPositionChanged)
    Q_PROPERTY(Player *player READ player CONSTANT)
    Q_PROPERTY(int cellSize READ cellSize WRITE setCellSize NOTIFY cellSizeChanged)

    Q_PROPERTY(QPoint currentPlayerField READ currentPlayerField NOTIFY currentPlayerFieldChanged)
    Q_PROPERTY(Fields *occupiedFields READ occupiedFields CONSTANT)

public:
    explicit World(QObject *parent = nullptr);

    QSize size() const;
    void setSize(const QSize &size);

    QSize sceneSize() const;
    void setSceneSize(const QSize &sceneSize);

    QSize boundingSize() const;
    void setBoundingSize(const QSize &boundingSize);


    int cellSize() const;
    void setCellSize(const int &cellSize);

    Player *player();

    Fields *occupiedFields();

    QPoint currentPlayerField() const;
    QPointF boundingPosition() const;

    void setForwardPressed(bool forwaredPressed);
    void setBackwardPressed(bool backwardPressed);
    void setLeftPressed(bool leftPressed);
    void setRightPressed(bool rightPressed);

private:
    QSize m_size;
    QSize m_sceneSize;
    QSize m_boundingSize;
    int m_cellSize;

    Player *m_player = nullptr;

    bool m_forwaredPressed = false;
    bool m_backwardPressed = false;
    bool m_leftPressed = false;
    bool m_rightPressed = false;

    QPoint m_currentPlayerField;
    QPointF m_boundingPosition;

    Fields *m_occupiedFields = nullptr;

    void setCurrentPlayerField(const QPoint &currentField);

    void setboundingPosition(const QPointF &boundingPosition);

    void moveKeyBoard();
    void moveKeyBoardMouse();
    void moveTouchscreen();

    void moveCamera();

    void movePlayer(const QPointF newPosition);

signals:
    void sizeChanged(const QSize &size);
    void sceneSizeChanged(const QSize &sceneSize);
    void boundingSizeChanged(const QSize &boundingSize);
    void cellSizeChanged(const int &cellSize);

    void currentPlayerFieldChanged(const QPoint &currentPlayerField);
    void boundingPositionChanged(const QPointF &boundingPosition);

private slots:
    void onPlayerPositionChanged();

public slots:
    void tick();

};

#endif // WORLD_H
