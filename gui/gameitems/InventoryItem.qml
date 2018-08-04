import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: "#c2c2c2ff"
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Rectangle {
                id: inventoryBackground
                Layout.fillWidth: true
                Layout.minimumHeight: root.height * 0.1
                color: "blue"; opacity: 0.5;
            }

        }

        RowLayout {

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "red"; opacity: 0.5;
            }


            Rectangle {
                Layout.minimumWidth: root.width * 0.2
                Layout.fillHeight: true
                color: "yellow"; opacity: 0.5;
            }
        }

        RowLayout {

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: root.height * 0.1
                color: "green"; opacity: 0.5;
            }
        }

    }

}
