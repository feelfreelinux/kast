#include "DLNARenderer.h"
#include "DLNAPlaybackInfo.h"

#include <QDebug>

DLNARenderer::DLNARenderer(QUrl url, QObject *parent) : QObject(parent), sam(new SOAPActionManager()), serverUrl(url) {
    // Pass SOAPActionManager's signals to parent class
    connect(sam, SIGNAL(receivePlaybackInfo(DLNAPlaybackInfo*)), this, SIGNAL(receivePlaybackInfo(DLNAPlaybackInfo*)));
    connect(sam, SIGNAL(receivedResponse(const QString,const QString)), this, SIGNAL(receivedResponse(const QString, const QString)));
}

QString DLNARenderer::getName() { return serverName; }
QUrl DLNARenderer::getUrl() { return serverUrl; }

void DLNARenderer::setName(const QString & name) { serverName = name; }

void DLNARenderer::setControlUrl(const QString & url)
{
    fullcontrolUrl = serverUrl;
    fullcontrolUrl.setPath(url);
}

void DLNARenderer::setPlaybackUrl(const QUrl & url, const QFileInfo &fileInfo)
{
    QMap<QString, QString> dataMap;
    // Add URI to request, URL-encode it
    QString urlString = url.toString().replace(url.fileName(), QString(QUrl::toPercentEncoding(url.fileName())));
    dataMap.insert("CurrentURI", urlString);
    dataMap.insert("CurrentURIMetaData",sam->generateMetadata(fileInfo, urlString));
    sam->doAction(
                "SetAVTransportURI", // Action
                dataMap,  // Action Data
                fullcontrolUrl); // Control url
}

void DLNARenderer::setNextPlaybackUrl(const QUrl & url)
{
    QMap<QString, QString> dataMap;
    // Add URI to request, URL-encode it
    QString urlString = url.toString().replace(url.fileName(), QString(QUrl::toPercentEncoding(url.fileName())));
    dataMap.insert("NextURI", urlString);
    dataMap.insert("NextURIMetaData", "");
    sam->doAction("SetNextAVTransportURI", dataMap, fullcontrolUrl);
}

void DLNARenderer::queryPlaybackInfo()
{
    sam->doAction("GetPositionInfo", QMap<QString, QString>(), fullcontrolUrl);
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

void DLNARenderer::stopPlayback()
{
    sam->doAction("Stop", QMap<QString, QString>(), fullcontrolUrl);
}

void DLNARenderer::seekPlayback(QTime time)
{
    QMap<QString, QString> dataMap;
    dataMap.insert("InstanceID", "0");
    dataMap.insert("Unit", "REL_TIME");
    dataMap.insert("Target", time.toString());
    sam->doAction("Seek", dataMap, fullcontrolUrl);
}

void DLNARenderer::previousItem()
{
    sam->doAction("Previous", QMap<QString, QString>(), fullcontrolUrl);
}

void DLNARenderer::nextItem()
{
    sam->doAction("Next", QMap<QString, QString>(), fullcontrolUrl);
}
