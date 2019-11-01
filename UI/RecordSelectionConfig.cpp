#include "RecordSelectionConfig.h"

RecordSelectionConfig::RecordSelectionConfig(QObject *parent) : QObject(parent)
{
    this->m_database = RecordDb::NGAWest2Subset1000;
    this->m_error = ErrorMetric::RMSE;
}

RecordSelectionConfig::RecordDb RecordSelectionConfig::database() const
{
    return m_database;
}

void RecordSelectionConfig::setDatabase(const RecordDb &database)
{
    if(m_database != database)
    {
        m_database = database;
        emit this->databaseChanged(m_database);
    }
}

RecordSelectionConfig::ErrorMetric RecordSelectionConfig::error() const
{
    return m_error;
}

void RecordSelectionConfig::setError(const ErrorMetric &error)
{
    if(m_error != error)
    {
        m_error = error;
        emit this->errorChanged(m_error);
    }
}

QJsonObject RecordSelectionConfig::getJson()
{
    QJsonObject target;
    target.insert("Type", "MeanSpectrum");

    QJsonObject db;
    db.insert("Type", "CSV");

    QJsonObject criteria;
    QJsonObject magnitude;
    QJsonObject distance;
    QJsonObject vs30;
    magnitude.insert("Min", 4.0);
    magnitude.insert("Max", 8.0);
    distance.insert("Min", 0.0);
    distance.insert("Max", 100.0);
    vs30.insert("Min", 150.0);
    vs30.insert("Max", 1500.0);

    criteria.insert("Magnitude", magnitude);
    criteria.insert("Distance", distance);
    criteria.insert("Vs30", vs30);

    QJsonObject selection;

    selection.insert("Target", target);
    selection.insert("Database", db);
    selection.insert("Criteria", criteria);

    return selection;
}
