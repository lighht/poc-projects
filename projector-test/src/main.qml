import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3
import QtCharts 2.2

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
                property bool openGL: true
                property bool openGLSupported: true
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
                    max: 256
                }

                LineSeries {
                    id: lineSeries1
                    name: "Projector"
                    axisX: axisX
                    axisY: axisY1
                    useOpenGL: true
                }
                LineSeries {
                    id: lineSeries2
                    name: "Motor"
                    axisX: axisX
                    axisYRight: axisY2
                    useOpenGL: true
                }

                Timer {
                    id: refreshTimer
                    interval: 1 / 20 * 1000 // 20 Hz. Change data acquisition rate if this is changed
                                            //If fps is increased, pwm measurements can be slower.
                    running: false
                    repeat: true
                    onTriggered: {
                        dataSource.updateData(chartView.series(0), chartView.series(1));
                        axisX.max = dataSource.getIndex();
                        axisX.min = dataSource.getIndex() - 256;
                    }
                }
            }
//            //            Heating {

//            //            }

//            //            Security {

//            //            }
//        }

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
                    id: startButton
                    x: 50
                    y: 6
                    text: qsTr("Start")
                    Material.accent: Material.Orange
                    Material.background: Material.color(Material.Green)
                    onClicked: {
                        refreshTimer.running = true
                    }
                }

                Button {
                    id: pauseButton
                    x: 50
                    y: 64
                    text: qsTr("Pause")
                    Material.background: Material.color(Material.Red)
                    onClicked: {
                        refreshTimer.running = false
                    }
                }

                Button {
                    id: resetButton
                    x: 50
                    y: 122
                    text: qsTr("Reset")
                    onClicked: {
                        refreshTimer.running = false
                        dataSource.reset()
                        refreshTimer.running = true
                    }
                }

                Button {
                    id: stopButton
                    x: 50
                    y: 180
                    text: qsTr("Stop")
                    onClicked: {
                        refreshTimer.running = false
                        dataSource.saveData()
                        dataSource.reset()
                    }
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
                    refreshTimer.running = false
                    Qt.quit()
                }
            }
        }
}
