#include "field.h"
#include "gameobject.h"
#include "debugcategories.h"
#include "collisiondetector.h"

#include <QtMath>
#include <QVector2D>

CollisionDetector::CollisionDetector(QObject *parent) :
    QObject(parent)
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

    // TODO: object point collision for bullets and arrows

    return false;
}

double CollisionDetector::calculateCenterDistance(GameObject *firstObject, GameObject *secondObject)
{
    double dx = firstObject->centerPosition().x() - secondObject->centerPosition().x();
    double dy = firstObject->centerPosition().y() - secondObject->centerPosition().y();
    return qSqrt(dx * dx + dy * dy);
}

bool CollisionDetector::checkRectangleRectangleCollision(GameObject *rectangleOne, GameObject *rectangleTwo)
{
    bool collisionX = rectangleOne->position().x() + rectangleOne->size().width() >= rectangleTwo->position().x()
            && rectangleTwo->position().x() + rectangleTwo->size().width() >= rectangleOne->position().x();

    bool collisionY = rectangleOne->position().y() + rectangleOne->size().height() >= rectangleTwo->position().y()
            && rectangleTwo->position().y() + rectangleTwo->size().height() >= rectangleOne->position().y();

    return collisionX && collisionY;
}

bool CollisionDetector::checkCircleCircleCollision(GameObject *firstCirlce, GameObject *secondCircle)
{
    double centerDistance = calculateCenterDistance(firstCirlce, secondCircle);
    double minimalDistance = (static_cast<double>(firstCirlce->size().width()) / 2.0 + static_cast<double>(secondCircle->size().width()) / 2.0);
    return centerDistance <= minimalDistance;
}

bool CollisionDetector::checkRectangleCircleCollision(GameObject *rectangle, GameObject *circle)
{
    double radius = circle->size().width() / 2.0;
    double dx = circle->centerPosition().x() - qMax(rectangle->position().x(), qMin(circle->centerPosition().x(), rectangle->position().x() + rectangle->size().width()));
    double dy = circle->centerPosition().y() - qMax(rectangle->position().y(), qMin(circle->centerPosition().y(), rectangle->position().y() + rectangle->size().height()));

    return (dx * dx + dy * dy) < (radius * radius);
}
