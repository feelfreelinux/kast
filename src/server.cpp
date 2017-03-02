#include "server.h"
#include "ssdpdiscovery.h"
#include "dlnarenderer.h"
#include "httpfileserver.h"

#include <QDebug>

Server::Server(QStringList & files, QObject *parent) : QObject(parent), filesList(files)
{
    qDebug() << "Starting server...";

    fileServer = new HttpFileServer(port, QHostAddress::Any, this);
    SSDPdiscovery *test = new SSDPdiscovery(this);
    connect(test, SIGNAL(foundRenderer(DLNARenderer*)), this, SLOT(foundRenderer(DLNARenderer*)));
}
void Server::foundRenderer(DLNARenderer *server)
{
    // Get local address
    int id = fileServer->serveFile(QUrl(filesList[0])); // File to serve
    QString fileName = fileServer->getFilenameFromID(id),
            local_address = getLocalAddress().toString(),
            port_number = QString::number(port);

    server->setPlaybackUrl(QUrl(QString("http://%1:%2/%3/%4").arg(local_address, port_number, QString::number(id), fileName)));
    server->playPlayback();
}
QHostAddress Server::getLocalAddress()
{
    // see http://stackoverflow.com/questions/13835989/get-local-ip-address-in-qt
    //TODO: remove foreach
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol
                && address != QHostAddress(QHostAddress::LocalHost) // Check if it is local adress
                && address.toString().section( ".",-1,-1 ) != "1") // Check if it is virtual machine
             return address;
    }
    return QHostAddress();
}


