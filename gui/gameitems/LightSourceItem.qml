import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

Item {
    id: root

    property LightSource light: null
    property bool itemDebugEnabled: false

    z: GameObject.LayerBackground
    //opacity: 0.8

    Image {
        id: lightSourceImage
        anchors.fill: parent
        fillMode: Image.Stretch
        source: {
            var sourcePath = dataDirectory + "/lights/spotlight.svg"
            if (!root.light)
                return sourcePath

            switch (light.lightType) {
            case LightSource.LightTypeSpotlight:
                sourcePath = dataDirectory + "/lights/spotlight.svg"
                break;
            }

            return sourcePath
        }
    }


    ColorOverlay {
        id: colorizedLight
        anchors.fill: parent
        source: lightSourceImage
        color: root.light ? light.color : "transparent"
    }

//    GaussianBlur {
//        anchors.fill: colorizedLight
//        source: colorizedLight
//        radius: app.gridSize * 3
//        samples: 32
//    }

    Loader {
        anchors.fill: parent
        active: root.itemDebugEnabled
        source: "../components/ItemDebugFrame.qml"
    }
}
