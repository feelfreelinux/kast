#ifndef HTTPFILESERVER_H
#define HTTPFILESERVER_H

#include <QtNetwork>
#include <QString>

class HttpFileServer : public QObject
{
    Q_OBJECT
public:
    HttpFileServer(int port, QHostAddress address, QObject *parent = 0);
    // Adds file to server stack and returns the ID of added file
    int serveFile(const QUrl & path);

    QString getFilenameFromID(int id);
private:
    QTcpServer *server;
    QMap<int, QUrl> fileMap;
private slots:
    void handleIncoming();
};

#endif // HTTPFILESERVER_H
