#include "kodipvrchannel.h"

//KodiPVRChannel::KodiPVRChannel(QObject *parent) : QObject(parent)
//{
//
//}


#include <QObject>
#include <QString>
#include <QStringList>
#include <QImage>
#include <QDateTime>
#include <QDebug>

// Initialize static member of class
int KodiPVRChannel::m_count = 0;

KodiPVRChannel::KodiPVRChannel(const QString &channel,
                               const int &channelid,
                               const QString &label,
                               const QString &title,
                               const QString &episodename,
                               const QString &titleNext,
                               const QString &episodenameNext,
                               const double &progresspercentage,
                               const QDateTime &starttime,
                               const QDateTime &endtime,
                               const QDateTime &lastplayed,
                               const QString &thumbnail)
    : m_channel(channel),
      m_channelid(channelid),
      m_label(label),
      m_title(title),
      m_episodename(episodename),
      m_titleNext(titleNext),
      m_episodenameNext(episodenameNext),
      m_progresspercentage(progresspercentage),
      m_starttime(starttime),
      m_endtime(endtime),
      m_lastplayed(lastplayed),
      m_thumbnail(thumbnail)
{
    m_count++;
}

QString KodiPVRChannel::channel() const
{
    return m_channel;
}

int KodiPVRChannel::channelid() const
{
    return m_channelid;
}

QString KodiPVRChannel::label() const
{
    return m_label;
}

QString KodiPVRChannel::title() const
{
    return m_title;
}

QString KodiPVRChannel::episodename() const
{
    return m_episodename;
}

QString KodiPVRChannel::titleNext() const
{
    return m_titleNext;
}

QString KodiPVRChannel::episodenameNext() const
{
    return m_episodenameNext;
}

double KodiPVRChannel::progresspercentage() const
{
    return m_progresspercentage;
}

QDateTime KodiPVRChannel::starttime() const
{
    return m_starttime;
}

QDateTime KodiPVRChannel::endtime() const
{
    return m_endtime;
}

QDateTime KodiPVRChannel::lastplayed() const
{
    return m_lastplayed;
}

int KodiPVRChannel::count() const
{
    return m_count;
}

QString KodiPVRChannel::thumbnail() const
{
    return m_thumbnail;
}

bool KodiPVRChannel::setChannel(QString channel)
{
    m_channel = channel;
    return true;
}

bool KodiPVRChannel::setChannelid(int channelid)
{
    m_channelid = channelid;
    return true;
}

bool KodiPVRChannel::setLabel(QString label)
{
    m_label = label;
    return true;
}

bool KodiPVRChannel::setTitle(QString title)
{
    m_title = title;
    return true;
}

bool KodiPVRChannel::setEpisodename(QString episodename)
{
    m_episodename = episodename;
    return true;
}

bool KodiPVRChannel::setTitleNext(QString titleNext)
{
    m_titleNext = titleNext;
    return true;
}

bool KodiPVRChannel::setEpisodenameNext(QString episodenameNext)
{
    m_episodenameNext = episodenameNext;
    return true;
}

bool KodiPVRChannel::setProgresspercentage(double progresspercentage)
{
    m_progresspercentage = progresspercentage;
    return true;
}

bool KodiPVRChannel::setStarttime(QDateTime starttime)
{
    m_starttime = starttime;
    return true;
}

bool KodiPVRChannel::setEndtime(QDateTime endtime)
{
    m_endtime = endtime;
    return true;
}

bool KodiPVRChannel::setLastplayed(QDateTime lastplayed)
{
    m_lastplayed = lastplayed;
    return true;
}

bool KodiPVRChannel::setThumbnail(QString thumbnail)
{
    m_thumbnail = thumbnail;
    return true;
}
