#ifndef KAST_H
#define KAST_H

#include "DLNARenderer.h"
#include "HTTPFileServer.h"

#include <QtNetwork>

// HTTPServer's port
const int port = 5437;

class Kast : public QObject
{
    Q_OBJECT
public:
    Kast(QStringList & core, QObject *parent = 0);
    QHostAddress getLocalAddress();
private:
    HttpFileServer *fileServer;
    QStringList filesList;
private slots:
    void foundRenderer(DLNARenderer *renderer);
};
#endif // KAST_H
