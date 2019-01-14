import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import Chronicles 1.0

Item {
    id: root

    Rectangle {
        id: backgroundItem
        anchors.fill: parent
        anchors.margins: app.margins / 2
        color: "gray"
        border.color: "white"
        border.width: app.borderWidth
        radius: app.margins / 2
        opacity: 0.4
    }

    ColumnLayout {
        id: indicatorColumn
        anchors.fill: backgroundItem
        anchors.margins: app.margins / 2

        PercentageIndicator {
            id: healthIndicator
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: app.healthColor
            percentage: Game.world.player.healthPercentage
        }

        PercentageIndicator {
            id: manaIndicator
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: app.manaColor
            percentage: Game.world.player.manaPercentage
        }
    }
}
