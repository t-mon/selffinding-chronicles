import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0
import "components"

GamePage {
    id: root

    ListView {
        anchors.fill: parent
        model: Game.engine.dataManager.saveGames
        delegate: Item {
            height: app.gridSize * 3
            width: parent.width

            Rectangle {
                anchors.fill: parent
                color: "white"
            }

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: app.margins
                anchors.rightMargin: app.margins

                Item {
                    Layout.fillHeight: true
                    Layout.preferredWidth: app.gridSize * 4

                    Image {
                        id: screenshotImage
                        source: Qt.resolvedUrl("file:/" + model.screenshotFileName)
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                    }
                }

                GameLabel {
                    Layout.fillWidth: true
                    text: model.mapName
                }

                GameLabel {
                    text: model.dateTimeString
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Game.engine.dataManager.loadGame(Game.engine.dataManager.saveGames.get(model.index))
                    pageStack.push(Qt.resolvedUrl("LoadingPage.qml"))
                }
            }
        }
    }
}
