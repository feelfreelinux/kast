#ifndef SSDPDISCOVERY_H
#define SSDPDISCOVERY_H

#include <QtNetwork>
#include <DLNARenderer.h>
#include <QSet>
#include <QList>

class SSDPdiscovery : public QObject
{
    Q_OBJECT
public:
    explicit SSDPdiscovery(QObject *parent = 0);
    void findRendererFromUrl(const QUrl & url);
    void run();
private slots:
    void processData(QNetworkReply*);
    void processPendingDatagrams();
private:
    QNetworkAccessManager *nmgr;
    QUdpSocket *udpSocket;
    QSet<QString> known_urls;
    QList<DLNARenderer*> known_renderers;
signals:
    void foundRenderer(DLNARenderer *renderer);
};

#endif // SSDPDISCOVERY_H
