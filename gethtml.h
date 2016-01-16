#ifndef GETHTML_H
#define GETHTML_H

#include <QObject>
#include <QUrl>
#include <QTime>
#include <QByteArray>
#include <QNetworkAccessManager>
//#include "textprogressbar.h"

class GetHTML : public QObject
{
    Q_OBJECT
public:
    explicit GetHTML(QObject *parent = 0);
    void getSource(const QUrl &url);
    QByteArray source;

signals:
    void finished();

private:
    QNetworkAccessManager manager;
    QNetworkReply *currentDownload;
    QTime downloadTime;
    //TextProgressBar progressBar;

private slots:
    //void downloadReadyRead();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    //void downloadReadyRead()

public slots:
};

#endif // GETHTML_H
