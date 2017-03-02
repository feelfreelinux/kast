#ifndef DLNARENDERER_H
#define DLNARENDERER_H

#include "soapactionmanager.h"

#include <QtNetwork>
#include <QObject>

class DLNARenderer : public QObject
{
    Q_OBJECT
public:
    explicit DLNARenderer(QUrl url, QObject *parent = 0);
    
    QUrl getUrl();
    QString getControlUrl();
    QString getName();
    
    void setControlUrl(const QString & name);
    void setName(const QString & name);
    void setPlaybackUrl(QUrl url);
    void playPlayback();
    void pausePlayback();
    void seekPlayback(QTime time);
    void setUrl(QUrl url);
private:
    SOAPActionManager *sam;
    QUrl serverUrl, fullcontrolUrl;
    QString serverName, controlUrl;
};

#endif // DLNARENDERER_H
