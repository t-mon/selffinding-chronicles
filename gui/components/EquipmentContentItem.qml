import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

import "../components"

Row {
    id: equipmentRow
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    anchors.bottomMargin: app.margins
    spacing: app.margins

    ContentItem {
        id: weaponItem
        item: Game.world.player.weapon
        width: app.menuItemSize
        height: width
    }

    ContentItem {
        id: firearmItem
        item: Game.world.player.firearm
        width: app.menuItemSize
        height: width
    }
}
