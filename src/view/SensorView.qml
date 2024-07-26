import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    property var sensorController
    property var sensorData

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Text {
            text: "Sensor Values"
            font.bold: true
            font.pixelSize: 20
        }

        Repeater {
            model: sensorData.values.length

            RowLayout {
                spacing: 10

                Text {
                    text: "Sensor " + index + ":"
                    font.pixelSize: 16
                }

                Text {
                    text: sensorData.values[index]
                    font.pixelSize: 16
                }
            }
        }

        Button {
            text: sensorController.isReading ? "Stop Reading" : "Start Reading"
            onClicked: {
                if (sensorController.isReading) {
                    sensorController.stopReading()
                } else {
                    sensorController.startReading()
                }
            }
        }
    }
}
