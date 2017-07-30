#ifndef HTTPFILECLIENT_H
#define HTTPFILECLIENT_H

#include <QtNetwork>
#include <QString>

class HttpFileClient : public QObject
{
    Q_OBJECT
public:
    explicit HttpFileClient(QObject *parent = 0);
    void setSocket(qintptr descriptor);
    void setFileStack(QMap<int, QUrl>);
private:
    QMap<int, QUrl> sharedFiles;
    QString writtenAction;
    QByteArray block;
    QFile *file;
public slots:
    void connected();
    void disconnected();
    void readyRead();
    void bytesWritten(qint64 bytes);
signals:

private:
    QTcpSocket *socket;
};

#endif // HTTPFILECLIENT_H
