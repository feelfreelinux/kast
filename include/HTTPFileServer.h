#ifndef HTTPFILESERVER_H
#define HTTPFILESERVER_H

#include <QtNetwork>
#include "HTTPFileClient.h"

// HTTPServer's port
const int port = 5437;

class HttpFileServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit HttpFileServer(QObject *parent = 0);
    void startServer();
    // Adds file to server stack and returns the ID of added file
    int serveFile(const QUrl & path);
    QString getFilenameFromID(const int id);
private:
    QMap<int, QUrl> sharedFiles;
protected:
    void incomingConnection(qintptr socketDescriptor);
};
#endif // HTTPFILESERVER_H
