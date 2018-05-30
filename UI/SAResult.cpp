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
