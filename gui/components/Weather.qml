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


    GridLayout {
        id: weatherControllerGrid
        anchors.fill: parent

        property real weatherCellSize: 10 * app.gridSize

        rows: Math.round(root.height / weatherCellSize)
        columns: Math.round(root.width / weatherCellSize)

//        Repeater {
//            id: weatherControllerRepeater
//            anchors.fill: parent
//            model: weatherControllerGrid.rows * weatherControllerGrid.columns
//            delegate: WeatherController {
//                Layout.fillWidth: true
//                Layout.fillHeight: true
//                raining: root.raining
//                snowing: root.snowing
//                turbulence: debugControls.turbulenceEnabled
//            }
//        }

    }


}
