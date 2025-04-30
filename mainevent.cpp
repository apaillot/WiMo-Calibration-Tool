//============================================================================//
// Fichier .........: "mainevent.cpp"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2020/01/28
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef MAIN_EVENT_CPP
#define MAIN_EVENT_CPP

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include "mainevent.h"
#include <QDebug>
#include "treemodel.h"
#include "main.h"
#include <WIMOParameters.h>
#include <WIMOClientModBus.h>
#include <QTimer>

//============================================================================//
// Déclaration des constantes
//============================================================================//

//============================================================================//
// Déclaration des prototypes de fonctions non exportées
//============================================================================//

//============================================================================//
// Déclaration des types
//============================================================================//

//============================================================================//
// Déclaration des variables exportées
//============================================================================//

//============================================================================//
// Déclaration des variables non exportées
//============================================================================//

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des prototypes de fonctions exportées
//============================================================================//

//****************************************************************************//
// Le code des fonctions
//****************************************************************************//
//----------------------------------------------------------
// Constructeur de la classe
//----------------------------------------------------------
MainEvent::MainEvent( QObject *parent ): QObject(parent)
{

}

//----------------------------------------------------------
// Fonction CPP de mise à jour numéro de version
//----------------------------------------------------------
void MainEvent::vFSetSoftwareVersion( QString sVersion )
{
 qDebug("==vFSetSoftwareVersion== %s", sVersion.toLatin1().data() );
 m_sVersion = sVersion;
 //m_uiUpdateLoaderValue = uiLoaderValue;
 emit setSoftwareVersionSignal( sVersion );
}

//----------------------------------------------------------
// Fonction CPP de mise à jour numéro de version
//----------------------------------------------------------
QString MainEvent::sFGetSoftwareVersion( void )
{
 // Retourne le numéro de version
 return( m_sVersion );
}

//----------------------------------------------------------
// Ouverture de la fenêtre principale
//----------------------------------------------------------
void MainEvent::vFMainWindowOpen( void )
{
 // Lancement du signal
 emit mainWindowOpen();
}

//----------------------------------------------------------
// Ouverture de la fenêtre d'erreur
//----------------------------------------------------------
void MainEvent::vFMainWindowErrorOpen( QString sErrorMsg )
{
 // Lancement du signal
 emit mainWindowErrorOpen( sErrorMsg );
}

//----------------------------------------------------------
// Ouverture de la fenêtre d'erreur
//----------------------------------------------------------
void MainEvent::vFMainWindowInfoOpen( QString sMsg )
{
 // Lancement du signal
 emit mainWindowInfoOpen( sMsg );
}

//----------------------------------------------------------
// Ouverture de la fenêtre de détection de mise à jour
//----------------------------------------------------------
void MainEvent::vFMainWindowUpdateDetectedOpen( QString sNewVersionNumber )
{
 // Lancement du signal
 emit mainWindowUpdateDetectedOpen( sNewVersionNumber );
}

//----------------------------------------------------------
// Ouverture de la fenêtre de détection de mise à jour
//----------------------------------------------------------
Q_INVOKABLE void MainEvent::vFMainWindowStartUpdate( void )
{
 // Lancement du signal
// emit mainWindowUpdateDetectedOpen( );
 //
 qDebug( "!!!START UPDATE!!!!" );
 // Lancement executable UpdateLauncher
 QByteArray tUpdateExePath = "Updater/QtUpdater.exe";

 system( "start "+tUpdateExePath );

 // Fermeture application WiMo
 emit this->quit();
}

//----------------------------------------------------------
// Ouverture de la fenêtre display configuration
//----------------------------------------------------------
Q_INVOKABLE void MainEvent::vFMainEventDisplayParamWindow( void )
{
 qDebug("==MainEvent::vFMainEventDisplayParamWindow===");
 QString sBufferTree2 = "Test\t\t\t\t4";
 QString sBufferTree = "Test\t\t\t\t5";

sBufferTree = sBufferTree.asprintf(
"tSensor\t\t\t\t-\n"
"    tHeader\t\t\t\t-\n"
"        tProduct\t\t\t\t-\n"
"            cActive\t\t\t\t%c\n"
"            tcIdentifier\t\t\t\t%s\n"
"            tcManufacturer\t\t\t\t%s\n"
"            tcVersion\t\t\t\t%s\n"
"            cType\t\t\t\t%d\n"
"            cWiperCompatibility\t\t\t\t%c\n"
"            ucSerialNumberY\t\t\t\t%d\n"
"            ucSerialNumberN\t\t\t\t%d\n",
tWIMOParametersSensor.tHeader.tProduct.cActive,
tWIMOParametersSensor.tHeader.tProduct.tcIdentifier,
tWIMOParametersSensor.tHeader.tProduct.tcManufacturer,
tWIMOParametersSensor.tHeader.tProduct.tcVersion,
tWIMOParametersSensor.tHeader.tProduct.cType,
tWIMOParametersSensor.tHeader.tProduct.cWiperCompatibility,
tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY,
tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN );

sBufferTree += QString::asprintf(
"        tSensor\t\t\t\t-\n"
"            ucChannelNumber\t\t\t\t%d\n"
"            uiPowerOnTime\t\t\t\t%d\n"
"            uiAcquisitionTime\t\t\t\t%d\n",
tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber,
tWIMOParametersSensor.tHeader.tSensor.uiPowerOnTime,
tWIMOParametersSensor.tHeader.tSensor.uiAcquisitionTime );

//-------------
// Tbd / Fluo evosens
//-------------
if(  ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )5  )
  || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )13 )
  || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )15 )
  || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )16 )
  || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )17 )
  || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )18 ) )
 {
  sBufferTree += QString::asprintf(
  "            tEvosens\t\t\t\t-\n"
  "                uiIntegrationTime\t\t\t\t%d\n"
  "                uiIntegrationSum\t\t\t\t%d\n"
  "                ucRange\t\t\t\t%d\n"
  "                uiAverageSamples\t\t\t\t%d\n",
  tWIMOParametersSensor.tHeader.tSensor.tEvosens.uiIntegrationTime,
  tWIMOParametersSensor.tHeader.tSensor.tEvosens.uiIntegrationSum,
  tWIMOParametersSensor.tHeader.tSensor.tEvosens.ucRange,
  tWIMOParametersSensor.tHeader.tSensor.tEvosens.uiAverageSamples );
 }

if( tWIMOParametersSensor.tHeader.tProduct.cType == 'W' )
 {
  sBufferTree += QString::asprintf(
  "        tWiper\t\t\t\t-\n"
  "            uiClockwiseAngle\t\t\t\t%d\n"
  "            uiCounterClockwiseAngle\t\t\t\t%d\n"
  "            cPeriod\t\t\t\t%d\n"
  "            uiParkingAngle\t\t\t\t%d\n"
  "            uiMin\t\t\t\t%d\n"
  "            uiMax\t\t\t\t%d\n"
  "            uiOffset\t\t\t\t%d\n",
  tWIMOParametersSensor.tHeader.tWiper.uiClockwiseAngle,
  tWIMOParametersSensor.tHeader.tWiper.uiCounterClockwiseAngle,
  tWIMOParametersSensor.tHeader.tWiper.cPeriod,
  tWIMOParametersSensor.tHeader.tWiper.uiParkingAngle ,
  tWIMOParametersSensor.tHeader.tWiper.uiMinValue,
  tWIMOParametersSensor.tHeader.tWiper.uiMaxValue,
  tWIMOParametersSensor.tHeader.tWiper.uiOffset );
 }
if( tWIMOParametersSensor.tHeader.tProduct.cType == 'U' )
 {
  sBufferTree += QString::asprintf(
  "        tCleanerUV\t\t\t\t-\n"
  "            cPeriod\t\t\t\t%d\n"
  "            cDuration\t\t\t\t%d\n",
  tWIMOParametersSensor.tHeader.tCleanerUV.cPeriod,
  tWIMOParametersSensor.tHeader.tCleanerUV.cDuration );
 }
 sBufferTree += QString::asprintf(
 "        tConsumption\t\t\t\t-\n"
 "            fSleep_mA\t\t\t\t%f\n"
 "            fRunning_mA\t\t\t\t%f\n",
 tWIMOParametersSensor.tHeader.tConsumption.fSleep_mA,
 tWIMOParametersSensor.tHeader.tConsumption.fRunning_mA );

if( tWIMOParametersSensor.tHeader.tProduct.cType == 'S' )
 {
  // Boucle sur le nombre de voie
  TUINT uiCptChannel;
  TUINT uiCptCalibPoint;
  for( uiCptChannel = ( TUINT )0; uiCptChannel < tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber; uiCptChannel++ )
   {
    sBufferTree += QString::asprintf(
    "        ttChannel[%d]\t\t\t\t-\n"
    "            cActive\t\t\t\t%c\n"
    "            ucParameter\t\t\t\t%d\n"
    "            uiAverage\t\t\t\t%d\n"
    "            tcName\t\t\t\t%s\n"
    "            tcUnit\t\t\t\t%s\n"
    "            cFormat\t\t\t\t%c\n"
    "            ttPointTheo[0].fRaw\t\t\t\t%f\n"
    "            ttPointTheo[0].fPhysical\t\t\t\t%f\n"
    "            ttPointTheo[1].fRaw\t\t\t\t%f\n"
    "            ttPointTheo[1].fPhysical\t\t\t\t%f\n"
    "            cCalibration\t\t\t\t%c\n",
    uiCptChannel,
    tWIMOParametersSensor.ttChannel[uiCptChannel].cActive,
    tWIMOParametersSensor.ttChannel[uiCptChannel].ucParameter,
    tWIMOParametersSensor.ttChannel[uiCptChannel].uiAverage,
    tWIMOParametersSensor.ttChannel[uiCptChannel].tcName,
    tWIMOParametersSensor.ttChannel[uiCptChannel].tcUnit,
    tWIMOParametersSensor.ttChannel[uiCptChannel].cFormat,
    tWIMOParametersSensor.ttChannel[uiCptChannel].ttPointTheo[0].fRaw,
    tWIMOParametersSensor.ttChannel[uiCptChannel].ttPointTheo[0].fPhysical,
    tWIMOParametersSensor.ttChannel[uiCptChannel].ttPointTheo[1].fRaw,
    tWIMOParametersSensor.ttChannel[uiCptChannel].ttPointTheo[1].fPhysical,
    tWIMOParametersSensor.ttChannel[uiCptChannel].cCalibration/*,
    tWIMOParametersSensor.ttChannel[uiCptChannel].cActive,
    tWIMOParametersSensor.ttChannel[uiCptChannel].cActive,
    tWIMOParametersSensor.ttChannel[uiCptChannel].cActive,
    tWIMOParametersSensor.ttChannel[uiCptChannel].cActive,
    tWIMOParametersSensor.ttChannel[uiCptChannel].cActive*/ );

    // Parcourt des points de calibration
    for( uiCptCalibPoint = ( TUINT )0; uiCptCalibPoint < ( TUINT )14; uiCptCalibPoint++ )
     {
      //
      sBufferTree += QString::asprintf(
       "            ttPoints[%d].fPhysical\t\t\t\t%f\n"
       "            ttPoints[%d].fRaw\t\t\t\t%f\n",
       uiCptCalibPoint,
       tWIMOParametersSensor.ttChannel[uiCptChannel].ttPoints[uiCptCalibPoint].fPhysical,
       uiCptCalibPoint,
       tWIMOParametersSensor.ttChannel[uiCptChannel].ttPoints[uiCptCalibPoint].fRaw);
     }
    sBufferTree += QString::asprintf(
     "            uliDateTimeCalibration\t\t\t\t%lu\n",
     tWIMOParametersSensor.ttChannel[uiCptChannel].uliDateTimeCalibration );

    // pH ou ISE
    if(  ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )8 )
      || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )20 )
      || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )21 )
      || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )22 )
      || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )23 )
      || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )24 ) )
     {
      sBufferTree += QString::asprintf(
       "            tPH.fCalibrationTemperature\t\t\t\t%f\n"
       "            tPH.uiAcquisitionTime\t\t\t\t%d\n",
       tWIMOParametersSensor.ttChannel[uiCptChannel].tPH.fCalibrationTemperature,
       tWIMOParametersSensor.ttChannel[uiCptChannel].tPH.uiAcquisitionTime );
     }
    else
     {
      sBufferTree += QString::asprintf(
       "            uiAcquisitionTime\t\t\t\t%d\n",
       tWIMOParametersSensor.ttChannel[uiCptChannel].uiAcquisitionTime );
     }
   }
 }

 //-------------
 // O2
 //-------------
 if(  ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )6 )
   || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )7 ) )
  {
   sBufferTree += QString::asprintf(
   "tWIMOPyroO2Configuration\t\t\t\t-\n"
   "    bExternalTemperature\t\t\t\t%d\n"
   "    liTemperature\t\t\t\t%ld\n"
   "    uliPressure\t\t\t\t%lu\n"
   "    ucSensorType\t\t\t\t%c\n"
   "    ucIntensity\t\t\t\t%c\n"
   "    ucAmp\t\t\t\t%d\n"
   "    uliCodeDphi0\t\t\t\t%lu\n"
   "    uliCodeDphi100\t\t\t\t%lu\n"
   "    liTemperatureOffset\t\t\t\t%ld\n"
   "    uliDummy\t\t\t\t%lu\n"
   "    bCalibrated\t\t\t\t%d\n"
   "    bExternalTemperature100\t\t\t\t%d\n"
   "    liTemperature100\t\t\t\t%ld\n"
   "    uliPressure100\t\t\t\t%lu\n"
   "    bExternalTemperature0\t\t\t\t%d\n"
   "    liTemperature0\t\t\t\t%ld\n",
   tWIMOPyroO2Configuration.bExternalTemperature,
   tWIMOPyroO2Configuration.liTemperature,
   tWIMOPyroO2Configuration.uliPressure,
   tWIMOPyroO2Configuration.ucSensorType,
   tWIMOPyroO2Configuration.ucIntensity,
   tWIMOPyroO2Configuration.ucAmp,
   tWIMOPyroO2Configuration.uliCodeDphi0,
   tWIMOPyroO2Configuration.uliCodeDphi100,
   tWIMOPyroO2Configuration.liTemperatureOffset,
   tWIMOPyroO2Configuration.uliDummy,
   tWIMOPyroO2Configuration.bCalibrated,
   tWIMOPyroO2Configuration.bExternalTemperature100,
   tWIMOPyroO2Configuration.liTemperature100,
   tWIMOPyroO2Configuration.uliPressure100,
   tWIMOPyroO2Configuration.bExternalTemperature0,
   tWIMOPyroO2Configuration.liTemperature0 );
  }


 //-------------
 // CT
 //-------------
 if(  ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )4 )
   || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )3 ) )
  {
   sBufferTree += QString::asprintf(
   "tDataCTSensor\t\t\t\t-\n"
   "    tCompensation.fA1\t\t\t\t%f\n"
   "    tCompensation.fA2\t\t\t\t%f\n"
   "    tCompensation.fA3\t\t\t\t%f\n"
   "    tCompensation.fRaw20\t\t\t\t%f\n",
   tWIMOParametersSensor.tHeader.tCT.tCompensation.fA1,
   tWIMOParametersSensor.tHeader.tCT.tCompensation.fA2,
   tWIMOParametersSensor.tHeader.tCT.tCompensation.fA3,
   tWIMOParametersSensor.tHeader.tCT.tCompensation.fRaw20 );
   sBufferTree += QString::asprintf(
   "ptDataSensorCT\t\t\t\t-\n"
   "    fA1\t\t\t\t%s\n"
   "    fA2\t\t\t\t%s\n"
   "    fA3\t\t\t\t%s\n"
   "    fRaw20\t\t\t\t%s\n",
   ptDataSensorCT->getTCompensationA().data(),
   ptDataSensorCT->getTCompensationB().data(),
   ptDataSensorCT->getTCompensationC().data(),
   ptDataSensorCT->getTCompensationRaw20().data() );
  }

 //ptParamTreeModelEmpty->updateData("Test3");
 //ptContext->setContextProperty("tParamTreeModel", ptParamTreeModelEmpty);
 // Mise à jour de la donnée
 //ptParamTreeModel->updateData("Test4");
 ptParamTreeModel->updateData(sBufferTree);
 //ptContext->setContextProperty("tParamTreeModel", ptParamTreeModelEmpty);

 ptContext->setContextProperty("tParamTreeModel", ptParamTreeModel);

 // Appel du constructeur de la classe QFile
 QFile file("config.txt");

 if (file.open(QIODevice::WriteOnly | QIODevice::Text) )
  {
   // Si l'ouverture du fichier en écriture à réussie
    //QByteArray tByteArray( sBufferTree );
   // écrire dans le fichier
   file.write(sBufferTree.toLatin1().data());

   // Fermer le fichier
   file.close();
  }
}

//----------------------------------------------------------------------------//
// Effacement de la fenêtre parameter
//----------------------------------------------------------------------------//
Q_INVOKABLE void MainEvent::clearParamWindows( void )
{
 ptContext->setContextProperty("tParamTreeModel", ptParamTreeModelEmpty);
}

//----------------------------------------------------------------------------//
// Test de l'état du grafcet modbus
//----------------------------------------------------------------------------//
Q_INVOKABLE bool MainEvent::isModbusBusyState( void )
{
 // Retourne
 return( bIsWIMOModbusGrafcetBusy() );
}

//----------------------------------------------------------
// Get de la configuration global
//----------------------------------------------------------
Q_INVOKABLE void MainEvent::getSensorConfiguration( void )
{
 qDebug("--getSensorConfiguration--");
 //qDebug("uiChannel = %d",uiChannel);
 // Lecture capteur
 //vFWIMOModbusGetChannelFromSensor( ( TUCHAR )uiChannel );
}

//----------------------------------------------------------
// Sauvegarde de la configuration dde la configuration général vers le capteur
//----------------------------------------------------------
Q_INVOKABLE void MainEvent::saveSensorConfiguration( void )
{
 qDebug("saveSensorConfiguration");
 // Ecriture capteur
 //vFWIMOModbusSaveChannelToSensor( ( TUCHAR )uiChannel );
}

//----------------------------------------------------------
// Get de la configuration d'une voie vers le capteur
//----------------------------------------------------------
Q_INVOKABLE void MainEvent::getChannelConfiguration( unsigned int uiChannel )
{
 qDebug("--getChannelConfiguration--");
 qDebug("uiChannel = %d",uiChannel);
 // Lecture capteur
 vFWIMOModbusGetChannelFromSensor( ( TUCHAR )uiChannel );
}

//----------------------------------------------------------
// Sauvegarde de la configuration d'une voie vers le capteur
//----------------------------------------------------------
Q_INVOKABLE void MainEvent::saveChannelConfiguration( unsigned int uiChannel )
{
 qDebug("uiChannel = %d",uiChannel);
 // Ecriture capteur
 vFWIMOModbusSaveChannelToSensor( ( TUCHAR )uiChannel );
}

//----------------------------------------------------------
// Stop mesure liveview init
//----------------------------------------------------------
Q_INVOKABLE bool MainEvent::isChannelActivated( int uiChannelIndex )
{
 // Intégrité : pas supérieur au nombre max de voie
 if( uiChannelIndex >= tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber )
  // Faux
  return( ( bool )false );
 // Retourne les résultats
 return( tWIMOParametersSensor.ttChannel[uiChannelIndex].cActive == ( TCHAR )'1' );
}

//----------------------------------------------------------
// Stop mesure liveview init
//----------------------------------------------------------
Q_INVOKABLE void MainEvent::setChannelActivation( int uiChannelIndex, bool bActivation )
{
 // Intégrité : pas supérieur au nombre max de voie
 if( uiChannelIndex >= tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber )
  // Erreur
  return;
 if( bActivation )
  tWIMOParametersSensor.ttChannel[uiChannelIndex].cActive = ( TCHAR )'1';
 else
  tWIMOParametersSensor.ttChannel[uiChannelIndex].cActive = ( TCHAR )'0';
}

//----------------------------------------------------------
// Clear data dans la fenêtre principale
//----------------------------------------------------------
void MainEvent::vFMain_ClearData( void )
{
 emit vFMain_ClearDataSignal();
}

//----------------------------------------------------------
// Get pH calibration temperature
//----------------------------------------------------------
Q_INVOKABLE float MainEvent::getPHCalibrationTemperature( void )
{
 return( ( TFLOAT )tWIMOParametersSensor.ttChannel[0].tPH.fCalibrationTemperature );
}

//----------------------------------------------------------
// Set pH calibration temperature
//----------------------------------------------------------
Q_INVOKABLE void MainEvent::setPHCalibrationTemperature( float fCalibrationTemperature )
{
 // Ecrit la valeur du pH
 tWIMOParametersSensor.ttChannel[0].tPH.fCalibrationTemperature = fCalibrationTemperature;
}

//----------------------------------------------------------
// Display screen
//----------------------------------------------------------
Q_INVOKABLE void MainEvent::displayScreen( void )
{
 // Fonctionnel
 m_splash = new QSplashScreen;
 m_splash->setPixmap(QPixmap(":/Logo/SplashScreen/Splash_screen_little.jpg"));
 m_splash->show();
 // Maintient 2 secondes du splash screen
 QTimer::singleShot(2000, m_splash, &QWidget::close);
}

//----------------------------------------------------------
// OUverture explorateur Windows
//----------------------------------------------------------
Q_INVOKABLE void MainEvent::openWindowsExplorer( void )
{
 system("start ..\\Measure");
 /*
 // Fonctionnel
 m_splash = new QSplashScreen;
 m_splash->setPixmap(QPixmap(":/Logo/SplashScreen/Splash_screen_little.jpg"));
 m_splash->show();
 // Maintient 2 secondes du splash screen
 QTimer::singleShot(2000, m_splash, &QWidget::close);
 */
}

//----------------------------------------------------------
// Start acquisition premier échantillon
//----------------------------------------------------------
Q_INVOKABLE void MainEvent::beginAcquisition( void )
{
 tAcquisitionStartTime = QDateTime();
 //tAcquisitionStartTime.set ;
 qDebug() << "Start time is: " << tAcquisitionStartTime;
}

#endif

