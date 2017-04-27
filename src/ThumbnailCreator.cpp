#include "ThumbnailCreator.h"

ThumbnailCreator::ThumbnailCreator()
{
}

ThumbnailCreator::~ThumbnailCreator()
{
}

bool ThumbnailCreator::obtainThumbnail(const QString &path, int width, int height, QImage &output)
{
    MimeGuesser mimeGuesser;
    QString mimeType = mimeGuesser.getMediaType(path);
    if (mimeType == "video")
        return createVideoThumbnail(path, width, height, output);
    return false;
}

bool ThumbnailCreator::createVideoThumbnail(const QString &path, int width, int height, QImage &output)
{
    videoThumbnailer.setThumbnailSize(width);
    // 20% seek inside the video to generate the preview
    videoThumbnailer.setSeekPercentage(20);
    videoThumbnailer.setSmartFrameSelection(true);
    videoThumbnailer.setMaintainAspectRatio(true);
    videoThumbnailer.generateThumbnail(path, output);
    return !output.isNull();
}
