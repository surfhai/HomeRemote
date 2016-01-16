#include "kodipvrchannelmodel.h"
#include "kodipvrchannel.h"

#include <QAbstractListModel>
#include <QDebug>

KodiPVRChannelModel::KodiPVRChannelModel(QObject *parent)
    : QAbstractListModel(parent)
{
    updateRoles.clear();
    minIndex = QModelIndex();
    maxIndex = QModelIndex();
    qDebug() << "QModelIndex().isValid" << minIndex.isValid();
}

void KodiPVRChannelModel::addChannel(const KodiPVRChannel &kodiPVRChannel)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_kodiPVRChannels << kodiPVRChannel;
    endInsertRows();
}

int KodiPVRChannelModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_kodiPVRChannels.count();
}

QVariant KodiPVRChannelModel::data(const QModelIndex & index, int role) const
{
    if (index.row() < 0 || index.row() >= m_kodiPVRChannels.count())
        return QVariant();

    const KodiPVRChannel &kodiPVRChannel = m_kodiPVRChannels[index.row()];
    if (role == ChannelIDRole)
        return kodiPVRChannel.channelid();
    else if (role == ChannelRole)
        return kodiPVRChannel.channel();
    else if (role == LabelRole)
        return kodiPVRChannel.label();
    else if (role == TitleRole)
        return kodiPVRChannel.title();
    else if (role == EpisodeNameRole)
        return kodiPVRChannel.episodename();
    else if (role == TitleNextRole)
        return kodiPVRChannel.titleNext();
    else if (role == EpisodeNameNextRole)
        return kodiPVRChannel.episodenameNext();
    else if (role == ProgressPercentageRole)
        return kodiPVRChannel.progresspercentage();
    else if (role == StartTimeRole)
        return kodiPVRChannel.starttime();
    else if (role == EndTimeRole)
        return kodiPVRChannel.endtime();
    else if (role == LastPlayedRole)
        return kodiPVRChannel.lastplayed();
    else if (role == ThumbnailRole)
        return kodiPVRChannel.thumbnail();
    return QVariant();
}

QHash<int, QByteArray> KodiPVRChannelModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ChannelIDRole] = "channelID";
    roles[ChannelRole] = "channel";
    roles[LabelRole] = "label";
    roles[TitleRole] = "title";
    roles[EpisodeNameRole] = "episodeName";
    roles[TitleNextRole] = "titleNext";
    roles[EpisodeNameNextRole] = "episodeNameNext";
    roles[ProgressPercentageRole] = "progressPercentage";
    roles[StartTimeRole] = "startTime";
    roles[EndTimeRole] = "endTime";
    roles[LastPlayedRole] = "lastPlayed";
    roles[ThumbnailRole] = "thumbnail";
    return roles;
}

bool KodiPVRChannelModel::setData(const QModelIndex &index, const KodiPVRChannel &kodiPVRChannel, const bool update)
{
    if (index.isValid())
    {
        if (m_kodiPVRChannels[index.row()].title() != kodiPVRChannel.title())
        {
            m_kodiPVRChannels[index.row()].setTitle(kodiPVRChannel.title());
            if (!updateRoles.contains(TitleRole))
                updateRoles.append(TitleRole);
        }
        if (m_kodiPVRChannels[index.row()].episodename() != kodiPVRChannel.episodename())
        {
            m_kodiPVRChannels[index.row()].setEpisodename(kodiPVRChannel.episodename());
            if (!updateRoles.contains(EpisodeNameRole))
                updateRoles.append(EpisodeNameRole);
        }
        if (m_kodiPVRChannels[index.row()].titleNext() != kodiPVRChannel.titleNext())
        {
            m_kodiPVRChannels[index.row()].setTitleNext(kodiPVRChannel.titleNext());
            if (!updateRoles.contains(TitleNextRole))
                updateRoles.append(TitleNextRole);
        }
        if (m_kodiPVRChannels[index.row()].episodenameNext() != kodiPVRChannel.episodenameNext())
        {
            m_kodiPVRChannels[index.row()].setEpisodenameNext(kodiPVRChannel.episodenameNext());
            if (!updateRoles.contains(EpisodeNameNextRole))
                updateRoles.append(EpisodeNameNextRole);
        }
        if (m_kodiPVRChannels[index.row()].progresspercentage() != kodiPVRChannel.progresspercentage())
        {
            m_kodiPVRChannels[index.row()].setProgresspercentage(kodiPVRChannel.progresspercentage());
            if (!updateRoles.contains(ProgressPercentageRole))
                updateRoles.append(ProgressPercentageRole);
        }
        if (m_kodiPVRChannels[index.row()].starttime() != kodiPVRChannel.starttime())
        {
            m_kodiPVRChannels[index.row()].setStarttime(kodiPVRChannel.starttime());
            if (!updateRoles.contains(StartTimeRole))
                updateRoles.append(StartTimeRole);
        }
        if (m_kodiPVRChannels[index.row()].endtime() != kodiPVRChannel.endtime())
        {
            m_kodiPVRChannels[index.row()].setEndtime(kodiPVRChannel.endtime());
            if (!updateRoles.contains(EndTimeRole))
                updateRoles.append(EndTimeRole);
        }
        if (m_kodiPVRChannels[index.row()].lastplayed() != kodiPVRChannel.lastplayed())
        {
            m_kodiPVRChannels[index.row()].setLastplayed(kodiPVRChannel.lastplayed());
            if (!updateRoles.contains(LastPlayedRole))
                updateRoles.append(LastPlayedRole);
        }

        if (index.row() < minIndex.row() || !minIndex.isValid())
            minIndex = index;
        if (index.row() > maxIndex.row() || !maxIndex.isValid())
            maxIndex = index;

        if (update)
        {
            emit dataChanged(minIndex, maxIndex);
            updateRoles.clear();
            minIndex = QModelIndex();
            maxIndex = QModelIndex();
        }
        return true;
    }
    return false;
}
