#ifndef SERVER_H
#define SERVER_H

#include "dlnarenderer.h"
#include "httpfileserver.h"

#include <QtNetwork>

const int port = 1337;

class Server : public QObject
{
    Q_OBJECT
public:
    Server(QStringList & core, QObject *parent = 0);
    QHostAddress getLocalAddress();
private:
    HttpFileServer *fileServer;
    QStringList filesList;
private slots:
    void foundRenderer(DLNARenderer *renderer);
};
#endif // SERVER_H
