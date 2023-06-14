#include "SensorUpdateControler.h"
#include "main.h"
#include <QFile>
#include <WIMOClientModBus.h>
#include <WIMOParameters.h>

SensorUpdateControler::SensorUpdateControler(
  SensorUpdateData * ptSensorUpdateData,
  TINIConfig * ptINIConfig,
  QObject* parent ):
 QObject(parent),
 m_ptINIConfig( ptINIConfig ),
 m_ptSensorUpdateData( ptSensorUpdateData )
{
 // Init du chemin de la mise à jour
 //m_sHTTPPath = m_ptINIConfig->tSensorUpdate().sHTTPPath();
 //m_sHTTPPath = m_ptINIConfig->tSensorUpdate_sHTTPPath();
 // Init du chemin de la mise à jour
 m_sHTTPPath = m_ptINIConfig->ptFSensorUpdate()->sHTTPPath();
}

//=============================================================
// Fonction annexe/utilitaire
//=============================================================

//-------------------------------------------------------------
// Ecriture des données en entrée dans un fichier
//-------------------------------------------------------------
static bool bFWriteToFile( QString sFileName, QByteArray tByteArray )
{
 // Création d'un objet QFile
 QFile file( sFileName );
 // On ouvre notre fichier en lecture seule et on vérifie l'ouverture
 if (!file.open(QIODevice::WriteOnly))
  {
   qDebug("Error opening file");
   return( ( bool )false );
  }
 file.write( tByteArray, tByteArray.length() );
 file.close();
 return( ( bool )true );
}

//=============================================================
// Code
//=============================================================

//-------------------------------------------------------------
// Vérification si une mise à jour est dispo
//-------------------------------------------------------------
Q_INVOKABLE void SensorUpdateControler::checkUpdateAvailability( void )
{
 // Vérification des fichiers sur le serveur
 this->startFilelistRequest();
}


//-------------------------------------------------------------
// Requête du fichier liste
//-------------------------------------------------------------
void SensorUpdateControler::startFilelistRequest( void )
{
 // HTTP Path
 m_tHTTPProcess.setHTTPPath( m_sHTTPPath );
 // Nom du fichier qui va être requêté
 //m_sFileName = "list.php";
 m_tHTTPProcess.setFileName( m_sUpdateListFilename );

 // Lancement de la requête de download version
 m_tHTTPProcess.startRequest();
 // Connexion de la réception de la version
 //connect( &m_tHTTPProcess, &GetHTTPFile::dataLoaded, this, &MainProcess::versionLoaded );
 connect( &m_tHTTPProcess, &GetHTTPFile::dataLoaded, this, &SensorUpdateControler::fileListLoaded );
 //qDebug("End MainProcess::startUpdate");
}

//-------------------------------------------------------------
// Le fichier liste a été reçue
//-------------------------------------------------------------
void SensorUpdateControler::fileListLoaded( void )
{
 qDebug("== SensorUpdateControler::fileListLoaded ==");

 // Déconnexion du signal
 disconnect( &m_tHTTPProcess, &GetHTTPFile::dataLoaded, nullptr, nullptr );

 QByteArray tFileList = m_tHTTPProcess.getData();

 // Test si fichier à zéro
 if( !tFileList.length() )
  {
   emit this->noSensorUpdateSignal();
   return;
  }

 qDebug( tFileList );
 //qDebug( "New program version detected: V"+ m_tVersion );
 // Ecriture de la donnée dans un fichier
 bFWriteToFile( "filelist.txt", tFileList );

 QByteArrayList tsFileListObj = tFileList.split('\n');
 //m_sFileName = tFileList.split('\n')[0].split('\t')[2];
 m_sFileName = tFileList.split('\n')[0].split('\t')[2];

 qDebug( "tsFileListObj.size(): %u", tsFileListObj.size() );

 QString sFileNameUpdate;
 quint8 uiFileListIndex;
 QByteArray tqVersion;
 quint16 uiUpdateMajor, uiUpdateMinor, uiUpdateBuild;
 quint16 uiSensorMajor, uiSensorMinor, uiSensorBuild;
 char tcSensorID[16];

 char tcSensorVersion[16];

 sprintf( tcSensorID, "S%02u", tWIMOParametersSensor.ttChannel[0].ucParameter );
 sprintf( tcSensorVersion, "%u.%u.%u", tSensorVersion.ucMajor, tSensorVersion.ucMinor, tSensorVersion.uiBuild );

 // Numéro de version du capteur
 uiSensorMajor = tSensorVersion.ucMajor;
 uiSensorMinor = tSensorVersion.ucMinor;
 uiSensorBuild = tSensorVersion.uiBuild;

 qDebug("tcSensorID = %s", tcSensorID);
 qDebug("tcSensorVersion = %s", tcSensorVersion);

 // Parcourt des fichiers présents sur le serveur
 for( uiFileListIndex = ( quint8 )0; uiFileListIndex < tsFileListObj.size(); uiFileListIndex++ )
  {
   // Si ligne vide
   if( tsFileListObj[uiFileListIndex] == "" )
    {
     break;
    }

   //qDebug( "uiFileListIndex: %u", uiFileListIndex );
   sFileNameUpdate = QString( tsFileListObj[uiFileListIndex].split('\t')[2] );
   //sFileNameDebug = tFileList.split('\n')[0].split('\t')[2];

   // Test si le capteur en cours n'est pas à jour
   //if( struct.config.sensorID ==  )
   //memcmp( tcSensorID, tsFileListObj[uiFileListIndex].split('\t')[0], 3 );
   if( memcmp( tcSensorID, tsFileListObj[uiFileListIndex].split('\t')[0], 3 ) == ( int )0 )
    {
     tqVersion = tsFileListObj[uiFileListIndex].split('\t')[1];
     // Mise à jour
     uiUpdateMajor = tsFileListObj[uiFileListIndex].split('\t')[1].split('.')[0].toUShort();
     uiUpdateMinor = tsFileListObj[uiFileListIndex].split('\t')[1].split('.')[1].toUShort();
     uiUpdateBuild = tsFileListObj[uiFileListIndex].split('\t')[1].split('.')[2].toUShort();
     qDebug( "Find filename: %s", sFileNameUpdate.toLatin1().data() );
     qDebug( "uiUpdateMajor: %u", uiUpdateMajor );
     qDebug( "uiUpdateMinor: %u", uiUpdateMinor );
     qDebug( "uiUpdateBuild: %u", uiUpdateBuild );
     qDebug( "uiSensorMajor: %u", uiSensorMajor );
     qDebug( "uiSensorMinor: %u", uiSensorMinor );
     qDebug( "uiSensorBuild: %u", uiSensorBuild );
     if(  ( uiUpdateMajor > uiSensorMajor )
       || (  ( uiUpdateMajor == uiSensorMajor )
          && ( uiUpdateMinor >  uiSensorMinor ) )
       || (  ( uiUpdateMajor == uiSensorMajor )
          && ( uiUpdateMinor == uiSensorMinor )
          && ( uiUpdateBuild >  uiSensorBuild ) ) )
      {
       qDebug( "Update Needed" );

       // Version de la mise à jour
       m_ptSensorUpdateData->setUpdateVersion(tqVersion);
       m_sFileName = sFileNameUpdate;

       m_ptSensorUpdateData->setSensorInfo( tWIMOParametersSensor.tHeader.tProduct.tcIdentifier );
       m_ptSensorUpdateData->setSensorVersion(tcSensorVersion);
       m_ptSensorUpdateData->setUpdateVersion(tqVersion);
       m_ptSensorUpdateData->setDownloadProgression(0);
       m_ptSensorUpdateData->setUploadProgression(0);

       // Ouverture de la fenêtre de mise à jour
       emit this->openSensorUpdateWindowSignal();
       // On quitte
       return;
      }
    }
  }
 // emit this->
 emit this->noSensorUpdateSignal();
 qDebug( "End of loop" );
}


//-------------------------------------------------------------
// Start bouton mise à jour
//-------------------------------------------------------------
void SensorUpdateControler::startButtonClicked( void )
{
 qDebug("== onStartButtonClicked ==");

 //
 m_ptSensorUpdateData->setUpdateBusy(true);
 //
 m_tHTTPProcess.setHTTPPath( m_sHTTPPath );
 // Récupération
 m_tHTTPProcess.setFileName( m_sFileName );
 // Lancement de la requête de download version
 m_tHTTPProcess.startRequest();
 // Connexion de la réception de la version
 connect( &m_tHTTPProcess, &GetHTTPFile::dataLoaded, this, &SensorUpdateControler::onDownloadFinished );
 // Création de la connexion pour fin de réception de donnée
 connect( &m_tHTTPProcess, &GetHTTPFile::dataLoadProgressSignal, this, &SensorUpdateControler::onDownloadProgress );
}

//-------------------------------------------------------------
// Fin du download du fichier
//-------------------------------------------------------------
extern SensorUpdateControler * ptSensorUpdateControler;
void SensorUpdateControler::onDownloadProgress( qint8 ucDataProgress )
{
 qDebug("== onDataProgress == %d", ucDataProgress);
 m_ptSensorUpdateData->setDownloadProgression( ucDataProgress );
}

//-------------------------------------------------------------
// Fin du download du fichier
//-------------------------------------------------------------
void SensorUpdateControler::onDownloadFinished( void )
{
 qDebug("== onDownloadFinished ==");

 bFWriteToFile( m_sFileName, m_tHTTPProcess.getData() );

 // Déconnexion de la réception de la version
 disconnect( &m_tHTTPProcess, &GetHTTPFile::dataLoaded, this, &SensorUpdateControler::onDownloadFinished );
 // Déconnexion de la connexion pour fin de réception de donnée
 disconnect( &m_tHTTPProcess, &GetHTTPFile::dataLoadProgressSignal, this, &SensorUpdateControler::onDownloadProgress );

 // Connexion de la réception de la version
 connect( this, &SensorUpdateControler::updateProgressSignal, this, &SensorUpdateControler::onUpdateProgress );
 // Connexion de la réception de la version
 connect( this, &SensorUpdateControler::updateEndSignal, this, &SensorUpdateControler::onUpdateFinished );
 // Modbus - Modbus start Update
 vFWIMOModbusStartUpdate( m_sFileName );
 // Clear de l'objet
 //m_tHTTPProcess.disconnect( &m_tHTTPProcess );
 //delete m_ptHTTPProcess;
 //m_ptHTTPProcess->deleteLater();
}

//-------------------------------------------------------------
// Fin upload du fichier
//-------------------------------------------------------------
void SensorUpdateControler::onUpdateFinished( void )
{
 qDebug("== onUploadFinished ==");

 // Déconnexion de la réception de la version
 disconnect( this, &SensorUpdateControler::updateProgressSignal, this, &SensorUpdateControler::onUpdateProgress );
 // Déconnexion de la connexion pour fin de réception de donnée
 disconnect( this, &SensorUpdateControler::updateEndSignal, this, &SensorUpdateControler::onUpdateFinished );
 //
 m_ptSensorUpdateData->setUpdateBusy(false);
 //
 emit sensorUpdateSuccessSignal();
}

//-------------------------------------------------------------
// Progression de l'update capteur
//-------------------------------------------------------------
void SensorUpdateControler::onUpdateProgress( qint8 ucUpdateProgress )
{
 //qDebug("== onDataProgress == %d", ucUpdateProgress);
 m_ptSensorUpdateData->setUploadProgression( ucUpdateProgress );
}
