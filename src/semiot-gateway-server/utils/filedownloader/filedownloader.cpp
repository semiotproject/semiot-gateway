#include "filedownloader.h"
#include <QEventLoop>
#include <QDir>

FileDownloader::FileDownloader(QUrl url, QObject *parent) :
QObject(parent)
    ,m_url(url)
    ,m_downloaded(false)
{
    QString fileName = m_url.fileName();
    m_file = new QFile(fileName);
    if (!m_file->open(QIODevice::WriteOnly)) {
        delete m_file;
        m_file = 0;
        m_downloaded = true;
    }
}

FileDownloader::~FileDownloader() { }


void FileDownloader::httpReadyRead()
{
    if (m_file) {
        m_DownloadedData = m_reply->readAll();
        QString replacedString((m_DownloadedData).replace("${ROOT_URL}","http://semiot.ru")); // FIXME: regexp
        m_file->write(replacedString.toUtf8()); // FIXME: replacing to other class
    }
}

QByteArray FileDownloader::downloadedData() const {
    return m_DownloadedData;
}

QString FileDownloader::download()
{
    QEventLoop waiterLoop;
    connect(this, SIGNAL(downloaded()), &waiterLoop, SLOT(quit()));
    startRequest();
    waiterLoop.exec();
    return "file://"+QDir::currentPath()+"/"+m_file->fileName(); // FIXME: To QUrl
}

void FileDownloader::startRequest()
{
    QNetworkRequest request(m_url);
    m_reply = m_WebCtrl.get(request);
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
    connect(m_reply, SIGNAL(finished()),
            this, SLOT(httpDownloadFinished()));
}

void FileDownloader::httpDownloadFinished()
{
    QVariant redirectionTarget = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (m_reply->error()) {
        m_file->remove();
    }
    else if (!redirectionTarget.isNull()) {
        QUrl newUrl = m_url.resolved(redirectionTarget.toUrl());
        m_url = newUrl;
        m_reply->deleteLater();
        m_file->open(QIODevice::WriteOnly);
        m_file->resize(0);
        startRequest();
    }
    else {
        m_file->close();
        emit downloaded();
    }
}
