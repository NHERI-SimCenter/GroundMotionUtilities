#ifndef SCENARIOPROCESSOR_H
#define SCENARIOPROCESSOR_H

#include <QObject>
#include "Site.h"
#include "PointSourceRupture.h"
#include "GMPE.h"
#include "IntensityMeasure.h"
#include "RecordSelectionConfig.h"

class ScenarioProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ScenarioProcessor(Site& site, PointSourceRupture& rupture, GMPE& gmpe, IntensityMeasure& intensityMeasure, RecordSelectionConfig& selectionConfig, QObject *parent = nullptr);

signals:

public slots:
        void processScenario();
private:
        Site& m_site;
        PointSourceRupture& m_rupture;
        GMPE& m_gmpe;
        IntensityMeasure& m_intensityMeasure;
        RecordSelectionConfig& m_selectionConfig;

        // JsonSerializable interface
public:
};

#endif // SCENARIOPROCESSOR_H
