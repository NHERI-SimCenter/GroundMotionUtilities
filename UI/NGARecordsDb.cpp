#include "NGARecordsDb.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QString>
#include <QtConcurrent/QtConcurrent>

NGARecordsDb& NGARecordsDb::getInstance(QString dbPath)
{
    static NGARecordsDb instance(dbPath);
    return instance;
}

QVector<double> NGARecordsDb::getSpectrum(int id)
{
    return m_spectrumsMap[id];
}

QVector<double> NGARecordsDb::getPeriods()
{
    return m_periods;
}

NGARecordsDb::NGARecordsDb(QString dbPath)
{
    QtConcurrent::run(this, &NGARecordsDb::readDb, dbPath);

}

void NGARecordsDb::readDb(QString dbPath)
{
    QMap<double, int> saIndex;

    QFile dbFile(dbPath);
    if (dbFile.open(QIODevice::ReadOnly))
    {
       QTextStream dbStream(&dbFile);
       bool isHeaderProcessed = false;
       while (!dbStream.atEnd())
       {
          QString line = dbStream.readLine();

          if(!isHeaderProcessed)
          {
              //We will process the header
              QStringList columnNames = line.split(",");

              for (int i = 0; i < columnNames.size(); i++)
              {
                  QString& column = columnNames[i];

                  if(column.startsWith("T") && column.endsWith("S"))
                  {
                      double period = column.mid(1, column.size() - 2).toDouble();
                      saIndex[period] = i;
                  }
              }
              m_periods = saIndex.keys().toVector();
              isHeaderProcessed = true;
          }
          else
          {
              QStringList tokens = line.split(",");
              int id = tokens[0].toInt();
              QVector<double> spectrum;
              foreach(double period, m_periods)
              {
                  double sa = tokens[saIndex[period]].toDouble();
                  spectrum.push_back(sa);
              }
              m_spectrumsMap.insert(id, spectrum);
          }
       }
       dbFile.close();
    }
}

