#include "HTTPFileServer.h"

HttpFileServer::HttpFileServer(QObject *parent) :
    QTcpServer(parent)
{

}

void HttpFileServer::startServer()
{
    if (listen(QHostAddress::Any, port))
        qDebug() << "FileServer started";
    else qFatal("Cannot start file server. Maybe port is unavailable?");
}

void HttpFileServer::incomingConnection(qintptr socketDescriptor)
{
    HttpFileClient *client = new HttpFileClient(this);
    client->setFileStack(sharedFiles);
    client->setSocket(socketDescriptor);
}

int HttpFileServer::serveFile(const QUrl & path)
{
    sharedFiles.insert(sharedFiles.count(), path);
    return sharedFiles.count() - 1;
}

QString HttpFileServer::getFilenameFromID(const int id)
{
    return QUrl(sharedFiles[id]).fileName();
}
