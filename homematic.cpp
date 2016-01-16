#include "homematic.h"
#include "maiaXmlRpcClient.h"
#include "maiaXmlRpcServer.h"
#include "gethtml.h"

#include <QUrl>
#include <QString>
#include <QVariantList>
#include <QXmlStreamReader>

bool HomeMatic::instanceFlag = false;
HomeMatic* HomeMatic::homematic = NULL;

HomeMatic::HomeMatic(QObject *parent) : QObject(parent)
{
    // wireshark filter: (tcp.port == 8888 || tcp.port == 2001) && (http.chat)

    // HomeMatic CCU IP
    hmHost = "192.168.2.103";

    // HomeMatic XML RPC Port
    hmPortBC = "2001";

    // local IP address
    localHost = "192.168.2.101";

    // local Port for XML RPC events from HomeMatic CCU
    localXmlRpcPort = 8888;

    // ID for XML RPC to register the local server at the HomeMatic CCU
    localHmXmlRpcID = "RPi";

    client = new MaiaXmlRpcClient(QUrl("http://" + hmHost + ":" + hmPortBC), this);
    server = new MaiaXmlRpcServer(localXmlRpcPort, this);

    QSslConfiguration config = client->sslConfiguration();
    config.setProtocol(QSsl::AnyProtocol);
    client->setSslConfiguration(config);

    server->addMethod("event", this, "eventResponse");
    server->addMethod("system.listMethods", this, "listMethods");
    server->addMethod("system.multicall", this, "multicall");
    server->addMethod("listDevices", this, "listDevices");

    QVariantList params;
    params << "http://" + localHost + ":" + localXmlRpcPort;
    params << localHmXmlRpcID;

    client->call("init", params,
                 this, SLOT(response(QVariant&)),
                 this, SLOT(faultRespone(int,QString)));

    // http://192.168.2.103/config/xmlapi/devicelist.cgi
    source = new GetHTML();
    source->getSource(QUrl("http://" + hmHost + "/config/xmlapi/devicelist.cgi"));

    connect (source, &GetHTML::finished,
             this, &HomeMatic::hmInit);
}

HomeMatic::~HomeMatic()
{
    instanceFlag = false;
    QVariantList params;
    params << "http://" + localHost + ":" + localXmlRpcPort;
    // empty paramter to unregister local XML RPC server
    params << "";

    client->call("init", params,
                 this, SLOT(response(QVariant&)),
                 this, SLOT(faultRespone(int,QString)));
    source->deleteLater();
    client->deleteLater();
    server->deleteLater();
}

void HomeMatic::hmInit()
{
    QXmlStreamReader xmlReader(source->source);
    while(!xmlReader.atEnd() && !xmlReader.hasError())
    {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartElement)
        {
            QStringRef name = xmlReader.name();
            if (name == "deviceList")
            {
                parseDeviceList(xmlReader);
            }
            return;
        }
    }
}

void HomeMatic::parseDeviceList(QXmlStreamReader &xml)
{
    QString name;
    QString address;
    int iseID;
    QString interface;
    QString deviceType;
    bool readyConfig;

    bool newDevice;

    int counter = 0;
    QXmlStreamReader::TokenType token = xml.readNext();
    while (token != QXmlStreamReader::EndDocument)
    {
        if (token == QXmlStreamReader::EndDocument || token == QXmlStreamReader::EndElement || xml.name().isEmpty())
        {
            token = xml.readNext();
            continue;
        }
        QStringRef name = xml.name();
        qDebug() << "NAME:" << name;
        if (name == "device")
        {
            newDevice = true;

        }
       if (name == "channel")
       {
       }


        // exit while loop after
        if (counter > 10000)
            break;
        counter++;

        token = xml.readNext();
    }
}

void HomeMatic::handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    qDebug() << "SSL Error:" << errors;
    reply->ignoreSslErrors(); // don't do this in real code! Fix your certs!
}

void HomeMatic::response(QVariant &arg)
{
    qDebug() << "response:" << arg;
}

void HomeMatic::faultRespone(int error, const QString &message)
{
    qDebug() << "An Error occured, Code:" << error << "Message:" << message;
}

QString HomeMatic::eventResponse(QVariant &arg)
{
    qDebug() << "eventResponse() params:" << arg;
    return QString();
}

QVariantList HomeMatic::listMethods(QVariant &arg)
{
    qDebug() << "listMethods(QVariant &arg) params:" << arg;
    QVariantList params;
    params << "event";
    return params;
}

QVariantList HomeMatic::listMethods(QString arg)
{
    qDebug() << "listMehods() params:" << arg;
    QVariantList params;
    params << "event";
    return params;
}

QVariantList HomeMatic::multicall(QVariantList arg)
{
    for (int i = 0; i < arg.count(); i++)
    {
        if (arg[i].toMap()["methodName"] == "event")
        {
        }
        qDebug() << "seperating:" << arg[i].toMap()["methodName"];
        qDebug() << "seperating:" << arg[i].toMap()["params"].toList();
    }
    qDebug() << "multicall() params:" << arg;
    QVariantList params;
    params << "event";
    return params;
}

QVariantList HomeMatic::listDevices(QString arg)
{
    qDebug() << "listDevices() params:" << arg;
    QVariantList params;
    params << "event";
    return params;
}
