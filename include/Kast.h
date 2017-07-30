#ifndef KAST_H
#define KAST_H

#include "DLNARenderer.h"
#include "HTTPFileServer.h"
#include "DLNAPlaybackInfo.h"

#include <QtNetwork>


class Kast : public QObject
{
    Q_OBJECT
public:
    Kast(QObject *parent = 0);
    void addItemToQueue(QString & item_url);
    QHostAddress getLocalAddress();
private:
    HttpFileServer *fileServer;
    QStringList queue;
private slots:
    void foundRenderer(DLNARenderer*);
    void handleResponse(const QString, const QString);
};
#endif // KAST_H
