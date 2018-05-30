#include "SiteResult.h"

SiteResult::SiteResult(QObject *parent):QObject(parent)
{

}

Location &SiteResult::location()
{
    return m_location;
}

SAResult &SiteResult::saResult()
{
    return m_saResult;
}

PGAResult &SiteResult::pgaResult()
{
    return m_pgaResult;
}

RecordSelection &SiteResult::recordSelection()
{
    return m_recordSelection;
}

Location *SiteResult::getLocationPtr()
{
    return &m_location;
}

SAResult *SiteResult::getSAResultPtr()
{
    return &m_saResult;
}

PGAResult *SiteResult::getPGAResultPtr()
{
    return &m_pgaResult;
}

RecordSelection *SiteResult::getRecordSelectionPtr()
{
    return &m_recordSelection;
}
