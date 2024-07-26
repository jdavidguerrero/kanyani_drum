import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    property var sensorController

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Text {
            text: "Sensor Values"
            font.bold: true
            font.pixelSize: 20
        }

        Repeater {
            model: sensorController.sensorValues.length

            RowLayout {
                spacing: 10

                Text {
                    text: "Sensor " + index + ":"
                    font.pixelSize: 16
                }

                Text {
                    text: sensorController.sensorValues[index]
                    font.pixelSize: 16
                }
            }
        }

        Button {
            text: "Configuration"
            onClicked: {
                stackView.push("qrc:/src/view/ConfigurationView.qml")
            }
        }
    }
}
