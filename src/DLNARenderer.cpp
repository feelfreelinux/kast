#include "DLNARenderer.h"
#include "DLNAPlaybackInfo.h"

#include <QDebug>

DLNARenderer::DLNARenderer(QUrl url, QObject *parent) : QObject(parent), sam(new SOAPActionManager()), serverUrl(url) {
    connect(sam, SIGNAL(receivePlaybackInfo(DLNAPlaybackInfo*)), this, SIGNAL(receivePlaybackInfo(DLNAPlaybackInfo*)));
}

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
    QMap<QString, QString> dataMap;
    dataMap.insert("CurrentURI", "<![CDATA["+url.toString()+"]]>");
    dataMap.insert("CurrentURIMetaData", "");
    sam->doAction(
                "SetAVTransportURI", // Action
                dataMap,  // Action Data
                fullcontrolUrl); // Control url
}

void DLNARenderer::setNextPlaybackUrl(const QUrl & url)
{
    QMap<QString, QString> dataMap;
    dataMap.insert("NextURI", url.toString());
    dataMap.insert("NextURIMetaData", "");
    sam->doAction("SetNextAVTransportURI", dataMap, fullcontrolUrl);
}

void DLNARenderer::queryPlaybackInfo()
{
    sam->doAction("GetPositionInfo", QMap<QString, QString>(), fullcontrolUrl);
}

void DLNARenderer::queryPlaybackInfo()
{
    sam->doAction("GetPositionInfo", "", fullcontrolUrl);
}

void DLNARenderer::playPlayback()
{
    QMap<QString, QString> dataMap;
    dataMap.insert("Speed", "1");
    sam->doAction("Play", dataMap, fullcontrolUrl);
}

void DLNARenderer::pausePlayback()
{
    sam->doAction("Pause", QMap<QString, QString>(), fullcontrolUrl);
}

void DLNARenderer::previousPlayback()
{
    sam->doAction("Previous", QMap<QString, QString>(), fullcontrolUrl);
}

void DLNARenderer::nextPlayback()
{
    sam->doAction("Next", QMap<QString, QString>(), fullcontrolUrl);
}

void DLNARenderer::seekPlayback(QTime time)
{
    QMap<QString, QString> dataMap;
    dataMap.insert("InstanceID", "0");
    dataMap.insert("Unit", "REL_TIME");
    dataMap.insert("Target", time.toString());
    sam->doAction("Seek", dataMap, fullcontrolUrl);
}
