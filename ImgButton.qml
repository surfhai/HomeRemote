import QtQuick 2.5

Image {
    id: imgButton
    //source: "Images/NaviOSD.svg"
    width: 60
    height: 60
    states: [
        State {
            name: "NORMAL"
            PropertyChanges { target: imgButton; opacity: 1.0 }
        },
        State {
            name: "HOVER"
            PropertyChanges { target: imgButton; opacity: 0.7 }
        }

    ]
    MouseArea {
        anchors.fill: parent
        onClicked: naviClicked("showOSD")
        hoverEnabled: true
        onEntered: { parent.state = "HOVER"; }
        onExited: { parent.state = "NORMAL"; }
    }
}
