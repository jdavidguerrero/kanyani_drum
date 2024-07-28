import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

Item {
    id: configurationView
    property var thresholds: []
    property var samples: []

    width: parent.width
    height: parent.height

    ColumnLayout {
        anchors.centerIn: parent

        Repeater {
            model: thresholds.length
            delegate: ColumnLayout {
                RowLayout {
                    Label {
                        text: "Sensor " + (index + 1) + ":"
                    }
                    Slider {
                        from: 0
                        to: 15000
                        value: thresholds[index]
                        onValueChanged: {
                            thresholds[index] = value;
                            sensorController.setThresholds(thresholds);
                        }
                    }
                        Label {
                        text: thresholds[index].toFixed(0)
                        width: 40
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                RowLayout {
                    Label {
                        text: "Sample for Sensor " + (index + 1) + ":"
                    }
                    Button {
                        text: samples[index]
                        onClicked: fileDialog.open()
                    }
                    FileDialog {
                        id: fileDialog
                        title: "Select Sample"
                        nameFilters: ["Audio files (*.wav *.mp3)"]
                        onAccepted: {
                            var fullPath = fileDialog.fileUrl.toString().substring("file://".length);
                            console.log("Selected file: " + fullPath);
                            sensorController.setSampleForChannel(fullPath, index)
                        }
                        
                    }
                }
            }
        }
    }
}
