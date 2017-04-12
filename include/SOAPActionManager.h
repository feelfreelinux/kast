#ifndef SOAPACTIONMANAGER_H
#define SOAPACTIONMANAGER_H
#include "DLNAPlaybackInfo.h"
#include <QObject>
#include <QtNetwork>

// Xml request's body
const QString SOAPXmlHeader = "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\" ?>"
                              "<s:Envelope s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" "
                              "xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body><u:",
              SOAPXmlInstanceId = " xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>0</InstanceID>",
              SOAPXmlActions = "</u:",
              SOAPXmlFooter = "></s:Body></s:Envelope>",
              DIDLLiteString = "<DIDL-Lite xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\""
                               " xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\""
                               " xmlns:dc=\"http://purl.org/dc/elements/1.1/\""
                               " xmlns:sec=\"http://www.sec.co.kr/\"><item id=\"f-0\" parentID=\"0\" restricted=\"0\">"
                               "<dc:title>%1</dc:title>"
                               "<dc:creator>%2</dc:creator>"
                               "<upnp:class>object.item.%3Item</upnp:class>"
                               "<res protocolInfo=\"http-get:*:%4:DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000\" >"
                               "%5</res></item></DIDL-Lite>";

class SOAPActionManager : public QObject
{
    Q_OBJECT
public:
    explicit SOAPActionManager(QObject *parent = 0);
    void doAction(const QString &action, const QMap<QString, QString> &dataMap, const QUrl &controlUrl);
    QString generateDIDLLite(const QFileInfo &fileInfo, const QString & address);
signals:
    void receivePlaybackInfo(DLNAPlaybackInfo*);
private:
    QNetworkAccessManager *mgr;
private slots:
    void processData(QNetworkReply* reply);
    void processPlaybackInfo(QNetworkReply* reply);
};

#endif // SOAPACTIONMANAGER_H
