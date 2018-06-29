import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0
import "components"

Page {
    id: root

    header: ChroniclesHeader {
        text: qsTr("About %1").arg(app.title)
        backButtonVisible: true
        onBackPressed: pageStack.pop()
    }


    //    Flickable {
    //        anchors.fill: parent
    //        contentHeight: aboutColumn.implicitHeight
    //        anchors.leftMargin: app.margins
    //        anchors.rightMargin: app.margins


    //        ColumnLayout {
    //            id: aboutColumn
    //            width: parent.width

    //            Label {
    //                text: "Hello"
    //            }


    //            Label {
    //                text: "World"
    //            }
    //        }
    //    }

    ListView {
        id: mapGrid

        anchors.fill: parent

        //        cellHeight: 50
        //        cellWidth: 50

        model: 80
        delegate: Label {
            text: model.index
        }

    }

}
