//============================================================================//
// Fichier .........: "factoryevent.cpp"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2020/01/28
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef O2_EVENT_CPP
#define O2_EVENT_CPP

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include "o2event.h"
#include <WIMOClientModBus.h>
#include <WIMOParameters.h>
#include "main.h"
#include <WIMOClientModBus.h>

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
O2Event::O2Event( QObject *parent ): QObject(parent)
{

}

//----------------------------------------------------------
// Envoi de la commande passage en mode calibration
//----------------------------------------------------------
void O2Event::sendSwitchToCalibrationModeCMD( void )
{
 // Lancement de la commande
 vFWIMOModbusO2SwitchToCalibrationMode();
}

//----------------------------------------------------------
// Envoi de la commande pour lecture configuration dans le capteur
//----------------------------------------------------------
void O2Event::sendReadCMD( void )
{
 // Lancement de la commande
 vFWIMOModbus_O2_sendReadCMD();
}

//----------------------------------------------------------
// Envoi de la commande pour lecture sensor offset dans le capteur
//----------------------------------------------------------
void O2Event::sendReadTemperatureOffsetCMD( void )
{
 // Lancement de la commande
 vFWIMOModbus_O2_sendReadTemperatureOffsetCMD();
}

//----------------------------------------------------------
// Envoi de la commande pour écriture sensor offset dans le capteur
//----------------------------------------------------------
void O2Event::sendWriteTemperatureOffsetCMD( void )
{
 // Lancement de la commande
 vFWIMOModbus_O2_sendWriteTemperatureOffsetCMD();
}

//----------------------------------------------------------
// Envoi de la commande pour lecture configuration dans le capteur
//----------------------------------------------------------
void O2Event::readConfFromI2CSensor( void )
{
 // Lancement de la commande
 vFWIMOModbus_O2_ReadConfFromI2CSensor();
}

//----------------------------------------------------------
// Fonction CPP d'affichage de la config
//----------------------------------------------------------
void O2Event::configO2Display( void )
{

 // Sauvegarde de la config
 emit configO2DisplaySignal( tWIMOPyroO2Configuration.bExternalTemperature,
                             tWIMOPyroO2Configuration.liTemperature,
                             tWIMOPyroO2Configuration.uliPressure,
                             // SC7-540-200
                             tWIMOPyroO2Configuration.ucSensorType,
                             tWIMOPyroO2Configuration.ucIntensity,
                             ( tWIMOPyroO2Configuration.ucAmp + ( unsigned char )1),
                             // Phase shift with sensor code
                             tWIMOPyroO2Configuration.uliCodeDphi0,
                             tWIMOPyroO2Configuration.uliCodeDphi100,
                             // Temperature offset
                             tWIMOPyroO2Configuration.liTemperatureOffset,
                             tWIMOPyroO2Configuration.uliDummy,
                             // Etat de la calibration
                             tWIMOPyroO2Configuration.bCalibrated,
                             // Calibration
                             tWIMOPyroO2Configuration.bExternalTemperature0,
                             tWIMOPyroO2Configuration.liTemperature0,
                             tWIMOPyroO2Configuration.bExternalTemperature100,
                             tWIMOPyroO2Configuration.liTemperature100,
                             tWIMOPyroO2Configuration.uliPressure100 );
}

//----------------------------------------------------------
// Enregistrement des settings - condition in the sample
//----------------------------------------------------------
void O2Event::setFactoryModeSensorCode(
  // SC7-540-200
  QChar ucSensorType,
  QChar ucIntensity,
  unsigned char ucAmp,
  // Phase shift with sensor code
  unsigned int uliCodeDphi0,
  unsigned int uliCodeDphi100 )
{
 qDebug( "== setFactoryModeSensorCode ==" );
 // Mise à jour des points
 tWIMOPyroO2Configuration.ucSensorType = ( TUCHAR )ucSensorType.toLatin1();
 tWIMOPyroO2Configuration.ucIntensity  = ( TUCHAR )ucIntensity.toLatin1();
 tWIMOPyroO2Configuration.ucAmp        = ( ucAmp - ( unsigned char)1 );

 tWIMOPyroO2Configuration.uliCodeDphi0   = uliCodeDphi0;
 tWIMOPyroO2Configuration.uliCodeDphi100 = uliCodeDphi100;
 // Mode factory activé | 100%/0% désactivé
 tWIMOPyroO2Configuration.bCalibrated = false;

 qDebug("tWIMOPyroO2Configuration.ucSensorType = %c", tWIMOPyroO2Configuration.ucSensorType );
 qDebug("tWIMOPyroO2Configuration.ucIntensity  = %c", tWIMOPyroO2Configuration.ucIntensity );
 qDebug("tWIMOPyroO2Configuration.ucAmp        = %u", tWIMOPyroO2Configuration.ucAmp );
 qDebug("tWIMOPyroO2Configuration.uliCodeDphi0   = %lu", tWIMOPyroO2Configuration.uliCodeDphi0 );
 qDebug("tWIMOPyroO2Configuration.uliCodeDphi100 = %lu", tWIMOPyroO2Configuration.uliCodeDphi100 );
}

//----------------------------------------------------------
// Enregistrement du sensor code en mode 2 points
//----------------------------------------------------------
void O2Event::set2PointsModeSensorCode(
  // SC7-540-200
  QChar ucSensorType,
  QChar ucIntensity,
  unsigned char ucAmp )
{
 qDebug( "== set2PointsModeSensorCode ==" );
 // Mise à jour des points
 tWIMOPyroO2Configuration.ucSensorType = ( TUCHAR )ucSensorType.toLatin1();
 tWIMOPyroO2Configuration.ucIntensity  = ( TUCHAR )ucIntensity.toLatin1();
 tWIMOPyroO2Configuration.ucAmp        = ( ucAmp - ( unsigned char)1 );
 // Mode factory désactivé | 100%/0% activé
 tWIMOPyroO2Configuration.bCalibrated = true;
}

//----------------------------------------------------------
// Enregistrement condition in the sample
//----------------------------------------------------------
void O2Event::setConditionInTheSample(
  // Calibration
  bool bExternalTemperature,
  int  liTemperature,
  unsigned int uliPressure )
{
 qDebug( "== setConditionInTheSample ==" );
 qDebug( "uliPressure = %lu", uliPressure );
 // Mise à jour des points
 tWIMOPyroO2Configuration.bExternalTemperature = bExternalTemperature;
 if( tWIMOPyroO2Configuration.bExternalTemperature )
  tWIMOPyroO2Configuration.liTemperature = ( TLONGINT )-300000;
 else
  tWIMOPyroO2Configuration.liTemperature       = liTemperature;
 tWIMOPyroO2Configuration.uliPressure          = uliPressure;
 /*
 // Mise à jour des points
 tWIMOPyroO2Configuration.bExternalTemperature100 = bExternalTemperature;
 tWIMOPyroO2Configuration.liTemperature100        = liTemperature;
 tWIMOPyroO2Configuration.uliPressure100          = uliPressure;
 tWIMOPyroO2Configuration.bExternalTemperature0   = bExternalTemperature;
 tWIMOPyroO2Configuration.liTemperature0          = liTemperature;
 */
 //%%AP - 2021.04.15 - Prends la pression
 //tWIMOPyroO2Configuration.bExternalTemperature100 = ( TBOOL )aFALSE;
 //tWIMOPyroO2Configuration.liTemperature100        = ( TLONGINT )20000;
 //tWIMOPyroO2Configuration.uliPressure100          = uliPressure;
 //tWIMOPyroO2Configuration.bExternalTemperature0   = ( TBOOL )aFALSE;
 //tWIMOPyroO2Configuration.liTemperature0          = ( TLONGINT )20000;
}

//----------------------------------------------------------
// Enregistrement condition in the sample
//----------------------------------------------------------
void O2Event::setTemperatureOffset(
  int liTemperatureOffset )
{
 qDebug( "== setTemperatureOffset ==" );
 // Mise à jour des points
 tWIMOPyroO2Configuration.liTemperatureOffset = liTemperatureOffset;
}

//----------------------------------------------------------
// Fonction CPP d'affichage de la config
//----------------------------------------------------------
void O2Event::setCalibPointAirManually( unsigned int uliDphi100 )
{
 // Modification des points
 tWIMOPyroO2Configuration.uliCodeDphi100 = uliDphi100;
}
//----------------------------------------------------------
// Fonction CPP d'affichage de la config
//----------------------------------------------------------
void O2Event::setCalibPoint0PManually( unsigned int uliDphi0 )
{
 // Modification des points
 tWIMOPyroO2Configuration.uliCodeDphi0   = uliDphi0;
}
//----------------------------------------------------------
// Fonction CPP d'affichage de la config
//----------------------------------------------------------
void O2Event::setCalibPointManually( unsigned int uliDphi0, unsigned int uliDphi100 )
{
 // Modification des points
 tWIMOPyroO2Configuration.uliCodeDphi0   = uliDphi0;
 tWIMOPyroO2Configuration.uliCodeDphi100 = uliDphi100;
}

//----------------------------------------------------------
// Enregistrement de la température - set air calibration
//----------------------------------------------------------
void O2Event::setAirCalibrationTemperature( int liTemperature100 )
{
 qDebug( "== setAirCalibrationTemperature ==" );
 // Mise à jour des points
 tWIMOPyroO2Configuration.liTemperature100 = liTemperature100;

 qDebug( "tWIMOPyroO2Configuration.liTemperature100 : %ld", tWIMOPyroO2Configuration.liTemperature100 );
}

//----------------------------------------------------------
// Enregistrement de la température - set air calibration
//----------------------------------------------------------
void O2Event::setAirCalibrationPressure( int uliPressure100 )
{
 qDebug( "== setAirCalibrationPressure ==" );
 // Mise à jour des points
 tWIMOPyroO2Configuration.uliPressure100 = ( unsigned long int )uliPressure100;
}

//----------------------------------------------------------
// Enregistrement des settings - set air calibration
//----------------------------------------------------------
void O2Event::setAirCalibrationSettings(
  bool bExternalTemperature100,
  int  liTemperature100,
  unsigned int uliPressure100 )
{
 qDebug( "== setAirCalibration ==" );
 // Mise à jour des points
 tWIMOPyroO2Configuration.liTemperature100 = liTemperature100;
 tWIMOPyroO2Configuration.uliPressure100   = uliPressure100;
 tWIMOPyroO2Configuration.bExternalTemperature100 = bExternalTemperature100;

 qDebug( "tWIMOPyroO2Configuration.liTemperature100 : %ld", tWIMOPyroO2Configuration.liTemperature100 );
 qDebug( "tWIMOPyroO2Configuration.uliPressure100 : %lu", tWIMOPyroO2Configuration.uliPressure100 );
}

//----------------------------------------------------------
// Enregistrement de la température - set 0 calibration
//----------------------------------------------------------
void O2Event::set0CalibrationTemperature( int liTemperature0 )
{
 qDebug( "== set0CalibrationTemperature ==" );
 // Mise à jour des points
 tWIMOPyroO2Configuration.liTemperature0 = liTemperature0;

 qDebug( "tWIMOPyroO2Configuration.liTemperature0 : %ld", tWIMOPyroO2Configuration.liTemperature0 );
}

//----------------------------------------------------------
// Enregistrement de settings - Set 0% calibration
//----------------------------------------------------------
void O2Event::set0PCalibrationSettings(
  bool bExternalTemperature0,
  int  liTemperature0 )
{
 qDebug( "== set0PCalibration ==" );
 // Mise à jour des points
 tWIMOPyroO2Configuration.liTemperature0        = liTemperature0;
 tWIMOPyroO2Configuration.bExternalTemperature0 = bExternalTemperature0;

 qDebug( "tWIMOPyroO2Configuration.liTemperature0 : %ld", tWIMOPyroO2Configuration.liTemperature0 );
}

//----------------------------------------------------------
// Enregistrement des settings - Envoi de la configuration vers l'interface I2C
//----------------------------------------------------------
void O2Event::sendConfToI2CSensor( void )
{
 // Envoi de la commande d'envoi des paramètres
 vFWIMOModbus_O2_sendConfToI2CSensor();
}
//----------------------------------------------------------
// Enregistrement des settings - Envoi de la commande modbus
//----------------------------------------------------------
void O2Event::sendConfigWriteCMD( void )
{
 // Envoi de la commande d'envoi des paramètres
 vFWIMOModbus_O2_sendConfigWriteCMD();
}

//----------------------------------------------------------
// Enregistrement des settings - condition in the sample
//----------------------------------------------------------
/*
void O2Event::setSensorSettings(
  // SC7-540-200
  QChar ucSensorType,
  QChar ucIntensity,
  unsigned char ucAmp,
  // Phase shift with sensor code
  unsigned int uliCodeDphi0,
  unsigned int uliCodeDphi100,
  // Calibration
  bool bExternalTemperature,
  int  liTemperature,
  unsigned int uliPressure )
{
 qDebug( "== setSensorSettings ==" );
 // Mise à jour des points
 tWIMOPyroO2Configuration.ucSensorType = ( TUCHAR )ucSensorType.toLatin1();
 tWIMOPyroO2Configuration.ucIntensity  = ( TUCHAR )ucIntensity.toLatin1();
 tWIMOPyroO2Configuration.ucAmp        = ucAmp;

 tWIMOPyroO2Configuration.uliCodeDphi0   = uliCodeDphi0;
 tWIMOPyroO2Configuration.uliCodeDphi100 = uliCodeDphi100;
 tWIMOPyroO2Configuration.bExternalTemperature = bExternalTemperature;
 tWIMOPyroO2Configuration.liTemperature  = liTemperature;
 tWIMOPyroO2Configuration.uliPressure    = uliPressure;

 qDebug( "tWIMOPyroO2Configuration.ucSensorType : %c", tWIMOPyroO2Configuration.ucSensorType );
 qDebug( "tWIMOPyroO2Configuration.ucIntensity : %c", tWIMOPyroO2Configuration.ucIntensity );
 qDebug( "tWIMOPyroO2Configuration.ucAmp : %u", tWIMOPyroO2Configuration.ucAmp );

 qDebug( "tWIMOPyroO2Configuration.uliCodeDphi0 : %lu", tWIMOPyroO2Configuration.uliCodeDphi0 );
 qDebug( "tWIMOPyroO2Configuration.uliCodeDphi100 : %lu", tWIMOPyroO2Configuration.uliCodeDphi100 );
 qDebug( "tWIMOPyroO2Configuration.liTemperature : %ld", tWIMOPyroO2Configuration.liTemperature );
 qDebug( "tWIMOPyroO2Configuration.uliPressure : %lu", tWIMOPyroO2Configuration.uliPressure );

 // Envoi de la commande d'envoi des paramètres
 vFWIMOModbusSetSensorSettings();
}
*/

//----------------------------------------------------------
// Start calibration
//----------------------------------------------------------
/*
void O2Event::startCalibration( void )
{
 qDebug( "== startCalibration ==" );

 // Envoi de la commande d'envoi des paramètres
 vFWIMOModbusStartCalibrationCMD();
}
*/

//----------------------------------------------------------
// Set air calibration CMD
//----------------------------------------------------------
void O2Event::setAirCalibrationCMD( void )
{
 qDebug( "== setAirCalibration CMD ==" );

 // Envoi de la commande d'envoi des paramètres
 vFWIMOModbusO2SetAir();
}

//----------------------------------------------------------
// Set 0% calibration
//----------------------------------------------------------
void O2Event::set0PCalibrationCMD( void )
{
 qDebug( "== set0PCalibration CMD ==" );

 // Envoi de la commande d'envoi des paramètres
 vFWIMOModbusO2Set0();
}

//----------------------------------------------------------
// Reset calibration
//----------------------------------------------------------
/*
void O2Event::resetCalibration( void )
{
 qDebug( "== reset calibration ==" );
 // Mise à jour des points
 tWIMOPyroO2Configuration.liTemperature0 = liTemperature0;
 tWIMOPyroO2Configuration.bExternalTemperature0 = bExternalTemperature0;

 qDebug( "tWIMOPyroO2Configuration.liTemperature0 : %ld", tWIMOPyroO2Configuration.liTemperature0 );

 // Envoi de la commande d'envoi des paramètres
 vFWIMOModbusO2Set0();
}
*/

//----------------------------------------------------------
// Save register permanently
//----------------------------------------------------------
void O2Event::saveRegisterPermanently( void )
{
 qDebug( "== saveRegisterPermanently ==" );

 // Envoi de la commande d'envoi des paramètres
 vFWIMOModbusO2Save();
}

#endif
