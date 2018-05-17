#include "GmCommon.h"
#include <QStandardPaths>
#include <QDir>

QString GmCommon::getWorkFilePath(QString filename)
{
    QString dataDirectory = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::GenericDataLocation);
    QString workDir = QDir::cleanPath(dataDirectory + QDir::separator() + "SimCenter-EQSS");
    return QDir::cleanPath(workDir + QDir::separator() + filename);
}

GmCommon::GmCommon()
{

}
