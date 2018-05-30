#ifndef SARESULT_H
#define SARESULT_H

#include <QObject>
#include <QVector>

class SAResult : public QObject
{
    Q_OBJECT
public:
    explicit SAResult(QObject *parent = nullptr);

signals:
    void meansChanged(QVector<double> newMeans);
    void stdDevsChanged(QVector<double> newStdDevs);

public slots:
    void setMeans(QVector<double> means);
    void setStdDevs(QVector<double> stdDevs);

private:
    QVector<double> m_means;
    QVector<double> m_stdDevs;
    QVector<double> m_interEventStdDevs;
    QVector<double> m_intraEventStdDevs;
};

#endif // SARESULT_H
