#include "gamemapeditor.h"
#include "debugcategories.h"
#include "dataloader.h"

GameMapEditor::GameMapEditor(QObject *parent) : QObject(parent)
{
    m_availableItems = new GameItems(this);
    loadAvailableGameItems();
}

GameItems *GameMapEditor::availableItems() const
{
    return m_availableItems;
}

void GameMapEditor::loadAvailableGameItems()
{
    QString gameDataPath = ":/gamedata/";
    QDir resourceDirectory(gameDataPath);
    foreach (const QString &gameDataEntry, resourceDirectory.entryList()) {
        qCDebug(dcMapEditor()) << "--> Resource entry" << QString(gameDataPath + gameDataEntry);
        QDir typeDirectory(gameDataPath + gameDataEntry);
        foreach (const QString &entry, typeDirectory.entryList()) {
            qCDebug(dcMapEditor()) << "          --> entry" << typeDirectory.path() + "/" + entry;
            m_availableItems->addGameItem(DataLoader::loadGameItemFromResourcePath(typeDirectory.path() + "/" + entry));
        }
    }
}
