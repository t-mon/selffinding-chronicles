import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import Chronicles 1.0

ApplicationWindow {
    id: app
    visible: true
    width: 800
    height: 600
    title: qsTr("Self finding chronicles")


    property int margins: 14
    property int bigMargins: 20
    property int smallFont: 14
    property int mediumFont: 16
    property int largeFont: 20
    property int iconSize: 30
    property int delegateHeight: 60

    StackView {
        id: pageStack
        anchors.fill: parent

        Keys.onPressed: Game.keyPressed(event.key)
        Keys.onReleased: Game.keyReleased(event.key)
    }

    Component.onCompleted: pageStack.push(Qt.resolvedUrl("MainPage.qml"))

}
