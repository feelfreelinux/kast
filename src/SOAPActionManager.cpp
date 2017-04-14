#include "SOAPActionManager.h"
#include "DLNAPlaybackInfo.h"
#include <QDebug>

/**
 * This class handles network requests from DLNARenderer.
 */

SOAPActionManager::SOAPActionManager(QObject *parent) : QObject(parent)
{
    mgr = new QNetworkAccessManager(this);
}

void SOAPActionManager::doAction(const QString &action, const QMap<QString, QString> &dataMap, const QUrl &controlUrl)
{
    QNetworkRequest request;

    // Build xml data string
    QString actionData = "";
    if (!dataMap.isEmpty() || !dataMap.isDetached()) {
        for (const QString &key : dataMap.keys()) {
            actionData.append("<" + key + ">");
            actionData.append(dataMap.value(key));
            actionData.append("</" + key + ">");
        }
    }

    // Build xml request body
    QString data = SOAPXmlHeader + action + SOAPXmlInstanceId + actionData + SOAPXmlActions + action + SOAPXmlFooter;
    QByteArray actionHeader = QString("\"urn:schemas-upnp-org:service:AVTransport:1#" + action + "\"").toUtf8();
    // Set needed headers
    request.setUrl(controlUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=utf-8");
    request.setRawHeader("SOAPaction", actionHeader);
    // To do not duplicate code, just check, is this action with needed data processing, or not.
    // If you want to add new action, which needs data processing, do it here.

    if (action == "GetPositionInfo")
        connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(processPlaybackInfo(QNetworkReply*)));

    // Only used for debbuging
    else connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(processData(QNetworkReply*)));

    mgr->post(request, data.toUtf8());
}

void SOAPActionManager::processData(QNetworkReply* reply)
{
    // Write debug log
    qDebug() << reply->readAll().data();
    reply->close();

    // We want to be able to connect it to few slots, so lets disconnect it for now
    disconnect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(processData(QNetworkReply*)));
}

void SOAPActionManager::processPlaybackInfo(QNetworkReply *reply)
{
    // Construct xml parser, from reply's data, close socket
    QXmlStreamReader xml(reply->readAll());
    reply->close();

    DLNAPlaybackInfo playbackInfo;
    // Parse return url
    while(!xml.hasError() && !xml.atEnd())
    {
        xml.readNextStartElement();
        if(xml.name() == "RelTime")
            playbackInfo.relTime = QTime::fromString(xml.readElementText(), "hh:mm:ss");
        else if(xml.name() == "TrackDuration")
            playbackInfo.trackDuration = QTime::fromString(xml.readElementText(), "hh:mm:ss");
    }

    emit receivePlaybackInfo(&playbackInfo);
    // We want to be able to connect it to few slots, so lets disconnect it for now
    disconnect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(processPlaybackInfo(QNetworkReply*)));
}

// Generates DIDL-Lite metadata
QString SOAPActionManager::generateMetadata(const QFileInfo &fileInfo, const QString &address)
{
    // Get mime-type of file
    QMimeDatabase db;
    QString mimetype = db.mimeTypeForFile(fileInfo).name();

    // Construct DIDL-Lite
    return DIDLLiteString.arg(fileInfo.fileName(), fileInfo.owner(), mimetype.split("/").first(), mimetype, address);
}
