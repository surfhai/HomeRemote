#include "gethtml.h"
#include <QTimer>
#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <stdio.h>

GetHTML::GetHTML(QObject *parent) : QObject(parent)
{
}

void GetHTML::getSource(const QUrl &url)
{
    QNetworkRequest request(url);
    currentDownload = manager.get(request);

    //connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
    //        SLOT(downloadProgress(qint64,qint64)));
    connect(currentDownload, SIGNAL(finished()),
            SLOT(downloadFinished()));
    //connect(currentDownload, SIGNAL(readyRead()),
            //SLOT(downloadReadyRead()));

    // prepare the output
    qDebug() << "Downloading" << url.toEncoded().constData() << "...";
    //printf("Downloading %s...\n", url.toEncoded().constData());
    downloadTime.start();
}

void GetHTML::downloadFinished()
{
    //progressBar.clear();

    if (currentDownload->error()) {
        // download failed
        qDebug() << "Failed:" << qPrintable(currentDownload->errorString());
        //fprintf(stderr, "Failed: %s\n", qPrintable(currentDownload->errorString()));
    } else {
        qDebug() << "Succeeded.";
        //printf("Succeeded.\n");
    }

    currentDownload->deleteLater();

    // output source
    source = currentDownload->readAll();
    //printf(currentDownload->readLine());
    //printf(currentDownload->readAll());

    // start next download
    emit finished();
}

void GetHTML::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    //progressBar.setStatus(bytesReceived, bytesTotal);

    // calculate the download speed
    double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
    QString unit;
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }

    //progressBar.setMessage(QString::fromLatin1("%1 %2")
    //                       .arg(speed, 3, 'f', 1).arg(unit));
    //progressBar.update();
}

//void GetHTML::downloadReadyRead()
//{
//
//}
