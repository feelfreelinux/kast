#include "dlnarenderer.h"
#include "soapactionmanager.h"

#include <QDebug>

DLNARenderer::DLNARenderer(QUrl url, QObject *parent) : QObject(parent), sam(new SOAPActionManager()), serverUrl(url) { }

QString DLNARenderer::getControlUrl() { return controlUrl; }

QString DLNARenderer::getName() { return serverName; }

QUrl DLNARenderer::getUrl() { return serverUrl; }

void DLNARenderer::setName(const QString & name) { serverName = name; }

void DLNARenderer::setControlUrl(const QString & url)
{
    controlUrl = url;
    fullcontrolUrl = serverUrl;
    fullcontrolUrl.setPath(url);
}

// TODO: Soap action's xml data should be generated somewhere in SOAPActionManager. Not sure how to handle it, so keeping it as is
// TODO: fix line length!
void DLNARenderer::setPlaybackUrl(QUrl url)
{
    QString requestData = "<s:Envelope s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body><u:SetAVTransportURI xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>0</InstanceID><CurrentURI><![CDATA["+url.toString()+"]]></CurrentURI><CurrentURIMetaData></CurrentURIMetaData></u:SetAVTransportURI></s:Body></s:Envelope>";
    sam->doAction("SetAVTransportURI", requestData, fullcontrolUrl);
}

void DLNARenderer::playPlayback()
{
    QString requestData = "<?xml version=\"1.0\" encoding=\"utf-8\"?><s:Envelope s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body><u:Play xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>0</InstanceID><Speed>1</Speed></u:Play></s:Body></s:Envelope>";
    sam->doAction("Play", requestData, fullcontrolUrl);
}

void DLNARenderer::pausePlayback()
{
    QString requestData = "<?xml version=\"1.0\" encoding=\"utf-8\"?><s:Envelope s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body><u:Pause xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>0</InstanceID></u:Pause></s:Body></s:Envelope>";
    sam->doAction("Pause", requestData, fullcontrolUrl);
}
