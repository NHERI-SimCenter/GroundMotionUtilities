import QtQuick 2.4
import QtLocation 5.9
import QtPositioning 5.6
import QtQuick.Controls 2.2
import org.designsafe.ci.simcenter 1.0

MapItemView {
    model: sitesModel

    delegate: MapQuickItem
    {
        zoomLevel:11
        visible: (gmApp.mode === GMWidget.Edit) && (siteConfig.type === SiteConfig.Grid) && !siteGridMarker.dragged
        //visible: false

        sourceItem: Rectangle {
        width:10
        height:10
        radius: width/2
        visible: true
        anchors.centerIn: parent
        color: "blue"
        opacity:0.1

        border.color: "black"
        border.width: 2
        }

        coordinate
        {
            latitude: latitude
            longitude: longitude
        }
    }
}
