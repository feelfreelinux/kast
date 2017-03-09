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

void SOAPActionManager::doAction(const QString & action, const QString & actionData, const QUrl & controlUrl)
{
    QNetworkRequest request;

    // Build xml request body
    QString data = SOAPXmlHeader + action + SOAPXmlInstanceId + actionData + SOAPXmlActions + action + SOAPXmlFooter;
    QByteArray actionHeader = QString("\"urn:schemas-upnp-org:service:AVTransport:1#" + action + "\"").toUtf8();

    // Set needed headers
    request.setUrl(controlUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=utf-8");
    request.setRawHeader("SOAPAction", actionHeader);

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
            playbackInfo.setRelTime(QTime::fromString(xml.readElementText(), "hh:mm:ss"));
        else if (xml.name() == "TrackDuration")
            playbackInfo.setTrackDuration(QTime::fromString(xml.readElementText(), "hh:mm:ss"));
    }

    emit receivePlaybackInfo(&playbackInfo);
    // We want to be able to connect it to few slots, so lets disconnect it for now
    disconnect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(processPlaybackInfo(QNetworkReply*)));
}
