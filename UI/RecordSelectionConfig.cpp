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
