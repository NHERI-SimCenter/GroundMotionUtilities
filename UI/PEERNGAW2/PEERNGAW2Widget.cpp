#include "PEERNGAW2Widget.h"
#include <QGridLayout>

PEERNGAW2Widget::PEERNGAW2Widget(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout* layout = new QGridLayout();
    layout->addWidget(&m_webView);
    m_webView.setUrl(QUrl("https://ngawest2.berkeley.edu/users/sign_in"));
    loginPEER("el7addad@hotmail.com", "PeerPassword123");
    this->setLayout(layout);
}

PEERNGAW2Widget::~PEERNGAW2Widget()
{

}

void PEERNGAW2Widget::loginPEER(QString username, QString password)
{
    QWebEnginePage* signInPage = m_webView.page();
    signInPage->runJavaScript();
}
