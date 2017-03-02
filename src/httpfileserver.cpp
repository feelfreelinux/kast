#include "httpfileserver.h"

#include <QTcpServer>
#include <QMimeDatabase>
#include <QFile>
#include <QFileInfo>
#include <QTcpSocket>
#include <QUrl>

QMap<int, QUrl> fileMap;

HttpFileServer::HttpFileServer(int port, QHostAddress address, QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(handleIncoming()));
    server->listen(address, port);
}

void HttpFileServer::handleIncoming()
{
    QMimeDatabase db;
    QFileInfo fileinfo;
    bool error = false;

    QTcpSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));
    clientConnection->waitForReadyRead();
    QUrl filePath;
    QMap<QString, QString> requestMap;
    while (!clientConnection->atEnd())
    {
            QString line(clientConnection->readLine());
            
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
                    error = true;
                    continue;
                }
            }

            if(line.indexOf(":") <= 0 || line.isEmpty())
                continue;
            line.replace("\r\n", "");
            QString key(line.left(line.indexOf(":"))),
                    value(line.mid(line.indexOf(":") + 2, line.length()));
                    
            requestMap.insert(key, value);
    }
    // If not error occured, send and process file
    if(!error && !filePath.isEmpty())
    {
            QFile *file = new QFile(filePath.toString());
            file->open(QFile::ReadOnly);
            QByteArray block;
            QString header, filesize(QString::number(file->size()));

            if(requestMap.contains("Range"))
            {
                QString range = requestMap["Range"];
                range = range.mid(6, range.length()); // 'bytes=' is 6 chars
                qint64 seek = range.left(range.indexOf("-")).toInt();
                if (range.endsWith("-"))
                    range.append(QString::number(file->size() - 1));
                header = "HTTP/1.0 206 PARTIAL CONTENT\r\n"
                         "Content-Length: "+filesize+"\r\n"
                         "Content-Range: bytes "+range+"/"+filesize + "\r\n"
                         "Content-Type: "+db.mimeTypeForFile(fileinfo).name()+"\r\n\r\n";
                file->seek(seek);
            }
            else
                header = "HTTP/1.0 200 OK\r\n"
                            "Content-Length: "+filesize+"\r\n"
                            "Content-Type: "+db.mimeTypeForFile(fileinfo).name()+"\r\n\r\n";

            block.append(header);
            clientConnection->write(block);
            block.clear();
            clientConnection->waitForBytesWritten();
            block.resize(65536);

            while(!file->atEnd())
            {
                qint64 read = file->read(block.data(), 65536);
                clientConnection->write(block, read);

            }

            file->close();
        }
        else
        {
            // Send 404
            QByteArray response;
            response.append("HTTP/1.0 404 NOT FOUND\r\n");
            clientConnection->write(response);
        }
        clientConnection->flush();
        clientConnection->disconnectFromHost();
}

int HttpFileServer::serveFile(QUrl path)
{
    fileMap.insert(fileMap.count(), path);
    return fileMap.count() - 1;
}

QString HttpFileServer::getFilenameFromID(int id)
{
    return QUrl(fileMap[id]).fileName();
}
