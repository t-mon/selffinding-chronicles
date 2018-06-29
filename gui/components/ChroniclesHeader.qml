import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.1
import Chronicles 1.0

ToolBar {
    id: root
    Material.elevation: 1

    property string text
    property alias backButtonVisible: backButton.visible
    //property alias menuButtonVisible: menuButton.visible

    signal backPressed();
    signal menuPressed();

    RowLayout {
        id: layout
        //anchors { fill: parent; leftMargin: app.margins; rightMargin: app.margins }

        ToolButton {
            id: backButton
            objectName: "backButton"
            //imageSource: "../images/back.svg"
            onClicked: root.backPressed();
        }
        Label {
            id: label
            Layout.fillWidth: true
            Layout.fillHeight: true
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            text: root.text
        }
    }
}
