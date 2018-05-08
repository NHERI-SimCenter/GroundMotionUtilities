#ifndef SITERESULTSMODEL_H
#define SITERESULTSMODEL_H
#include <QAbstractListModel>
#include <SiteResult.h>

class SiteResultsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    SiteResultsModel(QObject *parent = 0);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    SiteResult* createNewResult();
    void clear();

private:
    QList<SiteResult*> m_siteResults;
};

#endif // SITERESULTSMODEL_H
