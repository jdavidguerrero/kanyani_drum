import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    property int channel
    property var sensorController

    RowLayout {
        spacing: 10

        Text {
            text: "Sensitivity for Channel " + channel + ":"
            font.pixelSize: 16
        }

        Slider {
            from: 0
            to: 1000
            value: sensorController.thresholds[channel]
            onValueChanged: sensorController.thresholds[channel] = value
        }

        Text {
            text: sensorController.thresholds[channel]
            font.pixelSize: 16
        }
    }
}
