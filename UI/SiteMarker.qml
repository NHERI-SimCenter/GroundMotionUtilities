import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Controls 2.2

MapQuickItem {
    id: siteMarker

    sourceItem: Rectangle {
        id:markerRect
        width:30
        height:30
        radius: width/2
        visible: true
        anchors.centerIn: parent

        color: "blue"
        opacity:0.2

        border.color: "black"
        border.width: 2

        MouseArea
        {
            property bool hovered: false
            anchors.fill: parent

            ToolTip.visible: hovered

            ToolTip.text:  String(qsTr("Site\nLatitude: %1\nLongitude: %2")).arg(site.location.latitude).arg(site.location.longitude)
            ToolTip.timeout: 5000

            onEntered: hovered = true
            onExited: hovered = false
            hoverEnabled: true
        }
    }

    coordinate {
        latitude: site.location.latitude
        longitude: site.location.longitude
    }


}
