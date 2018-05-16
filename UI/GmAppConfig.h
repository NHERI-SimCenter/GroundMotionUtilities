#ifndef GMAPPCONFIG_H
#define GMAPPCONFIG_H

#include <QObject>

class GmAppConfig : public QObject
{
    Q_OBJECT
public:
    explicit GmAppConfig(QObject *parent = nullptr);

    QString eqHazardPath() const;
    QString simulateIMPath() const;
    QString selectRecordPath() const;
    QString NGAW2DbPath() const;
    QString NGAW2SubsetDbPath() const;
    bool validate();

signals:
    void eqHazardPathChanged(QString newPath);
    void simulateIMPathChanged(QString newPath);
    void selectRecordPathChanged(QString newPath);
    void NGAW2DbPathChanged(QString newPath);
    void NGAW2SubsetDbChanged(QString newPath);

public slots:
    void setEqHazardPath(const QString &eqHazardPath);
    void setSimulateIMPath(const QString &simulateIMPath);
    void setSelectRecordPath(const QString &selectRecordPath);
    void setNGAW2DbPath(const QString &NGAW2DbPath);
    void setNGAW2SubsetDbPath(const QString &NGAW2SubsetDbPath);

private:
    QString m_eqHazardPath;
    QString m_simulateIMPath;
    QString m_selectRecordPath;
    QString m_NGAW2DbPath;
    QString m_NGAW2SubsetDbPath;
};

#endif // GMAPPCONFIG_H
