#ifndef DLNARENDERER_H
#define DLNARENDERER_H

#include "soapactionmanager.h"

#include <QtNetwork>

class DLNARenderer : public QObject
{
    Q_OBJECT
public:
    explicit DLNARenderer(QUrl url, QObject *parent = 0);
    
    QUrl getUrl();
    QString getControlUrl();
    QString getName();
    
    void setName(const QString & name);
    void setControlUrl(const QString & name);
    void setPlaybackUrl(const QUrl & url);

    void playPlayback();
    void pausePlayback();
private:
    SOAPActionManager *sam;
    QUrl serverUrl, fullcontrolUrl;
    QString serverName, controlUrl;
};

#endif // DLNARENDERER_H
