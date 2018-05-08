#ifndef SITECONFIGWIDGET_H
#define SITECONFIGWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include "SiteConfig.h"

class SiteConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SiteConfigWidget(SiteConfig& siteconfig, QWidget *parent = nullptr);

signals:

public slots:

private:
    SiteConfig& m_siteConfig;
    QButtonGroup* m_typeButtonsGroup;
    QStackedWidget* m_stackedWidgets;

    void setupConnections();
};

#endif // SITECONFIGWIDGET_H
