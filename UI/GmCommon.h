#ifndef GMCOMMON_H
#define GMCOMMON_H
#include <QString>

class GmCommon
{
public:
    static QString getWorkFilePath(QString filename);
private:
    GmCommon();
};

#endif // GMCOMMON_H
