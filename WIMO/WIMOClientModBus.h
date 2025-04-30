//============================================================================//
// Fichier .........: "WIMOModbus.h"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2019/01/27
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef WIMO_MODBUS_H
#define WIMO_MODBUS_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeC.h>
#include <ModBusDataConfiguration_2_04.h>
#include <ModBusData_2_04.h>
#include <QString>
#include "WIMOParameters.h"

//============================================================================//
// Déclaration des constantes
//============================================================================//

//============================================================================//
// Déclaration des types
//============================================================================//

//============================================================================//
// Déclaration des variables exportées
//============================================================================//
// Interface version
extern TVersionObject tWIMOModbusInterfaceVersion;
// Date de début d'acquisition
extern QDateTime tAcquisitionStartTime;

//============================================================================//
// Déclaration des variables non exportées
//============================================================================//
// Etat du grafcet modbus
//static TUCHAR ucModbusState;
// Taille de la réponse modbus attendu
//static TUCHAR ucModbusSize;

//============================================================================//
// Déclaration des macros
//============================================================================//
// Macro d'attente du programme
#define Sleep(__DURATION_MS__)  QThread::msleep(__DURATION_MS__)
// Macro date courante
#define Now()                   QDateTime::currentDateTime()
// Macro date courante sous forme de seconde
#define NowInSec()              QDateTime::currentDateTime().time().second()

//============================================================================//
// Déclaration des fonctions exportées
//============================================================================//

//----------------------------------------------------------------------------//
// Initialisation
extern void vFWIMOModbusInitialization( void );

//----------------------------------------------------------------------------//
// Demande numéro de version
extern void vFWIMOModbusGetVersionNumber( void );

//----------------------------------------------------------------------------//
// Demande numéro de version de l'interface/outil
//----------------------------------------------------------------------------//
extern void vFWIMOModbusGetInterfaceVersionNumber( void );

//----------------------------------------------------------------------------//
// Taille de réponse Modbus attendu (pour composant modbus)
// Paramètre :
// - Type de message
// Résultat :
// - Taille
extern TUCHAR ucFWIMOModbusMessageSize( TUCHAR );

//----------------------------------------------------------------------------//
// Déclenchement du balai
extern void vFWIMOModbusStartCalibration( void );

//----------------------------------------------------------------------------//
// Déclenchement du balai
extern void vFWIMOModbusStartClean( void );

//----------------------------------------------------------------------------//
// Déclenchement de la mesure
extern void vFWIMOModbusStartMeasure( void );

//----------------------------------------------------------------------------//
// Sauvegarde de la configuration dans le capteur factory
extern void vFWIMOModbusWriteConfigToSensorFactory( void );

//----------------------------------------------------------------------------//
// Sauvegarde de la configuration dans le capteur
extern void vFWIMOModbusSaveConfigToSensor( void );

//----------------------------------------------------------------------------//
// Sauvegarde du channel 0 dans le capteur
extern void vFWIMOModbusSaveGeneralConfOnlyToSensor( void );

//----------------------------------------------------------------------------//
// Sauvegarde de la configuration d'un channel dans le capteur
// Paramètre :
// - Voie à sauvegarder
extern void vFWIMOModbusSaveChannelToSensor( TUCHAR );

//----------------------------------------------------------------------------//
// Get de la configuration d'un channel dans le capteur
// Paramètre :
// - Voie à obtenir
extern void vFWIMOModbusGetChannelFromSensor( TUCHAR );

//----------------------------------------------------------------------------//
// Mise en forme d'une date nke en string
//----------------------------------------------------------------------------//
extern void vFWIMOModbusGetCalibrationDateTime( TUCHAR, TCHAR * );

//----------------------------------------------------------------------------//
// Formatage formule calibration avec coefficient
//----------------------------------------------------------------------------//
extern QString sFWIMOModbusFormatCalibrationFormula( TUCHAR, TBOOL );

//----------------------------------------------------------------------------//
// Démarrage mise à jour
//----------------------------------------------------------------------------//
extern void vFWIMOModbusStartUpdate( QString );

//----------------------------------------------------------------------------//
// Interface - CMD de passage en mode bootloader
//----------------------------------------------------------------------------//
extern bool bFWIMOModbusStartUpdateInterfaceCMD( QByteArray & );

//----------------------------------------------------------------------------//
// Reset capteur
//----------------------------------------------------------------------------//
extern void vFWIMOModbusStartReset( void );

//----------------------------------------------------------------------------//
// O2 - Lecture de la structure
//----------------------------------------------------------------------------//
extern void vFWIMOModbus_O2_ReadConfFromI2CSensor( void );

//----------------------------------------------------------------------------//
// O2 - Envoi de la commande de lecture
//----------------------------------------------------------------------------//
extern void vFWIMOModbus_O2_sendReadCMD( void );

//----------------------------------------------------------------------------//
// O2 - Envoi de la commande de lecture de l'offset en temperature
//----------------------------------------------------------------------------//
extern void vFWIMOModbus_O2_sendReadTemperatureOffsetCMD( void );

//----------------------------------------------------------------------------//
// O2 - Envoi de la commande d'écriture de l'offset en temperature
//----------------------------------------------------------------------------//
extern void vFWIMOModbus_O2_sendWriteTemperatureOffsetCMD( void );

//----------------------------------------------------------------------------//
// O2 - CMD pour écriture de la configuration capteur initiale
//----------------------------------------------------------------------------//
extern void vFWIMOModbus_O2_sendConfToI2CSensor( void );

//----------------------------------------------------------------------------//
// O2 - CMD pour écriture de la configuration capteur initiale
//----------------------------------------------------------------------------//
extern void vFWIMOModbus_O2_sendConfigWriteCMD( void );

//----------------------------------------------------------------------------//
// O2 - CMD pour écriture de la configuration capteur initiale
//----------------------------------------------------------------------------//
//extern void vFWIMOModbusSetSensorSettings( void );

//----------------------------------------------------------------------------//
// O2 - CMD pour start de la calibration
//----------------------------------------------------------------------------//
extern void vFWIMOModbusStartCalibrationCMD( void );

//----------------------------------------------------------------------------//
// O2 - CMD pour set Air
//----------------------------------------------------------------------------//
extern void vFWIMOModbusO2SetAir( void );

//----------------------------------------------------------------------------//
// O2 - CMD pour set 0%
//----------------------------------------------------------------------------//
extern void vFWIMOModbusO2Set0( void );

//----------------------------------------------------------------------------//
// O2 - Set point manually
//----------------------------------------------------------------------------//
extern void vFWIMOModbusSetPointManually( void );

//----------------------------------------------------------------------------//
// O2 - Save register permanently
//----------------------------------------------------------------------------//
extern void vFWIMOModbusO2Save( void );

//----------------------------------------------------------------------------//
// O2 - Passage du capteur en mode calibration
//----------------------------------------------------------------------------//
extern void vFWIMOModbusO2SwitchToCalibrationMode( void );

//----------------------------------------------------------------------------//
// O2 - Lecture de la structure
//----------------------------------------------------------------------------//
extern void vFWIMOModbusReadO2Struct( void );

//----------------------------------------------------------------------------//
// Test si le grafcet est à l'état de repos
// Résultat :
// - aTRUE si actif
// - aFALSE sinon
extern bool bIsWIMOModbusGrafcetBusy( void );

//----------------------------------------------------------------------------//
// Gestion Modbus
// Résultat :
// - aTRUE si actif
// - aFALSE sinon
extern TBOOL bGWIMOModbusManager( void );

//----------------------------------------------------------------------------//
// Mise en forme d'une date nke en string
//----------------------------------------------------------------------------//
extern QString sFWIMOModbusGetCalibrationDateTime( TUCHAR );

#endif
