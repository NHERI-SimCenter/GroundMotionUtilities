#ifndef RECORDSELECTIONCONFIG_H
#define RECORDSELECTIONCONFIG_H

#include <QObject>

class RecordSelectionConfig : public QObject
{
    Q_OBJECT

public:
    explicit RecordSelectionConfig(QObject *parent = nullptr);
    enum RecordDb {NGAWest2 = 0, NGAWest2Subset1000};
    Q_ENUM(RecordDb)

    enum ErrorMetric {AbsSum = 0, RMSE = 1, MSE = 2, MAPE = 3};
    Q_ENUM(ErrorMetric)

    RecordDb database() const;
    ErrorMetric error() const;

signals:
    void databaseChanged(RecordDb newDatabase);
    void errorChanged(ErrorMetric newError);

public slots:
    void setDatabase(const RecordDb &database);
    void setError(const ErrorMetric &error);

private:
    RecordDb m_database;
    ErrorMetric m_error;
};

#endif // RECORDSELECTIONCONFIG_H
