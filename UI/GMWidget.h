#ifndef GMWIDGET_H
#define GMWIDGET_H

#include <QWidget>
#include <QQuickItem>
#include "SiteWidget.h"
#include "RuptureWidget.h"
#include "GMPEWidget.h"
#include "IntensityMeasureWidget.h"
#include "RecordSelectionWidget.h"
#include "ScenarioProcessor.h"
#include "SiteConfigWidget.h"
#include "LocationsListModel.h"

namespace Ui {
class GMWidget;
}

class GMWidget : public QWidget
{
    Q_OBJECT
    Q_ENUMS(ApplicationMode)
    Q_PROPERTY(ApplicationMode mode READ mode WRITE setMode NOTIFY modeChanged)
public:
    explicit GMWidget(QWidget *parent = 0);
    ~GMWidget();
    enum ApplicationMode{Edit = 0, Results = 1};
    ApplicationMode mode() const;

signals:
    void modeChanged(ApplicationMode newMode);
    void locationsChanged();

public slots:
    void setMode(ApplicationMode mode);
    void updateLocations();

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
    ApplicationMode m_mode;
    LocationsListModel* m_locationsModel;
    QQuickItem* m_mapObject;

    void setupConnections();
};

#endif // GMWIDGET_H
