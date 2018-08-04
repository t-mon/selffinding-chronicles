#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include <QObject>

class GameObject;

class CollisionDetector : public QObject
{
    Q_OBJECT
public:
    explicit CollisionDetector(QObject *parent = nullptr);

    bool checkCollision(GameObject *firstObject, GameObject *secondObject);

private:
    bool checkRectangleRectangleCollision(GameObject *rectangleOne, GameObject *rectangleTwo);
    bool checkCircleCircleCollision(GameObject *firstCirlce, GameObject *secondCircle);
    bool checkRectangleCircleCollision(GameObject *rectangle, GameObject *circle);

    float getClamp(float value, float minValue, float maxValue);
};

#endif // COLLISIONDETECTOR_H
