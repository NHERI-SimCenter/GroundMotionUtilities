#ifndef SITERESULT_H
#define SITERESULT_H
#include <QObject>
#include <QtCharts/QLineSeries>
#include <Location.h>

using namespace QtCharts;

class SiteResult: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int recordId READ recordId WRITE setRecordId NOTIFY recordChanged)
    Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleChanged)
    Q_PROPERTY(Location* location READ getLocationPtr CONSTANT)
    Q_PROPERTY(double mean READ getMean0 NOTIFY meanChanged)

public:
    SiteResult(QObject *parent = nullptr);

    int recordId() const;
    void setRecordId(int recordId);

    double scaleFactor() const;
    void setScaleFactor(double scaleFactor);

    void setMeans(QVector<double> means);
    Location& location();

signals:
    void recordChanged(int newRecordId);
    void scaleChanged(double newScaleFactor);
    void meanChanged(double newMean);

private:
    Location m_location;
    QVector<double> m_means;
    QVector<double> m_stdDevs;
    QVector<double> m_interEventStdDevs;
    QVector<double> m_intraEventStdDevs;
    int m_recordId;
    double m_scaleFactor;

    Location* getLocationPtr();
    double getMean0();
};

#endif // SITERESULT_H
