import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Controls 2.2

Map {
    id:map
    objectName: "map"

    anchors.fill: parent
    width: 800
    height: 800
    zoomLevel: 9
    center: QtPositioning.coordinate(37.9358, -122.3477) // Richmond, CA

    Rectangle
    {
        id: mapBorder
        visible: true
        anchors.fill: parent
        border.color: "black"
        border.width: 1
        color: "transparent"
    }

    SiteMarker
    {
        id:siteMarker
    }

    PointRuptureMarker
    {
        id:ruptureMarker
    }

    SiteGridMarker
    {
        id:siteGridMarker
    }

    GridPoints{
        id:gridPoint
    }

    Connections
    {
        target: siteGrid.latitude
    }

    SiteResultsView
    {
        id:resultsView
    }
}
