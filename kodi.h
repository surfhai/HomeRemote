#ifndef KODI_H
#define KODI_H

#include "globals.h"
#include "xbmcclient.h"
#include "kodipvrchannelmodel.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QNetworkRequest>
#include <QVariantMap>
#include <QNetworkCookieJar>
#include <QByteArray>
#include <QDebug>
#include <QAuthenticator>
#include <QList>
#include <QQmlEngine>
#include <QStandardItemModel>

class Kodi : public QObject
{
    Q_OBJECT
public:
    //explicit Kodi(QObject *parent = 0);
    ~Kodi()
    {
        instanceFlag = false;
        delete xbmcEventClient;
    }
    static Kodi* getInstance()
    {
        if (!instanceFlag)
        {
            kodi = new Kodi();
            instanceFlag = true;
            //init();
            return kodi;
        }
        else
        {
            return kodi;
        }
    }
    void init();
    KodiPVRChannelModel *kodiPVRChannelModel;

public slots:
    void inputAction(QString action);
    void switchChannel(int channelID);

private:
    static bool instanceFlag;
    static Kodi *kodi;
    explicit Kodi(QObject *parent = 0) : QObject(parent)
    {
        // private constructor
    }
    //Globals *glb;
    QDebug dbg = qDebug();

    QNetworkAccessManager *naManager;
    QNetworkRequest req;
    QString ip;
    QString port;
    QString user;
    QString pass;
    CXBMCClient *xbmcEventClient;
    void getPVRChannels();
    int jsonID;
    QMap<int, QString> parseJson;


private slots:
    void onSSLErrors(QNetworkReply *);
    void onQueryFinished(QNetworkReply *);
    void provideAuthenication(QNetworkReply *reply, QAuthenticator *auth);
};

#endif // KODI_H
