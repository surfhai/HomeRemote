#include "hmdevice.h"

QList<HMDevice*> HMDevice::m_devices;
QStringList HMDevice::m_rooms;
int HMDevice::m_count = 0;

HMDevice::HMDevice(QObject *parent) : QObject(parent)
{
    m_devices.append(this);
    m_count++;
}

int HMDevice::count()
{
    return m_count;
}

HMDevice* HMDevice::getDevice(int index)
{
    return m_devices[index];
}

QStringList HMDevice::getRooms()
{
    return m_rooms;
}
