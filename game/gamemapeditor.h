#ifndef GAMEMAPEDITOR_H
#define GAMEMAPEDITOR_H

#include <QObject>
#include <QDir>

#include "items/gameitems.h"

class GameMapEditor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(GameItems *availableItems READ availableItems NOTIFY availableItemsChanged)

public:
    explicit GameMapEditor(QObject *parent = nullptr);

    GameItems *availableItems() const;

    void loadAvailableGameItems();

private:
    GameItems *m_availableItems = nullptr;

signals:
    void availableItemsChanged(GameItems *gameItems);

public slots:


};

#endif // GAMEMAPEDITOR_H
