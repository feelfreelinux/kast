#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include "dlnarenderer.h"
#include "httpfileserver.h"
class Server : public QObject
{
    Q_OBJECT
public:
    Server(QObject *parent = 0);
    QHostAddress getLocalAddress();
private:
    HttpFileServer *fileServer;
private slots:
    void foundRenderer(DLNARenderer *renderer);
};
#endif // SERVER_H
