import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import Qt.labs.settings 1.0

import Chronicles 1.0

ApplicationWindow {
    id: app
    title: gameName
    visible: true
    width: Game.settings.windowSize.width
    height: Game.settings.windowSize.height
    x: Game.settings.windowPosition.x
    y: Game.settings.windowPosition.y
    visibility: ApplicationWindow.Windowed

    property int controlMode: PlayerController.ControlModeKeyBoard
    property string gameName: qsTr("Self finding chronicles")

    // General UI configurations
    property int margins: 14
    property int bigMargins: 20
    property int borderWidth: 3

    property int smallFont: 14
    property int mediumFont: 18
    property int largeFont: 24

    property int iconSize: 40
    property int delegateHeight: 60

    property string fontFamily: gameFont.name
    property real gridSize: Math.min(app.width / 30, app.height / 20)
    property bool antialiasing: false

    property color backgroundColor: "#333333"

    onWidthChanged: Game.settings.windowSize = Qt.size(app.width, app.height)
    onHeightChanged: Game.settings.windowSize = Qt.size(app.width, app.height)
    onXChanged: Game.settings.windowPosition = Qt.point(app.x, app.y)
    onYChanged: Game.settings.windowPosition = Qt.point(app.x, app.y)

    Connections {
        id: settingsConnections
        target: Game.settings
        onVisibilityChanged: app.visibility = Game.settings.visibility
    }

    FontLoader {
        id: gameFont
        //source: "/fonts/EBGaramond/EBGaramond12-Regular.ttf"
        source: "/fonts/blackchancery/blkchcry.ttf"
    }

    StackView {
        id: pageStack
        anchors.fill: parent
    }

    Component.onCompleted: {
        app.visibility = Game.settings.visibility
        pageStack.push(Qt.resolvedUrl("MainPage.qml"))
    }
}
