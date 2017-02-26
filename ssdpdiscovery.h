#ifndef SSDPDISCOVERY_H
#define SSDPDISCOVERY_H

#include <QObject>
#include <QtNetwork>
#include <dlnarenderer.h>

class SSDPdiscovery : public QObject
{
    Q_OBJECT
public:
    explicit SSDPdiscovery(QObject *parent = 0);
private slots:
    void processData();
    void processPendingDatagrams();
private:
    QNetworkAccessManager *nmgr;
    QNetworkReply *reply;
    QUdpSocket *udpSocket;
    QHostAddress groupAddress;
signals:
    void foundRenderer( DLNARenderer *renderer );
};

#endif // SSDPDISCOVERY_H
