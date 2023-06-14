#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FileDownloader : public QObject
{
 Q_OBJECT
 public:
  explicit FileDownloader(QUrl imageUrl, QObject *parent = 0);
  virtual ~FileDownloader();
  QByteArray downloadedData() const;

 signals:
  void downloaded();
  void downloadProgressSignal( qint8 );

 private slots:
  void fileDownloaded(QNetworkReply* pReply);
  // Gestion des erreurs
  void errorHandler(QNetworkReply::NetworkError);
  // Avancement du télechargement
  void downloadProgress( qint64, qint64 );

 private:
  QNetworkAccessManager m_WebCtrl;
  QByteArray m_DownloadedData;
  // Présence d'une erreur dans la requête
  bool m_bReplyError = false;
};

#endif // FILEDOWNLOADER_H
