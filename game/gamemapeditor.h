#ifndef GAMEMAPEDITOR_H
#define GAMEMAPEDITOR_H

#include <QDir>
#include <QRectF>
#include <QObject>

#include "mapscene.h"
#include "datamanager.h"
#include "game/map.h"
#include "gameobjectsproxy.h"
#include "items/gameitems.h"

class GameMapEditor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Map *map READ map NOTIFY mapChanged)
    Q_PROPERTY(DataManager *dataManager READ dataManager CONSTANT)
    Q_PROPERTY(MapScene *mapScene READ mapScene CONSTANT)

    Q_PROPERTY(Tool tool READ tool WRITE setTool NOTIFY toolChanged)
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)

    Q_PROPERTY(GameObjects *availableObjects READ availableObjects CONSTANT)
    Q_PROPERTY(GameItems *availableItems READ availableItems CONSTANT)
    Q_PROPERTY(GameItems *availableCharacters READ availableCharacters CONSTANT)
    Q_PROPERTY(GameItems *availableEnemies READ availableEnemies CONSTANT)

    Q_PROPERTY(GameObject *selectedGameObject READ selectedGameObject NOTIFY selectedGameObjectChanged)
    Q_PROPERTY(GameItem *selectedGameItem READ selectedGameItem NOTIFY selectedGameItemChanged)



public:
    enum Tool {
        ToolSelect,
        ToolMove,
        ToolPlace
    };
    Q_ENUM(Tool)

    enum Mode {
        ModeObjects,
        ModeItems,
        ModeCharacters,
        ModeEnemies
    };
    Q_ENUM(Mode)

    explicit GameMapEditor(QObject *parent = nullptr);

    Map *map() const;
    MapScene *mapScene() const;
    DataManager *dataManager() const;

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

    GameMapEditor::Mode mode() const;
    void setMode(GameMapEditor::Mode mode);

    GameItem *selectedGameItem() const;
    Q_INVOKABLE void createSelectedGameItem(const QString &resourcePath);

    GameObject *selectedGameObject() const;
    void createSelectedGameObject(const QString &resourcePath);


    QRectF viewWindow() const;
    void setViewWindow(const QRectF &viewWindow);

    Q_INVOKABLE void createNewMap(const QString &mapName, const QSize &mapSize);
    Q_INVOKABLE void placeItemOnMap(const QString &resourcePath, const QPointF &position);
    Q_INVOKABLE void addBackgroundLightSource(LightSource::LightType lightType, const QColor &color, const QPointF &position);

    Q_INVOKABLE void removeGameItem(GameItem *item);
    Q_INVOKABLE void saveMap();
    Q_INVOKABLE void deleteAll();


private:
    Tool m_tool = ToolSelect;
    Mode m_mode = ModeItems;

    GameObjects *m_availableObjects = nullptr;
    GameItems *m_availableItems = nullptr;
    GameItems *m_availableCharacters = nullptr;
    GameItems *m_availableEnemies = nullptr;

    DataManager *m_dataManager = nullptr;
    MapScene *m_mapScene = nullptr;

    Map *m_map = nullptr;
    GameItem *m_selectedGameItem = nullptr;
    GameObject *m_selectedGameObject = nullptr;

signals:
    void toolChanged(GameMapEditor::Tool tool);
    void modeChanged(GameMapEditor::Mode mode);

    void availableItemsChanged(GameItems *gameItems);
    void availableObjectsChanged(GameObjects *gameObjects);
    void mapChanged(Map *map);
    void editorViewOffsetChanged(const QPointF &editorViewOffset);
    void selectedGameItemChanged(GameItem *selectedGameItem);
    void selectedGameObjectChanged(GameObject *selectedGameObject);

private slots:
    void onDataManagerStateChanged(DataManager::State state);

};

#endif // GAMEMAPEDITOR_H
