import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Rectangle {
    Material.theme: Material.Dark
    Material.accent: Material.DeepOrange
    width: 800
    height: 600
    color: "#191919"
    property alias label: label
    clip: true
    property alias title: label.text

    Label {
        id: label
        x: 8
        y: 18
        text: qsTr("")
        font.pixelSize: 22
    }

    Rectangle {
        id: rectangle1
        x: 715
        y: -31
        width: 400
        height: 200
        color: "#3a3a3a"
        rotation: 45
    }


}
