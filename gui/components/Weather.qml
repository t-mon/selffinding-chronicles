import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Particles 2.0

import Chronicles 1.0

Item {
    id: root

    property bool snowing: false
    property bool raining: false
    property bool turbulence: false

    Repeater {
        id: weatherRepeater
        anchors.fill: parent
        model: Game.engine.mapScene.activeWeatherAreas
        delegate: WeatherController {
            raining: root.raining
            snowing: root.snowing
            turbulence: debugControls.turbulenceEnabled
            width: model.size.width * app.gridSize
            height: model.size.height * app.gridSize
            x: model.position.x * app.gridSize
            y: model.position.y * app.gridSize
        }
    }
}

