#ifndef GAMEMAPEDITOR_H
#define GAMEMAPEDITOR_H

#include <QObject>
#include <QDir>

#include "game/map.h"
#include "datamanager.h"
#include "gameobjectsproxy.h"
#include "items/gameitems.h"

class GameMapEditor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(GameMapEditor::Tool tool READ tool WRITE setTool NOTIFY toolChanged)
    Q_PROPERTY(GameObjects *availableObjects READ availableObjects CONSTANT)
    Q_PROPERTY(GameItems *availableItems READ availableItems CONSTANT)
    Q_PROPERTY(GameItems *availableCharacters READ availableCharacters CONSTANT)
    Q_PROPERTY(GameItems *availableEnemies READ availableEnemies CONSTANT)
    Q_PROPERTY(GameItem *selectedGameItem READ selectedGameItem NOTIFY selectedGameItemChanged)

    Q_PROPERTY(Map *map READ map NOTIFY mapChanged)
    Q_PROPERTY(DataManager *dataManager READ dataManager CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeItems READ activeItems CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeEnemies READ activeEnemies CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeCharacters READ activeCharacters CONSTANT)


public:
    enum Tool {
        ToolSelect,
        ToolMove,
        ToolPlace
    };
    Q_ENUM(Tool)

    explicit GameMapEditor(QObject *parent = nullptr);

    GameObjects *availableObjects() const;
    GameItems *availableItems() const;
    GameItems *availableCharacters() const;
    GameItems *availableEnemies() const;

    Q_INVOKABLE void loadAvailableGameItems();
    Q_INVOKABLE void loadAvailableObjects();
    Q_INVOKABLE void loadAvailableCharacters();
    Q_INVOKABLE void loadAvailableEnemies();

    GameMapEditor::Tool tool() const;
    void setTool(GameMapEditor::Tool tool);

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
    Tool m_tool = ToolSelect;
    GameObjects *m_availableObjects = nullptr;
    GameItems *m_availableItems = nullptr;
    GameItems *m_availableCharacters = nullptr;
    GameItems *m_availableEnemies = nullptr;

    DataManager *m_dataManager = nullptr;
    GameObjectsProxy *m_activeObjects = nullptr;
    GameItemsProxy *m_activeItems = nullptr;
    GameItemsProxy *m_activeEnemies = nullptr;
    GameItemsProxy *m_activeCharacters = nullptr;

    QPointF m_editorViewOffset;
    QSizeF m_editorViewSize;

    Map *m_map = nullptr;
    GameItem *m_selectedGameItem = nullptr;

signals:
    void toolChanged(GameMapEditor::Tool tool);
    void availableItemsChanged(GameItems *gameItems);
    void availableObjectsChanged(GameObjects *gameObjects);
    void mapChanged(Map *map);
    void selectedGameItemChanged(GameItem *selectedGameItem);

private slots:
    void onDataManagerStateChanged(DataManager::State state);

public slots:
    void onEditorViewOffsetChanged(const QPointF &offset);
    void onEditorViewSizeChanged(const QSizeF &size);

};

#endif // GAMEMAPEDITOR_H
