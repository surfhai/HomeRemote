#ifndef HOMEMATIC_H
#define HOMEMATIC_H

#include "maiaXmlRpcClient.h"
#include "maiaXmlRpcServer.h"
#include "gethtml.h"

#include <QString>
#include <QStringList>

class HomeMatic : public QObject
{
    Q_OBJECT
public:
    //explicit HomeMatic(QObject *parent = 0);
    ~HomeMatic();
    static HomeMatic* getInstance()
    {
        if (!instanceFlag)
        {
            homematic = new HomeMatic();
            instanceFlag = true;
            return homematic;
        }
        else
        {
            return homematic;
        }
    }


signals:

public slots:
    void hmInit();
    void handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void response(QVariant &arg);
    void faultRespone(int error, const QString &message);
    QString eventResponse(QVariant &arg);
    QVariantList listMethods(QVariant &arg);
    QVariantList listMethods(QString arg);
    QVariantList multicall(QVariantList arg);
    QVariantList listDevices(QString arg);

private:
    static bool instanceFlag;
    static HomeMatic *homematic;
    explicit HomeMatic(QObject *parent = 0);

    MaiaXmlRpcClient *client;
    MaiaXmlRpcClient *client2;
    MaiaXmlRpcServer *server;
    void hmInitLists();
    QString myMethod(int param1, QString param2);

    GetHTML *source;
    QString hmHost;
    QString hmPortBC;
    QString localHost;
    int localXmlRpcPort;
    QString localHmXmlRpcID;
    QStringList hmInitQueries;

    void parseDeviceList(QXmlStreamReader &xml);
};

#endif // HOMEMATIC_H
