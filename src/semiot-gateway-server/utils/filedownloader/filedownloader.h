#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QDir>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QUrl url, QObject *parent = 0);
    virtual ~FileDownloader();
    QByteArray downloadedData() const;
    QString download();
signals:
    void downloaded();
    
private slots:
    void httpReadyRead();
    void httpDownloadFinished();
    
private:
    void startRequest();
    QUrl m_url;
    bool m_downloaded;
    QNetworkAccessManager m_WebCtrl;
    QNetworkReply *m_reply;
    QByteArray m_DownloadedData;
    QFile* m_file;

};

#endif // FILEDOWNLOADER_H
