import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import Qt.labs.settings 1.0

import Chronicles 1.0

ApplicationWindow {
    id: app
    visible: true
    width: 800
    height: 600
    title: qsTr("Self finding chronicles")
    visibility: ApplicationWindow.Windowed

    property int controlMode: PlayerController.ControlModeKeyBoard

    // General UI configurations
    property int margins: 14
    property int bigMargins: 20

    property int smallFont: 14
    property int mediumFont: 18
    property int largeFont: 24

    property int iconSize: 40
    property int delegateHeight: 60

    property string fontFamily: "EB Garamond"
    property string fontStyleName: "12 Regular"

    Settings {
        id: settings
        property string playerName: "Player"
        property alias viewMode: app.visibility
        property alias controlMode: app.controlMode

        onControlModeChanged: Game.world.playerController.controlMode = controlMode
    }

    StackView {
        id: pageStack
        anchors.fill: parent
    }

    Component.onCompleted: {
        Game.world.player.name = settings.playerName
        Game.world.playerController.controlMode = settings.controlMode

        pageStack.push(Qt.resolvedUrl("MainPage.qml"))
    }
}
