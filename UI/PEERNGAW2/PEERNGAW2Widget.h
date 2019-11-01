#ifndef PEERNGAW2WIDGET_H
#define PEERNGAW2WIDGET_H

#include <QWidget>
#include <QWebEngineView>

class PEERNGAW2Widget : public QWidget
{
    Q_OBJECT
private:
    QWebEngineView m_webView;

public:
    PEERNGAW2Widget(QWidget *parent = 0);
    ~PEERNGAW2Widget();
    void loginPEER(QString username, QString password);
};

#endif // PEERNGAW2WIDGET_H
