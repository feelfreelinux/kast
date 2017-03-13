#ifndef SSDPDISCOVERY_H
#define SSDPDISCOVERY_H

#include <QObject>
#include <QtNetwork>
#include <DLNARenderer.h>

class SSDPdiscovery : public QObject
{
    Q_OBJECT
public:
    explicit SSDPdiscovery(QObject *parent = 0);
    void findRendererFromUrl(const QUrl &url);
    void begin();
private slots:
    void processData(QNetworkReply*);
    void processPendingDatagrams();
private:
    QNetworkAccessManager *nmgr;
    QUdpSocket *udpSocket;
    QHostAddress groupAddress;
signals:
    void foundRenderer(DLNARenderer *renderer);
};

#endif // SSDPDISCOVERY_H
