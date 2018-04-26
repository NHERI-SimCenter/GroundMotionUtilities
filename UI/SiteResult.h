#ifndef SITERESULT_H
#define SITERESULT_H
#include <QObject>
#include <QList>

class SiteResult
{
public:
    SiteResult();

private:
    QList<double> m_means;
    QList<double> m_stdDevs;
    QList<double> m_interEventStdDevs;
    QList<double> m_intraEventStdDevs;
};

#endif // SITERESULT_H
