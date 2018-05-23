import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Controls 2.2

Rectangle
{
    id:scenarioMapContainer
    objectName: "scenarioMapContainer"
    visible: true
    anchors.fill: parent
    border.color: "black"
    border.width: 1

    property var scenarioMap;

    Component.onCompleted:
    {
        setDefaultMap();
    }
    Plugin {
        id: cartoMapPlugin
        name: "osm"
        // specify plugin parameters if necessary
        PluginParameter { name: "osm.mapping.providersrepository.disabled";value: true}
        PluginParameter { name: "osm.mapping.highdpi_tiles";value: true}
        PluginParameter { name: "osm.mapping.host";value: "https://tiles.basemaps.cartocdn.com/rastertiles/voyager/"}
        PluginParameter { name: "osm.useragent"; value: "Simcenter" }
        PluginParameter { name: "osm.mapping.cache.directory"; value: gmApp.cacheLocation +"/Tiles/Carto"  }
        PluginParameter { name: "osm.mapping.custom.mapcopyright"; value: "<a href=\"https://www.openstreetmap.org/copyright\">OpenStreetMap</a> Contributions, &copy; <a href=\"https://carto.com/attribution/\">Carto</a>" }
        }

    Plugin {
        id: stamenDesignPlugin
        name: "osm"
        // specify plugin parameters if necessary
        PluginParameter { name: "osm.mapping.providersrepository.disabled";value: true}
        PluginParameter { name: "osm.mapping.host";value: "http://tile.stamen.com/terrain/"}
        PluginParameter { name: "osm.useragent"; value: "Simcenter" }
        PluginParameter { name: "osm.mapping.cache.directory"; value: gmApp.cacheLocation +"/Tiles/StamenDesign"  }
        PluginParameter { name: "osm.mapping.custom.mapcopyright"; value: "Map tiles by <a href=\"https://stamen.com/\">Stamen Design</a> Under CC BY 3.0, Data By &copy; <a href=\"https://www.openstreetmap.org/copyright\">OpenStreetMap</a>, under ODbl" }
        }

    Plugin {
        id: mapBoxPlugin
        name: "mapbox"
        PluginParameter { name: "mapbox.access_token"; value: "" }
    }

    function setTerrainMap()
    {
        if(scenarioMap)
            scenarioMap.destroy()
        scenarioMap = Qt.createQmlObject('import QtQuick 2.0; ScenarioMap {
                            id:scenarioMap
                            plugin:stamenDesignPlugin
                            activeMapType: supportedMapTypes[supportedMapTypes.length - 1]
                            }',
                            scenarioMapContainer);
    }

    function setDefaultMap()
    {
        if(scenarioMap)
            scenarioMap.destroy()
        scenarioMap = Qt.createQmlObject('import QtQuick 2.0; ScenarioMap {
                            id:scenarioMap
                            plugin:cartoMapPlugin
                            activeMapType: supportedMapTypes[supportedMapTypes.length - 1]
                            }',
                            scenarioMapContainer);
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

//    ScenarioMap
//    {
//        id:scenarioMap
//        plugin:cartoMapPlugin
//        activeMapType: supportedMapTypes[supportedMapTypes.length - 1]
//    }

//    Map {
//        id:map
//        objectName: "map"
//        plugin:cartoMapPlugin
//        activeMapType: supportedMapTypes[supportedMapTypes.length - 1]
//        anchors.fill: parent
//        width: 800
//        height: 800
//        zoomLevel: 9
//        center: QtPositioning.coordinate(37.9358, -122.3477) // Richmond, CA

//        Rectangle
//        {
//            id: mapBorder
//            visible: true
//            anchors.fill: parent
//            border.color: "black"
//            border.width: 1
//            color: "transparent"
//        }

//        SiteMarker
//        {
//            id:siteMarker
//        }

//        PointRuptureMarker
//        {
//            id:ruptureMarker
//        }

//        SiteGridMarker
//        {
//            id:siteGridMarker
//        }

//        GridPoints{
//            id:gridPoint
//        }

//        Connections
//        {
//            target: siteGrid.latitude
//        }

//        SiteResultsView
//        {
//            id:resultsView
//        }
//    }

}
