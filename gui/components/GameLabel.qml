import QtQuick 2.5
import QtQuick.Controls 2.1

import Chronicles 1.0

Label {
    id: root

    font.bold: true
    font.family: app.fontFamily
    font.styleName: app.fontStyleName
    font.pixelSize: app.mediumFont
    font.weight: Font.ExtraBold
    z: Map.Layer4Highest
}
