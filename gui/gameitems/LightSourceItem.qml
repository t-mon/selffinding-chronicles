import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

Item {
    id: root

    property LightSource lightSource: null
    property bool itemDebugEnabled: false

    onLightSourceChanged: {
        if (lightSource) {
            if (lightSource.lightEnabled) {
                appearAnimation.start()
            } else {
                opacity = 0
            }
        }
    }

    Connections {
        target: root.lightSource
        function onLightEnabledChanged(lightEnabled) {
            if (lightEnabled) {
                console.log("Light enabled")
                appearAnimation.start()
            } else {
                console.log("Light disabled")
                disappearAnimation.start()
            }
        }
    }

    PropertyAnimation {
        id: appearAnimation
        target: root
        properties: "scale, opacity"
        from: 0.0
        to: 1.0
        duration: 500
        easing.type: Easing.OutCubic
        loops: 1
    }

    PropertyAnimation {
        id: disappearAnimation
        target: root
        properties: "scale, opacity"
        loops: 1
        to: 0.0
        duration: 600
        easing.type: Easing.InCubic
    }

    RadialGradient {
        id: lightImage
        width: parent.width
        height: parent.height
        //opacity: lightSource && lightSource.enabled ? 1 : 0
        anchors.centerIn: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: lightSource ? lightSource.color : "transparent" }
            GradientStop { position: 0.3; color: lightSource ? Qt.rgba(lightSource.color.r, lightSource.color.g, lightSource.color.b, lightSource.color.a / 2) : "transparent" }
            GradientStop { position: 0.5; color:  "transparent" }
        }


    }

    //    Loader {
    //        id: lightAnimationLoader
    //        active: lightSource ? lightSource.lightBehaviour !== LightSource.LightBehaviourStill : false
    //        sourceComponent: {
    //            switch (lightSource) {
    //            case lightSource.LightBehaviourGlowing:
    //                return glowAnimationComponent
    //            case lightSource.LightBehaviourFlickering:
    //                return glowAnimationComponent
    //            }
    //        }

    //        Component {
    //            id: glowAnimationComponent

    //            SequentialAnimation {
    //                running: true
    //                loops: SequentialAnimation.Infinite

    //                property int randomDuration: Game.generateRandomNumber(800, 1000)

    //                ScaleAnimator {
    //                    target: lightImage;
    //                    from: 1;
    //                    to: 0.9;
    //                    easing.type: Easing.OutQuad
    //                    duration: randomDuration
    //                }

    //                ScaleAnimator {
    //                    target: lightImage;
    //                    from: 0.9;
    //                    to: 1;
    //                    easing.type: Easing.InQuad
    //                    duration: randomDuration
    //                }
    //            }
    //        }


    //        Component {
    //            id: flickeringAnimationComponent

    //            SequentialAnimation {
    //                running: true
    //                loops: SequentialAnimation.Infinite

    //                property int randomDuration: Game.generateRandomNumber(200, 400)

    //                ScaleAnimator {
    //                    target: lightImage;
    //                    from: 1;
    //                    to: 0.8;
    //                    easing.type: Easing.OutQuad
    //                    duration: randomDuration
    //                }

    //                ScaleAnimator {
    //                    target: lightImage;
    //                    from: 0.8;
    //                    to: 1;
    //                    easing.type: Easing.InQuad
    //                    duration: randomDuration
    //                }
    //            }
    //        }
    //    }

    Loader {
        anchors.fill: parent
        active: root.itemDebugEnabled
        source: "../components/ItemDebugFrame.qml"
    }
}
