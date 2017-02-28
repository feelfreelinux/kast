#include <QDebug>
#include "server.h"
#include <QtNetwork>
#include "ssdpdiscovery.h"
#include "dlnarenderer.h"
#include "httpfileserver.h"

int port = 1337;
QStringList filesList;

Server::Server(QCoreApplication &core, QObject *parent) : QObject(parent)
{
    qDebug() << "Starting server...";
    fileServer = new HttpFileServer(port, QHostAddress::Any, this);
    SSDPdiscovery *test = new SSDPdiscovery(this);
    connect(test, SIGNAL(foundRenderer(DLNARenderer*)), this, SLOT(foundRenderer(DLNARenderer*)));
    filesList = core.arguments();
    filesList.removeFirst();
}
void Server::foundRenderer(DLNARenderer *server){
    // Get local adress
    foreach(const QString &file, filesList) {
        QString id = QString::number(fileServer->serveFile(QUrl(file))); // File to serve
        server->setPlaybackUrl( QUrl("http://"+getLocalAddress().toString()+":"+QString::number(port)+"/"+id+".mp4") ); // Eh, need to be made better
        server->playPlayback();
    }
}
QHostAddress Server::getLocalAddress() {
    // @see http://stackoverflow.com/questions/13835989/get-local-ip-address-in-qt
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol
                && address != QHostAddress(QHostAddress::LocalHost) // Check is it not local adress
                && address.toString().section( ".",-1,-1 ) != "1") // Check, is it not virtual machine @see
             return address;
    }
}


