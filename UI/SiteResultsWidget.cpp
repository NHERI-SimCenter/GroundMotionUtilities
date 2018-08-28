#include "SiteResultsWidget.h"
#include <QLineSeries>
#include <QChartView>
#include <QVBoxLayout>
#include <QPushButton>

SiteResultsWidget::SiteResultsWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    //Add tree view
    initTree();
    layout->addWidget(&m_resultsTree, 0.8);

    m_spectrumChart.setTitle("Response Spectra");
    m_spectrumChart.addSeries(&m_meanSeries);
    m_spectrumChart.addSeries(&m_lowerSeries);
    m_spectrumChart.addSeries(&m_upperSeries);
    m_spectrumChart.addSeries(&m_simulationSeries);
    m_spectrumChart.addSeries(&m_selectionSeries);

    QPen blackPen;
    blackPen.setColor(Qt::black);
    m_meanSeries.setPen(blackPen);
    blackPen.setStyle(Qt::DotLine);
    m_lowerSeries.setPen(blackPen);
    m_upperSeries.setPen(blackPen);

    //Setting names
    m_meanSeries.setName("Mean");
    m_lowerSeries.setName("Mean - Std. Dev.");
    m_upperSeries.setName("Mean + Std. Dev.");
    m_simulationSeries.setName("Simulated");
    m_selectionSeries.setName("Selected");

    //Setting x Axis
    xAxis.setTitleText("Time [sec.]");
    m_spectrumChart.addAxis(&xAxis, Qt::AlignBottom);
    xAxis.setLabelFormat("%1.2f");
    xAxis.setMinorTickCount(-1);

    //Attaching x Axis
    m_meanSeries.attachAxis(&xAxis);
    m_lowerSeries.attachAxis(&xAxis);
    m_upperSeries.attachAxis(&xAxis);
    m_simulationSeries.attachAxis(&xAxis);
    m_selectionSeries.attachAxis(&xAxis);

    //Setting y Axis
    yAxis.setTitleText("Spectral Acceleration [g.]");
    m_spectrumChart.addAxis(&yAxis, Qt::AlignLeft);
    yAxis.setLabelFormat("%1.2f");
    yAxis.setMinorTickCount(-1);
    m_meanSeries.attachAxis(&yAxis);
    m_lowerSeries.attachAxis(&yAxis);
    m_upperSeries.attachAxis(&yAxis);
    m_simulationSeries.attachAxis(&yAxis);
    m_selectionSeries.attachAxis(&yAxis);

    QChartView* chartView = new QChartView(&m_spectrumChart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumWidth(400);
    chartView->setMinimumHeight(400);

    layout->addWidget(chartView);
    QPushButton* hideButton = new QPushButton(tr("&Hide"), this);
    connect(hideButton, &QPushButton::pressed, this, &QWidget::hide);
    layout->addWidget(hideButton);
    this->setMinimumWidth(400);
}

void SiteResultsWidget::setRecordsDb(NGARecordsDb* db)
{
    m_recordsDb = db;
}

void SiteResultsWidget::setResult(int id, SiteResult *result)
{
    m_meanSeries.clear();
    m_upperSeries.clear();
    m_lowerSeries.clear();
    m_simulationSeries.clear();
    m_selectionSeries.clear();

    QVector<double> periods = result->periods();
    QVector<double> saMeans = result->saResult().means();
    QVector<double> stdDevs = result->saResult().stdDevs();
    QVector<double> simulation = result->simulatedSpectrum();

    for (int i = 0; i < periods.size(); i++)
    {
        m_meanSeries << QPointF(periods[i], saMeans[i]);
        m_upperSeries << QPointF(periods[i], exp(log(saMeans[i]) - stdDevs[i]));
        m_lowerSeries << QPointF(periods[i], exp(log(saMeans[i])  + stdDevs[i]));
        m_simulationSeries << QPointF(periods[i], simulation[i]);
    }
    if(NULL != m_recordsDb)
    {
        QVector<double> recordSAs = m_recordsDb->getSpectrum(result->recordSelection().recordId());
        QVector<double> recordPeriods = m_recordsDb->getPeriods();
        for (int i = 0; i < recordPeriods.size(); i++)
        {
            m_selectionSeries << QPointF(recordPeriods[i], result->recordSelection().scaleFactor() * recordSAs[i]);
        }
    }

    xAxis.setMin(periods[0]);
    xAxis.setMax(periods[periods.size() - 1]);

    yAxis.setMin(0.01);
    yAxis.setMax(10);

    xAxis.setMinorTickCount(-1);
    yAxis.setMinorTickCount(-1);

    setTree(id, result);
}

void SiteResultsWidget::setTree(int id, SiteResult *result)
{
    //Site
    m_siteId.setText(1, QString::number(id));

    //Set location
    m_latitudeItem.setText(1, QString::number(result->location().latitude()));
    m_longitudeItem.setText(1, QString::number(result->location().longitude()));

    //Set record selection
    m_selectedId.setText(1, QString::number(result->recordSelection().recordId()));
    m_scaleFactor.setText(1, QString::number(result->recordSelection().scaleFactor()));

    //Set SiteData
    foreach(QTreeWidgetItem* pChild, m_siteDataItem.takeChildren())
        delete pChild;
    QHash<QString, QVariant>& siteData = result->siteData();
    QHash<QString, QVariant>& siteDataSources = result->siteDataSources();

    for(int i = 0; i < siteData.size(); i++)
    {
        QString dataType = siteData.keys()[i];
        if (dataType == "Vs30")
            dataType.append(" [m/s]");
        else if (dataType.startsWith("Depth", Qt::CaseInsensitive))
            dataType.append(" [km]");
        QTreeWidgetItem* siteDataItem = new QTreeWidgetItem(QStringList() << dataType << siteData.values()[i].toString());
        m_siteDataItem.addChild(siteDataItem);
        QTreeWidgetItem* siteDataSourceItem = new QTreeWidgetItem(QStringList() << "Source" << siteDataSources[siteData.keys()[i]].toString());
        siteDataItem->addChild(siteDataSourceItem);
        siteDataItem->setExpanded(true);
    }

    //Set PGA
    m_pgaMeanItem.setText(1, QString::number(result->pgaResult().mean()));
    m_pgaStdDevItem.setText(1, QString::number(result->pgaResult().stdDev()));
    m_pgaInterEvSDItem.setText(1, QString::number(result->pgaResult().interEvStdDev()));
    m_pgaIntraEvSDItem.setText(1, QString::number(result->pgaResult().intraEvStdDev()));

    //Set Spectral Accelerations
    QVector<double> periods = result->periods();
    QVector<double> saMeans = result->saResult().means();
    QVector<double> stdDevs = result->saResult().stdDevs();
    QVector<double> interEvStdDev = result->saResult().interEvStdDevs();
    QVector<double> intraEvStdDev = result->saResult().intraEvStdDevs();

    //SA Means
    foreach(QTreeWidgetItem* pChild, m_saMeansItem.takeChildren())
        delete pChild;

    for(int i = 0; i < periods.size(); i++)
    {
        QTreeWidgetItem* aMean = new QTreeWidgetItem(QStringList() << QString::number(periods[i]) << QString::number(saMeans[i]));
        m_saMeansItem.addChild(aMean);
    }

    //SA Std Devs
    foreach(QTreeWidgetItem* pChild, m_saStdDevsItem.takeChildren())
        delete pChild;

    for(int i = 0; i < periods.size(); i++)
    {
        QTreeWidgetItem* aStdDev = new QTreeWidgetItem(QStringList() << QString::number(periods[i]) << QString::number(stdDevs[i]));
        m_saStdDevsItem.addChild(aStdDev);
    }

    //SA Inter Event Std Devs
    foreach(QTreeWidgetItem* pChild, m_saInterEvStdDevsItem.takeChildren())
        delete pChild;

    for(int i = 0; i < periods.size(); i++)
    {
        QTreeWidgetItem* aStdDev = new QTreeWidgetItem(QStringList() << QString::number(periods[i]) << QString::number(interEvStdDev[i]));
        m_saInterEvStdDevsItem.addChild(aStdDev);
    }

    //SA Inter Event Std Devs
    foreach(QTreeWidgetItem* pChild, m_saIntraEvStdDevsItem.takeChildren())
        delete pChild;

    for(int i = 0; i < periods.size(); i++)
    {
        QTreeWidgetItem* aStdDev = new QTreeWidgetItem(QStringList() << QString::number(periods[i]) << QString::number(intraEvStdDev[i]));
        m_saIntraEvStdDevsItem.addChild(aStdDev);
    }
}

void SiteResultsWidget::initTree()
{
    //Site
    m_siteId.setText(0, "Site");
    m_resultsTree.addTopLevelItem(&m_siteId);

    //Location
    m_locationItem.setText(0, "Location");
    m_latitudeItem.setText(0, QString::fromWCharArray(L"Latitude [\u00B0]"));
    m_longitudeItem.setText(0, QString::fromWCharArray(L"Longitude [\u00B0]"));

    m_locationItem.addChild(&m_latitudeItem);
    m_locationItem.addChild(&m_longitudeItem);
    m_resultsTree.addTopLevelItem(&m_locationItem);

    //Site Data
    m_siteDataItem.setText(0, "Site Data");
    m_resultsTree.addTopLevelItem(&m_siteDataItem);

    //Record Selection
    m_recordSelection.setText(0, "Ground Motion");
    m_selectedId.setText(0, "Selected Record");
    m_recordSelection.addChild(&m_selectedId);
    m_scaleFactor.setText(0, "Scale Factor");
    m_recordSelection.addChild(&m_scaleFactor);
    m_resultsTree.addTopLevelItem(&m_recordSelection);

    //Peak Ground Accelerations
    m_pgaItem.setText(0, "Peak Ground Acceleration");
    m_resultsTree.addTopLevelItem(&m_pgaItem);
    m_pgaItem.addChild(&m_pgaMeanItem);
    m_pgaMeanItem.setText(0, "Mean [g]");
    m_pgaItem.addChild(&m_pgaStdDevItem);
    m_pgaStdDevItem.setText(0, "Std. Dev.");
    m_pgaItem.addChild(&m_pgaInterEvSDItem);
    m_pgaInterEvSDItem.setText(0, "Inter Ev. Std. Dev.");
    m_pgaItem.addChild(&m_pgaIntraEvSDItem);
    m_pgaIntraEvSDItem.setText(0, "Intra Ev. Std. Dev.");

    m_resultsTree.setHeaderLabels(QStringList() << "Property" << "Value");
    m_resultsTree.setColumnWidth(0, 160);

    //Spectral Accelerations
    m_saItem.setText(0, "Spectral Accelerations");
    m_saMeansItem.setText(0, "Mean [g]");
    m_saMeansItem.setExpanded(true);
    m_saItem.addChild(&m_saMeansItem);
    m_saStdDevsItem.setText(0, "Std. Dev.");
    m_saItem.addChild(&m_saStdDevsItem);
    m_saInterEvStdDevsItem.setText(0, "Inter Ev.Std. Dev.");
    m_saItem.addChild(&m_saInterEvStdDevsItem);
    m_saIntraEvStdDevsItem.setText(0, "Intra Ev.Std. Dev.");
    m_saItem.addChild(&m_saIntraEvStdDevsItem);

    m_resultsTree.addTopLevelItem(&m_saItem);
}
