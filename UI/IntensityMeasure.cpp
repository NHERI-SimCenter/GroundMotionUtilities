#include <QtAlgorithms>
#include <QJsonArray>
#include "IntensityMeasure.h"

IntensityMeasure::IntensityMeasure(QObject *parent) : QObject(parent)
{
    this->setType("All");
    setIsCorrelated(true);
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


void IntensityMeasure::setIsCorrelated(bool isCorrelated)
{
    if(m_isCorrelated != isCorrelated)
    {
        m_isCorrelated = isCorrelated;
        emit correlationChanged(m_isCorrelated);
    }
}

QJsonObject IntensityMeasure::getJson()
{
    QJsonObject im;
    im.insert("Type", m_type);
    im.insert("EnableJsonOutput", true);
    im.insert("EnableGeoJsonOutput", true);

    return im;
}

const QStringList &IntensityMeasure::validTypes()
{
    static QStringList validTypes = QStringList()
            << "PGA"
            << "SA"
            <<  "All";

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

bool IntensityMeasure::IsCorrelated() const
{
    return m_isCorrelated;
}
