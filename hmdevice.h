#ifndef HMDEVICE_H
#define HMDEVICE_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QStringList>

class HMDevice : public QObject
{
    Q_OBJECT
public:
    explicit HMDevice(QObject *parent = 0);


    int count();
    HMDevice* getDevice(int index);
    QStringList getRooms();

signals:

public slots:

private:
    static QList<HMDevice*> m_devices;
    static int m_count;
    static QStringList m_rooms;
    QString m_address;
    int m_iseID;
    QString m_craft; // Gewerk
    QString m_room;
    QString m_name;
    QString m_type;

    QMap<QString, QString> m_dataPoints;
};

#endif // HMDEVICE_H
