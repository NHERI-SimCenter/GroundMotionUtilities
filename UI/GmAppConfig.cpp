 #include "GmAppConfig.h"
#include "QFile"

GmAppConfig::GmAppConfig(QObject *parent) : QObject(parent)
{
}

QString GmAppConfig::eqHazardPath() const
{
    return m_eqHazardPath;
}

void GmAppConfig::setEqHazardPath(const QString &eqHazardPath)
{
    if(m_eqHazardPath != eqHazardPath)
    {
        m_eqHazardPath = eqHazardPath;
        emit eqHazardPathChanged(m_eqHazardPath);
    }
}

QString GmAppConfig::simulateIMPath() const
{
    return m_simulateIMPath;
}

void GmAppConfig::setSimulateIMPath(const QString &simulateIMPath)
{
    if(m_simulateIMPath != simulateIMPath)
    {
        m_simulateIMPath = simulateIMPath;
        emit simulateIMPathChanged(m_simulateIMPath);
    }
}

QString GmAppConfig::selectRecordPath() const
{
    return m_selectRecordPath;
}

void GmAppConfig::setSelectRecordPath(const QString &selectRecordPath)
{
    if(m_selectRecordPath != selectRecordPath)
    {
        m_selectRecordPath = selectRecordPath;
        emit selectRecordPathChanged(m_selectRecordPath);
    }
}

QString GmAppConfig::NGAW2DbPath() const
{
    return m_NGAW2DbPath;
}

void GmAppConfig::setNGAW2DbPath(const QString &NGAW2DbPath)
{
    if(m_NGAW2DbPath != NGAW2DbPath)
    {
        m_NGAW2DbPath = NGAW2DbPath;
        emit NGAW2DbPathChanged(m_NGAW2DbPath);
    }
}

QString GmAppConfig::NGAW2SubsetDbPath() const
{
    return m_NGAW2SubsetDbPath;
}

bool GmAppConfig::validate()
{
    if(!QFile::exists(m_eqHazardPath))
        return false;

    if(!QFile::exists(m_simulateIMPath))
        return false;

    if(!QFile::exists(m_selectRecordPath))
        return false;

    if(!QFile::exists(m_NGAW2DbPath))
        return false;

    if(!QFile::exists(m_NGAW2SubsetDbPath))
        return false;

    return true;
}

void GmAppConfig::setNGAW2SubsetDbPath(const QString &NGAW2SubsetDbPath)
{
    if(m_NGAW2SubsetDbPath != NGAW2SubsetDbPath)
    {
        m_NGAW2SubsetDbPath = NGAW2SubsetDbPath;
        emit NGAW2SubsetDbChanged(m_NGAW2SubsetDbPath);
    }
}
