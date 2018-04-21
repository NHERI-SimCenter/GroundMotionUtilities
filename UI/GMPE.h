#ifndef GMPE_H
#define GMPE_H

#include <QObject>

class GMPE : public QObject
{
    Q_OBJECT
public:
    explicit GMPE(QObject *parent = nullptr);

    QString type() const;
    const QStringList& validTypes();

signals:
    void typeChanged(QString newType);

public slots:
    bool setType(const QString &type);

private:
    QString m_type;
};

#endif // GMPE_H
