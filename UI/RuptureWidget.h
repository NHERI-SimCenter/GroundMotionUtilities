#ifndef RUPTUREWIDGET_H
#define RUPTUREWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include "PointSourceRupture.h"

class RuptureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuptureWidget(PointSourceRupture& rupture, QWidget *parent = nullptr);

signals:

public slots:

private:
    PointSourceRupture& m_eqRupture;
    QGroupBox* m_siteGroupBox;

    QDoubleSpinBox* m_magnitudeBox;

    QGroupBox* m_locationGroupBox;
    QDoubleSpinBox* m_latitudeBox;
    QDoubleSpinBox* m_longitudeBox;
    QDoubleSpinBox* m_depthBox;

    QGroupBox* m_geometryGroupBox;
    QDoubleSpinBox* m_dipBox;
    QDoubleSpinBox* m_rakeBox;

    void setupConnections();
};

#endif // RUPTUREWIDGET_H
