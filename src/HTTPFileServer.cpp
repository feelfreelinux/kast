#include "HTTPFileServer.h"
#include "MimeGuesser.h"

#include <QTcpServer>
#include <QFile>
#include <QFileInfo>
#include <QTcpSocket>
#include <QUrl>
#include <QDebug>

HttpFileServer::HttpFileServer(QHostAddress address, int port, QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(handleIncoming()));
    server->listen(address, port);
    qDebug() << "FILE SERVER: started";
}

// Handles incoming connection
void HttpFileServer::handleIncoming()
{
    // Connect needed signals
    QTcpSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));
    connect(clientConnection, SIGNAL(readyRead()),
            this, SLOT(readSocketData()));

}

void HttpFileServer::readSocketData()
{
    MimeGuesser mg;
    QFileInfo fileinfo;
    QUrl filePath;
    QMap<QString, QString> requestMap;
    QString requestType; // Request handled for now - GET and HEAD

    // Retreive socket
    QTcpSocket *clientConnection = qobject_cast<QTcpSocket *>(sender());

    // Read client's request
    while(!clientConnection->atEnd())
    {
        QString line = clientConnection->readLine();

        // Get type of request
        if(line.startsWith("GET /")) requestType = "GET";
        else if (line.startsWith("HEAD /")) requestType = "HEAD";

        // Get filename from request
        if(!requestType.isEmpty() && line.contains(requestType))
        {
            line.replace(requestType+" /", ""); // HTTP header left part
            line.chop(11); // HTTP header right part

            int id = line.left(line.lastIndexOf("/")).toInt();
            // Get filename from request, url-decode it
            QString fileName(QUrl::fromPercentEncoding(line.right(line.length() - line.lastIndexOf("/") - 1).toUtf8()));
            filePath = sharedFiles[id];
            // Ensures the file is valid
            qDebug() << "File accessed: " + filePath.toString();
            fileinfo.setFile(filePath.toString());
            if(fileinfo.suffix() != line.split(".").last() ||
                    fileinfo.fileName() != fileName ||
                    !fileinfo.exists() ||
                    !fileinfo.isFile() ||
                    !fileinfo.isReadable())
            {
                // If file is not valid, write 404 and return
                qDebug() << "File not valid: " + fileName;
                clientConnection->write("HTTP/1.0 404 NOT FOUND\r\n");
                clientConnection->disconnectFromHost();
                return;
            }
        }
        // Parses headers
        if(line.indexOf(":") <= 0 || line.isEmpty())
            continue;
        line.replace("\r\n", "");
        QString key(line.left(line.indexOf(":"))),
                value(line.mid(line.indexOf(":") + 2, line.length()));

        requestMap.insert(key, value);

    }
    // If no error occured, send and process file
    if(!filePath.isEmpty() && !requestType.isEmpty())
    {
        QFile file(filePath.toString());
        file.open(QFile::ReadOnly);
        QString header, filesize = QString::number(file.size());

        // Detect, does Range header exist in request
        if(requestMap.contains("Range") && requestType=="GET")
        {
            // Parse request, get seek value
            QString range = requestMap["Range"];
            range = range.mid(6, range.length()); // 'bytes=' is 6 chars
            qint64 seek = range.left(range.indexOf("-")).toInt();
            // Check, is requested filesize acceptable
            if(seek<=filesize.toInt())
            {
                if(range.endsWith("-"))
                    range.append(QString::number(file.size() - 1));
                // Construct 206 header, with content lenght
                header = QString("HTTP/1.0 206 PARTIAL CONTENT\r\n"
                                 "Content-Length: %1\r\n"
                                 "Content-Range: bytes %2/%1\r\n"
                                 "Content-Type: %3\r\n\r\n").arg(filesize, range,
                                                                 mg.fileMimeType(fileinfo));
                // Finally, seek the file
                file.seek(seek);
            }
            else
            {
                // Handle 416 Range Not Satisfiable
                requestType = "FOOBAR"; // Break data sending
                // Construct 416
                header = QString("HTTP/1.0 416 Range Not Satisfiable\r\n"
                                 "Content-Length: %1\r\n"
                                 "Content-Type: %2\r\n\r\n").arg(filesize,
                                                                 mg.fileMimeType(fileinfo));
            }
        }
        else
            // If Range header is not detected, construct 200 OK header
            header = QString("HTTP/1.0 200 OK\r\n"
                             "Content-Length: %1\r\n"
                             "Content-Type: %2\r\n\r\n").arg(filesize,
                                                             mg.fileMimeType(fileinfo));
        // Write header to client
        clientConnection->write(header.toUtf8());
        // If type is GET, send data of file.
        if(requestType=="GET")
        {
            QByteArray block; block.resize(65536);

            while(!file.atEnd() && clientConnection->state() == QTcpSocket::ConnectedState)
            {
                // Send part of file to client
                qint64 read = file.read(block.data(), 65536);
                clientConnection->write(block, read);
            }

            file.close();
        }
    }
    else
        // Send 404 if path is not valid
        clientConnection->write("HTTP/1.0 404 NOT FOUND\r\n");
    clientConnection->disconnectFromHost();
}

int HttpFileServer::serveFile(const QUrl & path)
{
    sharedFiles.insert(sharedFiles.count(), path);
    return sharedFiles.count() - 1;
}

QString HttpFileServer::getFilenameFromID(const int id)
{
    return QUrl(sharedFiles[id]).fileName();
}
