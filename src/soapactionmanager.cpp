#include "soapactionmanager.h"
#include <QDebug>

QString SOAPXmlHeader = "<s:Envelope s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" "
                        "xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body><u:";
QString SOAPXmlInstanceId = " xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>0</InstanceID>";
QString SOAPXmlActions = "</u:";
QString SOAPXmlFooter = "></s:Body></s:Envelope>";

SOAPActionManager::SOAPActionManager(QObject *parent) : QObject(parent)
{
    mgr = new QNetworkAccessManager(this);
}

void SOAPActionManager::doAction(QString action, QString actionData, QUrl controlUrl)
{
    QNetworkRequest request;

    // Build xml request body
    QString data = SOAPXmlHeader + action + SOAPXmlInstanceId + actionData + SOAPXmlActions + action + SOAPXmlFooter;
    QByteArray actionHeader = QString("urn:schemas-upnp-org:service:AVTransport:1#" + action).toUtf8();

    request.setUrl(controlUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=utf-8");
    request.setRawHeader("SOAPAction", actionHeader);

    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(processData(QNetworkReply*)));

    mgr->post(request, data.toUtf8());
}

void SOAPActionManager::processData(QNetworkReply* reply)
{
    qDebug() << reply->readAll().data();
}
