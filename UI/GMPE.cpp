#include "GMPE.h"


GMPE::GMPE(QObject *parent) : QObject(parent)
{
    this->m_type = "Abrahamson, Silva & Kamai (2014)";
}

QString GMPE::type() const
{
    return m_type;
}

bool GMPE::setType(const QString &type)
{
    if(m_type!= type && this->validTypes().contains(type, Qt::CaseInsensitive))
    {
        m_type = type;
        emit typeChanged(m_type);
        return true;
    }

    return false;
}

const QStringList &GMPE::validTypes()
{
    static QStringList validTypes = QStringList()
            << "Abrahamson, Silva & Kamai (2014)"
            << "Boore, Stewart, Seyhan & Atkinson (2014)"
            << "Campbell & Bozorgnia (2014)"
            << "Chiou & Youngs (2014)"
            << "Idriss (2014)";

    return validTypes;
}
