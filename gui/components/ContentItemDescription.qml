import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

    property GameItem item: null

    Rectangle {
        anchors.fill: parent
        color: "#55000000"
        border.color: "white"
        border.width: app.borderWidth
    }

    GameLabel {
        anchors.centerIn: parent
        visible: !root.item
        color: "white"
        font.pixelSize: app.largeFont
        text: qsTr("No item selected.")
    }

    ColumnLayout {
        id: itemPropertyLayout
        spacing: app.margins / 2
        anchors.fill: parent
        anchors.margins: app.margins
        visible: root.item

        RowLayout {
            Layout.fillWidth: true

            ContentItemImage {
                id: weaponItem
                Layout.preferredWidth: app.menuItemSize
                Layout.preferredHeight: app.menuItemSize
                imageSource: root.item ? dataDirectory + root.item.imageName : ""
            }

            GameLabel {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                text: (root.item ? root.item.name : "")
                font.pixelSize: largeFont
                color: "white"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: app.borderWidth / 2
            color: "white"
        }

        GameLabel {
            color: "white"
            text: qsTr("Price") + ": " + (root.item ? root.item.price : "")
        }

        GameLabel {
            id: damageLabel
            color: "white"
            visible: {
                if (!root.item)
                    return false

                if (root.item.itemType !== GameItem.TypeWeapon && root.item.itemType !== GameItem.TypeFirearm)
                    return false

                return true
            }

            text: {
                if (!root.item || !visible)
                    return ""

                if (root.item.itemType === GameItem.TypeWeapon) {
                    var weaponItem = Game.castWeaponItem(root.item)
                    return qsTr("Damage") + ": " + weaponItem.damage
                } else if (root.item.itemType === GameItem.TypeFirearm) {
                    var firearmItem = Game.castFirearmItem(root.item)
                    return qsTr("Damage") + ": " + firearmItem.damage
                } else {
                    return ""
                }
            }
        }

        GameLabel {
            id: rangeLabel
            color: "white"
            visible: {
                if (!root.item || root.item.itemType !== GameItem.TypeFirearm)
                    return false

                return true
            }

            text: {
                if (!root.item || !visible)
                    return ""

                var firearmItem = Game.castFirearmItem(root.item)
                return qsTr("Range") + ": " + firearmItem.range
            }
        }

        GameLabel {
            id: healingLabel
            color: "white"
            visible: {
                if (!root.item || root.item.itemType !== GameItem.TypePlant)
                    return false

                var plantItem = Game.castPlantItem(root.item)
                if (plantItem.healing === 0)
                    return false

                return true
            }
            text: {
                if (!root.item || !visible)
                    return ""

                var plantItem = Game.castPlantItem(root.item)
                return qsTr("Healing") + ": " + plantItem.healing
            }
        }

        GameLabel {
            id: manaLabel
            color: "white"
            visible: {
                if (!root.item || root.item.itemType !== GameItem.TypePlant)
                    return false

                var plantItem = Game.castPlantItem(root.item)
                if (plantItem.mana === 0)
                    return false

                return true
            }

            text: {
                if (!root.item || root.item.itemType !== GameItem.TypePlant)
                    return ""

                var plantItem = Game.castPlantItem(root.item)
                return qsTr("Mana") + ": " + plantItem.mana
            }
        }

        Item {
            id: spacingItem
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
