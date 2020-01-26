import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

import "../components"

Row {
    id: root

    Connections {
        id: playerControllerConnections
        target: Game.engine.playerController
        onWeaponPressed: weaponClicked()
        onFirearmPressed: firearmClicked()
    }

    function weaponClicked() {
        console.log("Weapon clicked")
        if (!Game.engine.player)
            return

        if (!Game.engine.player.weapon)
            return

        if (Game.engine.player.armed === Character.ArmedWeapon) {
            Game.engine.player.armed = Character.ArmedNone
        } else {
            Game.engine.player.armed = Character.ArmedWeapon
        }
    }

    function firearmClicked() {
        console.log("Firearm clicked")
        if (!Game.engine.player)
            return

        if (!Game.engine.player.firearm)
            return

        if (Game.engine.player.armed === Character.ArmedFirearm) {
            Game.engine.player.armed = Character.ArmedNone
        } else {
            Game.engine.player.armed = Character.ArmedFirearm
        }

    }

    ContentItemImage {
        id: weaponItem
        width: app.menuItemSize
        height: width
        imageSource: Game.engine.player ? (Game.engine.player.weapon ? dataDirectory + Game.engine.player.weapon.imageName : "") : ""
        selected: {
            if (!Game.engine.player)
                return false

            if (!Game.engine.player.weapon)
                return false

            return Game.engine.player.armed === Character.ArmedWeapon
        }

        MouseArea {
            anchors.fill: parent
            onClicked: weaponClicked()
        }
    }

    ContentItemImage {
        id: firearmItem
        width: app.menuItemSize
        height: width
        imageSource: Game.engine.player ? (Game.engine.player.firearm ? dataDirectory + Game.engine.player.firearm.imageName : "") : ""

        selected: {
            if (!Game.engine.player)
                return false

            if (!Game.engine.player.firearm)
                return false

            return Game.engine.player.armed === Character.ArmedFirearm
        }

        MouseArea {
            anchors.fill: parent
            onClicked: firearmClicked()
        }

    }
}
