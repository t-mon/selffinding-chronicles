import QtQuick 2.9
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3

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

    // Theme
    Material.theme: Material.Dark

    Material.accent: Material.BlueGrey
    Material.primary: Material.BlueGrey
    Material.foreground: Material.BlueGrey

    property int controlMode: PlayerController.ControlModeKeyBoard
    property string gameName: qsTr("Self finding chronicles")

    // This is the main scaling property. Everything scales with the gridSize
    property real gridSize: Math.min(app.width / 30, app.height / 20)
    property real menuItemSize: gridSize * 2.5

    // General UI configurations
    property real margins: gridSize / 2
    property real bigMargins: gridSize * 2 / 3
    property real borderWidth: gridSize / 14

    property real smallFont: gridSize * 0.5
    property real mediumFont: gridSize * 0.7
    property real largeFont: gridSize

    property real iconSize: 40
    property real delegateHeight: 60

    property string fontFamily: gameFont.name
    property string fontMonoFamily: gameMonoFont.name
    property bool antialiasing: false

    property color backgroundColor: "#333333"
    property color healthColor: "#9c2f25"
    property color manaColor: "#0765d4"

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
        source: "/fonts/blackchancery/blkchcry.ttf"
    }

    FontLoader {
        id: gameMonoFont
        source: "/fonts/DejaVu/DejaVuSansMono.ttf"
    }

    StackView {
        id: pageStack
        anchors.fill: parent

        Loader {
            id: gamePageLoader
            anchors.fill: parent
            asynchronous: true
        }
    }

    Component.onCompleted: {
        app.visibility = Game.settings.visibility
        pageStack.push(Qt.resolvedUrl("MainPage.qml"))
    }
}
