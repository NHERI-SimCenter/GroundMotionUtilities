#ifndef INTENSITYMEASURE_H
#define INTENSITYMEASURE_H

#include <QObject>
#include "JsonSerializable.h"

class IntensityMeasure : public QObject, JsonSerializable
{
    Q_OBJECT
public:
    explicit IntensityMeasure(QObject *parent = nullptr);

    QString type() const;

    const QStringList& validTypes();

    QList<double> periods() const;
    void setPeriods(const QList<double> &periods);
    void addPeriod(double period);
    bool IsCorrelated() const;

signals:
    void typeChanged(QString newType);
    void correlationChanged(bool newCorrelation);

public slots:
    bool setType(const QString &type);
    void setIsCorrelated(bool isCorrelated);

private:
    QString m_type;
    QList<double> m_periods;
    bool m_isCorrelated;//spatial correlation

    // JsonSerializable interface
public:
    QJsonObject getJson();
};

#endif // INTENSITYMEASURE_H
