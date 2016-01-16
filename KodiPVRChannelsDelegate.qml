import QtQuick 2.5

Item {
    signal channelClicked(int channel)

    id: delegate

    width: parent.width
    height: (title.length > 0) ? 90 : 0
    //border.width: 0
    visible: (title.length > 0) ? true : false
    //color: "#333333"
    Image {
        id: idImage
        source: thumbnail
        sourceSize.width: 60
        sourceSize.height: 60
        width: 60
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: idLabel
        anchors.top: parent.top
        anchors.left: idImage.right
        anchors.leftMargin: idImage.anchors.rightMargin
        font.pixelSize: 20
        font.weight: Font.Bold
        color: "#EEEEEE"
        text: label
    }
    Text {
        id: idChannelID
        anchors.verticalCenter: idLabel.verticalCenter
        anchors.left: idLabel.right
        font.pixelSize: 14
        color: "#AAAAAA"
        text: " (" + channelID + ")"
    }
    Text {
        id: idTitle
        anchors.top: idChannelID.bottom
        anchors.left: idLabel.left
        anchors.right: idTitleNext.left
        anchors.rightMargin: 10
        anchors.topMargin: 5
        elide: Text.ElideRight
        maximumLineCount: 1
        font.pixelSize: 14
        color: "#AAAAAA"
        text: title
    }
    Text {
        id: idTitleNext
        anchors.top: idChannelID.bottom
        anchors.left: idLabel.left
        anchors.right: parent.right
        anchors.topMargin: 5
        anchors.leftMargin: idProgressOutline.width / 2.0
        anchors.rightMargin: 5
        elide: Text.ElideRight
        maximumLineCount: 1
        font.pixelSize: 14
        color: "#777777"
        text: titleNext
    }
    Text {
        id: idEpisodeName
        anchors.top: idTitle.bottom
        anchors.left: idLabel.left
        anchors.right: idEpisodeNameNext.left
        anchors.rightMargin: 10
        elide: Text.ElideRight
        maximumLineCount: 1
        font.pixelSize: 14
        color: "#AAAAAA"
        text: episodeName
    }
    Text {
        id: idEpisodeNameNext
        anchors.top: idTitle.bottom
        anchors.left: idTitleNext.left
        anchors.right: parent.right
        anchors.rightMargin: 5
        elide: Text.ElideRight
        maximumLineCount: 1
        font.pixelSize: 14
        color: "#777777"
        text: episodeNameNext
    }
    Rectangle {
        id: idProgressOutline
        anchors.top: idEpisodeName.bottom
        anchors.left: idLabel.left
        anchors.right: parent.right
        height: 5
        anchors.rightMargin: 5
        border.width: 1
        border.color: "black"
        color: "#666666"
        Rectangle {
            id: idProgress
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: idProgressOutline.border.width
            anchors.leftMargin: idProgressOutline.border.width
            width: ((parent.width * progressPercentage / 100.0) < parent.width) ? (parent.width * progressPercentage / 100.0 - idProgressOutline.border.width * 2) : parent.width - idProgressOutline.border.width * 2
            height: parent.height - idProgressOutline.border.width * 2
            color: "#3DAEE9"
        }
    }
    Text {
        id: idStartTime
        anchors.top: idProgressOutline.bottom
        anchors.left: idTitle.left
        font.pixelSize: 12
        color: "#AAAAAA"
        text: Qt.formatDateTime(startTime, "HH:mm")
    }
    Text {
        id: idEndTime
        anchors.top: idProgressOutline.bottom
        anchors.right: idProgressOutline.right
        font.pixelSize: 12
        color: "#AAAAAA"
        text: Qt.formatDateTime(endTime, "HH:mm")
    }
    Rectangle {
        id: idLine
        height: 1
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        color: "#232323"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: channelClicked(channelID)
        //hoverEnabled: true
        //onEntered: { parent.state = "HOVER"; }
        //onExited: { parent.state = "NORMAL"; }
    }
}
