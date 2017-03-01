#ifndef HTTPFILESERVER_H
#define HTTPFILESERVER_H

#include <QtNetwork>
#include <QString>

class HttpFileServer : public QObject
{
    Q_OBJECT
public:
    HttpFileServer(int port, QHostAddress address, QObject *parent = 0);
    /**
     * @brief Adds file to server stack
     * @param Path of file
     * @return ID of added file
     */
    int serveFile(QUrl path);
    /**
     * @brief Returns the full filename of the given ID
     * @param id The ID of the file
     * @return Full filename of the file
     */
    QString getFilenameFromID(int id);
private:
    QTcpServer *server;
private slots:
    void handleIncoming();

};

#endif // HTTPFILESERVER_H
