import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Controls 2.2

Rectangle
{
    id:scenarioMap
    visible: true
    anchors.fill: parent
    border.color: "black"
    border.width: 2

    Plugin {
        id: mapPlugin
        name: "osm" //"mapbox"// , "esri", ...
        // specify plugin parameters if necessary
         PluginParameter { name: "osm.mapping.providersrepository.disabled";value: true}
         PluginParameter { name: "osm.mapping.highdpi_tiles";value: true}
         PluginParameter { name: "osm.mapping.host";value: "https://tiles.basemaps.cartocdn.com/rastertiles/voyager/"}
         PluginParameter { name: "osm.useragent"; value: "Simcenter" }
//         PluginParameter { name: "mapbox.access_token"; value: "pk.eyJ1IjoiZWw3YWRkYWQiLCJhIjoiY2pnaW5ra2xyMDJkaDJwbmI5aWdnN3hoMCJ9.PbM-__DXZNSVuqTYnjIdrQ" }


    }

    ListModel{
        id:sitesModel

        Component.onCompleted:
        {
            updateModel()

        }

        function updateModel() {
            sitesModel.clear()
            var latstep = (siteGrid.latitude.max - siteGrid.latitude.min)/siteGrid.latitude.divisions;
            var lonStep = (siteGrid.longitude.max - siteGrid.longitude.min)/siteGrid.longitude.divisions;
            var dTol = 1e-3

            for(var alat = siteGrid.latitude.min; alat - siteGrid.latitude.max <= dTol; alat += latstep)
            {
                for(var alon = siteGrid.longitude.min; alon - siteGrid.longitude.max <= dTol; alon += lonStep)
                {
                    sitesModel.append({latitude: alat, longitude : alon})
                }
            }
        }
    }

    Map {
        id:map
        plugin: mapPlugin
        anchors.fill: parent
        width: 800
        height: 800
        zoomLevel: 11
        center: QtPositioning.coordinate(37.9358, -122.3477) // Richmond, CA
        activeMapType: supportedMapTypes[supportedMapTypes.length -1]

        Rectangle
        {
            id: mapBorder
            visible: true
            anchors.fill: parent
            border.color: "black"
            border.width: 2
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

//        MouseArea
//        {
//            anchors.fill: parent
//            onClicked: {
//                console.debug(sitesModel.count)
//            }
//        }

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
}
