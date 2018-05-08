#ifndef GMWIDGET_H
#define GMWIDGET_H

#include <QWidget>
#include "SiteWidget.h"
#include "RuptureWidget.h"
#include "GMPEWidget.h"
#include "IntensityMeasureWidget.h"
#include "RecordSelectionWidget.h"
#include "ScenarioProcessor.h"
#include "SiteConfigWidget.h"

namespace Ui {
class GMWidget;
}

class GMWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GMWidget(QWidget *parent = 0);
    ~GMWidget();

private:
    Ui::GMWidget *ui;
    RuptureWidget* m_ruptureWidget;
    PointSourceRupture* m_eqRupture;
    GMPE* m_gmpe;
    GMPEWidget* m_gmpeWidget;
    IntensityMeasure* m_intensityMeasure;
    IntensityMeasureWidget* m_intensityMeasureWidget;
    RecordSelectionConfig* m_selectionconfig;
    RecordSelectionWidget* m_selectionWidget;
    SiteResult* m_siteResult;
    ScenarioProcessor* m_scenarioProcessor;
    SiteConfig* m_siteConfig;
    SiteConfigWidget* m_siteConfigWidget;
};

#endif // GMWIDGET_H
