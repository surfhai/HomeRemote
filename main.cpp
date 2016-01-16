#include "globals.h"
#include "kodi.h"
#include "kodipvrchannel.h"
#include "kodipvrchannelmodel.h"
#include "homematic.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QDebug>
#include <QSortFilterProxyModel>

static QObject *kodiSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    Kodi *kodi = Kodi::getInstance();
    //QQmlEngine::setObjectOwnership(kodi, QQmlEngine::CppOwnership);
    return kodi;
}

static QObject *homematicSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    HomeMatic *homematic = HomeMatic::getInstance();
    //QQmlEngine::setObjectOwnership(kodi, QQmlEngine::CppOwnership);
    return homematic;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    Globals *glb = Globals::getInstance();
    //glb->debug = true;

    Kodi *kodi;
    kodi = Kodi::getInstance();
    kodi->init();

    //HomeMatic *hm = new HomeMatic();
    HomeMatic *homematic;
    homematic = HomeMatic::getInstance();

    KodiPVRChannelModel *kodiPVRChannelModel = new KodiPVRChannelModel();
    kodi->kodiPVRChannelModel = kodiPVRChannelModel;

    QSortFilterProxyModel *kodiPVRChannelModelProxy = new QSortFilterProxyModel();
    kodiPVRChannelModelProxy->setSourceModel(kodiPVRChannelModel);
    //kodiPVRChannelModelProxy->setSortRole(kodiPVRChannelModel->KodiPVRChannelRoles::ChannelIDRole);

    //qmlRegisterType<Kodi>("Kodi.control", 1, 0, "Kodi");
    qmlRegisterSingletonType<Kodi>("Kodi.control", 1, 0, "Kodi", kodiSingletonProvider);
    qmlRegisterSingletonType<HomeMatic>("HomeMatic.control", 1, 0, "HomeMatic", homematicSingletonProvider);

    QQmlApplicationEngine engine;
    QQmlContext *ctxt = engine.rootContext();

    ctxt->setContextProperty("kodiPVRChannelModel", kodiPVRChannelModelProxy);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    
    return app.exec();
}
