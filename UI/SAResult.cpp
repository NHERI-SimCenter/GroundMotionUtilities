#include "SAResult.h"

SAResult::SAResult(QObject *parent) : QObject(parent)
{

}

void SAResult::setMeans(QVector<double> means)
{
    m_means = means;
    emit meansChanged(m_means);
}

void SAResult::setStdDevs(QVector<double> stdDevs)
{
    m_stdDevs = stdDevs;
    emit stdDevsChanged(m_stdDevs);
}

void SAResult::setInterEvStdDevs(QVector<double> interEvstdDevs)
{
    m_interEventStdDevs = interEvstdDevs;
}

void SAResult::setIntraEvStdDevs(QVector<double> intraEvstdDevs)
{
    m_intraEventStdDevs = intraEvstdDevs;
}

QVector<double> SAResult::stdDevs() const
{
    return m_stdDevs;
}

QVector<double> SAResult::interEvStdDevs() const
{
    return m_interEventStdDevs;
}

QVector<double> SAResult::intraEvStdDevs() const
{
    return m_intraEventStdDevs;
}

QVector<double> SAResult::means() const
{
    return m_means;
}
