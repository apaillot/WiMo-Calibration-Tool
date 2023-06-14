#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl imageUrl, QObject *parent) :
 QObject(parent)
{
 // Réinit error
 m_bReplyError = false;
 //%%AP - 2020.11.06 - Ajout
 //m_WebCtrl.setStrictTransportSecurityEnabled(true);
 // Fin de réception du fichier
 connect( &m_WebCtrl, SIGNAL (finished(QNetworkReply*)),  this, SLOT (fileDownloaded(QNetworkReply*)) );

 QNetworkRequest request(imageUrl);
 //%%AP - 2020.11.06 - Ajout pour gestion requête https
 //request.setSslConfiguration(QSslConfiguration::defaultConfiguration());

 QNetworkReply * qReply = m_WebCtrl.get(request);
 // Gestion des erreurs
 connect( qReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorHandler(QNetworkReply::NetworkError)));
 // Avancement du téléchargement
 connect( qReply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64) ));
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
 // Si erreur : on quitte
 if( m_bReplyError )
  {
   ///%%TODO : on déclenche l'erreur
   /// emit error();
   return;
  }
 //
 m_DownloadedData = pReply->readAll();

 //emit a signal
 pReply->deleteLater();
 emit downloaded();
}

QByteArray FileDownloader::downloadedData() const {
 return m_DownloadedData;
}

void FileDownloader::errorHandler(QNetworkReply::NetworkError)
{
 qDebug("FileDownloader::errorHandler");
 // On indique qu'il y a eu une erreur au slot enregistrer
 m_bReplyError = true;

 QNetworkReply *r = qobject_cast<QNetworkReply*>(sender());
 qDebug("FileDownloader::errorHandler >> %s", r->errorString().toLatin1().data());
 //QMessageBox::critical(this, "Erreur", "Erreur lors du chargement. Vérifiez votre connexion internet ou réessayez plus tard <br /><br /> Code de l'erreur : <br /><em>" + r->errorString() + "</em>");
 //close();
}

//-------------------------------------------------------------
// Avancement du télechargement
//-------------------------------------------------------------
void FileDownloader::downloadProgress( qint64 bytesReceived, qint64 bytesTotal )
{
 if( bytesTotal > 0 )
  {
   emit downloadProgressSignal( ( qint8 )( bytesReceived * 100 / bytesTotal ) );
   //ptSensorUpdateControler->setDownloadProgression( bytesReceived / bytesTotal * 100 );
   //progression->setRange(0, bytesTotal);
   //progression->setValue(bytesReceived);
  }
}
