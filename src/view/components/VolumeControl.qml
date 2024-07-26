import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    property int channel
    property var sensorController

    RowLayout {
        spacing: 10

        Text {
            text: "Volume for Channel " + channel + ":"
            font.pixelSize: 16
        }

        Slider {
            from: 0
            to: 100
            value: sensorController.getVolumeForChannel(channel)
            onValueChanged: sensorController.setVolumeForChannel(channel, value)
        }
    }
}
