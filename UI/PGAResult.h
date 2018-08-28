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
    double stdDev() const;
    double interEvStdDev() const;
    double intraEvStdDev() const;

signals:
    void meanChanged(double newMean);

public slots:
    void setMean(double mean);
    void setStdDev(double stdDev);
    void setInterEvStdDev(double interEvStdDev);
    void setIntraEvStdDev(double intraEvStdDev);

private:
    double m_mean;
    double m_stdDev;
    double m_interEvStdDev;
    double m_intraEvStdDev;
};

#endif // PGARESULT_H
