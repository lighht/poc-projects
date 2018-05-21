import QtQuick 2.10
import QtQuick.Window 2.10
import QtCharts 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQml 2.2

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 800
    height: 480
    title: qsTr("Projector test")
    ChartView {
        id: chartView
        animationOptions: ChartView.NoAnimation
        theme: ChartView.ChartThemeDark
        anchors.left: pane.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.leftMargin: 0
        // Y axis for projector
        ValueAxis {
            id: axisY1
            min: 0
            max: 200
        }
        // Y axis for motor
        ValueAxis {
            id: axisY2
            min: -1.2
            max: 1.2
        }

        ValueAxis {
            id: axisX
            min: 0
            max: 1024
        }

        LineSeries {
            id: lineSeries1
            name: "Projector"
            axisX: axisX
            axisY: axisY1
        }
        LineSeries {
            id: lineSeries2
            name: "Motor"
            axisX: axisX
            axisYRight: axisY2
        }

        BusyIndicator {
            id: busyIndicator
            x: 270
            y: 210
            running: true
        }

        Component.onCompleted: {
            dataReader.updateData(chartView.series(0), chartView.series(1));
        }

        Connections{
            target: dataReader
            onReadingChanged:  {
                console.log("reading changed")
                busyIndicator.running = false
            }
        }
    }

    function handleJumpButton(){
        axisX.min = textField.text*1
        axisX.max = axisX.min + 1024
    }

    Pane {
        id: pane
        width: 200
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        Material.theme: Material.Dark

        Label {
            id: label
            x: 0
            y: -6
            width: 180
            height: 54
            text: qsTr("Projector Test")
            font.pointSize: 17
            verticalAlignment: Text.AlignVCenter
            color: Material.color(Material.DeepOrange)
        }

        GroupBox {
            id: groupBox
            y: 48
            height: 273
            anchors.right: parent.right
            anchors.rightMargin: 2
            anchors.left: parent.left
            anchors.leftMargin: -2
            title: qsTr("Controls")

            Button {
                id: jumpButton
                x: 44
                y: 64
                text: qsTr("Jump")
                Material.accent: Material.Orange
                Material.background: Material.color(Material.Green)
                onClicked: handleJumpButton()

            }

            TextField {
                id: textField
                x: 16
                y: 0
                text: qsTr("")
                placeholderText: "Switch to index"
            }
        }

        Button {
            id: exitButton
            x: 54
            y: 399
            text: qsTr("Exit")
            flat:true
            Material.foreground: Material.color(Material.Red)
            onClicked: {
                Qt.quit()
            }
        }
    }

}
