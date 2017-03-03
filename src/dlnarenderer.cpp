#include "dlnarenderer.h"

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

void DLNARenderer::setPlaybackUrl(const QUrl & url)
{
    sam->doAction(
                "SetAVTransportURI", // Action
                "<CurrentURI>"+url.toString()+"</CurrentURI><CurrentURIMetaData></CurrentURIMetaData>",  // Action Data
                fullcontrolUrl); // Control url
}

void DLNARenderer::playPlayback()
{
    sam->doAction("Play", "<Speed>1</Speed>", fullcontrolUrl);
}

void DLNARenderer::pausePlayback()
{
    sam->doAction("Pause", "", fullcontrolUrl);
}

void DLNARenderer::seekPlayback(QTime time)
{
    sam->doAction("Seek", "<InstanceID>0</InstanceID><Unit>REL_TIME</Unit><Target>"+time.toString()+"</Target>", fullcontrolUrl);
}
