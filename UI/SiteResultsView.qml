import QtQuick 2.4
import QtLocation 5.9
import QtPositioning 5.6
import QtQuick.Controls 2.2
import org.designsafe.ci.simcenter 1.0

MapItemView {
    model: siteResultsModel
    delegate: MapQuickItem
    {
        zoomLevel:11
        sourceItem: Rectangle {
            width: 30 * model.display.mean
            height:width
            radius: width/2
            visible: true
            anchors.centerIn: parent
            color: getcolor(model.display.mean)

            border.color: "black"
            border.width: 1

            MouseArea
            {
                property bool hovered: false
                anchors.centerIn: parent
                anchors.fill: parent
                ToolTip.text:  String(qsTr("Site\nLocation: (%1, %2)\nMean IM:%3\nSelected Record Id:%4\nScale Factor:%5")).arg(model.display.location.latitude).arg(model.display.location.longitude).arg(model.display.mean).arg(model.display.recordId).arg(model.display.scaleFactor)
                ToolTip.timeout: 5000
                ToolTip.visible: hovered
                onEntered: hovered = true
                onExited: hovered = false
                hoverEnabled: true
            }
        }

        coordinate
        {
            latitude: model.display.location.latitude
            longitude: model.display.location.longitude
        }

        function getcolor(value) {
                if (value < 0.25)
                    return Qt.rgba(0, 1, 1-value/0.25)
                else if(value >= 0.25 && value < 0.5)
                    return Qt.rgba(1-(value-0.25)/0.25, 1, 0)
                else if(value >= 0.5 && value < 1.0)

                    return Qt.rgba(value*0.9, 1 - (value-0.5)/0.5 , 0)
                else
                    return Qt.rgba(0.9 - 0.1*value, 0, 0)
             }
    }

}
