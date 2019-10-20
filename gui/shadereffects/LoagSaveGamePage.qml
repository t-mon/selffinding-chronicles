import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0
import "components"

GamePage {
    id: root


    Item {
        anchors.fill: parent
        anchors.margins: app.margins * 8


        ListModel {
            ListElement {
                name: "Test environment"
                number: "555 3264"
            }
            ListElement {
                name: "Test map"
                number: "555 8426"
            }
            ListElement {
                name: "Test monster"
                number: "555 0473"
            }
        }

        ListView {
            anchors.fill: parent



        }


    }
}
