#include "soapactionmanager.h"
#include <QtNetwork>
#include <QDebug>

SOAPActionManager::SOAPActionManager(QObject *parent) : QObject(parent)
{
    mgr = new QNetworkAccessManager(this);
}

void SOAPActionManager::doAction(QString action, QString requestData, QUrl controlUrl)
{
    QNetworkRequest request;
    QByteArray data, actionData("urn:schemas-upnp-org:service:AVTransport:1#");
    data.append(requestData);
    actionData.append(action);

    request.setUrl(controlUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=utf-8");
    request.setRawHeader("SOAPAction", actionData);

    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(processData(QNetworkReply*)));

    mgr->post(request, data);
}

void SOAPActionManager::processData(QNetworkReply* reply)
{
    qDebug() << reply->readAll().data();
}
