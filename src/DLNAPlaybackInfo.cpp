#include "DLNAPlaybackInfo.h"
#include <QTime>
/*
 * Keeps playback info data from `SOAPActionManager::processPlaybackInfo`
 */
DLNAPlaybackInfo::DLNAPlaybackInfo() {}
void DLNAPlaybackInfo::setRelTime(QTime time)
{
    relTime = time;
}
void DLNAPlaybackInfo::setTrackDuration(QTime time)
{
    trackDuration = time;
}
QTime DLNAPlaybackInfo::getRelTime() {
    return relTime;
}
QTime DLNAPlaybackInfo::getTrackDuration() {
    return trackDuration;
}
