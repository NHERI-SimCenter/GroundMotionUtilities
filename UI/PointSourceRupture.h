#ifndef POINTSOURCERUPTURE_H
#define POINTSOURCERUPTURE_H

#include <QObject>
#include "RuptureLocation.h"

class PointSourceRupture : public QObject
{
    Q_OBJECT
public:
    PointSourceRupture(double magnitude, const RuptureLocation& location, double averageRake, double averageDip, QObject *parent = nullptr);

    double magnitude() const;
    double averageRake() const;
    double averageDip() const;

    RuptureLocation& location();

signals:
    void magnitudeChanged(double newMagnitude);
    void dipChanged(double newDip);
    void rakeChanged(double newRake);

public slots:
    void setMagnitude(double magnitude);
    void setAverageDip(double averageDip);
    void setAverageRake(double averageRake);

private:
    double m_magnitude;
    double m_averageRake;
    double m_averageDip;
    RuptureLocation m_location;
};

#endif // POINTSOURCERUPTURE_H
