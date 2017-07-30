#include "Kast.h"
#include "SSDPDiscovery.h"
#include "DLNAPlaybackInfo.h"
#include <QDebug>

Kast::Kast(QObject *parent) : QObject(parent)
{
    qDebug() << "Starting server...";

    fileServer = new HttpFileServer();
    fileServer->startServer();
    SSDPdiscovery * discovery = new SSDPdiscovery(this);
    connect(discovery, SIGNAL(foundRenderer(DLNARenderer*)), this, SLOT(foundRenderer(DLNARenderer*)));
    // Start SSDP discovery
    discovery->run();
}

void Kast::addItemToQueue(QString &item_url)
{
    queue.append(item_url);
}

void Kast::foundRenderer(DLNARenderer *renderer)
{
    qDebug() << "Renderer found: " + renderer->getName();
    connect(renderer, SIGNAL(receivedResponse(QString,QString)), this, SLOT(handleResponse(QString,QString)));
    // Stop playback. Responses will be handled in handleResponse
    renderer->stopPlayback();
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

void Kast::handleResponse(const QString responseType, const QString data)
{
    Q_UNUSED(data); //May be needed in the future
    // Get renderer object
    DLNARenderer *renderer = qobject_cast<DLNARenderer *>(sender());
    qDebug() << "^Detected response type: "+responseType;

    // Handle responses
    if(responseType == "StopResponse")
    {
        // Host file, and send its url to renderer
        int id = fileServer->serveFile(QUrl(queue[0])); // File to serve

        QString fileName = fileServer->getFilenameFromID(id),
                local_address = getLocalAddress().toString(),
                port_number = QString::number(port);

        renderer->setPlaybackUrl(QUrl(QString("http://%1:%2/%3/%4").arg(local_address, port_number,QString::number(id), fileName)),
                                 QFileInfo(queue[0]));

    }
    else if(responseType == "SetAVTransportURIResponse")
        renderer->playPlayback(); // Just play the playback url
}


