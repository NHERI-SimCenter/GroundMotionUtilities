#ifndef SITERESULTSWIDGET_H
#define SITERESULTSWIDGET_H

#include <QWidget>
#include <QLineSeries>
#include <QLogValueAxis>
#include <QChart>
#include <QTreeWidget>
#include "SiteResult.h"
#include "NGARecordsDb.h"

class SiteResultsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SiteResultsWidget(QWidget *parent = nullptr);
    void setRecordsDb(NGARecordsDb* db);
    void setSelectionSuite();

signals:

public slots:
    void setResult(int id, SiteResult* result);

private:
    QChart m_spectrumChart;
    QLineSeries m_meanSeries;
    QLineSeries m_lowerSeries;
    QLineSeries m_upperSeries;
    QLineSeries m_simulationSeries;
    QList<QSharedPointer<QLineSeries>> selectedSpectraSeries;

    QTreeWidget m_resultsTree;

    QLogValueAxis xAxis;
    QLogValueAxis yAxis;

    QTreeWidgetItem m_siteId;

    QTreeWidgetItem m_locationItem;
    QTreeWidgetItem m_latitudeItem;
    QTreeWidgetItem m_longitudeItem;

    QTreeWidgetItem m_recordSelection;

    QTreeWidgetItem m_saItem;
    QTreeWidgetItem m_saMeansItem;
    QTreeWidgetItem m_saStdDevsItem;
    QTreeWidgetItem m_saInterEvStdDevsItem;
    QTreeWidgetItem m_saIntraEvStdDevsItem;

    QTreeWidgetItem m_pgaItem;
    QTreeWidgetItem m_pgaMeanItem;
    QTreeWidgetItem m_pgaStdDevItem;
    QTreeWidgetItem m_pgaInterEvSDItem;
    QTreeWidgetItem m_pgaIntraEvSDItem;

    QTreeWidgetItem m_siteDataItem;

    void setTree(int id, SiteResult* result);
    void initTree();
    NGARecordsDb* m_recordsDb = NULL;
};

#endif // SITERESULTSWIDGET_H
