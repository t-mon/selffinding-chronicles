import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "components"

Page {
    id: root

    title: qsTr("Conversation creator")

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            HeaderButton {
                imageSource: dataDirectory + "/icons/back.svg"
                onClicked: pageStack.pop()
            }

            Label {
                text: qsTr("Conversation creator")
                elide: Label.ElideRight
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
        }
    }

    Item {
        anchors.fill: parent
        anchors.margins: app.margins

        GameLabel {
            id: welcomeLabel
            text: qsTr("Welcome to the dialog creator. This tool allowes to create in game dialogs in an convinient way.")
            anchors.centerIn: parent
        }
    }

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent

            Item { Layout.fillWidth: true }

            HeaderButton {
                imageSource: dataDirectory + "/icons/next.svg"
                onClicked: pageStack.push(dialogPropertiesComponent)
            }
        }
    }

    Component {
        id: dialogPropertiesComponent
        Page {
            id: dialogPropertiesPage

            header: ToolBar {
                RowLayout {
                    anchors.fill: parent
                    HeaderButton {
                        imageSource: dataDirectory + "/icons/back.svg"
                        onClicked: pageStack.pop()
                    }

                    Label {
                        text: qsTr("Conversation properties")
                        elide: Label.ElideRight
                        verticalAlignment: Qt.AlignVCenter
                        Layout.fillWidth: true
                    }
                }
            }

            GridLayout {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: app.margins

                columns: 2

                GameLabel {
                    text: qsTr("Title")
                }

                GameTextField {
                    id: conversationTitleTextField
                    text: ""
                    Layout.fillWidth: true
                }

                GameLabel {
                    text: qsTr("Description")
                }

                GameTextField {
                    id: descriptionTextField
                    text: ""
                    Layout.fillWidth: true
                }
            }
        }
    }
}
