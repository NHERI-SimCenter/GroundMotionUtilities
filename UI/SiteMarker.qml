import QtQuick 2.7
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Controls 2.2
import org.designsafe.ci.simcenter 1.0

MapQuickItem {
    id: siteMarker
    anchorPoint.x: markerRect.width/2
    anchorPoint.y: markerRect.height/2
    visible:(siteConfig.type === SiteConfig.Single)

    sourceItem: Rectangle {
        id:markerRect
        z:10
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
            anchors.centerIn: parent
            ToolTip.visible: hovered

            ToolTip.text:  String(qsTr("Site\nLatitude: %1\nLongitude: %2\nSelected Record Id:%3\nScale Factor:%4")).arg(site.location.latitude).arg(site.location.longitude).arg(siteResult.recordId).arg(siteResult.scaleFactor)
            ToolTip.timeout: 5000

            onEntered: hovered = true
            onExited: hovered = false
            hoverEnabled: true
            drag.target: markerRect
            cursorShape: pressed ? "ClosedHandCursor": "OpenHandCursor";


            onPositionChanged: {
                 if(drag.active)
                 {
                     var newCoordinate = map.toCoordinate(mapToItem(map, mouse.x+ width/2, mouse.y+height/2), false);
                     site.location.setLatitude(newCoordinate.latitude)
                     site.location.setLongitude(newCoordinate.longitude)
                 }
            }
        }
    }

    coordinate {
        latitude: site.location.latitude
        longitude: site.location.longitude
    }
    z:10


}
