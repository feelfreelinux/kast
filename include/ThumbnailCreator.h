#ifndef THUMBNAILCREATOR_H
#define THUMBNAILCREATOR_H

#include <QObject>
#include <QImage>

#include <Thumbnailer/videothumbnailer.h>
#include <MimeGuesser.h>

class ThumbnailCreator : public QObject
{
public:
    ThumbnailCreator();
    virtual ~ThumbnailCreator();
    virtual bool obtainThumbnail(const QString &path, int width, int height, QImage &output);
private:
    bool createVideoThumbnail(const QString &path, int width, int height, QImage &output);
    ffmpegthumbnailer::VideoThumbnailer videoThumbnailer;
};

#endif // THUMBNAILCREATOR_H
