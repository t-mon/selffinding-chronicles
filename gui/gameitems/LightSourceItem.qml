import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

Item {
    id: root

    property LightSource lightSource: null
    property bool itemDebugEnabled: false

    antialiasing: true

    //    ShaderEffect {
    //        id: lightShader
    //        anchors.fill: parent

    //        property color lightColor: "red" //lightSource ? lightSource.color : "blue"
    //        blending: false

    //        fragmentShader: "
    //            varying highp vec2 qt_TexCoord0;
    //            uniform lowp float qt_Opacity;
    //            lowp vec4 lightColor;

    //            void main(void) {
    //                highp vec2 center = vec2(0.5);
    //                highp float radius = 0.4;
    //                highp float distance = distance(qt_TexCoord0, center);
    //                highp vec4 finalColor = vec4(lightColor.rgb, 1.0);
    //                if (distance <= radius) {
    //                    finalColor.a = 1.0;// - smoothstep(0.0, radius, distance);;
    //                } else {
    //                    finalColor.a = 0.0;
    //                }
    //                gl_FragColor = finalColor * qt_Opacity;
    //            }
    //        "
    //    }

    //    z: GameObject.LayerBackground
    //    //opacity: 0.8

//    Image {
//        id: lightSourceImage
//        anchors.fill: parent
//        fillMode: Image.Stretch
//        source: {
//            var sourcePath = dataDirectory + "/lights/spotlight.svg"
//            if (!root.light)
//                return sourcePath

//            switch (light.lightType) {
//            case LightSource.LightTypeSpotlight:
//                sourcePath = dataDirectory + "/lights/spotlight.svg"
//                break;
//            }

//            return sourcePath
//        }
//    }

//    ColorOverlay {
//        id: colorizedLight
//        anchors.fill: parent
//        source: lightSourceImage
//        color: root.light ? lightSource.color : "transparent"
//    }

//    Rectangle {
//        anchors.fill: parent
//        radius: height / 2
//        color: lightSource.color
//    }

    RadialGradient {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: lightSource.color }
            GradientStop { position: 0.5; color:  "transparent" }
        }
    }

    Loader {
        anchors.fill: parent
        active: root.itemDebugEnabled
        source: "../components/ItemDebugFrame.qml"
    }
}
