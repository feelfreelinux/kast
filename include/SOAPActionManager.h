#ifndef SOAPACTIONMANAGER_H
#define SOAPACTIONMANAGER_H
#include "DLNAPlaybackInfo.h"
#include <QObject>
#include <QtNetwork>

// Xml request's body
const QString SOAPXmlHeader = "<s:Envelope s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" "
                              "xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body><u:",
              SOAPXmlInstanceId = " xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>0</InstanceID>",
              SOAPXmlActions = "</u:",
              SOAPXmlFooter = "></s:Body></s:Envelope>";

class SOAPActionManager : public QObject
{
    Q_OBJECT
public:
    explicit SOAPActionManager(QObject *parent = 0);
    void doAction(const QString &action, const QMap<QString, QString> &dataMap, const QUrl &controlUrl);
signals:
    void receivePlaybackInfo(DLNAPlaybackInfo*);
private:
    QNetworkAccessManager *mgr;
private slots:
    void processData(QNetworkReply* reply);
    void processPlaybackInfo(QNetworkReply* reply);
};

#endif // SOAPACTIONMANAGER_H
