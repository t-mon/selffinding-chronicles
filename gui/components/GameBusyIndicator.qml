import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

BusyIndicator {
    id: control

    contentItem: Item {
        implicitWidth: app.gridSize * 3
        implicitHeight: app.gridSize * 3

        Item {
            id: item
            x: parent.width / 2 - 32
            y: parent.height / 2 - 32
            width: 64
            height: 64
            opacity: control.running ? 1 : 0

            Image {
                anchors.fill: parent
                source: dataDirectory + "/images/game/busy-indicator.svg"
            }

            Behavior on opacity {
                OpacityAnimator {
                    duration: 250
                }
            }

            RotationAnimator {
                target: item
                running: control.visible && control.running
                from: 0
                to: 360
                loops: Animation.Infinite
                duration: 1250
            }
        }
    }
}
