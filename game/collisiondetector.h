#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include <QObject>

class Field;
class GameObject;

class CollisionDetector : public QObject
{
    Q_OBJECT
public:
    explicit CollisionDetector(QObject *parent = nullptr);

    bool checkCollision(GameObject *firstObject, GameObject *secondObject);
    qreal calculateCenterDistance(GameObject *firstObject, GameObject *secondObject);

private:
    bool checkRectangleRectangleCollision(GameObject *rectangleOne, GameObject *rectangleTwo);
    bool checkCircleCircleCollision(GameObject *firstCirlce, GameObject *secondCircle);
    bool checkRectangleCircleCollision(GameObject *rectangle, GameObject *circle);
};

#endif // COLLISIONDETECTOR_H
