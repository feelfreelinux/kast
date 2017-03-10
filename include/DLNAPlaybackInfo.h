#ifndef DLNAPLAYBACKINFO_H
#define DLNAPLAYBACKINFO_H
#include <QTime>

class DLNAPlaybackInfo
{
public:
    DLNAPlaybackInfo();
    void setRelTime(QTime time);
    void setTrackDuration(QTime time);
    QTime getRelTime();
    QTime getTrackDuration();
private:
    QTime relTime, trackDuration;
};

#endif // DLNAPLAYBACKINFO_H
