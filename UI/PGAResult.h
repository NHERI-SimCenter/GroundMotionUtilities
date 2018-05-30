#ifndef PGARESULT_H
#define PGARESULT_H

#include <QObject>

class PGAResult : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double mean READ mean WRITE setMean NOTIFY meanChanged)
public:
    explicit PGAResult(QObject *parent = nullptr);

    double mean() const;

signals:
    void meanChanged(double newMean);

public slots:
    void setMean(double mean);

private:
    double m_mean;
    double m_interEvStdDev;
    double m_intraEvStdDev;
    double m_totalStdDev;
};

#endif // PGARESULT_H
