#ifndef GAMEMAPEDITOR_H
#define GAMEMAPEDITOR_H

#include <QObject>
#include <QDir>

#include "game/map.h"
#include "datamanager.h"
#include "items/gameitems.h"

class GameMapEditor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(GameItems *availableItems READ availableItems NOTIFY availableItemsChanged)
    Q_PROPERTY(GameItem *selectedGameItem READ selectedGameItem NOTIFY selectedGameItemChanged)

    Q_PROPERTY(Map *map READ map NOTIFY mapChanged)
    Q_PROPERTY(DataManager *dataManager READ dataManager CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeItems READ activeItems CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeEnemies READ activeEnemies CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeCharacters READ activeCharacters CONSTANT)


public:
    explicit GameMapEditor(QObject *parent = nullptr);

    GameItems *availableItems() const;    
    Q_INVOKABLE void loadAvailableGameItems();

    GameItem *selectedGameItem() const;
    Q_INVOKABLE void createSelectedGameItem(const QString &resourcePath);

    DataManager *dataManager() const;
    GameItemsProxy *activeItems() const;
    GameItemsProxy *activeCharacters() const;
    GameItemsProxy *activeEnemies() const;


    Map *map() const;
    Q_INVOKABLE void createNewMap();
    Q_INVOKABLE void placeItemOnMap(const QString &resourcePath, const QPointF &position);
    Q_INVOKABLE void saveMap();

private:
    GameItems *m_availableItems = nullptr;

    DataManager *m_dataManager = nullptr;
    GameItemsProxy *m_activeItems = nullptr;
    GameItemsProxy *m_activeEnemies = nullptr;
    GameItemsProxy *m_activeCharacters = nullptr;

    QPointF m_editorViewOffset;
    QSizeF m_editorViewSize;

    Map *m_map = nullptr;
    GameItem *m_selectedGameItem = nullptr;

signals:
    void availableItemsChanged(GameItems *gameItems);
    void mapChanged(Map *map);
    void selectedGameItemChanged(GameItem *selectedGameItem);

private slots:
    void onDataManagerStateChanged(DataManager::State state);

public slots:
    void onEditorViewOffsetChanged(const QPointF &offset);
    void onEditorViewSizeChanged(const QSizeF &size);

};

#endif // GAMEMAPEDITOR_H
