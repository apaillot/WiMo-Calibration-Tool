

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include "InterfaceUpdateControler.h"
#include "main.h"
#include <QFile>
#include <WIMOClientModBus.h>
#include <WIMOParameters.h>
#include <ModbusControler.h>
#include <MainData.h>

//============================================================================//
// Déclaration des prototypes de fonctions non exportées
//============================================================================//

//****************************************************************************//
// Le code des fonctions
//****************************************************************************//

//----------------------------------------------------------------------------//
// Constructeur
//----------------------------------------------------------------------------//
InterfaceUpdateControler::InterfaceUpdateControler(
  QSerialPort *ptSerialPort,
  InterfaceUpdateData * ptInterfaceUpdateData,
  MainData * ptMainData,
  TINIConfig * ptINIConfig,
  QObject* parent ):
 QObject(parent),
 m_ptInterfaceUpdateData( ptInterfaceUpdateData ),
 m_ptMainData( ptMainData ),
 m_ptINIConfig( ptINIConfig )
{
 // Controler modbus mise à jour interface
 m_ptModbusInterfaceUpdateControler = new ModbusInterfaceUpdateControler( ptSerialPort );
 // Init du chemin de la mise à jour
 m_sHTTPPath = m_ptINIConfig->ptFInterfaceUpdate()->sHTTPPath();
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
Q_INVOKABLE void InterfaceUpdateControler::checkUpdateAvailability( bool bUpdateCheckUser )
{
 this->m_bUpdateCheckUser = bUpdateCheckUser;
 // Demande préliminaire du numéro de version
 this->startInterfaceVersionCheck();
}

//-------------------------------------------------------------
// Requête de la version de l'interface USB
//-------------------------------------------------------------
void InterfaceUpdateControler::startInterfaceVersionCheck( void )
{
 // Lancement de la requête modbus pour le numéro de version
 ptModbusControler->requestInterfaceVersionNumber();
 // Signal de fin de requête
 connect( ptModbusControler, &ModbusControler::requestInterfaceVersionNumberEndSignal, this, &InterfaceUpdateControler::onRequestVersionNumberEnd );
}

//-------------------------------------------------------------
// Fin de la requête de la version de l'interface USB
//-------------------------------------------------------------
void InterfaceUpdateControler::onRequestVersionNumberEnd( void )
{
 // Signal de fin de requête
 disconnect( ptModbusControler, &ModbusControler::requestInterfaceVersionNumberEndSignal, this, &InterfaceUpdateControler::onRequestVersionNumberEnd );
 // Démarrage de la requête de la liste des fichiers de mise à jours
 this->startFilelistRequest();
}

//-------------------------------------------------------------
// Requête du fichier liste
//-------------------------------------------------------------
void InterfaceUpdateControler::startFilelistRequest( void )
{
 // HTTP Path
 m_tHTTPProcess.setHTTPPath( m_sHTTPPath );
 // Nom du fichier qui va être requêté
 //m_sFileName = "list.php";
 //m_tHTTPProcess.setFileName( m_sFileName );
 m_tHTTPProcess.setFileName( m_sUpdateListFilename );
 // Connexion de la réception de la version
 connect( &m_tHTTPProcess, &GetHTTPFile::dataLoaded, this, &InterfaceUpdateControler::fileListLoaded );
 // Lancement de la requête de download version
 m_tHTTPProcess.startRequest();
}

//-------------------------------------------------------------
// Le fichier liste a été reçue
//-------------------------------------------------------------
void InterfaceUpdateControler::fileListLoaded( void )
{
 qDebug("== InterfaceUpdateControler::fileListLoaded ==");

 // Déconnexion du signal
 disconnect( &m_tHTTPProcess, &GetHTTPFile::dataLoaded, nullptr, nullptr );

 QByteArray tFileList = m_tHTTPProcess.getData();

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
 quint16 uiInterfaceMajor, uiInterfaceMinor, uiInterfaceBuild;
 ///char tcInterfaceID[16];

 char tcInterfaceVersion[16];

 ///sprintf( tcInterfaceID, "S%02u", tWIMOParametersSensor.ttChannel[0].ucParameter );
 sprintf( tcInterfaceVersion, "%u.%u.%u", tSensorVersion.ucMajor, tSensorVersion.ucMinor, tSensorVersion.uiBuild );


// ptInterfaceUpdateData->getUpdateVersion()


 // Numéro de version du capteur
 uiInterfaceMajor = ( quint16 )m_ptMainData->getInterfaceVersionMajor();
 uiInterfaceMinor = ( quint16 )m_ptMainData->getInterfaceVersionMinor();
 uiInterfaceBuild = ( quint16 )m_ptMainData->getInterfaceVersionBuild();

 // Parcourt des fichiers présents sur le serveur
 for( uiFileListIndex = ( quint8 )0; uiFileListIndex < tsFileListObj.size(); uiFileListIndex++ )
  {
   // Si ligne vide
   if( tsFileListObj[uiFileListIndex] == "" )
    {
     break;
    }

   //qDebug( "uiFileListIndex: %u", uiFileListIndex );
   sFileNameUpdate = QString( tsFileListObj[uiFileListIndex].split('\t')[1] );
   //sFileNameDebug = tFileList.split('\n')[0].split('\t')[2];

   // Test si le capteur en cours n'est pas à jour
   //if( struct.config.sensorID ==  )
   //memcmp( tcInterfaceID, tsFileListObj[uiFileListIndex].split('\t')[0], 3 );
   //if( memcmp( tcInterfaceID, tsFileListObj[uiFileListIndex].split('\t')[0], 3 ) == ( int )0 )
   // {
   tqVersion = tsFileListObj[uiFileListIndex].split('\t')[0];
   qDebug() << "tqVersion: " + tqVersion;
   // Mise à jour
   uiUpdateMajor = tsFileListObj[uiFileListIndex].split('\t')[0].split('.')[0].toUShort();
   uiUpdateMinor = tsFileListObj[uiFileListIndex].split('\t')[0].split('.')[1].toUShort();
   uiUpdateBuild = tsFileListObj[uiFileListIndex].split('\t')[0].split('.')[2].toUShort();
   qDebug( "Find filename: %s", sFileNameUpdate.toLatin1().data() );
   qDebug( "uiUpdateMajor: %u", uiUpdateMajor );
   qDebug( "uiUpdateMinor: %u", uiUpdateMinor );
   qDebug( "uiUpdateBuild: %u", uiUpdateBuild );
   qDebug( "uiInterfaceMajor: %u", uiUpdateMajor );
   qDebug( "uiInterfaceMinor: %u", uiInterfaceMinor );
   qDebug( "uiInterfaceBuild: %u", uiInterfaceBuild );
   if(  ( uiUpdateMajor > uiInterfaceMajor )
     || (  ( uiUpdateMajor == uiInterfaceMajor )
        && ( uiUpdateMinor >  uiInterfaceMinor ) )
     || (  ( uiUpdateMajor == uiInterfaceMajor )
        && ( uiUpdateMinor == uiInterfaceMinor )
        && ( uiUpdateBuild >  uiInterfaceBuild ) ) )
    {
     qDebug( "Update Needed" );

     // Version de la mise à jour
     m_ptInterfaceUpdateData->setUpdateVersion(tqVersion);
     m_sFileName = sFileNameUpdate;

     m_ptInterfaceUpdateData->setInterfaceInfo( tWIMOParametersSensor.tHeader.tProduct.tcIdentifier );
     m_ptInterfaceUpdateData->setInterfaceVersion(tcInterfaceVersion);
     m_ptInterfaceUpdateData->setUpdateVersion(tqVersion);
     m_ptInterfaceUpdateData->setDownloadProgression(0);
     m_ptInterfaceUpdateData->setUploadProgression(0);

     // Ouverture de la fenêtre de mise à jour
     emit this->openInterfaceUpdateWindowSignal();
     // On quitte
     return;
    }
    //}
  }
 // emit this->
 if( this->m_bUpdateCheckUser ) emit this->noInterfaceUpdateSignal();
 qDebug( "End of loop" );
}

//-------------------------------------------------------------
// Start bouton mise à jour
//-------------------------------------------------------------
Q_INVOKABLE void InterfaceUpdateControler::startButtonClicked( void )
{
 qDebug("== onStartButtonClicked ==");

 //
 m_ptInterfaceUpdateData->setUpdateBusy(true);
 //
 m_tHTTPProcess.setHTTPPath( m_sHTTPPath );
 // Récupération
 m_tHTTPProcess.setFileName( m_sFileName );
 // Lancement de la requête de download version
 m_tHTTPProcess.startRequest();
 // Connexion de la réception de la version
 connect( &m_tHTTPProcess, &GetHTTPFile::dataLoaded, this, &InterfaceUpdateControler::onDownloadFinished );
 // Création de la connexion pour fin de réception de donnée
 connect( &m_tHTTPProcess, &GetHTTPFile::dataLoadProgressSignal, this, &InterfaceUpdateControler::onDownloadProgress );
}

//-------------------------------------------------------------
// Fin du download du fichier
//-------------------------------------------------------------
extern InterfaceUpdateControler * ptInterfaceUpdateControler;
void InterfaceUpdateControler::onDownloadProgress( qint8 ucDataProgress )
{
 qDebug("== onDataProgress == %d", ucDataProgress);
 m_ptInterfaceUpdateData->setDownloadProgression( ucDataProgress );
}

//-------------------------------------------------------------
// Fin du download du fichier
//-------------------------------------------------------------
void InterfaceUpdateControler::onDownloadFinished( void )
{
 qDebug("== onDownloadFinished ==");

 bFWriteToFile( m_sFileName, m_tHTTPProcess.getData() );

 // Déconnexion de la réception de la version
 disconnect( &m_tHTTPProcess, &GetHTTPFile::dataLoaded, this, &InterfaceUpdateControler::onDownloadFinished );
 // Déconnexion de la connexion pour fin de réception de donnée
 disconnect( &m_tHTTPProcess, &GetHTTPFile::dataLoadProgressSignal, this, &InterfaceUpdateControler::onDownloadProgress );

 // Connexion progression mise à jour
 connect( m_ptModbusInterfaceUpdateControler, &ModbusInterfaceUpdateControler::updateProgressSignal, this, &InterfaceUpdateControler::onUpdateProgress );
 // Connexion signal de fin de mise à jour
 connect( m_ptModbusInterfaceUpdateControler, &ModbusInterfaceUpdateControler::updateEndSignal, this, &InterfaceUpdateControler::onUpdateFinished );
 // Nom du fichier de mise à jour
 m_ptModbusInterfaceUpdateControler->setUpdateInterfaceFileName( m_sFileName );
 // Mise à jour manuel
 this->bManualUpdate = false;
 // Start update de l'interface
 m_ptModbusInterfaceUpdateControler->startUpdateInterface();
 // Modbus - Modbus start Update
 //vFWIMOModbusStartUpdate( m_sFileName );
 // Clear de l'objet
 //m_tHTTPProcess.disconnect( &m_tHTTPProcess );
 //delete m_ptHTTPProcess;
 //m_ptHTTPProcess->deleteLater();
}

//-------------------------------------------------------------
// Start mise à jour avec fichier
//-------------------------------------------------------------
Q_INVOKABLE void InterfaceUpdateControler::startManualUpdate( QString sFileName )
{
 qDebug( "== startManualUpdate ==" );

 qDebug() << "FileName : " << sFileName ;
 qDebug( "%s", sFileName.data() );

 // Connexion progression mise à jour
 connect( m_ptModbusInterfaceUpdateControler, &ModbusInterfaceUpdateControler::updateProgressSignal, this, &InterfaceUpdateControler::onUpdateProgress );
 // Connexion signal de fin de mise à jour
 connect( m_ptModbusInterfaceUpdateControler, &ModbusInterfaceUpdateControler::updateEndSignal, this, &InterfaceUpdateControler::onUpdateFinished );

 // Mise à jour manuel
 this->bManualUpdate = true;
 // Lancement de la mise à jour
 m_ptModbusInterfaceUpdateControler->setUpdateInterfaceFileName( sFileName );
 // Start update de l'interface
 m_ptModbusInterfaceUpdateControler->startUpdateInterface();
}

//-------------------------------------------------------------
// Progression de l'update capteur
//-------------------------------------------------------------
void InterfaceUpdateControler::onUpdateProgress( qint8 ucUpdateProgress )
{
 //qDebug("== onDataProgress == %d", ucUpdateProgress);
 m_ptInterfaceUpdateData->setUploadProgression( ucUpdateProgress );
}

//-------------------------------------------------------------
// Fin upload du fichier
//-------------------------------------------------------------
void InterfaceUpdateControler::onUpdateFinished( void )
{
 qDebug("== onUploadFinished ==");

 // Déconnexion progression mise à jour
 disconnect( m_ptModbusInterfaceUpdateControler, &ModbusInterfaceUpdateControler::updateProgressSignal, this, &InterfaceUpdateControler::onUpdateProgress );
 // Déconnexion signal de fin de mise à jour
 disconnect( m_ptModbusInterfaceUpdateControler, &ModbusInterfaceUpdateControler::updateEndSignal, this, &InterfaceUpdateControler::onUpdateFinished );

 // Requête - Interface version
 connect( ptModbusControler, &ModbusControler::requestInterfaceVersionNumberEndSignal, this, &InterfaceUpdateControler::onUpdateVersionRequestFinished );
 // Requête du nouveau numéro de version
 ptModbusControler->requestInterfaceVersionNumber();
}

//-------------------------------------------------------------
// Demande du numéro de version après mise à jour
//-------------------------------------------------------------
void InterfaceUpdateControler::onUpdateVersionRequestFinished( void )
{
 // Fin de mise à jour
 // Requête - Interface version
 disconnect( ptModbusControler, &ModbusControler::requestInterfaceVersionNumberEndSignal, this, &InterfaceUpdateControler::onUpdateVersionRequestFinished );
 //
 m_ptInterfaceUpdateData->setUpdateBusy(false);
 // Réinit
 m_ptInterfaceUpdateData->setDownloadProgression( 0 );
 m_ptInterfaceUpdateData->setUploadProgression( 0 );
 // Si mise à jour manuel
 if( this->bManualUpdate )
  emit manualUpdateSuccessSignal();
 else
  emit updateSuccessSignal();
}
