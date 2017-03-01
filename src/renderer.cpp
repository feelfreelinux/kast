#include "renderer.h"
#include <QDebug>

Renderer::Renderer(QObject *parent) : QObject(parent) { }

QString Renderer::getControlUrl() { return controlUrl; }

QString Renderer::getName() { return serverName; }

QUrl Renderer::getUrl() { return serverUrl; }

void Renderer::setName(QString & name) { serverName = name; }

void Renderer::setUrl(QUrl url) { serverUrl = url; }

void Renderer::setControlUrl(QString url)
{
    controlUrl = url;
    fullcontrolUrl = serverUrl; fullcontrolUrl.setPath( url );
}

void Renderer::setPlaybackUrl(QUrl url)
{
    qDebug() << "We are here";
    mgr = new QNetworkAccessManager(this);
    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(test(QNetworkReply*)));
    mgr->get(QNetworkRequest(QUrl("http://google.pl")));
    qDebug() << "We are here";
}

void Renderer::test(QNetworkReply* reply)
{
    qDebug() << reply->readAll().data();
}
