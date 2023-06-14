#include "HTTPUpdate.h"
#include "GetHTTPFile.h"
#include <QFile>
#include "main.h"
#include <mainevent.h>

HTTPUpdate::HTTPUpdate( QObject *parent ) :
 QObject(parent)
{
 qDebug("HTTPUpdate::HTTPUpdate");//QString("Version.txt")
}


//-------------------------------------------------------------
// Démarrage de la vérification de mise à jour
//-------------------------------------------------------------
void HTTPUpdate::startUpdateCheck( void )
{
 qDebug("HTTPUpdate::startUpdate");
 // Création d'un objet pour la requête version
 m_tHTTPProcess = new GetHTTPFile();
 m_tHTTPProcess->setHTTPPath( m_sHTTPPath );
 // Nom du fichier qui va être requêté
 m_sVersionFileName = "Version.txt";
 m_tHTTPProcess->setFileName( m_sVersionFileName );

 // Lancement de la requête de download version
 m_tHTTPProcess->startRequest();
 // Connexion de la réception de la version
 //connect( &m_tHTTPProcess, &GetHTTPFile::dataLoaded, this, &HTTPUpdate::versionLoaded );
 connect( m_tHTTPProcess, &GetHTTPFile::dataLoaded, this, &HTTPUpdate::versionLoaded );
 qDebug("End HTTPUpdate::startUpdate");
}

// La version a été reçue
void HTTPUpdate::versionLoaded( void )
{
 m_tVersion = m_tHTTPProcess->getData();
 qDebug( m_tVersion );
 // Ecriture de la donnée dans un fichier
 this->bFWriteToFile( m_sVersionFileName, m_tVersion );

 // Vérification conformité numéro de version
 // Format X.X.X
 ///%%TODO-----------------
 ///
 ///
}


//----------------------------------------
// Fonction annexe/utilitaire
//----------------------------------------
// Ecriture des données en entrée dans un fichier
bool HTTPUpdate::bFWriteToFile( QString sFileName, QByteArray tByteArray )
{
 // Création d'un objet QFile
 QFile file( m_sFilePath+sFileName );
 // On ouvre notre fichier en lecture seule et on vérifie l'ouverture
 if (!file.open(QIODevice::WriteOnly))
  {
   qDebug("Error opening file");
   return( ( bool )false );
  }
 file.write( tByteArray, tByteArray.length() );
 file.close();

 // Analyse du numéro de version
 QString sDetectedSoftwareVersion = QString( tByteArray );

 unsigned int uiMajor, uiMinor, uiBuild, uiRevision;
 if( sscanf( sDetectedSoftwareVersion.toLatin1().data(), "%d.%d.%d.%d", &uiMajor, &uiMinor, &uiBuild, &uiRevision ) == 3 )
  {
   qDebug( "HTTPUpdate::bFWriteToFile > detected version no" );
   qDebug( "%d.%d.%d.%d", uiMajor, uiMinor, uiBuild, uiRevision );
  }

 qDebug( "m_sCurrentSoftwareVersion: %s", m_sCurrentSoftwareVersion.toLatin1().data() );
 qDebug( "sDetectedSoftwareVersion: %s", sDetectedSoftwareVersion.toLatin1().data() );

 // Comparaison du numéro de version
 if( m_sCurrentSoftwareVersion != sDetectedSoftwareVersion )
  {
   // On a bien le numéro de version
   ptMainEvent->vFMainWindowUpdateDetectedOpen( tByteArray );
  }


 return( ( bool )true );
}

////------------------------------------------------------------
//// Setter
////------------------------------------------------------------
//// Receive path
//void HTTPUpdate::setDataReceivedPath( QString sFilePath )
//{

//}

//------------------------------------------------------------
// Setter
//------------------------------------------------------------
// Receive path
void HTTPUpdate::setDataReceivedPath( QString sFilePath )
{
 // Stockage du chemin d'enregistrement
 m_sFilePath = sFilePath;
}
// Destination path
void HTTPUpdate::setDestProgramPath( QString sFilePath )
{
 // Stockage du chemin d'enregistrement
 m_sDestProgramPath = sFilePath;
}
// HTTP path
void HTTPUpdate::setHTTPPath( QString sHTTPPath )
{
 // Stockage du chemin d'enregistrement
 m_sHTTPPath = sHTTPPath;
}
// HTTP path
void HTTPUpdate::setCurrentSoftwareVersion( QString sSoftwareVersion )
{
 // Stockage du chemin d'enregistrement
 m_sCurrentSoftwareVersion = sSoftwareVersion;
}
