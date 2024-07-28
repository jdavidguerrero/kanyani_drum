import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: homeView
    property var sensorValues: []

    width: parent.width
    height: parent.height

    ColumnLayout {
        anchors.centerIn: parent

        Text {
            text: "Home View"
            font.pixelSize: 24
        }

        Repeater {
            model: sensorValues.length
            delegate: RowLayout {
                Label {
                    text: "Sensor " + (index + 1) + ":"
                }
                Text {
                    text: sensorValues[index]
                }
            }
        }
    }
}
