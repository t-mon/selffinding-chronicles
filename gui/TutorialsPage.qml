import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0
import "components"

GamePage {
    id: root
    header: ChroniclesHeader {
        text: qsTr("Tutorials")
        backButtonVisible: true
        onBackPressed: pageStack.pop()
    }

    ListModel {
        id: tutorialsModel
        ListElement {
            name: "Character"
            mapPath: ":/maps/test-character.json"
        }
        ListElement {
            name: "Monster"
            mapPath: ":/maps/test-monster.json"
        }
        ListElement {
            name: "Door"
            mapPath: ":/maps/test-door.json"
        }
        ListElement {
            name: "Chest"
            mapPath: ":/maps/test-chest.json"
        }
    }


    ListView {
        anchors.fill: parent
        anchors.margins: app.margins
        model: tutorialsModel
        spacing: app.margins
        delegate: GameButton {
            text: model.name
            width: parent.width
            onClicked: {
                Game.engine.dataManager.startTutorial(model.mapPath)
                pageStack.push(Qt.resolvedUrl("LoadingPage.qml"))
            }
        }
    }
}
