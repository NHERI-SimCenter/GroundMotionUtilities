#ifndef SARESULT_H
#define SARESULT_H

#include <QObject>
#include <QVector>

class SAResult : public QObject
{
    Q_OBJECT
public:
    explicit SAResult(QObject *parent = nullptr);
    QVector<double> means() const;
    QVector<double> stdDevs() const;
    QVector<double> interEvStdDevs() const;
    QVector<double> intraEvStdDevs() const;

signals:
    void meansChanged(QVector<double> newMeans);
    void stdDevsChanged(QVector<double> newStdDevs);

public slots:
    void setMeans(QVector<double> means);
    void setStdDevs(QVector<double> stdDevs);
    void setInterEvStdDevs(QVector<double> interEvstdDevs);
    void setIntraEvStdDevs(QVector<double> intraEvstdDevs);

private:
    QVector<double> m_means;
    QVector<double> m_stdDevs;
    QVector<double> m_interEventStdDevs;
    QVector<double> m_intraEventStdDevs;
};

#endif // SARESULT_H
