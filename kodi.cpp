#include "globals.h"
#include "kodi.h"
#include "kodipvrchannel.h"
#include "kodipvrchannelmodel.h"
#include "xbmcclient.h"
#include "utils.h"

#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QAuthenticator>
#include <QTimer>

bool Kodi::instanceFlag = false;
Kodi* Kodi::kodi = NULL;

void Kodi::init()
{
    //glb = Globals::getInstance();
    dbg.noquote();

    //JSONRPC Access Details
    ip = "tg-server";
    port = "8080";
    user = "xbmc";
    pass= "xbmc";

    //set request details
    req.setUrl(QUrl("http://" + ip + ":" + port + "/jsonrpc"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json;");

    naManager = new QNetworkAccessManager(this);

    // finished query
    connect (naManager, &QNetworkAccessManager::finished,
             this, &Kodi::onQueryFinished);

    // handle SSL Errors
    connect (naManager, &QNetworkAccessManager::sslErrors,
             this, &Kodi::onSSLErrors);

    // connect to authenticator
    connect (naManager, &QNetworkAccessManager::authenticationRequired,
             this, &Kodi::provideAuthenication);

    // create a QCookieJar
    naManager->setCookieJar(new QNetworkCookieJar(naManager));

    // XBMC EventClient
    xbmcEventClient = new CXBMCClient("192.168.2.102", 9777);

    // update PVR Channellist
    QTimer *timerPVRChannels = new QTimer(this);
    connect (timerPVRChannels, &QTimer::timeout,
             this, &Kodi::getPVRChannels);
    timerPVRChannels->start(60000);
    jsonID = 0;
    getPVRChannels();
}

void Kodi::onSSLErrors(QNetworkReply *re)
{
    re->ignoreSslErrors();
}

void Kodi::onQueryFinished(QNetworkReply *re)
{
    // check for network erros
    if (re->error() != QNetworkReply::NoError)
    {
        qDebug() << re->errorString();
        re->deleteLater();
        return;
    }

    QByteArray result = re->readAll();
    re->deleteLater();

    // check for JSON errors
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(result, &err);
    if (err.error != QJsonParseError::NoError)
    {
        qDebug() << err.errorString();
        return;
    }

    // parse JSON
    QJsonObject obj = doc.object();
    QString parseString = parseJson.value(obj["id"].toInt());

    if (parseString == "PVR.GetChannels")
    {

        QJsonObject result = obj["result"].toObject();
        QJsonArray channels = result["channels"].toArray();

        for (int i = 0; i < channels.count(); i++)
        {
            QJsonObject broadcastnow = channels[i].toObject()["broadcastnow"].toObject();
            QJsonObject broadcastnext = channels[i].toObject()["broadcastnext"].toObject();
            QJsonObject channelsObj = channels[i].toObject();
            QString channel = channelsObj["channel"].toString();
            int channelid = channelsObj["channelid"].toInt();
            QString label = fixEncoding(channelsObj["label"].toString());
            QString title = fixEncoding(broadcastnow["title"].toString().left(50));
            QString episodename = fixEncoding(broadcastnow["episodename"].toString().left(50));
            qDebug() << "Textlänge:" << title.count() + episodename.count();
            QString titleNext = fixEncoding(broadcastnext["title"].toString().left(50));
            QString episodenameNext = fixEncoding(broadcastnext["episodename"].toString().left(50));
            double progresspercentage = broadcastnow["progresspercentage"].toDouble();
            if (progresspercentage < 0.1)
                progresspercentage = 0.001;
            else if (progresspercentage > 100.0)
                progresspercentage = 1.0;
            else
                progresspercentage = progresspercentage / 100.0;
            QDateTime starttime = QDateTime::fromString(broadcastnow["starttime"].toString(),
                                                        "yyyy-MM-dd HH:mm:ss");
            QDateTime endtime = QDateTime::fromString(broadcastnow["endtime"].toString(),
                                                      "yyyy-MM-dd HH:mm:ss");

            QString lastplayedStr = channelsObj["lastplayed"].toString();
            QDateTime lastplayed;
            if (lastplayedStr.length() > 10)
                lastplayed = QDateTime::fromString(lastplayedStr, "yyyy-MM-dd HH:mm:ss");
            else
                lastplayed = QDateTime::fromString(lastplayedStr, "yyyy-MM-dd");
            
            QString thumbnail = channelsObj["thumbnail"].toString();
            thumbnail.replace("image://", "");
            thumbnail.replace("127.0.0.1", ip);
            thumbnail.replace("%3a", ":");
            thumbnail.replace("%2f", "/");
            thumbnail.replace("%40", "@");

            KodiPVRChannel kodiPVRChannel(channel,
                                          channelid,
                                          label,
                                          title,
                                          episodename,
                                          titleNext,
                                          episodenameNext,
                                          progresspercentage,
                                          starttime,
                                          endtime,
                                          lastplayed,
                                          thumbnail);

            if (kodiPVRChannelModel->hasIndex(i, 0))
            {
                QModelIndex index = kodiPVRChannelModel->index(i, 0, QModelIndex());
                if (i == channels.count() - 1)
                    kodiPVRChannelModel->setData(index, kodiPVRChannel, true);
                else
                    kodiPVRChannelModel->setData(index, kodiPVRChannel, false);
            }
            else
            {
                kodiPVRChannelModel->addChannel(kodiPVRChannel);
            }

            /*
            qDebug() << "channel:" << channel << "\ttitle:" << title << "\ttitleNext:" << titleNext << "\tepisode:" << episodename << "\tepisodeNext:" << episodenameNext;
            qDebug() << "channel:" << channel
                     << "channelid:" << channelid
                     << "label:" << label
                     << "title:" << title
                     << "episodename:" << episodename
                     << "titleNext:" << titleNext
                     << "episodenameNext:" << episodenameNext
                     << "progresspercentage:" << progresspercentage
                     << "starttime:" << starttime.toString()
                     << "endtime:" << endtime.toString()
                     << "lastplayed:" << lastplayed.toString();
            */
        }
    }
    else
    {
        // parseJson was not matched
    }
}

void Kodi::provideAuthenication(QNetworkReply *reply, QAuthenticator *auth)
{
    Q_UNUSED(reply)
    auth->setUser(QString(user));
    auth->setPassword(QString(pass));
}

void Kodi::switchChannel(int channelID)
{
    jsonID++;
    parseJson.insert(jsonID, "Player.Open");

    // Player.Open
    QJsonObject jsonObj;
    QJsonObject jsonParams;
    QJsonObject jsonItem;
    jsonObj["jsonrpc"] = "2.0";
    jsonObj["id"] = QJsonValue(jsonID);
    jsonObj["method"] = "Player.Open";
    jsonItem["channelid"] = channelID;
    jsonParams["item"] = jsonItem;
    jsonObj["params"] = jsonParams;

    QJsonDocument jsonDoc(jsonObj);
    qDebug().noquote() << "JSON REQUEST" << jsonDoc.toJson(QJsonDocument::Compact);
    naManager->post(req, jsonDoc.toJson(QJsonDocument::Compact));
}

void Kodi::inputAction(QString action)
{
    QJsonObject jsonObj;
    jsonObj["jsonrpc"] = "2.0";
    jsonObj["id"] = 1;
    if (action == "right")
    {
        xbmcEventClient->SendButton("right", "R1", BTN_NO_REPEAT);
        qDebug() << "xbmcEventClient: SendButton RIGHT R1 BTN_NO_REPEAT";
        return;
    }
    if (action == "left")
    {
        xbmcEventClient->SendButton("left", "R1", BTN_NO_REPEAT);
        qDebug() << "xbmcEventClient: SendButton LEFT R1 BTN_NO_REPEAT";
        return;
    }
    if (action == "up")
    {
        xbmcEventClient->SendButton("up", "R1", BTN_NO_REPEAT);
        qDebug() << "xbmcEventClient: SendButton UP R1 BTN_NO_REPEAT";
        return;
    }
    if (action == "down")
    {
        xbmcEventClient->SendButton("down", "R1", BTN_NO_REPEAT);
        qDebug() << "xbmcEventClient: SendButton DOWN R1 BTN_NO_REPEAT";
        return;
    }
    if (action == "select")
    {
        xbmcEventClient->SendButton("enter", "KB", BTN_NO_REPEAT);
        qDebug() << "xbmcEventClient: SendButton ENTER KB BTN_NO_REPEAT";
        return;
    }
    if (action == "back")
    {
        jsonObj["method"] = "Input.ExecuteAction";
        QJsonObject jsonParams;
        jsonParams["action"] = "back";
        jsonObj["params"] = jsonParams;
    }
    if (action == "home")
    {
        jsonObj["method"] = "Input.Home";
    }
    if (action == "showOSD")
    {
        jsonObj["method"] = "Input.ShowOSD";
    }

    QJsonDocument jsonDoc(jsonObj);
    qDebug().noquote() << jsonDoc.toJson(QJsonDocument::Compact);
    naManager->post(req, jsonDoc.toJson(QJsonDocument::Compact));
}

void Kodi::getPVRChannels()
{
    jsonID++;

    // PVR.GetChannels
    QJsonObject jsonObj;
    jsonObj["jsonrpc"] = "2.0";
    QJsonValue jsonIDValue;
    jsonObj["id"] = QJsonValue(jsonID);
    jsonObj["method"] = "PVR.GetChannels";
    QJsonObject jsonParams;
    jsonParams["channelgroupid"] = "alltv";
    QJsonArray jsonArray;
    jsonArray.append("channel");
    jsonArray.append("channeltype");
    jsonArray.append("hidden");
    jsonArray.append("lastplayed");
    jsonArray.append("locked");
    jsonArray.append("thumbnail");
    jsonArray.append("broadcastnow");
    jsonArray.append("broadcastnext");
    jsonParams["properties"] = jsonArray;
    jsonObj["params"] = jsonParams;

    parseJson.insert(jsonID, "PVR.GetChannels");
    QJsonDocument jsonDoc(jsonObj);
    //if (glb->debug)
        //qDebug().noquote() << "JSON REQUEST" << jsonDoc.toJson(QJsonDocument::Compact) << endl;
    naManager->post(req, jsonDoc.toJson(QJsonDocument::Compact));
}







    /* Kodi JSONRPC requests
     * http://tg-server:8080/jsonrpc?request=
     *
     * JSON Formatter
     * https://jsonformatter.curiousconcept.com/
     *
     * request
     * {"jsonrpc":"2.0","id":1,"method":"Application.GetProperties","params":{"properties":["volume","muted"]}}
     * response
     * {"id":1,"jsonrpc":"2.0","result":{"muted":false,"volume":54}}
     *
     * request
     * {"jsonrpc":"2.0","id":2,"method":"Player.GetProperties","params":{"playerid":1,"properties":["audiostreams","canseek","currentaudiostream","currentsubtitle","partymode","playlistid","position","repeat","shuffled","speed","subtitleenabled","subtitles","time","totaltime","type"]}}
     * {"jsonrpc":"2.0","id":3,"method":"Player.GetItem","params":{"playerid":1,"properties":["album","albumartist","artist","director","episode","fanart","file","genre","plot","rating","season","showtitle","studio","imdbnumber","tagline","thumbnail","title","track","writer","year","streamdetails","originaltitle","cast","playcount"]}}
     * {"jsonrpc":"2.0","id":4,"method":"XBMC.GetInfoLabels","params":{"labels":["MusicPlayer.Codec","MusicPlayer.SampleRate","MusicPlayer.BitRate"]}}
     * response
     * {"id":2,"jsonrpc":"2.0","result":{"audiostreams":[{"bitrate":377110,"channels":2,"codec":"ac3","index":0,"language":"ger","name":"AC3 Stereo"},{"bitrate":0,"channels":6,"codec":"ac3","index":1,"language":"eng","name":"AC3 5.1"}],"canseek":true,"currentaudiostream":{"bitrate":377110,"channels":2,"codec":"ac3","index":0,"language":"ger","name":"AC3 Stereo"},"currentsubtitle":{},"partymode":false,"playlistid":1,"position":-1,"repeat":"off","shuffled":false,"speed":1,"subtitleenabled":false,"subtitles":[],"time":{"hours":0,"milliseconds":0,"minutes":8,"seconds":9},"totaltime":{"hours":0,"milliseconds":0,"minutes":50,"seconds":0},"type":"video"}}
     * {"id":3,"jsonrpc":"2.0","result":{"item":{"cast":"","director":"","fanart":"","genre":["Documentary"],"id":128,"imdbnumber":"","label":"NatGeo HD","originaltitle":"","plot":"3. Staffel, Folge 2: Irak, im November 2003: Eine Frachtmaschine des deutschen Logistikunternehmens DHL gerĦt urplĳtzlich þber Bagdad unter Beschuss. Eine Boden-Luft-Rakete vom Typ SAM-7 hat den Airbus getroffen, der mit brennender TragflĦche notlanden muss. NATIONAL GEOGRAPHIC zeichnet das Drama nach.\nUSA/CDN 2005. 50 Min.","rating":0,"thumbnail":"image://http%3a%2f%2fxbmc%3axbmc%40127.0.0.1%3a9981%2fimagecache%2f56/","title":"Mayday - Alarm im Cockpit","type":"channel","writer":"","year":0}}}
     * {"id":4,"jsonrpc":"2.0","result":{"MusicPlayer.BitRate":"","MusicPlayer.Codec":"","MusicPlayer.SampleRate":""}}
     *
     * request
     * {"jsonrpc":"2.0","id":455,"method":"PVR.GetChannels","params":{"channelgroupid":"alltv","properties":["channel","channeltype","hidden","lastplayed","locked","thumbnail","broadcastnow"]}}
     *
     * {
     *     "jsonrpc":"2.0",
     *     "id":455,
     *     "method":"PVR.GetChannels",
     *     "params":{
     *         "channelgroupid":"alltv",
     *         "properties":[
     *         "channel",
     *         "channeltype",
     *         "hidden",
     *         "lastplayed",
     *         "locked",
     *         "thumbnail",
     *         "broadcastnow"
     *         ]
     *     }
     * }
     */

/* REQUEST
 * {
 *   "jsonrpc":"2.0",
 *   "method":"Player.GetItem",
 *   "params":{
 *     "playerid":1,
 *     "properties":[
 *       "channeltype",
 *       "hidden",
 *       "locked",
 *       "channel",
 *       "plot",
 *       "channelnumber",
 *       "rating",
 *       "title",
 *       "plotoutline",
 *       "thumbnail",
 *       "starttime",
 *       "endtime",
 *       "runtime",
 *       "firstaired"
 *     ]
 *   },
 *   "id":1
 * }
 *
 * RESPONSE
 * {
 *   "id":1,
 *   "jsonrpc":"2.0",
 *   "result":{
 *     "item":{
 *       "channel":"Nat Geo Wild HD",
 *       "channelnumber":60,
 *       "channeltype":"tv",
 *       "endtime":"2016-01-03 02:15:00",
 *       "firstaired":"1970-01-01",
 *       "hidden":false,
 *       "id":51,
 *       "label":"Nat Geo Wild HD",
 *       "locked":false,
 *       "plot":"1. Staffel, Folge 1: Im Ozean Ã¾berleben nur die StÄ¦rksten und die Schnellsten.
 *               Steinfische sind begnadete JÄ¦ger. Sie saugen ihre Nahrung durch Unterdruck in
 *               ihr offenes Maul. Robben tÄ³ten ihre Opfer durch PeitschenschlÄ¦ge mit ihrer
 *               Flosse, und Fangschreckenkrebse kÄ³nnen ihre wuchtigen SchlÄ¦ge mit dem Tempo
 *               von Gewehrkugeln ausfÃ¾hren. Was die Geschwindigkeit angeht, liegen Meeresbewohner
 *              mit ihrer Ã¾ber drei Milliarden Jahre wÄ¦hrenden Evolution ganz weit vorne.\nZA 0. 50 Min.",
 *       "plotoutline":"Ozeane",
 *       "rating":0,
 *       "runtime":"50",
 *       "starttime":"2016-01-03 01:25:00",
 *       "thumbnail":"image://http%3a%2f%2fxbmc%3axbmc%40127.0.0.1%3a9981%2fimagecache%2f55/",
 *       "title":"TÄ³dliche Geschwindigkeit",
 *       "type":"channel"
 *     }
 *   }
 * }
 *
 * REQUEST switch channel
 * {
 *   "jsonrpc":"2.0",
 *   "id":501,
 *   "method":"Player.Open",
 *   "params":{
 *     "item":{
 *       "channelid":24
 *     }
 *   }
 * }
 *
 * REQUEST OSD shown?
 * {
 *   "jsonrpc":"2.0",
 *   "method":"GUI.GetProperties",
 *   "params":{
 *     "properties":[
 *       "fullscreen",
 *       "currentcontrol"
 *     ]
 *   },
 *   "id":1
 * }
 *
 * RESPONSE no OSD shown
 * {
 *   "id":1,
 *   "jsonrpc":"2.0",
 *   "result":{
 *     "currentcontrol":{
 *       "label":""
 *     },
 *     "fullscreen":true
 *   }
 * }
 * RESPONSE OSD shown
 * {
 *   "id":1,
 *   "jsonrpc":"2.0",
 *   "result":{
 *     "currentcontrol":{
 *       "label":"SOMETHING BUT NOT EMPTY"
 *     },
 *     "fullscreen":true
 *   }
 * }
 */


   /* REQUEST
    * {
    *     "jsonrpc":"2.0",
    *     "id":455,
    *     "method":"PVR.GetChannels",
    *     "params":{
    *         "channelgroupid":"alltv",
    *         "properties":[
    *         "channel",
    *         "channeltype",
    *         "hidden",
    *         "lastplayed",
    *         "locked",
    *         "thumbnail",
    *         "broadcastnow"
    *         ]
    *     }
    * }
    *
    * RESPONSE
    * {
    *     "id":455,
    *     "jsonrpc":"2.0",
    *     "result":{
    *         "channels":[
    *         {
    *             "broadcastnow":{
    *                 "broadcastid":69434831,
    *                 "cast":"",
    *                 "director":"",
    *                 "endtime":"2015-12-30 03:53:00",
    *                 "episodename":"Der JahresrÃ¼ckblick zum Mitraten und Mitspielen",
    *                 "episodenum":0,
    *                 "episodepart":0,
    *                 "filenameandpath":"pvr://guide/-001/2015-12-30 00:35:00.epg",
    *                 "firstaired":"1970-01-01",
    *                 "genre":[
    *                 "Other",
    *                 "Unknown"
    *                 ],
    *                 "hasrecording":false,
    *                 "hastimer":false,
    *                 "imdbnumber":"",
    *                 "isactive":true,
    *                 "originaltitle":"",
    *                 "parentalrating":0,
    *                 "plot":"Das ganze Jahr zum Mitraten und Mitspielen: Frank Plasberg prÃ¤sentiert
    *                         seine groÃŸe Familienshow \"2015 - Das Quiz\" im Ersten, mit den bewÃ¤hrten
    *                         Top-Kandidaten GÃ¼nther Jauch und Barbara SchÃ¶neberger, Annette Frier und
    *                         sowie noch ein weiterer prominenter Newcomer. Sie alle erleben die zurÃ¼ckliegenden
    *                         zwÃ¶lf Monate neu, an einem einzigen Abend - und spielen dabei um Geld fÃ¼r
    *                         den guten Zweck.Moderation: Frank PlasbergProduziert in HD",
    *                 "plotoutline":"Der JahresrÃ¼ckblick zum Mitraten und Mitspielen",
    *                 "progress":7994,
    *                 "progresspercentage":67.87879180908203125,
    *                 "rating":0,
    *                 "recording":"",
    *                 "runtime":"198",
    *                 "starttime":"2015-12-30 00:35:00",
    *                 "title":"2015 - Das Quiz",
    *                 "wasactive":false,
    *                 "writer":"",
    *                 "year":0
    *             },
    *             "channel":"Das Erste HD",
    *             "channelid":24,
    *             "channeltype":"tv",
    *             "hidden":false,
    *             "label":"Das Erste HD",
    *             "lastplayed":"2015-12-15",
    *             "locked":false,
    *             "thumbnail":"image://http%3a%2f%2fxbmc%3axbmc%40127.0.0.1%3a9981%2fimagecache%2f2/"
    *         },
    *         ...
    *
    * "thumbnail":"image://http%3a%2f%2fxbmc%3axbmc%40127.0.0.1%3a9981%2fimagecache%2f2/"
    * "thumbnail":"http://tg-server:9981/imagecache/2"
    */


        //jsonObj["method"] = "Input.Right";


        //jsonObj["method"] = "Input.ExecuteAction";
        //QJsonObject jsonParams;
        //jsonParams["action"] = "right";
        //jsonObj["params"] = jsonParams;


        //jsonObj["method"] = "Input.Left";


        //jsonObj["method"] = "Input.ExecuteAction";
        //QJsonObject jsonParams;
        //jsonParams["action"] = "left";
        //jsonObj["params"] = jsonParams;


        //jsonObj["method"] = "Input.Up";


        //jsonObj["method"] = "Input.ExecuteAction";
        //QJsonObject jsonParams;
        //jsonParams["action"] = "up";
        //jsonObj["params"] = jsonParams;


        //jsonObj["method"] = "Input.Down";


        //jsonObj["method"] = "Input.ExecuteAction";
        //QJsonObject jsonParams;
        //jsonParams["action"] = "down";
        //jsonObj["params"] = jsonParams;


        //jsonObj["method"] = "Input.ShowOSD";


        //jsonObj["method"] = "Input.ExecuteAction";
        //QJsonObject jsonParams;
        //jsonParams["action"] = "enter";
        //jsonObj["params"] = jsonParams;


        //QJsonObject jsonParams;
        //jsonParams["action"] = "home";
        //jsonObj["params"] = jsonParams;


//{ "jsonrpc":"2.0", "method":"Player.GetItem", "params":{ "playerid":1, "properties":[ "channelnumber" ] }, "id":1 }
