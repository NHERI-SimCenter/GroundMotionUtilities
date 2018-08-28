#ifndef NGARECORDSDB_H
#define NGARECORDSDB_H
#include <QString>
#include <QMap>
#include <QVector>

class NGARecordsDb
{
public:
    static NGARecordsDb& getInstance(QString dbPath);
    QVector<double> getSpectrum(int id);
    QVector<double> getPeriods();

private:
    NGARecordsDb(QString dbPath);
    NGARecordsDb(NGARecordsDb const&);
    void operator=(NGARecordsDb const&);
    void readDb(QString dbPath);

    QMap<int, QVector<double>> m_spectrumsMap;
    QVector<double> m_periods;

};

#endif // NGARECORDSDB_H
