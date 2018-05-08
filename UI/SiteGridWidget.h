#ifndef SITEGRIDWIDGET_H
#define SITEGRIDWIDGET_H

#include <QWidget>
#include <SiteGrid.h>
#include <QtWidgets>

class SiteGridWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SiteGridWidget(SiteGrid& siteGrid, QWidget *parent = nullptr);

signals:

public slots:

private:
    SiteGrid& m_siteGrid;
    QDoubleSpinBox* m_minLatitudeBox;
    QDoubleSpinBox* m_maxLatitudeBox;
    QSpinBox* m_latitudeDivBox;

    QDoubleSpinBox* m_minLongitudeBox;
    QDoubleSpinBox* m_maxLongitudeBox;
    QSpinBox* m_longitudeDivBox;
    void setupConnections();
};

#endif // SITEGRIDWIDGET_H
