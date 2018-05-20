import QtQuick 2.9
import QtCharts 2.2

ChartView {
    id: chartView
    animationOptions: ChartView.NoAnimation
    theme: ChartView.ChartThemeDark
    property bool openGL: true
    property bool openGLSupported: true
    onOpenGLChanged: {
        if (openGLSupported) {
            series("signal 1").useOpenGL = openGL;
            series("signal 2").useOpenGL = openGL;
        }
    }
    Component.onCompleted: {
        if (!series("signal 1").useOpenGL) {
            openGLSupported = false
            openGL = false
        }
    }
    // Y axis for projector
    ValueAxis {
        id: axisY1
        min: 0
        max: 12
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
        name: "signal 1"
        axisX: axisX
        axisY: axisY1
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries2
        name: "signal 2"
        axisX: axisX
        axisYRight: axisY2
        useOpenGL: chartView.openGL
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

//    function createAxis(min, max) {
//        // The following creates a ValueAxis object that can be then set as a x or y axis for a series
//        return Qt.createQmlObject("import QtQuick 2.0; import QtCharts 2.0; ValueAxis { min: "
//                                  + min + "; max: " + max + " }", chartView);
//    }


}
