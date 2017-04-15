#ifndef MIMEGUESSER_H
#define MIMEGUESSER_H

#include <QObject>
#include <QMimeDatabase>
#include <QFileInfo>

class MimeGuesser : public QObject
{
    Q_OBJECT
public:
    explicit MimeGuesser(QObject *parent = 0);
    QString fileMimeType(QFileInfo fileInfo);
    Q_INVOKABLE QString getMediaType(const QString & filePath);
private:
    QMimeDatabase mimedb;
};

#endif // MIMEGUESSER_H
