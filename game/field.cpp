#include "field.h"

#include "gameobject.h"
#include "items/gameitems.h"

Field::Field(QPoint position, QObject *parent) :
    QObject(parent),
    m_position(position)
{
    m_gameItems = new GameItems(this);
    connect(m_gameItems, &GameItems::countChanged, this, &Field::onGameItemsCountChanged);

    m_collitionObject = new GameObject(this);
    m_collitionObject->setName("Field");
    m_collitionObject->setSize(QSize(1, 1));
    m_collitionObject->setShape(GameObject::ShapeRectangle);
    m_collitionObject->setPosition(m_position);
}

QPoint Field::position() const
{
    return m_position;
}

QString Field::imageName() const
{
    return m_imageName;
}

void Field::setImageName(const QString &imageName)
{
    if (m_imageName == imageName)
        return;

    m_imageName = imageName;
    emit imageNameChanged(m_imageName);
}

bool Field::accessible() const
{
    return m_accessible;
}

void Field::setAccessible(bool accessible)
{
    if (m_accessible == accessible)
        return;

    m_accessible = accessible;
    emit accessibleChanged(m_accessible);
}

bool Field::playerOnField() const
{
    return m_playerOnField;
}

void Field::setPlayerOnField(bool playerOnField)
{
    if (m_playerOnField == playerOnField)
        return;

    m_playerOnField = playerOnField;
    emit playerOnFieldChanged(m_playerOnField);
}

bool Field::inPlayerRange() const
{
    return m_inPlayerRange;
}

void Field::setInPlayerRange(bool inPlayerRange)
{
    if (m_inPlayerRange == inPlayerRange)
        return;

    m_inPlayerRange = inPlayerRange;
    emit inPlayerRangeChanged(m_inPlayerRange);
}

bool Field::hasItem() const
{
    return !m_gameItems->gameItems().isEmpty();
}

GameItems *Field::gameItems() const
{
    return m_gameItems;
}

GameObject *Field::collitionObject() const
{
    return m_collitionObject;
}

Field *Field::northField() const
{
    return m_northField;
}

Field *Field::northEastField() const
{
    return m_northEastField;
}

Field *Field::eastField() const
{
    return m_eastField;
}

Field *Field::southEastField() const
{
    return m_southEastField;
}

Field *Field::southField() const
{
    return m_southField;
}

Field *Field::southWestField() const
{
    return m_southWestField;
}

Field *Field::westField() const
{
    return m_westField;
}

Field *Field::northWestField() const
{
    return m_northWestField;
}

QList<Field *> Field::getSurroundingFields() const
{
    QList<Field *> fields;
    if (m_northField) fields.append(m_northField);
    if (m_northEastField) fields.append(m_northEastField);
    if (m_eastField) fields.append(m_eastField);
    if (m_southEastField) fields.append(m_southEastField);
    if (m_southField) fields.append(m_southField);
    if (m_southWestField) fields.append(m_southWestField);
    if (m_westField) fields.append(m_westField);
    if (m_northWestField) fields.append(m_northWestField);
    return fields;
}

void Field::setNorthField(Field *field)
{
    m_northField = field;
}

void Field::setNorthEastField(Field *field)
{
    m_northEastField = field;
}

void Field::setEastField(Field *field)
{
    m_eastField = field;
}

void Field::setSouthEastField(Field *field)
{
    m_southEastField = field;
}

void Field::setSouthField(Field *field)
{
    m_southField = field;
}

void Field::setSouthWestField(Field *field)
{
    m_southWestField = field;
}

void Field::setWestField(Field *field)
{
    m_westField = field;
}

void Field::setNorthWestField(Field *field)
{
    m_northWestField = field;
}

void Field::onGameItemsCountChanged(int count)
{
    Q_UNUSED(count);
    emit hasItemChanged(hasItem());
}

QDebug operator<<(QDebug debug, Field *field)
{
    debug.nospace() << "Field(" << field->position();
    debug.nospace() << ", " << (field->accessible() ? "accessable" : "unaccessable");
    debug.nospace() << ") ";
    return debug;
}
