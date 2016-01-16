#ifndef KODIPVRCHANNELMODEL_H
#define KODIPVRCHANNELMODEL_H

#include "kodipvrchannel.h"

#include <QObject>
#include <QAbstractListModel>

class KodiPVRChannelModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum KodiPVRChannelRoles
    {
        ChannelIDRole = Qt::UserRole + 1,
        ChannelRole,
        LabelRole,
        TitleRole,
        EpisodeNameRole,
        TitleNextRole,
        EpisodeNameNextRole,
        ProgressPercentageRole,
        StartTimeRole,
        EndTimeRole,
        LastPlayedRole,
        ThumbnailRole
    };

    KodiPVRChannelModel(QObject *parent = 0);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void addChannel(const KodiPVRChannel &kodiPVRChannel);
    //bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool setData(const QModelIndex &index, const KodiPVRChannel &kodiPVRChannel);

protected:
    QHash <int, QByteArray> roleNames() const;

private:
    QList<KodiPVRChannel> m_kodiPVRChannels;

public slots:
};

#endif // KODIPVRCHANNELMODEL_H
