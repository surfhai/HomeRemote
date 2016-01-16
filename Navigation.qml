import QtQuick 2.5

Rectangle {
    signal naviClicked(string action)

    width: 240
    height: 180
    color: parent.color
    Rectangle {
        id: rectSidebar
        width: 60
        height: parent.height
        border.width: 1
        border.color: "black"
        color: parent.color
        Image {
            id: imgGoHome
            source: "Images/go-home.svg"
            width: 60
            height: 60
            anchors.top: parent.top
            anchors.left: parent.left
            states: [
                State {
                    name: "NORMAL"
                    PropertyChanges { target: imgGoHome; opacity: 1.0 }
                },
                State {
                    name: "HOVER"
                    PropertyChanges { target: imgGoHome; opacity: 0.7 }
                }

            ]
            MouseArea {
                anchors.fill: parent
                onClicked: naviClicked("home")
                hoverEnabled: true
                onEntered: { parent.state = "HOVER"; }
                onExited: { parent.state = "NORMAL"; }
            }
        }
        Image {
            id: imgNaviOSD
            source: "Images/NaviOSD.svg"
            width: 60
            height: 60
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            states: [
                State {
                    name: "NORMAL"
                    PropertyChanges { target: imgNaviOSD; opacity: 1.0 }
                },
                State {
                    name: "HOVER"
                    PropertyChanges { target: imgNaviOSD; opacity: 0.7 }
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
        Image {
            id: imgGoBack
            source: "Images/go-back.svg"
            width: 60
            height: 60
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            states: [
                State {
                    name: "NORMAL"
                    PropertyChanges { target: imgGoBack; opacity: 1.0 }
                },
                State {
                    name: "HOVER"
                    PropertyChanges { target: imgGoBack; opacity: 0.7 }
                }

            ]
            MouseArea {
                anchors.fill: parent
                onClicked: naviClicked("back")
                hoverEnabled: true
                onEntered: { parent.state = "HOVER"; }
                onExited: { parent.state = "NORMAL"; }
            }
        }
    }

    Rectangle {
        width: 180
        height: 180
        border.width: 1
        border.color: "black"
        //anchors.right: parent.right
        anchors.left: rectSidebar.right
        anchors.leftMargin: -1
        anchors.verticalCenter: parent.verticalCenter
        color: parent.color
        Image {
            source: "Images/NaviSelect.svg"
            width: 60
            height: 60
            anchors.centerIn: parent
            MouseArea {
                anchors.fill: parent
                onClicked: naviClicked("select")
            }
        }
        Image {
            source: "Images/NaviArrowUp.svg"
            width: 60
            height: 60
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            MouseArea {
                anchors.fill: parent
                onClicked: naviClicked("up")
            }
        }
        Image {
            source: "Images/NaviArrowLeft.svg"
            width: 60
            height: 60
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            MouseArea {
                anchors.fill: parent
                onClicked: naviClicked("left")
            }
        }
        Image {
            source: "Images/NaviArrowDown.svg"
            width: 60
            height: 60
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            MouseArea {
                anchors.fill: parent
                onClicked: naviClicked("down")
            }
        }
        Image {
            source: "Images/NaviArrowRight.svg"
            width: 60
            height: 60
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            MouseArea {
                anchors.fill: parent
                onClicked: naviClicked("right")
            }
        }
    }
}
