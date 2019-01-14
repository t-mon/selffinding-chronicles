import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

import "../components"

Row {
    id: root

    ContentItemImage {
        id: weaponItem
        width: app.menuItemSize
        height: width
        imageSource: Game.world.player.weapon ? dataDirectory + Game.world.player.weapon.imageName : ""
    }

    ContentItemImage {
        id: firearmItem
        width: app.menuItemSize
        height: width
        imageSource: Game.world.player.firearm ? dataDirectory + Game.world.player.firearm.imageName : ""
    }
}
