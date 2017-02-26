#include "dlnarenderer.h"
#include <QDebug>
DLNARenderer::DLNARenderer(QUrl url, QObject *parent) : QObject(parent){
    serverUrl = url;
    mgr = new QNetworkAccessManager(this);
}
QString DLNARenderer::getControlUrl() {
    return controlUrl;
}
QString DLNARenderer::getName() {
    return serverName;
}
QUrl DLNARenderer::getUrl() {
    return serverUrl;
}
void DLNARenderer::setName( QString name ) {
    serverName = name;
}
void DLNARenderer::setControlUrl( QString url ) {
    controlUrl = url;
    fullcontrolUrl = serverUrl; fullcontrolUrl.setPath( url );
}
void DLNARenderer::setPlaybackUrl( QUrl url ) {
    QNetworkRequest request;
    QByteArray data;
    data.append("<s:Envelope s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body><u:SetAVTransportURI xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>0</InstanceID><CurrentURI><![CDATA["+url.toString()+"]]></CurrentURI><CurrentURIMetaData></CurrentURIMetaData></u:SetAVTransportURI></s:Body></s:Envelope>");
    request.setUrl(fullcontrolUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=utf-8");
    request.setRawHeader("SOAPAction", "urn:schemas-upnp-org:service:AVTransport:1#SetAVTransportURI");
    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(test(QNetworkReply*)));
    mgr->post(request, data);
}
void DLNARenderer::playPlayback() {
    QNetworkRequest request;
    QByteArray data;
    data.append("<?xml version=\"1.0\" encoding=\"utf-8\"?><s:Envelope s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"><s:Body><u:Play xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>0</InstanceID><Speed>1</Speed></u:Play></s:Body></s:Envelope>");
    request.setUrl(fullcontrolUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=utf-8");
    request.setRawHeader("SOAPAction", "urn:schemas-upnp-org:service:AVTransport:1#Play");
    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(test(QNetworkReply*)));
    mgr->post(request, data);
}
void DLNARenderer::test(QNetworkReply* reply){
    qDebug()<<reply->readAll().data();
}
