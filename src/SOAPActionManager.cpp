#include "SOAPActionManager.h"
#include <QDebug>

SOAPActionManager::SOAPActionManager(QObject *parent) : QObject(parent)
{
    mgr = new QNetworkAccessManager(this);
}

void SOAPActionManager::doAction(const QString & action, const QString & actionData, const QUrl & controlUrl)
{
    QNetworkRequest request;

    // Build xml request body
    QString data = SOAPXmlHeader + action + SOAPXmlInstanceId + actionData + SOAPXmlActions + action + SOAPXmlFooter;
    QByteArray actionHeader = QString("\"urn:schemas-upnp-org:service:AVTransport:1#" + action + "\"").toUtf8();

    // Set needed headers
    request.setUrl(controlUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=utf-8");
    request.setRawHeader("SOAPAction", actionHeader);

    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(processData(QNetworkReply*)));

    mgr->post(request, data.toUtf8());
}

void SOAPActionManager::processData(QNetworkReply* reply)
{
    // Write debug log
    qDebug() << reply->readAll().data();
}
