import QtQuick 2.5

Rectangle {
    id: rectClock
    width: 300
    height: width / 2.2222222222
    border.width: 1
    border.color: "black"
    color: parent.color

    Rectangle {
        id: rectTime
        anchors.topMargin: 1
        anchors.leftMargin: 1
        anchors.rightMargin: 1
        width: parent.width - rectTime.anchors.leftMargin - rectTime.anchors.rightMargin
        height: parent.height - rectDate.height - rectTime.anchors.topMargin - rectDate.anchors.bottomMargin
        color: parent.color
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        Text {
            id: txtTime
            width: parent.width
            height: parent.height
            font.family: "DejaVu Sans"
            font.weight: Font.Bold
            fontSizeMode: Text.Fit
            font.pixelSize: 500
            minimumPixelSize: 10
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "#EEEEEE"
            text: "00:00"
        }
    }
    Rectangle {
        id: rectDate
        anchors.bottomMargin: 1
        anchors.leftMargin: 1
        anchors.rightMargin: 1
        width: parent.width - rectDate.anchors.leftMargin - rectDate.anchors.rightMargin
        height: parent.height / 5.0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: rectTime.bottom
        color: parent.color
        Text {
            id: txtDay
            font.family: "DejaVu Sans Condensed"
            fontSizeMode: Text.VerticalFit
            font.pixelSize: 200
            minimumPixelSize: 6
            width: parent.width / 2.0
            height: parent.height
            anchors.left: parent.left
            anchors.leftMargin: 5
            horizontalAlignment: Text.AlignLeft
            color: "#AAAAAA"
            text: "Montag"
        }
        Text {
            id: txtDate
            font.family: "DejaVu Sans Condensed"
            fontSizeMode: Text.VerticalFit
            font.pixelSize: 200
            minimumPixelSize: 6
            width: parent.width / 2.0
            height: parent.height
            anchors.right: parent.right
            anchors.rightMargin: 5
            horizontalAlignment: Text.AlignRight
            color: "#AAAAAA"
            text: "01.01.2015"
        }
    }
    Timer {
        interval: 1000
        running: true
        repeat: true

        onTriggered: {
            var date = new Date()

            txtTime.text = date.toLocaleTimeString(Qt.locale("de_DE"), "HH:mm")
            txtDay.text = date.toLocaleDateString(Qt.locale("de_DE"), "dddd")
            txtDate.text = date.toLocaleDateString(Qt.locale("de_DE"), "dd.MM.yyyy")
        }
    }
}
