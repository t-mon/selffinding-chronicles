import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

Item {
    id: root

    property LightSource lightSource
    property bool itemDebugEnabled: false

    antialiasing: app.antialiasing

    RadialGradient {
        id: lightGradient
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: root.lightSource ? root.lightSource.color : "transparent" }
            GradientStop { position: 0.1; color: root.lightSource ? root.lightSource.color : "transparent" }
            GradientStop { position: 0.5; color:  "transparent" }
        }

        SequentialAnimation {
            running: true
            loops: SequentialAnimation.Infinite
            ScaleAnimator {
                target: lightGradient;
                from: 1;
                to: 0.9;
                easing.type: Easing.OutQuad
                duration: 800
            }

            ScaleAnimator {
                target: lightGradient;
                from: 0.9;
                to: 1;
                easing.type: Easing.InQuad
                duration: 1000
            }
        }
    }

    Loader {
        anchors.fill: parent
        active: root.itemDebugEnabled
        source: "../components/ItemDebugFrame.qml"
    }
}
