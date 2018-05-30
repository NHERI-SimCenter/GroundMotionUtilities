#ifndef SITERESULT_H
#define SITERESULT_H
#include <QObject>
#include <QtCharts/QLineSeries>
#include <Location.h>
#include "SAResult.h"
#include "PGAResult.h"
#include "RecordSelection.h"

using namespace QtCharts;

class SiteResult: public QObject
{
    Q_OBJECT
    Q_PROPERTY(Location* location READ getLocationPtr CONSTANT)
    Q_PROPERTY(SAResult* saResult READ getSAResultPtr CONSTANT)
    Q_PROPERTY(PGAResult* pgaResult READ getPGAResultPtr CONSTANT)
    Q_PROPERTY(RecordSelection* recordSelection READ getRecordSelectionPtr CONSTANT)

public:
    SiteResult(QObject *parent = nullptr);
    Location& location();
    SAResult& saResult();
    PGAResult& pgaResult();
    RecordSelection& recordSelection();

signals:

private:
    Location m_location;
    SAResult m_saResult;
    PGAResult m_pgaResult;
    RecordSelection m_recordSelection;

    Location* getLocationPtr();
    SAResult* getSAResultPtr();
    PGAResult* getPGAResultPtr();
    RecordSelection* getRecordSelectionPtr();

};

#endif // SITERESULT_H
