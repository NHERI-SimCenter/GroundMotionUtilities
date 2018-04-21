#include <QtAlgorithms>
#include "IntensityMeasure.h"

IntensityMeasure::IntensityMeasure(QObject *parent) : QObject(parent)
{
    this->setType("SA");
    this->addPeriod(1.0);
}

QString IntensityMeasure::type() const
{
    return m_type;
}

bool IntensityMeasure::setType(const QString &type)
{
    if(m_type != type && validTypes().contains(type, Qt::CaseInsensitive))
    {
        m_type = type;
        emit typeChanged(m_type);
        return true;
    }
    return false;
}

const QStringList &IntensityMeasure::validTypes()
{
    static QStringList validTypes = QStringList()
            << "PGA"
            << "SA";

    return validTypes;
}

QList<double> IntensityMeasure::periods() const
{
    return m_periods;
}

void IntensityMeasure::setPeriods(const QList<double> &periods)
{
    m_periods = periods;
}

void IntensityMeasure::addPeriod(double period)
{
    this->m_periods.append(period);
    qSort(this->m_periods);
}
