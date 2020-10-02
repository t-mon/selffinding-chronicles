import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

Item {
    id: root

    property LightSource light: null
    property bool itemDebugEnabled: false

    Image {
        id: lightSourceImage
        anchors.fill: parent
        fillMode: Image.Stretch
        source: {
            var sourcePath
            switch (light.lightType) {
            case LightSource.LightTypeSpotlight:
                sourcePath = dataDirectory + "/lights/spotlight.svg"
                break;
            }

            return sourcePath
        }
    }

    ColorOverlay {
        anchors.fill: parent
        source: lightSourceImage
        color: light.color
    }

}
