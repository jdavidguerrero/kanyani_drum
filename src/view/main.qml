import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import SensorController 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Sensor App")

    SensorController {
        id: sensorController
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        HomeView {
            id: homeView
        }

        ConfigurationView {
            id: configurationView
            thresholds: sensorController.thresholds
            onThresholdsChanged: sensorController.setThresholds(thresholds)
        }
    }

    Component.onCompleted: {
        sensorController.startReading()
    }
}
