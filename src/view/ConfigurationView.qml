import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    property var sensorController

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Text {
            text: "Configuration"
            font.bold: true
            font.pixelSize: 20
        }

        Repeater {
            model: sensorController.thresholds.length

            RowLayout {
                spacing: 10

                Text {
                    text: "Sensor " + index + " Threshold:"
                    font.pixelSize: 16
                }

                Slider {
                    from: 0
                    to: 1023
                    value: sensorController.thresholds[index]
                    onValueChanged: {
                        var thresholds = sensorController.thresholds
                        thresholds[index] = value
                        sensorController.thresholds = thresholds
                    }
                }

                Text {
                    text: sensorController.thresholds[index]
                    font.pixelSize: 16
                }
            }
        }

        Button {
            text: "Back to Home"
            onClicked: {
                stackView.pop()
            }
        }
    }
}
