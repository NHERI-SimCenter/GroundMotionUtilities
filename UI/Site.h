#ifndef SITE_H
#define SITE_H

#include <QObject>
#include <Location.h>

class Site : public QObject
{
    Q_OBJECT
public:
    explicit Site(QObject *parent = nullptr);
    explicit Site(double longitude, double latitude, QObject *parent = nullptr);
    void setLocation(const Location &location);
    void setLocation(double latitude, double longitude);
    Location& location();

signals:

public slots:

private:
    Location m_location;
};

#endif // SITE_H
