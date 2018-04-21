#ifndef INTENSITYMEASURE_H
#define INTENSITYMEASURE_H

#include <QObject>

class IntensityMeasure : public QObject
{
    Q_OBJECT
public:
    explicit IntensityMeasure(QObject *parent = nullptr);

    QString type() const;

    const QStringList& validTypes();

    QList<double> periods() const;
    void setPeriods(const QList<double> &periods);
    void addPeriod(double period);

signals:
    void typeChanged(QString newType);

public slots:
    bool setType(const QString &type);

private:
    QString m_type;
    QList<double> m_periods;
};

#endif // INTENSITYMEASURE_H
