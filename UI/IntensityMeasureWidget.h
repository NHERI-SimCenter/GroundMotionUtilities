#ifndef INTENSITYMEASUREWIDGET_H
#define INTENSITYMEASUREWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include "IntensityMeasure.h"

class IntensityMeasureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IntensityMeasureWidget(IntensityMeasure& im, QWidget *parent = nullptr);

signals:

public slots:

private:
    IntensityMeasure& m_intensityMeasure;
    QComboBox* m_typeBox;

    void setupConnections();
};

#endif // INTENSITYMEASUREWIDGET_H
