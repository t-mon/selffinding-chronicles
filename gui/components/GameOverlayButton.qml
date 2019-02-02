import QtQuick 2.9

Item {
    id: root

    property string iconSource

    signal clicked()

    Rectangle {
        id: menuButton
        anchors.fill: parent
        width: app.gridSize * 2
        height: width
        radius:  width / 2
        color: "black"
        opacity: 0.6
    }

    Image {
        id: menuImage
        anchors.fill: parent
        anchors.margins: app.margins
        source: root.iconSource
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }

}
