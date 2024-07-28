import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import SensorController 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 480
    title: qsTr("Sensor App")

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: homeViewComponent
    }

    Component {
        id: homeViewComponent
        HomeView {
            sensorValues: sensorController.sensorValues
        }
    }

    Component {
        id: configurationViewComponent
        ConfigurationView {
            thresholds: sensorController.thresholds
            samples: sensorController.samples
        }
    }

    Row {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        Button {
            text: "Home"
            onClicked: stackView.pop()
        }

        Button {
            text: "Configuration"
            onClicked: stackView.push(configurationViewComponent)
        }
    }

    Component.onCompleted: {
        sensorController.startReading()
    }

    Connections {
        target: sensorController
        onThresholdsChanged: {
            if (stackView.currentItem === configurationViewComponent) {
                configurationViewComponent.forceLayout() // Refresh view if necessary
            }
        }
        onSamplesChanged: {
            if (stackView.currentItem === configurationViewComponent) {
                configurationViewComponent.forceLayout() // Refresh view if necessary
            }
        }
    }
}
