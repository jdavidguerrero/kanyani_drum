import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2

Item {
    property int channel
    property var sensorController

    RowLayout {
        spacing: 10

        Button {
            text: "Set Sample"
            onClicked: {
                fileDialog.open();
            }
        }

        FileDialog {
            id: fileDialog
            onAccepted: {
                sensorController.setSampleForChannel(fileDialog.fileUrl, channel);
            }
        }

        Text {
            text: "Current Sample: " + sensorController.getSampleForChannel(channel)
            font.pixelSize: 16
        }
    }
}
