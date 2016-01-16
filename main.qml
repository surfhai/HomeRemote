import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import Kodi.control 1.0

Window {
    id: window1
    visible: true
    width: 1366
    height: 766
    color: "#232323"

    Rectangle {
        id: recKodiPVRChannels
        color: "#333333"
        width: parent.width / 2
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left

        ListView {
            id: kodiPVRChannels
            anchors.fill: parent
            orientation: ListView.Vertical
            spacing: 0
            model: kodiPVRChannelModel

            delegate: KodiPVRChannelsDelegate {
                id: kodiPVRChannelsDelegate
                width: parent.width
                onChannelClicked: Kodi.switchChannel(channel)
            }

            ScrollBar {
                scrollArea: parent
                orientation: Qt.Vertical
                width: 8
                height: parent.height
                x: parent.width - width
            }
        }
    }

    Clock {
        width: 200
        anchors.top: parent.top
        anchors.right: parent.right
        color: "#333333"
    }

    Navigation {
        anchors.bottom: parent.bottom
        anchors.left: recKodiPVRChannels.right
        color: "#333333"
        onNaviClicked: Kodi.inputAction(action)
    }
}
