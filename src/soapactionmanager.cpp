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
    QByteArray data, actionHeader("urn:schemas-upnp-org:service:AVTransport:1#");

    // Build xml request body
    data.append(SOAPXmlHeader);
    data.append(action);
    data.append(SOAPXmlInstanceId);
    data.append(actionData);
    data.append(SOAPXmlActions);
    data.append(action);
    data.append(SOAPXmlFooter);

    actionHeader.append(action);

    request.setUrl(controlUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=utf-8");
    request.setRawHeader("SOAPAction", actionHeader);

    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(processData(QNetworkReply*)));

    mgr->post(request, data);
}

void SOAPActionManager::processData(QNetworkReply* reply)
{
    qDebug() << reply->readAll().data();
}
