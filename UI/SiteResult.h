#ifndef SITERESULT_H
#define SITERESULT_H
#include <QObject>
#include <QtCharts/QLineSeries>

using namespace QtCharts;

class SiteResult
{
public:
    SiteResult();

private:

    QLineSeries a;
    QLineSeries m_stdDevs;
    QLineSeries m_interEventStdDevs;
    QLineSeries m_intraEventStdDevs;
};

#endif // SITERESULT_H
