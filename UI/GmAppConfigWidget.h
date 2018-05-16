#ifndef GMAPPCONFIGWIDGET_H
#define GMAPPCONFIGWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "GmAppConfig.h"

class GmAppConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GmAppConfigWidget(GmAppConfig &appConfig, QWidget *parent = nullptr);

signals:

public slots:
private:
    GmAppConfig& m_appConfig;
    QLineEdit* m_eqHazardBox;
    QPushButton* m_eqHazardButton;
    QLineEdit* m_simulateIMBox;
    QPushButton* m_simulateIMButton;
    QLineEdit* m_selectRecordBox;
    QPushButton* m_selectRecordButton;
    QLineEdit* m_ngaw2DbBox;
    QPushButton* m_ngaw2DbButton;
    QLineEdit* m_ngaw2DbSubsetBox;
    QPushButton* m_ngaw2DbSubsetButton;

    void setupConnections();
};

#endif // GMAPPCONFIGWIDGET_H
