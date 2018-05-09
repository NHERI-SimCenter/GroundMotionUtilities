import QtQuick 2.7
import QtLocation 5.9
import QtPositioning 5.6
import QtQuick.Controls 2.2
import org.designsafe.ci.simcenter 1.0

MapItemGroup
{
    id: siteGridMarker
    objectName: "siteGridMarker"
    property bool dragged;
    visible: (siteConfig.type === SiteConfig.Grid)
    Component.onCompleted:
    {
        dragged = false
    }

    MapRectangle {
        color: 'blue'
        border.width: 2
        topLeft {
            latitude: siteGrid.latitude.max
            longitude: siteGrid.longitude.min
        }
        bottomRight {
            latitude: siteGrid.latitude.min
            longitude: siteGrid.longitude.max
        }


        opacity: 0.05
    }

    MapCircle
    {
        visible:(gmApp.mode === GMWidget.Edit)
        id:topRightCorner
        center{
            latitude: siteGrid.latitude.max
            longitude:siteGrid.longitude.max

        }
        radius: 1000
        color: 'blue'
        opacity: 0.2

        width: 2
        Drag.active: topRightHandle.drag.active
        Drag.onActiveChanged:
        {
            if(Drag.active)
            {
                dragged = true;
            }
            else
            {
                dragged = false;
                gmApp.updateLocations()
            }
        }
        MouseArea{
                    anchors.fill: parent
                    drag.target: parent
                    id: topRightHandle
                    cursorShape: pressed ? "ClosedHandCursor": "OpenHandCursor"
                }

        onCenterChanged: {
            if(!isNaN(topRightCorner.center.latitude))
                siteGrid.latitude.setMax(topRightCorner.center.latitude)

            if(!isNaN(topRightCorner.center.longitude))
                siteGrid.longitude.setMax(topRightCorner.center.longitude)
        }
    }

    MapCircle
    {
        id:topLeftCorner
        visible:(gmApp.mode === GMWidget.Edit)
        center{
            latitude: siteGrid.latitude.max
            longitude:siteGrid.longitude.min

        }
        radius: 1000
        color: 'blue'
        opacity: 0.2

        width: 2

        Drag.active: topLeftHandle.drag.active
        Drag.onActiveChanged:
        {
            if(Drag.active)
            {
                dragged = true;
            }
            else
            {
                dragged = false;
                gmApp.updateLocations()
            }
        }
        MouseArea{
                    anchors.fill: parent
                    drag.target: parent
                    id: topLeftHandle
                    cursorShape: pressed ? "ClosedHandCursor": "OpenHandCursor"
                }

        onCenterChanged: {
            if(!isNaN(topLeftCorner.center.latitude))
                siteGrid.latitude.setMax(topLeftCorner.center.latitude)

            if(!isNaN(topLeftCorner.center.longitude))
                siteGrid.longitude.setMin(topLeftCorner.center.longitude)
        }
    }

    MapCircle
    {
        id:bottomLeftCorner
        visible:(gmApp.mode === GMWidget.Edit)
        center{
            latitude: siteGrid.latitude.min
            longitude:siteGrid.longitude.min

        }
        radius: 1000
        color: 'blue'
        opacity: 0.2

        width: 2
        Drag.active: bottomLeftHandle.drag.active
        Drag.onActiveChanged:
        {
            if(Drag.active)
            {
                dragged = true;
            }
            else
            {
                dragged = false;
                gmApp.updateLocations()
            }
        }
        MouseArea{
            anchors.fill: parent
            drag.target: parent
            id: bottomLeftHandle
            cursorShape: pressed ? "ClosedHandCursor": "OpenHandCursor"
        }

        onCenterChanged: {
            if(!isNaN(center.latitude))
                siteGrid.latitude.setMin(center.latitude)

            if(!isNaN(center.longitude))
                siteGrid.longitude.setMin(center.longitude)

        }

    }

    MapCircle
    {
        id:bottomRightCorner
        visible:(gmApp.mode === GMWidget.Edit)
        center{
            latitude: siteGrid.latitude.min
            longitude:siteGrid.longitude.max

        }
        radius: 1000
        color: 'blue'
        opacity: 0.2

        width: 2
        Drag.active: bottomRightHandle.drag.active
        Drag.onActiveChanged:
        {
            if(Drag.active)
            {
                dragged = true;
            }
            else
            {
                dragged = false;
                gmApp.updateLocations()
            }
        }
        MouseArea{
            anchors.fill: parent
            drag.target: parent
            id: bottomRightHandle
            cursorShape: pressed ? "ClosedHandCursor": "OpenHandCursor"
        }

        onCenterChanged: {
            if(!isNaN(center.latitude))
                siteGrid.latitude.setMin(center.latitude)

            if(!isNaN(center.longitude))
                siteGrid.longitude.setMax(center.longitude)
        }
    }
}
