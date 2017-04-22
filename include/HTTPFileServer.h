#ifndef HTTPFILESERVER_H
#define HTTPFILESERVER_H

#include <QtNetwork>
#include <QString>

class HttpFileServer : public QObject
{
    Q_OBJECT
public:
    HttpFileServer(QHostAddress address, int port, QObject *parent = 0);
    // Adds file to server stack and returns the ID of added file
    int serveFile(const QUrl & path);
    QString getFilenameFromID(const int id);
private:
    QTcpServer *server;
    QMap<int, QUrl> sharedFiles;
private slots:
    void handleIncoming();
    void readSocketData();
};

#endif // HTTPFILESERVER_H
