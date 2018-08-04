#include "collisiondetector.h"
#include "gameobject.h"
#include "debugcategories.h"

#include <QtMath>
#include <QVector2D>

CollisionDetector::CollisionDetector(QObject *parent) : QObject(parent)
{

}

bool CollisionDetector::checkCollision(GameObject *firstObject, GameObject *secondObject)
{
    qCDebug(dcCollision()) << "Check collision between" << firstObject << secondObject;

    if (firstObject->shape() == GameObject::ShapeCircle && secondObject->shape() == GameObject::ShapeRectangle)
        return checkRectangleCircleCollision(secondObject, firstObject);

    if (firstObject->shape() == GameObject::ShapeRectangle && secondObject->shape() == GameObject::ShapeCircle)
        return checkRectangleCircleCollision(firstObject, secondObject);

    if (firstObject->shape() == GameObject::ShapeCircle && secondObject->shape() == GameObject::ShapeCircle)
        return checkCircleCircleCollision(firstObject, secondObject);

    if (firstObject->shape() == GameObject::ShapeRectangle && secondObject->shape() == GameObject::ShapeRectangle)
        return checkRectangleRectangleCollision(firstObject, secondObject);

    // TODO: point

    return false;
}

bool CollisionDetector::checkRectangleRectangleCollision(GameObject *rectangleOne, GameObject *rectangleTwo)
{

    bool collisionX = rectangleOne->position().x() + rectangleOne->size().width() >= rectangleTwo->position().x() &&
            rectangleTwo->position().x() + rectangleTwo->size().width() >= rectangleOne->position().x();

    bool collisionY = rectangleOne->position().y() + rectangleOne->size().height() >= rectangleTwo->position().y() &&
            rectangleTwo->position().y() + rectangleTwo->size().height() >= rectangleOne->position().y();

    return collisionX && collisionY;
}

bool CollisionDetector::checkCircleCircleCollision(GameObject *firstCirlce, GameObject *secondCircle)
{
    Q_UNUSED(firstCirlce)
    Q_UNUSED(secondCircle)

    return false;
}

bool CollisionDetector::checkRectangleCircleCollision(GameObject *rectangle, GameObject *circle)
{
    float radius = static_cast<float>(circle->size().width() / 2.0);

    float w = static_cast<float>(rectangle->size().width() / 2.0);
    float h = static_cast<float>(rectangle->size().height() / 2.0);

    float dx = static_cast<float>((qAbs(circle->centerPosition().x() - rectangle->centerPosition().x())));
    float dy = static_cast<float>(qAbs(circle->centerPosition().y() - rectangle->centerPosition().y()));

    if (dx > (radius + w) || dy > (radius + h))
        return false;

    QVector2D circleDistance(static_cast<float>(qAbs(circle->centerPosition().x() - rectangle->centerPosition().x() - static_cast<double>(w))),
                             static_cast<float>(qAbs(circle->centerPosition().y() - rectangle->centerPosition().y() - static_cast<double>(h))));

    if (circleDistance.x() <= w)
        return true;

    if (circleDistance.y() <= h)
        return true;

    return circleDistance.length() <= static_cast<float>(qPow(static_cast<double>(radius), 2));
}

float CollisionDetector::getClamp(float value, float minValue, float maxValue)
{
    return qMax(minValue, qMin(maxValue, value));
}
