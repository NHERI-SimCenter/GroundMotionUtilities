#include "SiteGrid.h"

SiteGrid::SiteGrid(QObject *parent) : QObject(parent)
{
    m_latitude.set(37.0, 38.2, 30);
    m_longitude.set(-122.6, -121.9, 30);
}

GridDivision &SiteGrid::latitude()
{
    return m_latitude;
}

GridDivision &SiteGrid::longitude()
{
    return m_longitude;
}

int SiteGrid::getNumSites()
{
    return (latitude().divisions() + 1) * (longitude().divisions() + 1);
}

GridDivision *SiteGrid::getLatitudePtr()
{
    return &m_latitude;
}

GridDivision *SiteGrid::getLongitudePtr()
{
    return &m_longitude;
}

QJsonObject SiteGrid::getJson()
{
    QJsonObject siteJson;
    siteJson.insert("Type","Grid");

    QJsonObject gridJson;
    QJsonObject latitudeJson;
    latitudeJson.insert("Min", m_latitude.min());
    latitudeJson.insert("Max", m_latitude.max());
    latitudeJson.insert("Divisions", m_latitude.divisions());
    gridJson.insert("Latitude", latitudeJson);

    QJsonObject longitudeJson;
    longitudeJson.insert("Min", m_longitude.min());
    longitudeJson.insert("Max", m_longitude.max());
    longitudeJson.insert("Divisions", m_longitude.divisions());
    gridJson.insert("Longitude", longitudeJson);

    siteJson.insert("Grid", gridJson);
    return siteJson;
}
