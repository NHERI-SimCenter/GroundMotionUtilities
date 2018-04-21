#ifndef SITEWIDGET_H
#define SITEWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include "Site.h"

class SiteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SiteWidget(Site& site, QWidget *parent = nullptr);

signals:

public slots:

private:
    Site& m_site;
    QGroupBox* m_siteGroupBox;
    QGroupBox* m_locationGroupBox;
    QDoubleSpinBox* m_latitudeBox;
    QDoubleSpinBox* m_longitudeBox;

    void setupConnections();
};

#endif // SITEWIDGET_H
