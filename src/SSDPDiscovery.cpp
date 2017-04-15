#include "SSDPDiscovery.h"

// DLNA renderers discovery class
SSDPdiscovery::SSDPdiscovery(QObject *parent) : QObject(parent)
{
    // Create network manager, for quering info of renderers, and connect signal
    nmgr = new QNetworkAccessManager(this);
    connect(nmgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(processData(QNetworkReply*)));
}

// Starts SSDP discovery. Dont call it, when you have IP adress specified, just procced to ->findRendererFromUrl
void SSDPdiscovery::run()
{
    // DLNA discovery adress
    QHostAddress groupAddress = QHostAddress("239.255.255.250");

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::AnyIPv4, 1901, QUdpSocket::ShareAddress);
    udpSocket->joinMulticastGroup(groupAddress);

    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));

    QByteArray datagram;
    // Multicast message, used for discovery of DLNA devices
    datagram.append( "M-SEARCH * HTTP/1.1\r\n" );
    datagram.append( "HOST: 239.255.255.250:1900\r\n" );
    datagram.append( "MX: 5\nMan: \"ssdp:discover\"\r\n" );
    datagram.append( "ST: urn:schemas-upnp-org:device:MediaRenderer:1\r\n\r\n" );

    udpSocket->writeDatagram(datagram.data(), datagram.size(), groupAddress, 1900);
}

// Process received datagrams, and get IP of DLNA DMR
void SSDPdiscovery::processPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QStringList list = QString(datagram.data()).split("\r\n");

        for(auto i : list)
        {
            // Get url of ip of renderer, send request to get more data
            if(i.toLower().startsWith("location:"))
            {
                findRendererFromUrl(QUrl(i.mid(10).simplified()));
                break;
            }
        }
    }
}

void SSDPdiscovery::findRendererFromUrl(const QUrl & url)
{
    // Query renderer info
    nmgr->get(QNetworkRequest(url));
}

// This function process request data, into DLNA Renderer
void SSDPdiscovery::processData(QNetworkReply *reply)
{
    QXmlStreamReader xml(reply->readAll());

    if(!known_urls.contains(reply->url().toString()))
    {
        // Construct renderer object
        DLNARenderer *renderer = new DLNARenderer(reply->url(), this);

        reply->close();

        // Parse return url
        while(!xml.hasError() && !xml.atEnd())
        {
            xml.readNextStartElement();
            if(xml.name()=="serviceId" && xml.readElementText()=="urn:upnp-org:serviceId:AVTransport")
            {
                while(xml.name()!="controlURL" && !xml.atEnd())
                    xml.readNextStartElement();
                if(xml.name()=="controlURL")
                    renderer->setControlUrl(xml.readElementText());
            }
            else if(xml.name()=="friendlyName")
                renderer->setName(xml.readElementText());
            // Parse icon list
            else if(xml.name()=="icon" && !xml.isEndElement())
            {
                xml.readNextStartElement();
                DLNARendererIcon icon;
                while(xml.name()!="icon")
                {
                    if(xml.name()=="mimetype") icon.mimetype = xml.readElementText();
                    else if(xml.name()=="width") icon.width = xml.readElementText().toInt();
                    else if(xml.name()=="height") icon.height = xml.readElementText().toInt();
                    else if(xml.name()=="url") icon.url = xml.readElementText();
                    xml.readNextStartElement();
                }
                // Choose the largest icon
                if(renderer->icon.width < icon.width) renderer->icon = icon;
            }
        }

        known_urls.insert(renderer->getUrl().toString());
        known_renderers.append(renderer);
        emit foundRenderer(renderer);
    }
    else
        reply->close();
}
