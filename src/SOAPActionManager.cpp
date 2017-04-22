#include "SOAPActionManager.h"
#include "DLNAPlaybackInfo.h"
#include "MimeGuesser.h"
#include <QDebug>


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

    else connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(processData(QNetworkReply*)));

    mgr->post(request, data.toUtf8());
}

void SOAPActionManager::processData(QNetworkReply* reply)
{
    QString data = reply->readAll();
    reply->close();

    // We want to be able to connect it to few slots, so lets disconnect it for now
    disconnect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(processData(QNetworkReply*)));

    qDebug() << "SOAPActionManager: Got xml response " + data;
    // Initial value, used if response type is not detected
    QString responseType = "UNDEFINED";

    // @TODO Add nice error handling here. It should be simple to implement, and could be also handled in receivedResponse signal.
    QXmlStreamReader xml(data);
    while(!xml.hasError() && !xml.atEnd() && !xml.name().contains("Response"))
        xml.readNextStartElement();
    if(xml.name().contains("Response"))
        responseType = xml.name().toString();
    // Emit signal with response's type, and raw data
    emit receivedResponse(responseType, data);

}

void SOAPActionManager::processPlaybackInfo(QNetworkReply *reply)
{
    // Construct xml parser, from reply's data, close socket
    QXmlStreamReader xml(reply->readAll());
    reply->close();

    // We want to be able to connect it to few slots, so lets disconnect it for now
    disconnect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(processPlaybackInfo(QNetworkReply*)));

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
}

// Generates DIDL-Lite metadata
QString SOAPActionManager::generateMetadata(const QFileInfo &fileInfo, const QString &address)
{
    MimeGuesser mg;
    // Construct DIDL-Lite
    return DIDLLiteString.arg(fileInfo.fileName(), fileInfo.owner(), mg.getMediaType(fileInfo.filePath()),
                              mg.fileMimeType(fileInfo), address);
}
