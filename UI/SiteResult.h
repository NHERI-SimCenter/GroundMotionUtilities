#ifndef SITERESULT_H
#define SITERESULT_H
#include <QObject>
#include <QtCharts/QLineSeries>
#include <Location.h>
#include <QHash>
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
    QVector<QSharedPointer<RecordSelection>>& recordSelection();
    QVector<double> periods();
    QVector<double> simulatedSpectrum();
    void addSiteData(QString name, QVariant value);
    void addSiteDataSource(QString name, QVariant value);
    void clearSiteData();
    void clearSiteDataSource();
    QHash<QString, QVariant>& siteData();
    QHash<QString, QVariant>& siteDataSources();

signals:

public slots:
    void setPeriods(QVector<double> periods);
    void setSimulatedSpectrum(QVector<double> simulatedSpectrum);

private:
    Location m_location;
    SAResult m_saResult;
    PGAResult m_pgaResult;
    QVector<QSharedPointer<RecordSelection>> m_recordSelection;
    QVector<double> m_periods;
    QVector<double> m_simulatedSpectrum;
    QHash<QString, QVariant> m_siteData;
    QHash<QString, QVariant> m_siteDataSources;

    Location* getLocationPtr();
    SAResult* getSAResultPtr();
    PGAResult* getPGAResultPtr();
    RecordSelection* getRecordSelectionPtr();
};

#endif // SITERESULT_H
