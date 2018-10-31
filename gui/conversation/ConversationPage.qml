import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property Conversation conversation: Game.world.currentConversation

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.1
    }

    states: [
        State {
            name: "playerTalking"
            when: conversation && conversation.conversationItem.speeker === ConversationItem.SpeekerPlayer && conversation.conversationItem.type === ConversationItem.TypeText
            PropertyChanges { target: textItemPlayer; visible: true }
            PropertyChanges { target: textItemCharacter; visible: false }
            PropertyChanges { target: choiseItem; visible: false }
        },
        State {
            name: "characterTalking"
            when: conversation && conversation.conversationItem.speeker === ConversationItem.SpeekerCharacter && conversation.conversationItem.type === ConversationItem.TypeText
            PropertyChanges { target: textItemPlayer; visible: false }
            PropertyChanges { target: textItemCharacter; visible: true }
            PropertyChanges { target: choiseItem; visible: false }
        },
        State {
            name: "choise"
            when: conversation && conversation.conversationItem.speeker === ConversationItem.SpeekerPlayer && conversation.conversationItem.type === ConversationItem.TypeChoise
            PropertyChanges { target: textItemPlayer; visible: false }
            PropertyChanges { target: textItemCharacter; visible: false }
            PropertyChanges { target: choiseItem; visible: true }
        }

    ]

    Item {
        id: textItemCharacter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: app.margins
        height: root.height / 2

        Rectangle {
            anchors.fill: parent
            opacity: 0.6
            color: "steelblue"
            radius: app.gridSize
        }

        ScaleAnimator {
            id: characterTextPopupAnimation
            target: textItemCharacter
            loops: 1
            from: 0
            to: 1
            duration: 200
            easing.type: Easing.OutCubic
        }
        onVisibleChanged: characterTextPopupAnimation.start()

        ColumnLayout {
            anchors.centerIn: parent

            GameLabel {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
                text: conversation ? conversation.character.name + ": " : ""
                font.pixelSize: app.largeFont
                color: "white"
            }

            GameLabel {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                text: conversation ? conversation.conversationItem.visibleText : ""
                horizontalAlignment: Text.AlignHCenter
                textFormat: Text.PlainText
                font.family: "Monospace"
                color: "white"
            }
        }

        Connections {
            target: Game.world.playerController
            onPrimaryActionPressedChanged: {
                if (pressed && textItemCharacter.visible) {
                    conversation.confirmPressed()
                }
            }

            onSecondaryActionPressedChanged: {
                if (pressed && textItemCharacter.visible) {
                    conversation.confirmPressed()
                }
            }
        }

    }

    Item {
        id: textItemPlayer
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: app.margins
        height: root.height / 2

        Rectangle {
            anchors.fill: parent
            opacity: 0.6
            color: "yellow"
            radius: app.gridSize
        }

        ScaleAnimator {
            id: playerTextPopupAnimation
            target: textItemPlayer
            loops: 1
            from: 0
            to: 1
            duration: 200
            easing.type: Easing.OutCubic
        }
        onVisibleChanged: playerTextPopupAnimation.start()

        ColumnLayout {
            anchors.centerIn: parent

            GameLabel {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
                text: Game.world.player.name + ": "
                font.pixelSize: app.largeFont
                color: "white"
            }

            GameLabel {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                text: conversation ? conversation.conversationItem.visibleText : ""
                horizontalAlignment: Text.AlignHCenter
                textFormat: Text.PlainText
                font.family: "Monospace"
                color: "white"
            }
        }

        Connections {
            target: Game.world.playerController
            onPrimaryActionPressedChanged: {
                if (pressed && textItemPlayer.visible) {
                    conversation.confirmPressed()
                }
            }

            onSecondaryActionPressedChanged: {
                if (pressed && textItemPlayer.visible) {
                    conversation.confirmPressed()
                }
            }
        }
    }

    Item {
        id: choiseItem
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: app.margins
        height: root.height / 2

        Rectangle {
            anchors.fill: parent
            opacity: 0.6
            color: "gray"
            radius: app.gridSize
        }

        ScaleAnimator {
            id: choisePopupAnimation
            target: choiseItem
            loops: 1
            from: 0
            to: 1
            duration: 200
            easing.type: Easing.OutCubic
        }
        onVisibleChanged: choisePopupAnimation.start()

        Component {
            id: choiseComponent
            ItemDelegate {
                id: choiseDelegate
                width: parent.width
                highlighted: ListView.isCurrentItem
                onClicked: {
                    console.log("Player selected " + model.uuid + " --> " + model.text )
                    choisesListView.currentIndex = index
                    conversation.selectItem(model.uuid)
                }

                GameLabel {
                    id: choiseText
                    text: model.text
                    anchors.left: parent.left
                    anchors.leftMargin: app.margins
                    anchors.verticalCenter: parent.verticalCenter
                    textFormat: Text.PlainText
                    font.family: "Monospace"
                    color: choiseDelegate.ListView.isCurrentItem ? "black" : "gray"
                }
            }
        }

        ListView {
            id: choisesListView
            anchors.fill: parent
            anchors.margins: app.gridSize
            model: conversation ? conversation.conversationItem.choises : 0
            delegate: choiseComponent

            Connections {
                target: Game.world.playerController
                onForwaredPressedChanged: {
                    if (pressed && choiseItem.visible) {
                        console.log("Choise up")
                        if (choisesListView.currentIndex - 1  < 0) {
                            choisesListView.currentIndex = conversation.conversationItem.choises.count() - 1
                            console.log("Current choise index " + choisesListView.currentIndex)
                        } else {
                            choisesListView.currentIndex -= 1
                        }
                    }
                }
                onBackwardsPressedChanged: {
                    if (pressed && choiseItem.visible) {
                        console.log("Choise down")
                        if (choisesListView.currentIndex + 1  >= conversation.conversationItem.choises.count()) {
                            choisesListView.currentIndex = 0
                            console.log("Current choise index " + choisesListView.currentIndex)
                        } else {
                            choisesListView.currentIndex += 1
                        }
                    }
                }

                onPrimaryActionPressedChanged: {
                    if (pressed && choiseItem.visible) {
                        console.log("Player selected " + conversation.conversationItem.choises.get(choisesListView.currentIndex).uuid + " --> " + conversation.conversationItem.choises.get(choisesListView.currentIndex).text )
                        conversation.selectItem(conversation.conversationItem.choises.get(choisesListView.currentIndex).uuid)
                    }
                }

                onSecondaryActionPressedChanged: {
                    if (pressed && choiseItem.visible) {
                        console.log("Player selected " + conversation.conversationItem.choises.get(choisesListView.currentIndex).uuid + " --> " + conversation.conversationItem.choises.get(choisesListView.currentIndex).text )
                        conversation.selectItem(conversation.conversationItem.choises.get(choisesListView.currentIndex).uuid)
                    }
                }
            }
        }
    }
}
