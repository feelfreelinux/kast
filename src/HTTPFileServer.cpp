#include "HTTPFileServer.h"

#include <QFile>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QUrl>
#include <QDebug>

HttpFileServer::HttpFileServer(int port, QHostAddress address, QObject *parent) : QObject(parent)
{
    // Construct QTcpServer, connect slot
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(handleIncoming()));
    server->listen(address, port);
    qDebug() << "FILE SERVER: started";
}

// Handles incoming connection
void HttpFileServer::handleIncoming()
{
    QMimeDatabase db;
    QFileInfo fileinfo;

    QTcpSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));
    clientConnection->waitForReadyRead();

    QUrl filePath;
    QMap<QString, QString> requestMap;

    // Read client's request
    while(!clientConnection->atEnd())
    {
            QString line = clientConnection->readLine();
            
            // Get filename from request
            if(line.startsWith("GET /"))
            {
                line.replace("GET /", ""); // HTTP header left part
                line.chop(11); // HTTP header right part
                
                int id = line.left(line.lastIndexOf("/")).toInt();
                QString fileName = line.right(line.length() - line.lastIndexOf("/") - 1);
                filePath = fileMap[id];
                
                // Ensures the file is valid
                fileinfo.setFile(filePath.toString());
                if(fileinfo.suffix() != line.split(".").last() ||
                        fileinfo.fileName() != fileName ||
                        !fileinfo.exists() ||
                        !fileinfo.isFile() ||
                        !fileinfo.isReadable())
                {
                    // If file is not valid, write 404 and return
                    qDebug() << "File not valid: " + filePath.toString();
                    clientConnection->write("HTTP/1.0 404 NOT FOUND\r\n");
                    clientConnection->flush();
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
    if(!filePath.isEmpty())
    {
            QFile file(filePath.toString());
            file.open(QFile::ReadOnly);
            QString header, filesize = QString::number(file.size());

            // Detect, does Range header exist in request
            if(requestMap.contains("Range"))
            {
                // Parse request, get seek value
                QString range = requestMap["Range"];
                range = range.mid(6, range.length()); // 'bytes=' is 6 chars
                qint64 seek = range.left(range.indexOf("-")).toInt();
                if(range.endsWith("-"))
                    range.append(QString::number(file.size() - 1));
                // Construct 206 header, with content lenght
                header = QString("HTTP/1.0 206 PARTIAL CONTENT\r\n"
                         "Content-Length: %1\r\n"
                         "Content-Range: bytes %2/%1\r\n"
                         "Content-Type: %3\r\n\r\n").arg(filesize, range,
                                                         db.mimeTypeForFile(fileinfo).name());
                // Finally, seek the file
                file.seek(seek);
            }
            else
                // If Range header is not detected, construct 200 OK header
                header = QString("HTTP/1.0 200 OK\r\n"
                         "Content-Length: %1\r\n"
                         "Content-Type: %2\r\n\r\n").arg(filesize,
                                                         db.mimeTypeForFile(fileinfo).name());
            // Write header to client
            clientConnection->write(header.toUtf8());
            clientConnection->waitForBytesWritten();
            QByteArray block; block.resize(65536);

            while(!file.atEnd())
            {
                // Send part of file to client
                qint64 read = file.read(block.data(), 65536);
                clientConnection->write(block, read);
            }

            file.close();
        }
        else
            // Send 404 if path is not valid
            clientConnection->write("HTTP/1.0 404 NOT FOUND\r\n");
        clientConnection->flush();
        clientConnection->disconnectFromHost();
}

int HttpFileServer::serveFile(const QUrl & path)
{
    fileMap.insert(fileMap.count(), path);
    return fileMap.count() - 1;
}

QString HttpFileServer::getFilenameFromID(int id)
{
    return QUrl(fileMap[id]).fileName();
}
