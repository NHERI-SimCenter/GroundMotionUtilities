#ifndef SITERESULT_H
#define SITERESULT_H
#include <QObject>
#include <QtCharts/QLineSeries>

using namespace QtCharts;

class SiteResult: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int recordId READ recordId WRITE setRecordId NOTIFY recordChanged)
    Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleChanged)


public:
    SiteResult(QObject *parent = nullptr);

    int recordId() const;
    void setRecordId(int recordId);

    double scaleFactor() const;
    void setScaleFactor(double scaleFactor);

signals:
    void recordChanged(int newRecordId);
    void scaleChanged(double newScaleFactor);

private:
    QLineSeries m_Means;
    QLineSeries m_stdDevs;
    QLineSeries m_interEventStdDevs;
    QLineSeries m_intraEventStdDevs;
    QList<QLineSeries> m_simulations;
    int m_recordId;
    double m_scaleFactor;
};

#endif // SITERESULT_H
