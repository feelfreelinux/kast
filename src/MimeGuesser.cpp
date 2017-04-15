#include "MimeGuesser.h"

MimeGuesser::MimeGuesser(QObject *parent) : QObject(parent)
{

}

QString MimeGuesser::fileMimeType(QFileInfo fileInfo)
{
    return mimedb.mimeTypeForFile(fileInfo).name();
}

QString MimeGuesser::getMediaType(const QString & filePath)
{
    return mimedb.mimeTypeForFile(filePath).name().section("/", 0, 0);
}
