#include "SiteResultsModel.h"

SiteResultsModel::SiteResultsModel(QObject *parent): QAbstractListModel(parent)
{

}


int SiteResultsModel::rowCount(const QModelIndex &parent) const
{
    return m_siteResults.size();
}

QVariant SiteResultsModel::data(const QModelIndex &index, int role) const
{
    return QVariant::fromValue<SiteResult*>(m_siteResults.at(index.row()));
}

SiteResult *SiteResultsModel::createNewResult()
{
    beginInsertRows(QModelIndex(), m_siteResults.size(), m_siteResults.size());
    SiteResult* newResult = new SiteResult();
    m_siteResults.append(newResult);
    endInsertRows();
    return newResult;
}

void SiteResultsModel::clear()
{
    beginResetModel();
    m_siteResults.clear();
    endResetModel();
}
