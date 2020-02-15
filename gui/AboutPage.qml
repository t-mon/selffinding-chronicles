import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "components"

Page {
    id: root


    header: ChroniclesHeader {
        text: qsTr("About %1").arg(app.title)
        backButtonVisible: true
        onBackPressed: pageStack.pop()
    }

    property var music: Game.soundEngine.registerMusicStream(dataDirectory + "/sounds/ambient/highflow-river.ogg")
    Component.onCompleted: music.play()
    Component.onDestruction: Game.soundEngine.unregisterMusicStream(root.music)

    GameSoundEffect {
        id: inventorySoundEffect
        source: dataDirectory + "/sounds/game/effects/coins-several-04.wav"
    }

    Flickable {
        anchors.fill: parent
        contentHeight: settingsColumn.implicitHeight
        anchors.margins: app.margins

        ColumnLayout {
            id: settingsColumn
            width: parent.width

            RowLayout {
                Layout.fillWidth: true

                spacing: app.margins

                GameLabel {
                    Layout.fillWidth: true
                    font.pixelSize: app.mediumFont
                    text: qsTr("Developer")
                }

                GameLabel {
                    Layout.fillWidth: true
                    font.pixelSize: app.mediumFont
                    text: "Simon Stürz"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: inventorySoundEffect.play()
                    }
                }
            }
        }
    }
}
