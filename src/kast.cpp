#include "kast.h"
#include "ssdpdiscovery.h"
#include "dlnarenderer.h"
#include "httpfileserver.h"
#include <QDebug>

Kast::Kast(QStringList & files, QObject *parent) : QObject(parent), filesList(files)
{
    qDebug() << "Starting server...";

    fileServer = new HttpFileServer(port, QHostAddress::Any, this);
    SSDPdiscovery *test = new SSDPdiscovery(this);
    connect(test, SIGNAL(foundRenderer(DLNARenderer*)), this, SLOT(foundRenderer(DLNARenderer*)));
}
void Kast::foundRenderer(DLNARenderer *renderer)
{
    qDebug() << "Renderer found: " + renderer->getName();
    // Get local address
    int id = fileServer->serveFile(QUrl(filesList[0])); // File to serve
    QString fileName = fileServer->getFilenameFromID(id),
            local_address = getLocalAddress().toString(),
            port_number = QString::number(port);
    // Set playback url, and play it
    renderer->setPlaybackUrl(QUrl(QString("http://%1:%2/%3/%4").arg(local_address, port_number, QString::number(id), fileName)));
    renderer->playPlayback();
}
QHostAddress Kast::getLocalAddress()
{
    // see http://stackoverflow.com/questions/13835989/get-local-ip-address-in-qt
    for(auto && address : QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol
                && address != QHostAddress(QHostAddress::LocalHost) // Check if it is local adress
                && address.toString().section( ".",-1,-1 ) != "1") // Check if it is virtual machine
             return address;
    }
    return QHostAddress();
}


