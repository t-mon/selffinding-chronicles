import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

    property GameItem item: null

    // Load different content description depending on the item type
    onItemChanged: {
        if (!item)
            return

        switch (item.itemType) {
        case GameItem.TypeWeapon:
            itemDescriptionLoader.sourceComponent = weaponComponent
            break;
        case GameItem.TypeFirearm:
            itemDescriptionLoader.sourceComponent = firearmComponent
            break;
        case GameItem.TypePlant:
            itemDescriptionLoader.sourceComponent = plantComponent
            break;
        case GameItem.TypeLiterature:
            itemDescriptionLoader.sourceComponent = literatureComponent
            break;
        case GameItem.TypeTeleportItem:
            itemDescriptionLoader.sourceComponent = teleporterComponent
            break;
        }
    }

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

        Loader {
            id: itemDescriptionLoader
            Layout.fillWidth: true
            Layout.fillHeight: true

            property GameItem contentItem: root.item
        }
    }


    Component {
        id: weaponComponent

        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top

            property WeaponItem weaponItem: Game.castWeaponItem(contentItem)

            GameLabel {
                id: priceLabel
                color: "white"
                text: qsTr("Price") + ": " + (weaponItem ? weaponItem.price : "")
            }

            GameLabel {
                id: damageLabel
                color: "white"
                text: qsTr("Damage") + ": " + (weaponItem ? weaponItem.damage : "")

            }
        }
    }

    Component {
        id: firearmComponent

        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top

            property FirearmItem firearmItem: Game.castFirearmItem(contentItem)

            GameLabel {
                id: priceLabel
                color: "white"
                text: qsTr("Price") + ": " + (firearmItem ? firearmItem.price : "")
            }

            GameLabel {
                id: damageLabel
                color: "white"
                text: qsTr("Damage") + ": " + (firearmItem ? firearmItem.damage : "")
            }

            GameLabel {
                id: rangeLabel
                color: "white"
                text: qsTr("Range") + ": " + (firearmItem ? firearmItem.range : "")
            }
        }
    }

    Component {
        id: plantComponent

        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top

            property PlantItem plantItem: Game.castPlantItem(contentItem)

            GameLabel {
                id: priceLabel
                color: "white"
                text: qsTr("Price") + ": " + (plantItem ? plantItem.price : "")
            }

            GameLabel {
                id: healingLabel
                color: "white"
                visible: plantItem ? plantItem.healing !== 0 : false
                text: qsTr("Healing") + ": " + (plantItem ? plantItem.healing : "")
            }

            GameLabel {
                id: manaLabel
                color: "white"
                visible: plantItem ? plantItem.mana !== 0 : false
                text: qsTr("Mana") + ": " + (plantItem ? plantItem.mana : "")
            }

            GameLabel {
                id: speedLabel
                color: "white"
                visible: plantItem ? plantItem.speed !== 0 : false
                text: qsTr("Speed") + ": " + (plantItem ? plantItem.speed : "")
            }
        }
    }

    Component {
        id: literatureComponent

        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top

            property LiteratureItem literatureItem: Game.castLiteratureItem(contentItem)

            GameLabel {
                id: titleLabel
                color: "white"
                text: qsTr("Title") + ": " + (literatureItem ? literatureItem.title : "")
            }

            GameLabel {
                id: descriptionLabel
                color: "white"
                text: literatureItem ? literatureItem.description : ""
            }

            GameLabel {
                id: priceLabel
                color: "white"
                text: qsTr("Price") + ": " + (literatureItem ? literatureItem.price : "")
            }
        }
    }

    Component {
        id: teleporterComponent

        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top

            property TeleporterItem teleporterItem: Game.castTeleporterItem(contentItem)

            GameLabel {
                id: targetMapLabel
                color: "white"
                text: teleporterItem ? teleporterItem.targetMap : ""
            }

            GameLabel {
                id: targetPointLabel
                color: "white"
                text: teleporterItem ? teleporterItem.targetPosition.x + "," + teleporterItem.targetPosition.y : ""
            }
        }
    }

}
