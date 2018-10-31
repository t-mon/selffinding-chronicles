import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.1

import Chronicles 1.0

ToolBar {
    id: root
    Material.elevation: 1

    property string text
    property alias backButtonVisible: backButton.visible

    signal backPressed();

    RowLayout {
        id: layout
        //anchors { fill: parent; leftMargin: app.margins; rightMargin: app.margins }

        ToolButton {
            id: backButton
            objectName: "backButton"
            text: "<"
            //imageSource: "../images/back.svg"
            onClicked: root.backPressed();
        }

        GameLabel {
            id: label
            Layout.fillWidth: true
            Layout.fillHeight: true
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            text: root.text
        }
    }
}
