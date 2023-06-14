#include "GetHTTPFile.h"

GetHTTPFile::GetHTTPFile( QObject *parent ) :
 QObject(parent)
{
 //qDebug("GetVersion::GetVersion");
 //qDebug("End GetVersion::GetVersion");
}

//=============================================================
// Getter/Setter
//=============================================================
void GetHTTPFile::setFileName( QString sFileName )
{
 m_sFileName = sFileName;
}
void GetHTTPFile::setHTTPPath( QString sFilePath )
{
 m_sHTTPPath = sFilePath;
}
QByteArray GetHTTPFile::getData( void )
{
 return( m_tData );
}

//=============================================================
// Code
//=============================================================

//-------------------------------------------------------------
//
//-------------------------------------------------------------
void GetHTTPFile::startRequest( void )
{
 //qDebug("GetHTTPFile::startRequest");
 // Test intégrité
 if(  ( m_sHTTPPath == "" )
   || ( m_sFileName == "" ) )
  {
   //
   qDebug( "GetHTTPFile::startRequest >> Error" );
   return;
  }
 // Création de l'url
 QString sUrl = m_sHTTPPath+m_sFileName;
 m_tHTTPUrl.clear();
 m_tHTTPUrl.setUrl( sUrl, QUrl::StrictMode );
 qDebug( "%s", m_tHTTPUrl.toString().toLatin1().data() );
 // Création de l'handler pour téléchargement du fichier
 m_ptHTTPCtrl = new FileDownloader( m_tHTTPUrl, this );
 // Création de la connexion pour fin de réception de donnée
 connect(m_ptHTTPCtrl, &FileDownloader::downloaded, this, &GetHTTPFile::loadData);
 // Connexion signal progression
 connect(m_ptHTTPCtrl, &FileDownloader::downloadProgressSignal, this, &GetHTTPFile::dataLoadProgressSignal);
}

//-------------------------------------------------------------
// Fin de reception de donnée
//-------------------------------------------------------------
void GetHTTPFile::loadData()
{
 m_tData = m_ptHTTPCtrl->downloadedData();
 // On lance le signal de réception
 emit dataLoaded();
}


