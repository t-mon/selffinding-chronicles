#include "literatureitem.h"
#include "../debugcategories.h"

LiteratureItem::LiteratureItem(QObject *parent) :
    GameItem(parent)
{
    setInteraction(GameItem::InteractionPick);
}

QString LiteratureItem::itemTypeName() const
{
    return tr("Literature");
}

GameItem::Type LiteratureItem::itemType() const
{
    return TypeLiterature;
}

void LiteratureItem::performInteraction()
{
    qCDebug(dcItem()) << itemTypeName() << name() << "perform interaction" << m_interaction;
}

LiteratureItem::LiteratureType LiteratureItem::literatureType() const
{
    return m_literatureType;
}

void LiteratureItem::setLiteratureType(LiteratureItem::LiteratureType literatureType)
{
    if (m_literatureType == literatureType)
        return;

    m_literatureType = literatureType;
    emit literatureTypeChanged(m_literatureType);
}

QString LiteratureItem::title() const
{
    return m_title;
}

void LiteratureItem::setTitle(const QString &title)
{
    if (m_title == title)
        return;

    m_title = title;
    emit titleChanged(m_title);
}

QString LiteratureItem::description() const
{
    return m_description;
}

void LiteratureItem::setDescription(const QString &description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged(m_description);
}

QString LiteratureItem::text() const
{
    return m_text;
}

void LiteratureItem::setText(const QString &text)
{
    if (m_text == text)
        return;

    m_text = text;
    emit textChanged(m_text);
}

QDebug operator<<(QDebug debug, LiteratureItem *literatureItem)
{
    debug.nospace() << "Literature("<< literatureItem->literatureType();
    debug.nospace() << ", " << literatureItem->name();
    debug.nospace() << ", " << literatureItem->description();
    debug.nospace() << ")";
    return debug.space();
}
