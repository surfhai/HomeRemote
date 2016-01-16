#ifndef KODIPVRCHANNEL_H
#define KODIPVRCHANNEL_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QImage>
#include <QDateTime>

class KodiPVRChannel
{
public:
    KodiPVRChannel(const QString &channel,
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
                   const QString &thumbnail);

    QString channel() const;
    int channelid() const;
    QString label() const;
    QString title() const;
    QString episodename() const;
    QString titleNext() const;
    QString episodenameNext() const;
    double progresspercentage() const;
    QDateTime starttime() const;
    QDateTime endtime() const;
    QDateTime lastplayed() const;
    QString thumbnail() const;
    int count() const;

    bool setChannel(QString channel);
    bool setChannelid(int channelid);
    bool setLabel(QString label);
    bool setTitle(QString title);
    bool setEpisodename(QString episodename);
    bool setTitleNext(QString titleNext);
    bool setEpisodenameNext(QString episodenameNext);
    bool setProgresspercentage(double progresspercentage);
    bool setStarttime(QDateTime starttime);
    bool setEndtime(QDateTime endtime);
    bool setLastplayed(QDateTime lastplayed);
    bool setThumbnail(QString thumbnail);

private:
    static int m_count;
    QString m_channel;
    int m_channelid;
    QString m_label;
    QString m_title;
    QString m_episodename;
    QString m_titleNext;
    QString m_episodenameNext;
    double m_progresspercentage;
    QDateTime m_starttime;
    QDateTime m_endtime;
    QDateTime m_lastplayed;
    QString m_thumbnail;
};

#endif // KODIPVRCHANNEL_H
