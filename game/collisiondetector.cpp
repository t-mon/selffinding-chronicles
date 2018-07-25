#include "collisiondetector.h"
#include "gameobject.h"
#include "debugcategories.h"

CollisionDetector::CollisionDetector(QObject *parent) : QObject(parent)
{

}

bool CollisionDetector::checkCollision(GameObject *firstObject, GameObject *secondObject)
{
//    qCDebug(dcCollision()) << "Checking" << rectangleOne << rectangleTwo;
//    if (firstObject->shape() == GameObject::ShapeRectangle && secondObject->shape() == GameObject::ShapeRectangle) {
//        return checkRectangleRectangleCollision(firstObject, secondObject);
//    }

//    qCWarning(dcCollision()) << "Missing collition detection for" << firstObject->shape() << secondObject->shape();
//    return false;
}

bool CollisionDetector::checkRectangleRectangleCollision(GameObject *rectangleOne, GameObject *rectangleTwo)
{
    bool collisionX = rectangleOne->centerPosition().x() + rectangleOne->size().width() / 2 >= rectangleTwo->centerPosition().x() - rectangleOne->size().width() / 2 &&
            rectangleTwo->position().x() + rectangleTwo->size().width() / 2 >= rectangleOne->position().x();

    bool collisionY = rectangleOne->position().y() + rectangleOne->size().height() >= rectangleTwo->position().y() &&
            rectangleTwo->position().y() + rectangleTwo->size().height() >= rectangleOne->position().y();

    return collisionX && collisionY;
}

bool CollisionDetector::checkCircleCircleCollision(GameObject *circleOne, GameObject *circleTwo)
{
    return false;
}

bool CollisionDetector::checkRectangleCircleCollision(GameObject *rectangle, GameObject *circle)
{
    return false;
}
