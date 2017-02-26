#ifndef HTTPFILESERVER_H
#define HTTPFILESERVER_H

#include <QtNetwork>
class HttpFileServer : public QObject
{
    Q_OBJECT
public:
    HttpFileServer(int port, QHostAddress address, QObject *parent = 0);
    int serveFile(QUrl path);
private:
    QTcpServer *server;
private slots:
    void handleIncoming();

};

#endif // HTTPFILESERVER_H