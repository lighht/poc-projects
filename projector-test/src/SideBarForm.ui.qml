import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3

PageBackground {
    id: sideBarBackground
    width: 200
    height: 600
    Material.theme: Material.Dark
    Material.accent: Material.DeepOrange

    Label {
        id: label
        x: 8
        y: 15
        width: 184
        height: 40
        text: qsTr("Projector Test")
        font.pointSize: 18
    }

    GroupBox {
        id: controlsGroupBox
        x: 0
        y: 73
        width: 200
        height: 225
        title: qsTr("Controls")

        Button {
            id: resetButton
            x: 56
            y: 130
            text: qsTr("reset")
            Material.accent: Material.DeepOrange
        }

        Button {
            id: stopButton
            x: 56
            y: 62
            text: qsTr("Stop")
            Material.accent: Material.DeepOrange
        }

        Button {
            id: startButton
            x: 56
            y: -6
            text: qsTr("Start")
            Material.accent: Material.DeepOrange
        }
    }

    GroupBox {
        id: statusGroupBox
        x: 0
        y: 321
        width: 200
        height: 243
        title: qsTr("Status")
        Material.accent: Material.DeepOrange

        Switch {
            id: switch1
            x: 27
            y: 11
            text: qsTr("Projector")
        }
    }
}
