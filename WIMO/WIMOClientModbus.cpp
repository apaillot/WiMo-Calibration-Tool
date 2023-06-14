//============================================================================//
// Fichier .........: "WIMOClientModbus.cpp"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2020/01/28
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef WIMO_CLIENT_MODBUS_CPP
#define WIMO_CLIENT_MODBUS_CPP

//============================================================================//
// Inclusion des librairies
//============================================================================//
// Type nke
#include <TypeC.h>
#include <TypeCMMN.h>
// Lib standart
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
// Qt
#include <QDateTime>
#include <QDate>
#include <QDebug>
#include <QQmlContext>
#include <QFile>
#include <QGuiApplication>
#include <QTranslator>
// Composant nke
#include <ModBusData_2_04.h>
#include <ModBusDriver_2_04.h>
#include <Endian_1_00.h>
// User
#include "WIMOClientModbus.h"
#include "WIMOParameters.h"
#include "TARGETHardware.h"
// Programme
//#include "acquisitionthread.h"
//#include "WiperThread.h"
#include <calibration.h>
#include <calibpoint.h>
#include <main.h>
#include <SensorUpdateControler.h>
#include <ModbusControler.h>
#include <WIMOSensor.h>

//============================================================================//
// Déclaration des constantes
//============================================================================//
//-----------------------
// Etat grafcet modbus
//-----------------------
/*
#define aMODBUS_STATE_FREE             0
// Etape de mesure
#define aMODBUS_STATE_WAIT             ( aMODBUS_STATE_FREE             + 1 )
#define aMODBUS_STATE_MEASURE          ( aMODBUS_STATE_WAIT             + 1 )
#define aMODBUS_STATE_MEASURING        ( aMODBUS_STATE_MEASURE          + 1 )
#define aMODBUS_STATE_RESULT           ( aMODBUS_STATE_MEASURING        + 1 )
// Etape du balai
#define aMODBUS_STATE_CLEAN            ( aMODBUS_STATE_RESULT           + 1 )
#define aMODBUS_STATE_CLEANING         ( aMODBUS_STATE_CLEAN            + 1 )
// Numéro de version
#define aMODBUS_STATE_GET_VERSION      ( aMODBUS_STATE_CLEANING         + 1 )
// Requête lecture configuration
#define aMODBUS_STATE_GET_CONF_PRODUCT ( aMODBUS_STATE_GET_VERSION      + 1 )
#define aMODBUS_STATE_GET_CONF_CH0     ( aMODBUS_STATE_GET_CONF_PRODUCT + 1 )
#define aMODBUS_STATE_GET_CONF_CH1     ( aMODBUS_STATE_GET_CONF_CH0     + 1 )
#define aMODBUS_STATE_GET_CONF_CH2     ( aMODBUS_STATE_GET_CONF_CH1     + 1 )
#define aMODBUS_STATE_GET_CONF_O2      ( aMODBUS_STATE_GET_CONF_CH2     + 1 )
// Requête écriture configuration
#define aMODBUS_STATE_SET_CONF_PRODUCT ( aMODBUS_STATE_GET_CONF_O2      + 1 )
#define aMODBUS_STATE_SET_CONF_CH0     ( aMODBUS_STATE_SET_CONF_PRODUCT + 1 )
#define aMODBUS_STATE_SET_CONF_CH1     ( aMODBUS_STATE_SET_CONF_CH0     + 1 )
#define aMODBUS_STATE_SET_CONF_CH2     ( aMODBUS_STATE_SET_CONF_CH1     + 1 )
#define aMODBUS_STATE_SET_CONF_O2      ( aMODBUS_STATE_SET_CONF_CH2     + 1 )
// Requête spécifique calibration O2
#define aMODBUS_STATE_SET_O2_0         ( aMODBUS_STATE_SET_CONF_O2      + 1 )
#define aMODBUS_STATE_SET_O2_100       ( aMODBUS_STATE_SET_O2_0         + 1 )
#define aMODBUS_STATE_SET_O2_SAVE      ( aMODBUS_STATE_SET_O2_100       + 1 )
*/
// Code fonction
#define aModBusSensorCF3     ( 0x03 )
#define aModBusSensorCF4     ( 0x04 )
#define aModBusSensorCF6     ( 0x06 )
#define aModBusSensorCF16    ( 0x10 )

//============================================================================//
// Déclaration des prototypes de fonctions non exportées
//============================================================================//

//============================================================================//
// Déclaration des types
//============================================================================//
//-----------------------
// Etat grafcet modbus
//-----------------------
typedef enum{
 // Etape de repos
 eMODBUS_STATE_FREE,          // 0
 // Etape de reset capteur
 eMODBUS_STATE_RESET,         // 1
 // Etape de mise à jour
 eMODBUS_STATE_UPDATE,        // 2
 eMODBUS_STATE_UPDATING_1,    // 3
 eMODBUS_STATE_UPDATING_2,    // 4
 eMODBUS_STATE_UPDATING_3,    // 5
 // Mise à jour outil
 eMODBUS_STATE_UPDATE_INTERFACE_CMD, // 6
 // Etape de mesure
 eMODBUS_STATE_WAIT,          // 7
 eMODBUS_STATE_MEASURE,       // 8
 eMODBUS_STATE_MEASURING,     // 9
 eMODBUS_STATE_RESULT,        // 10
 // Etape du balai
 eMODBUS_STATE_CLEAN,         // 11
 eMODBUS_STATE_CLEANING,      // 12
 // Numéro de version
 eMODBUS_STATE_GET_VERSION,
 // Numéro de version de l'interface
 eMODBUS_STATE_GET_INTERFACE_VERSION,
 // Requête lecture configuration
 eMODBUS_STATE_GET_CONF_PRODUCT,
 eMODBUS_STATE_GET_CONF_PRODUCT_ONLY,
 eMODBUS_STATE_GET_CONF_CH0,
 eMODBUS_STATE_GET_CONF_CH0_ONLY,
 eMODBUS_STATE_GET_CONF_CH1,
 eMODBUS_STATE_GET_CONF_CH1_ONLY,
 eMODBUS_STATE_GET_CONF_CH2,
 eMODBUS_STATE_GET_CONF_CH2_ONLY,
 eMODBUS_STATE_GET_CONF_O2,
 // Requête écriture periode de mesure (Modbus 9600)
 eMODBUS_STATE_RESET_CONF_MEASURE_PERIOD,
 eMODBUS_STATE_SET_CONF_MEASURE_PERIOD,
 eMODBUS_STATE_GET_MEASURE_DATA_9600_CMD,
 eMODBUS_STATE_GET_MEASURE_DATA_9600,
 // Requête écriture configuration
 eMODBUS_STATE_SET_CONF_PRODUCT,
 eMODBUS_STATE_SET_CONF_PRODUCT_ONLY,
 eMODBUS_STATE_SET_CONF_CH0,
 eMODBUS_STATE_SET_CONF_CH0_ONLY,
 eMODBUS_STATE_SET_CONF_CH1,
 eMODBUS_STATE_SET_CONF_CH1_ONLY,
 eMODBUS_STATE_SET_CONF_CH2,
 eMODBUS_STATE_SET_CONF_CH2_ONLY,
 eMODBUS_STATE_SET_CONF_O2,
 eMODBUS_STATE_SET_CONF_O2_WAIT,
 // Requête spécifique calibration O2
 eMODBUS_STATE_SET_O2_0,
 eMODBUS_STATE_SET_O2_0_IS_CMD_FINISHED_REQUEST,
 eMODBUS_STATE_SET_O2_0_IS_CMD_FINISHED,
 eMODBUS_STATE_SET_O2_100,
 eMODBUS_STATE_SET_O2_100_IS_CMD_FINISHED_REQUEST,
 eMODBUS_STATE_SET_O2_100_IS_CMD_FINISHED,
 eMODBUS_STATE_SET_O2_SAVE,
 eMODBUS_STATE_SET_O2_SAVE_WAIT,
 eMODBUS_STATE_SET_O2_READ_TEMPERATURE_OFFSET_CMD,
 eMODBUS_STATE_SET_O2_IS_READ_TEMPERATURE_OFFSET_CMD_FINISHED_REQUEST,
 eMODBUS_STATE_SET_O2_IS_READ_TEMPERATURE_OFFSET_CMD_FINISHED,
 eMODBUS_STATE_SET_O2_WRITE_TEMPERATURE_OFFSET_CMD,
 eMODBUS_STATE_SET_O2_IS_WRITE_TEMPERATURE_OFFSET_CMD_FINISHED_REQUEST,
 eMODBUS_STATE_SET_O2_IS_WRITE_TEMPERATURE_OFFSET_CMD_FINISHED,
 eMODBUS_STATE_SET_O2_READ_CMD,
 eMODBUS_STATE_SET_O2_IS_READ_CMD_FINISHED_REQUEST,
 eMODBUS_STATE_SET_O2_IS_READ_CMD_FINISHED,
 eMODBUS_STATE_SET_O2_SET_POINT_MANUALLY_CMD,
 eMODBUS_STATE_SET_O2_SET_SENSOR_SETTINGS_CMD,
 eMODBUS_STATE_O2_SET_SENSOR_SETTINGS_CMD_IS_FINISHED_REQUEST,
 eMODBUS_STATE_O2_SET_SENSOR_SETTINGS_CMD_IS_FINISHED,
 eMODBUS_STATE_SET_O2_START_CALIBRATION_CMD,
 eMODBUS_STATE_O2_SWITCH_TO_CALIBRATION_MODE_CMD,
 eMODBUS_STATE_O2_IS_SWITCH_CALIBRATION_FINISHED_CMD_SEND_CMD,
 eMODBUS_STATE_SET_O2_IS_SWITCH_CALIBRATION_CMD_FINISHED
} TEWIMOModbusState;

// Requête modbus
typedef struct
{
 TUCHAR ucFunction;
 TUCHAR ucReplySize;
 TUCHAR ucRequestSize;
 TUCHAR tucRequest[4];
}
TModBusRequestObject;

// Requête de donnée modbus
typedef struct
{
 TUCHAR ucFunction;
 TUCHAR ucReplySize;
 TUCHAR ucRequestSize;
 TUCHAR tucRequest[5];
 TUCHAR tucData[246];
}
TModBusRequestDataObject;

// Type de gestion du modbus
typedef struct
{
 // Etat du grafcet modbus
 TEWIMOModbusState eState;
 // Taille de la réponse modbus attendue
 TUCHAR ucReplySize;
 // Etat du flag capteur
 TUCHAR ucSensorFlag;
 // Nombre de relance
 TUCHAR ucRequestRetry;
 // Initialisation du mode modbus 9600
 TBOOL b9600Init;
 // Timeout de mesure
 TUINT uiMeasureTimeoutCpt;
} TWIMOModbusObject;

//============================================================================//
// Déclaration des variables exportées
//============================================================================//
// Interface version
TVersionObject tWIMOModbusInterfaceVersion;
// Date de début d'acquisition
QDateTime tAcquisitionStartTime;

//============================================================================//
// Déclaration des variables non exportées
//============================================================================//
//----------------
// Requête modbus
//----------------
// Requête numéro de version soft
static TCONST TModBusRequestObject tModbusReadVersion      = { 0x04,   5,   4, {0x00, 0x03, 0x00, 0x02} };
// Requête numéro de version soft interface
static TCONST TModBusRequestObject tModbusReadInterfaceVersion = { 0x04,   5,   4, {0x00, 0x04, 0x00, 0x02} };
// Requête de lecture de configuration
static TCONST TModBusRequestObject tModbusReadConfP        = { 0x03, 129,   4, {0x03, 0x00, 0x00, 0x40} };
static TCONST TModBusRequestObject tModbusReadConf0        = { 0x03, 247,   4, {0x03, 0x01, 0x00, 0x7B} };
static TCONST TModBusRequestObject tModbusReadConf1        = { 0x03, 247,   4, {0x03, 0x02, 0x00, 0x7B} };
static TCONST TModBusRequestObject tModbusReadConf2        = { 0x03, 247,   4, {0x03, 0x03, 0x00, 0x7B} };
//static TCONST TModBusRequestObject tModbusReadConfO2       = { 0x03,  37,   4, {0x04, 0x06, 0x00, 0x12} };
static TCONST TModBusRequestObject tModbusReadConfO2       = { 0x03,  sizeof( tWIMOPyroO2Configuration ) + 1,   4, {0x04, 0x06, 0x00, 0x12} };
// Requête reset du capteur
static TCONST TModBusRequestObject tModbusReset            = { 0x06,   4,   4, {0x00, 0x50, 0x12, 0x34} };
// Requête de mesure
static TCONST TModBusRequestObject tModbusMeasure          = { 0x06,   4,   4, {0x00, 0x10, 0x80, 0x00} };
static TCONST TModBusRequestObject tModbusMeasuring        = { 0x04,   3,   4, {0x00, 0x11, 0x00, 0x01} };
static TCONST TModBusRequestObject tModbusResult           = { 0x04,  13,   4, {0x02, 0x01, 0x00, 0x06} };
// Requête pour balai
static TCONST TModBusRequestObject tModbusClean            = { 0x06,   4,   4, {0x00, 0x20, 0x80, 0x00} };
static TCONST TModBusRequestObject tModbusCleaning         = { 0x04,   3,   4, {0x00, 0x21, 0x00, 0x01} };
// Requête de mise à jour de l'interface
static TCONST TModBusRequestObject tModbusStartInterfaceUpdate = { 0x06,   4,   4, {0xF0, 0x00, 0x80, 0x00} };
// Requête de mise à jour
static TCONST TModBusRequestObject tModbusStartUpdate      = { 0x06,   4,   4, {0x0F, 0x00, 0x80, 0x00} };
static TCONST TModBusRequestObject tModbusUpdateCommand    = { 0x06,   4,   4, {0x0F, 0x01, 0x00, 0x00} };
static TCONST TModBusRequestObject tModbusUpdateStop       = { 0x06,   4,   4, {0x0F, 0x03, 0x00, 0x00} };
// Requête set measure period (modbus 9600)
static TCONST TModBusRequestObject tModbusResetMeasurePeriod = { 0x06,   4,   4, {0x00, 0xB0, 0x00, 0x00} };
static TCONST TModBusRequestObject tModbusSetMeasurePeriod1sec   = { 0x06,   4,   4, {0x00, 0xB0, 0x00, 0x01} };
static TCONST TModBusRequestObject tModbusSetMeasurePeriod10sec  = { 0x06,   4,   4, {0x00, 0xB0, 0x00, 0x0A} };
static TCONST TModBusRequestObject tModbusResult9600         = { 0x04,   9,   4, {0x02, 0x01, 0x00, 0x04} };
// Requête spécifique calibration O2
static TCONST TModBusRequestObject tModbusO2Set0           = { 0x06,   4,   4, {0x00, 0xA0, 0x00, 0x01} };
static TCONST TModBusRequestObject tModbusO2Set100         = { 0x06,   4,   4, {0x00, 0xA0, 0x00, 0x02} };
static TCONST TModBusRequestObject tModbusO2Save           = { 0x06,   4,   4, {0x00, 0xA0, 0x00, 0x03} };
static TCONST TModBusRequestObject tModbusO2ReadCMD        = { 0x06,   4,   4, {0x00, 0xA0, 0x00, 0x04} };
static TCONST TModBusRequestObject tModbusO2SetPointManuallyCMD  = { 0x06,   4,   4, {0x00, 0xA0, 0x00, 0x05} };
static TCONST TModBusRequestObject tModbusO2SetSensorSettingsCMD = { 0x06,   4,   4, {0x00, 0xA0, 0x00, 0x06} };
static TCONST TModBusRequestObject tModbusO2StartCalibrationCMD  = { 0x06,   4,   4, {0x00, 0xA0, 0x00, 0x07} };
static TCONST TModBusRequestObject tModbusO2SwitchToCalibrationModeCMD  = { 0x06,   4,   4, {0x00, 0xA0, 0x00, 0x08} };
static TCONST TModBusRequestObject tModbusO2ReadTemperatureOffsetCMD    = { 0x06,   4,   4, {0x00, 0xA0, 0x00, 0x09} };
static TCONST TModBusRequestObject tModbusO2WriteTemperatureOffsetCMD   = { 0x06,   4,   4, {0x00, 0xA0, 0x00, 0x0A} };
// Requête d'écriture de configuration
/*
static TCONST TModBusRequestDataObject tModbusWriteConfP   = { 0x10,   4, 251, {0x03, 0x00, 0x00, 0x7B, 246} };
static TCONST TModBusRequestDataObject tModbusWriteConf0   = { 0x10,   4, 251, {0x03, 0x01, 0x00, 0x7B, 246} };
static TCONST TModBusRequestDataObject tModbusWriteConf1   = { 0x10,   4, 251, {0x03, 0x02, 0x00, 0x7B, 246} };
static TCONST TModBusRequestDataObject tModbusWriteConf2   = { 0x10,   4, 251, {0x03, 0x03, 0x00, 0x7B, 246} };
static TCONST TModBusRequestDataObject tModbusWriteConfO2  = { 0x10,   4, 251, {0x04, 0x06, 0x00, 0x7B, 246} };
static TCONST TModBusRequestDataObject tModbusUpdateData   = { 0x10,   4, 251, {0x0F, 0x02, 0x00, 0x7B, 246} };
*/
static TModBusRequestDataObject tModbusWriteConfP   = { 0x10,   4, 251, {0x03, 0x00, 0x00, 0x7B, 246} };
static TModBusRequestDataObject tModbusWriteConf0   = { 0x10,   4, 251, {0x03, 0x01, 0x00, 0x7B, 246} };
static TModBusRequestDataObject tModbusWriteConf1   = { 0x10,   4, 251, {0x03, 0x02, 0x00, 0x7B, 246} };
static TModBusRequestDataObject tModbusWriteConf2   = { 0x10,   4, 251, {0x03, 0x03, 0x00, 0x7B, 246} };
static TModBusRequestDataObject tModbusWriteConfO2  = { 0x10,   4, 251, {0x04, 0x06, 0x00, 0x7B, 246} };
//static TModBusRequestDataObject tModbusWriteConfO2  = { 0x10,   4, sizeof( tWIMOPyroO2Configuration )+5, {0x04, 0x06, 0x00, 0x7B, sizeof( tWIMOPyroO2Configuration )} };
static TModBusRequestDataObject tModbusUpdateData   = { 0x10,   4, 251, {0x0F, 0x02, 0x00, 0x7B, 246} };
//--------------
// Autre variable
//--------------
static TWIMOModbusObject tModbus;
// Pour mise à jour du capteur
static TUCHAR tucWIMOUpdateData[ ( TULONGINT )( ( TULONGINT )64 * ( TULONGINT )1024 ) ];
// Refresh après demande factory
static TBOOL bFactoryDataRefresh = ( TBOOL )aFALSE;
// Sensor software version
static QString sSensorSoftware;

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des prototypes de fonctions exportées
//============================================================================//

//============================================================================//
// Déclaration des prototypes de fonctions non-exportées
//============================================================================//
//----------------------------------------------------------------------------//
// Ecriture de la periode de measure à 0 - Modbus 9600
static void vFWIMOModbusSetConfMeasurePeriod( void );
//----------------------------------------------------------------------------//
// Ecriture de la periode de measure à 0 - Modbus 9600
static void vFWIMOModbusResetConfMeasurePeriod( void );
//----------------------------------------------------------------------------//
// Sauvegarde du channel 0 dans le capteur
static void vFWIMOModbusSaveCh0ToSensor( void );
//----------------------------------------------------------------------------//
// Sauvegarde du channel 1 dans le capteur
static void vFWIMOModbusSaveCh1ToSensor( void );
//----------------------------------------------------------------------------//
// Sauvegarde du channel 2 dans le capteur
static void vFWIMOModbusSaveCh2ToSensor( void );
//----------------------------------------------------------------------------//
// Get du channel 0 dans le capteur
static void vFWIMOModbusGetCh0FromSensor( void );
//----------------------------------------------------------------------------//
// Get du channel 1 dans le capteur
static void vFWIMOModbusGetCh1FromSensor( void );
//----------------------------------------------------------------------------//
// Get du channel 2 dans le capteur
static void vFWIMOModbusGetCh2FromSensor( void );
//----------------------------------------------------------------------------//
// Gestion des erreurs d'écriture du grafcet
static void vFWIMOModbusHandleRequestError( void );

//****************************************************************************//
// Le code des fonctions
//****************************************************************************//

//----------------------------------------------------------------------------//
// Affichage de l'état actuel du grafcet
//----------------------------------------------------------------------------//
void vFWIMOModbusPrintDebugtMdobusState( void )
{
 // Selon l'état
 switch( tModbus.eState )
  {
   // Etape de repos
   // eMODBUS_STATE_FREE,          // 0
   case( eMODBUS_STATE_FREE ):
    qDebug( "Debug state: eMODBUS_STATE_FREE" );
   break;
   // Etape de reset capteur
   // eMODBUS_STATE_RESET,         // 1
   case( eMODBUS_STATE_RESET ):
    qDebug( "Debug state: eMODBUS_STATE_RESET" );
   break;
   // Etape de mise à jour
   // eMODBUS_STATE_UPDATE,        // 2
   case( eMODBUS_STATE_UPDATE ):
    qDebug( "Debug state: eMODBUS_STATE_UPDATE" );
   break;
   // eMODBUS_STATE_UPDATING_1,    // 3
   case( eMODBUS_STATE_UPDATING_1 ):
    qDebug( "Debug state: eMODBUS_STATE_UPDATING_1" );
   break;
   // eMODBUS_STATE_UPDATING_2,    // 4
   case( eMODBUS_STATE_UPDATING_2 ):
    qDebug( "Debug state: eMODBUS_STATE_UPDATING_2" );
   break;
   // eMODBUS_STATE_UPDATING_3,    // 5
   case( eMODBUS_STATE_UPDATING_3 ):
    qDebug( "Debug state: eMODBUS_STATE_UPDATING_3" );
   break;
   // Mise à jour outil
   // eMODBUS_STATE_UPDATE_INTERFACE_CMD, // 6
   case( eMODBUS_STATE_UPDATE_INTERFACE_CMD ):
    qDebug( "Debug state: eMODBUS_STATE_UPDATE_INTERFACE_CMD" );
   break;
   // Etape de mesure
   // eMODBUS_STATE_WAIT,          // 7
   case( eMODBUS_STATE_WAIT ):
    qDebug( "Debug state: eMODBUS_STATE_WAIT" );
   break;
   // eMODBUS_STATE_MEASURE,       // 8
    case( eMODBUS_STATE_MEASURE ):
    qDebug( "Debug state: eMODBUS_STATE_MEASURE" );
   break;
   // eMODBUS_STATE_MEASURING,     // 9
   case( eMODBUS_STATE_MEASURING ):
    qDebug( "Debug state: eMODBUS_STATE_MEASURING" );
   break;
   // eMODBUS_STATE_RESULT,        // 10
   case( eMODBUS_STATE_RESULT ):
    qDebug( "Debug state: eMODBUS_STATE_RESULT" );
   break;
   // Etape du balai
   // eMODBUS_STATE_CLEAN,         // 11
   case( eMODBUS_STATE_CLEAN ):
    qDebug( "Debug state: eMODBUS_STATE_CLEAN" );
   break;
   // eMODBUS_STATE_CLEANING,      // 12
   case( eMODBUS_STATE_CLEANING ):
    qDebug( "Debug state: eMODBUS_STATE_CLEANING" );
   break;
   // Numéro de version
   // eMODBUS_STATE_GET_VERSION,
   case( eMODBUS_STATE_GET_VERSION ):
    qDebug( "Debug state: eMODBUS_STATE_GET_VERSION" );
   break;
   // Numéro de version de l'interface
   // eMODBUS_STATE_GET_INTERFACE_VERSION,
   case( eMODBUS_STATE_GET_INTERFACE_VERSION ):
    qDebug( "Debug state: eMODBUS_STATE_GET_INTERFACE_VERSION" );
   break;
   // Requête lecture configuration
   // eMODBUS_STATE_GET_CONF_PRODUCT,
   case( eMODBUS_STATE_GET_CONF_PRODUCT ):
    qDebug( "Debug state: eMODBUS_STATE_GET_CONF_PRODUCT" );
   break;
   // eMODBUS_STATE_GET_CONF_PRODUCT_ONLY,
   case( eMODBUS_STATE_GET_CONF_PRODUCT_ONLY ):
    qDebug( "Debug state: eMODBUS_STATE_GET_CONF_PRODUCT_ONLY" );
   break;
   // eMODBUS_STATE_GET_CONF_CH0,
   case( eMODBUS_STATE_GET_CONF_CH0 ):
    qDebug( "Debug state: eMODBUS_STATE_GET_CONF_CH0" );
   break;
   // eMODBUS_STATE_GET_CONF_CH0_ONLY,
   case( eMODBUS_STATE_GET_CONF_CH0_ONLY ):
    qDebug( "Debug state: eMODBUS_STATE_GET_CONF_CH0_ONLY" );
   break;
   // eMODBUS_STATE_GET_CONF_CH1,
   case( eMODBUS_STATE_GET_CONF_CH1 ):
    qDebug( "Debug state: eMODBUS_STATE_GET_CONF_CH1" );
   break;
   // eMODBUS_STATE_GET_CONF_CH1_ONLY,
   case( eMODBUS_STATE_GET_CONF_CH1_ONLY ):
    qDebug( "Debug state: eMODBUS_STATE_GET_CONF_CH1_ONLY" );
   break;
   // eMODBUS_STATE_GET_CONF_CH2,
   case( eMODBUS_STATE_GET_CONF_CH2 ):
    qDebug( "Debug state: eMODBUS_STATE_GET_CONF_CH2" );
   break;
   // eMODBUS_STATE_GET_CONF_CH2_ONLY,
   case( eMODBUS_STATE_GET_CONF_CH2_ONLY ):
    qDebug( "Debug state: eMODBUS_STATE_GET_CONF_CH2_ONLY" );
   break;
   // eMODBUS_STATE_GET_CONF_O2,
   case( eMODBUS_STATE_GET_CONF_O2 ):
    qDebug( "Debug state: eMODBUS_STATE_GET_CONF_O2" );
   break;
   // Requête écriture configuration
   // eMODBUS_STATE_SET_CONF_PRODUCT,
   case( eMODBUS_STATE_SET_CONF_PRODUCT ):
    qDebug( "Debug state: eMODBUS_STATE_SET_CONF_PRODUCT" );
   break;
   // eMODBUS_STATE_SET_CONF_PRODUCT_ONLY,
   case( eMODBUS_STATE_SET_CONF_PRODUCT_ONLY ):
    qDebug( "Debug state: eMODBUS_STATE_SET_CONF_PRODUCT_ONLY" );
   break;
   // eMODBUS_STATE_SET_CONF_CH0,
   case( eMODBUS_STATE_SET_CONF_CH0 ):
    qDebug( "Debug state: eMODBUS_STATE_SET_CONF_CH0" );
   break;
   // eMODBUS_STATE_SET_CONF_CH0_ONLY,
   case( eMODBUS_STATE_SET_CONF_CH0_ONLY ):
    qDebug( "Debug state: eMODBUS_STATE_SET_CONF_CH0_ONLY" );
   break;
   // eMODBUS_STATE_SET_CONF_CH1,
   case( eMODBUS_STATE_SET_CONF_CH1 ):
    qDebug( "Debug state: eMODBUS_STATE_SET_CONF_CH1" );
   break;
   // eMODBUS_STATE_SET_CONF_CH1_ONLY,
   case( eMODBUS_STATE_SET_CONF_CH1_ONLY ):
    qDebug( "Debug state: eMODBUS_STATE_SET_CONF_CH1_ONLY" );
   break;
   // eMODBUS_STATE_SET_CONF_CH2,
   case( eMODBUS_STATE_SET_CONF_CH2 ):
    qDebug( "Debug state: eMODBUS_STATE_SET_CONF_CH2" );
   break;
   // eMODBUS_STATE_SET_CONF_CH2_ONLY,
   case( eMODBUS_STATE_SET_CONF_CH2_ONLY ):
    qDebug( "Debug state: eMODBUS_STATE_SET_CONF_CH2_ONLY" );
   break;
   // eMODBUS_STATE_SET_CONF_O2,
   case( eMODBUS_STATE_SET_CONF_O2 ):
    qDebug( "Debug state: eMODBUS_STATE_SET_CONF_O2" );
   break;
   // Requête spécifique calibration O2
   // eMODBUS_STATE_SET_O2_0,
   case( eMODBUS_STATE_SET_O2_0 ):
    qDebug( "Debug state: eMODBUS_STATE_SET_O2_0" );
   break;
   // eMODBUS_STATE_SET_O2_100,
   case( eMODBUS_STATE_SET_O2_100 ):
    qDebug( "Debug state: eMODBUS_STATE_SET_O2_100" );
   break;
   // eMODBUS_STATE_SET_O2_SAVE,
   case( eMODBUS_STATE_SET_O2_SAVE ):
    qDebug( "Debug state: eMODBUS_STATE_SET_O2_SAVE" );
   break;
   // eMODBUS_STATE_SET_O2_READ_CMD,
   case( eMODBUS_STATE_SET_O2_READ_CMD ):
    qDebug( "Debug state: eMODBUS_STATE_SET_O2_READ_CMD" );
   break;
   // eMODBUS_STATE_SET_O2_SET_POINT_MANUALLY_CMD,
   case( eMODBUS_STATE_SET_O2_SET_POINT_MANUALLY_CMD ):
    qDebug( "Debug state: eMODBUS_STATE_SET_O2_SET_POINT_MANUALLY_CMD" );
   break;
   // eMODBUS_STATE_SET_O2_SET_SENSOR_SETTINGS_CMD,
   case( eMODBUS_STATE_SET_O2_SET_SENSOR_SETTINGS_CMD ):
    qDebug( "Debug state: eMODBUS_STATE_SET_O2_SET_SENSOR_SETTINGS_CMD" );
   break;
   // eMODBUS_STATE_SET_O2_START_CALIBRATION_CMD
   case( eMODBUS_STATE_SET_O2_START_CALIBRATION_CMD ):
    qDebug( "Debug state: eMODBUS_STATE_SET_O2_START_CALIBRATION_CMD" );
   break;
   default:
    qDebug("Error: state debug");
   break;
  }
}

//----------------------------------------------------------------------------//
// Initialisation
//----------------------------------------------------------------------------//
void vFWIMOModbusInitialization( void )
{
 // Initialisation composant modbus
 tModBus.vFInitialization();
 // INit de la variable
 tModbus.b9600Init = ( TBOOL )aFALSE;
 // On libère
 tModbus.eState = eMODBUS_STATE_FREE;
}

//----------------------------------------------------------------------------//
// Formatage formule calibration avec coefficient
//----------------------------------------------------------------------------//
QString sFWIMOModbusFormatCalibrationFormula( TUCHAR ucChannelIndex, TBOOL bPdf )
{
 QString sFormula;
 TUINT uiFactorNb;
 //TFLOAT fAX3, fBX2, fCX, fD;
 QString sAX3, sBX2, sCX, sD;
 TDOUBLE dTemp;

 // Pointeur sur la voie d'acquisition utilisée ici
 TWIMOParametersChannelObject * ptCh = &tWIMOParametersSensor.ttChannel[ucChannelIndex];

 // Test d'intégrité
 if( ucChannelIndex > 3) return( ( QString )"" );

 //tWIMOParametersSensor.ttChannel[ucChannelIndex].cCalibration
 // Selon la calibration utilisée

 // Nombre de coefficient du type de calibration choisi
 switch( tWIMOParametersSensor.ttChannel[ucChannelIndex].cCalibration ){
  //------------------------
  // None
  //------------------------
  case( 'N' ):
   uiFactorNb = 0;
   sFormula = QString(QObject::tr("No formula with current settings"));
  break;
  //------------------------
  // Multipoint
  //------------------------
  case( 'M' ):
   uiFactorNb = 0;
   sFormula = QObject::tr("No formula with current settings");
  break;
  //------------------------
  // Linear : AX+B
  //------------------------
  case( 'L' ):
   uiFactorNb = 2;
   sFormula = "y = ";
   // Coefficient AX
   if( ptCh->tWIMOMathLinear.fCoeffA != ( TFLOAT )0.0 ){
    // Float to String conversion
    //sCX.append(QString::number(ptCh->tWIMOMathLinear.fCoeffA,'f'));
    sCX.sprintf("%g",ptCh->tWIMOMathLinear.fCoeffA);
    //fCX = (ptCh->tWIMOMathLinear.fCoeffA>=0.0)?
    //      ptCh->tWIMOMathLinear.fCoeffA:" -"+(-parseFloat(dtd["c31"]));
    sFormula += sCX+"x ";
   }
   // Coefficient B
   if( ptCh->tWIMOMathLinear.fCoeffB != ( TFLOAT )0.0 ){
    // Test si A != 0
    if( ptCh->tWIMOMathLinear.fCoeffA != ( TFLOAT )0 ){
     sFormula += (ptCh->tWIMOMathLinear.fCoeffB>=0)?" + ":" - ";
     dTemp = (ptCh->tWIMOMathLinear.fCoeffB>=0)?ptCh->tWIMOMathLinear.fCoeffB:(-ptCh->tWIMOMathLinear.fCoeffB);
     sD.sprintf( "%g", dTemp );
    }
    else{
     sD.sprintf( "%g", ptCh->tWIMOMathLinear.fCoeffB );
    }
    // Float to String conversion
    sFormula += sD;
   }
   // Si les deux coefficients sont nulls
   if(  ( ptCh->tWIMOMathLinear.fCoeffA == ( TFLOAT )0.0 )
     && ( ptCh->tWIMOMathLinear.fCoeffB == ( TFLOAT )0.0 ) ){
    sFormula = "y = 0";
   }
  break;
  //------------------------
  // Steinhart
  //------------------------
  case( 'S' ):
   uiFactorNb = 3;
   sFormula = QObject::tr("No formula with current settings");
  break;
  //------------------------
  // Bennett
  //------------------------
  case( 'B' ):
   uiFactorNb = 0;
   sFormula = QObject::tr("No formula with current settings");
  break;
  //------------------------
  // 2nd degree polynomial
  //------------------------
  case( '2' ):
   uiFactorNb = 3;
   sFormula = "y = ";
   //-----
   // Coefficient A
   if( ptCh->tWIMOMathPolynome2.dCoeffA != ( TDOUBLE )0.0 ){
    //sBX2.append(QString::number(ptCh->tWIMOMathPolynome2.dCoeffA,'f'));
    sBX2.sprintf( "%g", ptCh->tWIMOMathPolynome2.dCoeffA );
    //%%AP - 2020/11/05 - Modification
    if( bPdf )
     sFormula += sBX2+"x&#178; ";
    else
     sFormula += sBX2+"x² ";
   }
   //-----
   // Coefficient B
   if( ptCh->tWIMOMathPolynome2.dCoeffB != ( TDOUBLE )0.0 ){
    if( ptCh->tWIMOMathPolynome2.dCoeffA != ( TFLOAT )0.0 )
     {
      sFormula += (ptCh->tWIMOMathPolynome2.dCoeffB>=0)?" + ":" - ";
      dTemp = (ptCh->tWIMOMathPolynome2.dCoeffB>=0)?ptCh->tWIMOMathPolynome2.dCoeffB:(-ptCh->tWIMOMathPolynome2.dCoeffB);
      sCX.sprintf( "%g", dTemp );
     }
    else{
     sCX.sprintf( "%g", ptCh->tWIMOMathPolynome2.dCoeffB );
    }
    sFormula += sCX+"x ";
   }
   //-----
   // Coefficient C
   if( ptCh->tWIMOMathPolynome2.dCoeffC != ( TFLOAT )0.0 ){
    // Conversion
    if(  ( ptCh->tWIMOMathPolynome2.dCoeffA != 0 )
      || ( ptCh->tWIMOMathPolynome2.dCoeffB != 0 ) ){
     // Ajout du signe
     sFormula += (ptCh->tWIMOMathPolynome2.dCoeffC>=0)?" + ":" - ";
     dTemp = (ptCh->tWIMOMathPolynome2.dCoeffC>=0)?ptCh->tWIMOMathPolynome2.dCoeffC:(-ptCh->tWIMOMathPolynome2.dCoeffC);
     sD.sprintf( "%g", dTemp );
    }
    else{
     sD.sprintf( "%g", ptCh->tWIMOMathPolynome2.dCoeffC );
    }
    sFormula += sD;
   }
   if(  ( ptCh->tWIMOMathPolynome2.dCoeffA == 0 )
     && ( ptCh->tWIMOMathPolynome2.dCoeffB == 0 )
     && ( ptCh->tWIMOMathPolynome2.dCoeffC == 0 ) ){
    sFormula = "y = 0";
   }
  break;
  //------------------------
  // 3rd degree polynomial
  //------------------------
  //31 ptCh->tWIMOMathPolynome3.dCoeffA
  //32 ptCh->tWIMOMathPolynome3.dCoeffB
  //33 ptCh->tWIMOMathPolynome3.dCoeffC
  //34 ptCh->tWIMOMathPolynome3.dCoeffD
  case( '3' ):
   uiFactorNb = 4;
   sFormula = "y = ";

   //-----
   // Coefficient A
   if( ptCh->tWIMOMathPolynome3.dCoeffA != ( TDOUBLE )0.0 ){
    sAX3.sprintf( "%g", ptCh->tWIMOMathPolynome3.dCoeffA );
    //%%AP - 2020/11/05 - Modification
    if( bPdf )
     sFormula += sAX3+"x&#179; ";
    else
     sFormula += sAX3+"x³ ";
   }
   //-----
   // Coefficient B
   if( ptCh->tWIMOMathPolynome3.dCoeffB != ( TDOUBLE )0.0 ){
    if( ptCh->tWIMOMathPolynome3.dCoeffA != ( TFLOAT )0.0 )
     {
      sFormula += (ptCh->tWIMOMathPolynome3.dCoeffB>=0)?" + ":" - ";
      dTemp = (ptCh->tWIMOMathPolynome3.dCoeffB>=0)?ptCh->tWIMOMathPolynome3.dCoeffB:(-ptCh->tWIMOMathPolynome3.dCoeffB);
      //dTemp = ( ( TDOUBLE )( ( TULONGINT )( dTemp*10 ) ) ) / 10;
      sBX2.sprintf( "%g", dTemp );
     }
    else{
     sBX2.sprintf( "%g", ptCh->tWIMOMathPolynome3.dCoeffB );
    }
    //%%AP - 2020/11/05 - Modification
    if( bPdf )
     sFormula += sBX2+"x&#178; ";
    else
     sFormula += sBX2+"x² ";
   }
   //-----
   // Coefficient C
   if( ptCh->tWIMOMathPolynome3.dCoeffC != ( TFLOAT )0.0 ){
    // Conversion
    if(  ( ptCh->tWIMOMathPolynome3.dCoeffA != 0 )
      || ( ptCh->tWIMOMathPolynome3.dCoeffB != 0 ) ){
     // Ajout du signe
     sFormula += (ptCh->tWIMOMathPolynome3.dCoeffC>=0)?" + ":" - ";
     dTemp = (ptCh->tWIMOMathPolynome3.dCoeffC>=0)?ptCh->tWIMOMathPolynome3.dCoeffC:(-ptCh->tWIMOMathPolynome3.dCoeffC);
     sCX.sprintf( "%g", dTemp );
    }
    else{
     sCX.sprintf( "%g", ptCh->tWIMOMathPolynome3.dCoeffC );
    }
    sFormula += sCX+"x ";
   }
   //-----
   // Coefficient D
   if( ptCh->tWIMOMathPolynome3.dCoeffD != ( TFLOAT )0.0 ){
    // Conversion
    if(  ( ptCh->tWIMOMathPolynome3.dCoeffA != 0 )
      || ( ptCh->tWIMOMathPolynome3.dCoeffB != 0 )
      || ( ptCh->tWIMOMathPolynome3.dCoeffC != 0 ) ){
     // Ajout du signe
     sFormula += (ptCh->tWIMOMathPolynome3.dCoeffD>=0)?" + ":" - ";
     dTemp = (ptCh->tWIMOMathPolynome3.dCoeffD>=0)?ptCh->tWIMOMathPolynome3.dCoeffD:(-ptCh->tWIMOMathPolynome3.dCoeffD);
     sD.sprintf( "%g", dTemp );
    }
    else{
     sD.sprintf( "%g", ptCh->tWIMOMathPolynome3.dCoeffD );
    }
    sFormula += sD;
   }
   if(  ( ptCh->tWIMOMathPolynome3.dCoeffA == 0 )
     && ( ptCh->tWIMOMathPolynome3.dCoeffB == 0 )
     && ( ptCh->tWIMOMathPolynome3.dCoeffC == 0 )
     && ( ptCh->tWIMOMathPolynome3.dCoeffD == 0 ) ){
    sFormula = "y = 0";
   }
  break;
 }
 // Retourne le string
 return( ( QString )sFormula );
}

//----------------------------------------------------------------------------//
// Mise en forme d'une date nke en string
//----------------------------------------------------------------------------//
void vFWIMOModbusGetCalibrationDateTime( TUCHAR ucChannel, TCHAR * pcBufferDate )
{
 TDateTimeObject tDateTime;
 // Test d'intégrité
 if( ucChannel > ( TUCHAR )3 ) return;

 vFDATETIMESecToDateTime( &tDateTime, tWIMOParametersSensor.ttChannel[ucChannel].uliDateTimeCalibration );
 // Test si date non-initialisée
 if( tWIMOParametersSensor.ttChannel[ucChannel].uliDateTimeCalibration == ( TULONGINT )0xFFFFFFFF )
  {
   //strcpy( pcBufferDate, "--" );
   strncpy( pcBufferDate, "Not calibrated", strlen( "Not calibrated" ) );
   pcBufferDate[ strlen( "Not calibrated" ) ] = ( TCHAR )'\0';
  }
 else
  {
   pcBufferDate[0] = ( TCHAR )'2';
   pcBufferDate[1] = ( TCHAR )'0';
   bFDateTimeToStr( &pcBufferDate[2], &tDateTime );
  }
}

//----------------------------------------------------------------------------//
// Mise en forme d'une date nke en string
//----------------------------------------------------------------------------//
QString sFWIMOModbusGetCalibrationDateTime( TUCHAR ucChannel )
{
 TDateTimeObject tDateTime;
 QString sDatetime = "";
 TCHAR tcDateTime[64] = {0};

 // Test d'intégrité
 if( ucChannel > ( TUCHAR )3 ) return( ( QString )sDatetime );

 vFDATETIMESecToDateTime( &tDateTime, tWIMOParametersSensor.ttChannel[ucChannel].uliDateTimeCalibration );
 // Test si date non-initialisée
 if( tWIMOParametersSensor.ttChannel[ucChannel].uliDateTimeCalibration == ( TULONGINT )0xFFFFFFFF )
  {
   sDatetime = QObject::tr("Not calibrated");
  }
 else
  {
   tcDateTime[0] = ( TCHAR )'2';
   tcDateTime[1] = ( TCHAR )'0';
   bFDateTimeToStr( &tcDateTime[2], &tDateTime );
   sDatetime = tcDateTime;
  }
  return( ( QString )sDatetime );
}

//----------------------------------------------------------------------------//
// Demande numéro de version
//----------------------------------------------------------------------------//
void vFWIMOModbusGetVersionNumber( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
 // Si en mode modbus 9600
 if( ptMainData->getCOMSlowMode() )
  {
   vFWIMOModbusResetConfMeasurePeriod();
   return;
  }

 vFQDebug( "== vFWIMOModbusGetVersionNumber ==" );

 // Requête
 tModbus.ucReplySize = tModbusReadVersion.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadVersion.ucFunction,
                     ( TUCHAR * )tModbusReadVersion.tucRequest,
                     tModbusReadVersion.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_GET_VERSION;
   qDebug("tModbus.eState = eMODBUS_STATE_GET_VERSION");
   //Form2->cxLabel1->Caption = "Reading firmware version...";
   //Form2->Show();
  }
 // Echec
 else
  {
   qDebug("vFWIMOModbusGetVersionNumber > Send error");
   ptMainEvent->vFMainWindowErrorOpen("Communication error");
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// Demande numéro de version de l'interface/outil
//----------------------------------------------------------------------------//
void vFWIMOModbusGetInterfaceVersionNumber( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }


 vFQDebug( "== vFWIMOModbusGetInterfaceVersionNumber ==" );

 // Requête
 tModbus.ucReplySize = tModbusReadInterfaceVersion.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadInterfaceVersion.ucFunction,
                     ( TUCHAR * )tModbusReadInterfaceVersion.tucRequest,
                     tModbusReadInterfaceVersion.ucRequestSize ) )
  {
   //Sleep( 100 );
   qDebug( QTime::currentTime().toString().toLatin1() );
   tModbus.eState = eMODBUS_STATE_GET_INTERFACE_VERSION;
   qDebug("tModbus.eState = eMODBUS_STATE_GET_INTERFACE_VERSION");
  }
 // Echec
 else
  {
   qDebug("Send error");
   return;
  }
}

//----------------------------------------------------------------------------//
// Taille de réponse modbus
//----------------------------------------------------------------------------//
TUCHAR ucFWIMOModbusMessageSize( TUCHAR ucFunction )
{
 // On retourne la taille attendue
 return( ( TUCHAR )tModbus.ucReplySize );
}

//----------------------------------------------------------------------------//
// Déclenchement du balai
//----------------------------------------------------------------------------//
void vFWIMOModbusStartClean( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
 // Requête
 tModbus.ucReplySize = tModbusClean.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusClean.ucFunction, ( TUCHAR * )tModbusClean.tucRequest, tModbusClean.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_CLEAN;
  }
 // Echec
 else
  {
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// Déclenchement de la mesure
//----------------------------------------------------------------------------//
void vFWIMOModbusStartMeasure( void )
{
 // Si en état idle
 if( tModbus.eState == eMODBUS_STATE_FREE )
  {
   // Si en mode modbus 9600
   if( ptMainData->getCOMSlowMode() )
    {
     if( !tModbus.b9600Init )
      // Validation de la période de mesure modbus 9600
      vFWIMOModbusSetConfMeasurePeriod();
     // Demande de mesure
     else
      // On passe en récupération du résultat
      tModbus.eState = eMODBUS_STATE_GET_MEASURE_DATA_9600_CMD;
    }
   else
    {
     // On passe en état lancement de mesure
     tModbus.eState = eMODBUS_STATE_WAIT;
    }
  }
 else
  {
   qDebug("Measure already activated");
   // Pour debug
   vFWIMOModbusPrintDebugtMdobusState();
  }
}

//----------------------------------------------------------------------------//
// Ecriture de la periode de measure à 0 - Modbus 9600
//----------------------------------------------------------------------------//
static void vFWIMOModbusResetConfMeasurePeriod( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   // Déjà en cours
   qDebug( "vFWIMOModbusSetConfMeasurePeriod > Not free" );
   return;
  }
 qDebug( "vFWIMOModbusSetConfMeasurePeriod > Send CMD" );
 // Requête
 tModbus.ucReplySize = tModbusResetMeasurePeriod.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80,
                     tModbusResetMeasurePeriod.ucFunction,
                     ( TUCHAR * )tModbusResetMeasurePeriod.tucRequest,
                     tModbusResetMeasurePeriod.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_RESET_CONF_MEASURE_PERIOD;
  }
 // Echec
 else
  {
   qDebug("vFWIMOModbusSetConfMeasurePeriod > Sending error");
   return;
  }
}

//----------------------------------------------------------------------------//
// Ecriture de la periode de measure à 10 - Modbus 9600
//----------------------------------------------------------------------------//
static void vFWIMOModbusSetConfMeasurePeriod( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   // Déjà en cours
   qDebug( "vFWIMOModbusSetConfMeasurePeriod > Not free" );
   return;
  }
 qDebug( "vFWIMOModbusSetConfMeasurePeriod > Send CMD" );
 // Requête
 tModbus.ucReplySize = tModbusSetMeasurePeriod1sec.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80,
                     tModbusSetMeasurePeriod1sec.ucFunction,
                     ( TUCHAR * )tModbusSetMeasurePeriod1sec.tucRequest,
                     tModbusSetMeasurePeriod1sec.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_SET_CONF_MEASURE_PERIOD;
  }
 // Echec
 else
  {
   qDebug("vFWIMOModbusSetConfMeasurePeriod > Sending error");
   return;
  }
}

//----------------------------------------------------------------------------//
// Sauvegarde de la configuration dans le capteur (factory)
//----------------------------------------------------------------------------//
void vFWIMOModbusWriteConfigToSensorFactory( void )
{
 // Marquage du flag
 bFactoryDataRefresh = ( TBOOL )aTRUE;
 // Sauvegarde de la configuration
 vFWIMOModbusSaveConfigToSensor();
}

//----------------------------------------------------------------------------//
// Sauvegarde de la configuration dans le capteur
//----------------------------------------------------------------------------//
void vFWIMOModbusSaveConfigToSensor( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
 // Requête
 tModbus.ucReplySize = tModbusWriteConfP.ucReplySize;
 memcpy( tModbusWriteConfP.tucData, tWIMOParametersSensor.tHeader.tucBuffer, sizeof( TWIMOParametersHeaderObject ) );
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusWriteConfP.ucFunction, ( TUCHAR * )tModbusWriteConfP.tucRequest, tModbusWriteConfP.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_SET_CONF_PRODUCT;
   qDebug("tModbus.eState = eMODBUS_STATE_SET_CONF_PRODUCT");
   //Form2->cxLabel1->Caption = "Reading firmware version...";
   //Form2->Show();
  }
 // Echec
 else
  {
   vFQDebug("vFWIMOModbusSaveConfigToSensor > Send error");
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// Sauvegarde du channel 0 dans le capteur
//----------------------------------------------------------------------------//
void vFWIMOModbusSaveGeneralConfOnlyToSensor( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   vFQDebug("vFWIMOModbusSaveGeneralConfOnlyToSensor > Not free");
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
 vFQDebug("vFWIMOModbusSaveGeneralConfOnlyToSensor > Sending");
 // Requête
 tModbus.ucReplySize = tModbusWriteConfP.ucReplySize;
 memcpy( tModbusWriteConfP.tucData, tWIMOParametersSensor.tHeader.tucBuffer, sizeof( TWIMOParametersHeaderObject ) );
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusWriteConfP.ucFunction, ( TUCHAR * )tModbusWriteConfP.tucRequest, tModbusWriteConfP.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_SET_CONF_PRODUCT_ONLY;
   //%%AP - 2021.10.07 - Délais :  la lecture se fait entre la carte interface carte
   Sleep( 100 );
   //Form2->cxLabel1->Caption = "Writing channel 0 configuration...";
  }
 // Echec
 else
  {
   vFQDebug("vFWIMOModbusSaveGeneralConfOnlyToSensor > Send error");
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// Sauvegarde de la configuration d'un channel dans le capteur
//----------------------------------------------------------------------------//
void vFWIMOModbusSaveChannelToSensor( TUCHAR ucChannel )
{
 // Test intégrité
 if( ucChannel >= ( TUCHAR )3 ) return;

 // Réinit
 tModbus.ucRequestRetry = ( TUCHAR )0;

 // Selon la voie utilisée
 switch( ucChannel )
  {
   //----------
   // Voie 0
   //----------
   case( ( TUCHAR )0 ):
    // Sauvegarde de la configuration de la voie
    vFWIMOModbusSaveCh0ToSensor();
   break;
   //----------
   // Voie 1
   //----------
   case( ( TUCHAR )1 ):
    // Sauvegarde de la configuration de la voie
    vFWIMOModbusSaveCh1ToSensor();
   break;
   //----------
   // Voie 2
   //----------
   case( ( TUCHAR )2 ):
    // Sauvegarde de la configuration de la voie
    vFWIMOModbusSaveCh2ToSensor();
    // Reset point de calibration
    vFCalibrationResetPointToQmlWithChannel(2);
   break;
   default:
   break;
  }
}

//----------------------------------------------------------------------------//
// Sauvegarde du channel 0 dans le capteur
//----------------------------------------------------------------------------//
static void vFWIMOModbusSaveCh0ToSensor( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   vFQDebug("vFWIMOModbusSaveCh0ToSensor > Not free");
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }

 vFQDebug("vFWIMOModbusSaveCh0ToSensor > Sending");
 // Requête
 tModbus.ucReplySize = tModbusWriteConf0.ucReplySize;
 memcpy( tModbusWriteConf0.tucData, tWIMOParametersSensor.ttChannel[0].tucBuffer, sizeof( TWIMOParametersChannelObject ) );
 // Envoi de la requête
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusWriteConf0.ucFunction, ( TUCHAR * )tModbusWriteConf0.tucRequest, tModbusWriteConf0.ucRequestSize ) )
  {
   // Modification de l'état
   tModbus.eState = eMODBUS_STATE_SET_CONF_CH0_ONLY;
   // Délais :  la lecture se fait entre la carte interface carte
   Sleep( 100 );
  }
 // Echec
 else
  {
   vFQDebug("vFWIMOModbusSaveCh0ToSensor > Send error ch0 1");
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// Sauvegarde du channel 1 dans le capteur
//----------------------------------------------------------------------------//
static void vFWIMOModbusSaveCh1ToSensor( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   vFQDebug("vFWIMOModbusSaveCh1ToSensor > Not free");
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }

 vFQDebug("vFWIMOModbusSaveCh1ToSensor > Sending");
 // Requête
 tModbus.ucReplySize = tModbusWriteConf1.ucReplySize;
 memcpy( tModbusWriteConf1.tucData, tWIMOParametersSensor.ttChannel[1].tucBuffer, sizeof( TWIMOParametersChannelObject ) );
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusWriteConf1.ucFunction, ( TUCHAR * )tModbusWriteConf1.tucRequest, tModbusWriteConf1.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_SET_CONF_CH1_ONLY;
   // Délais :  la lecture se fait entre la carte interface carte
   Sleep( 100 );
  }
 // Echec
 else
  {
   vFQDebug("vFWIMOModbusSaveCh1ToSensor > Send error Ch1 1");
   return;
  }
}

//----------------------------------------------------------------------------//
// Sauvegarde du channel 2 dans le capteur
//----------------------------------------------------------------------------//
static void vFWIMOModbusSaveCh2ToSensor( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   vFQDebug("vFWIMOModbusSaveCh2ToSensor > Not free");
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }

 vFQDebug("vFWIMOModbusSaveCh2ToSensor > Sending");
 // Requête
 tModbus.ucReplySize = tModbusWriteConf2.ucReplySize;
 memcpy( tModbusWriteConf2.tucData, tWIMOParametersSensor.ttChannel[2].tucBuffer, sizeof( TWIMOParametersChannelObject ) );
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusWriteConf2.ucFunction, ( TUCHAR * )tModbusWriteConf2.tucRequest, tModbusWriteConf2.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_SET_CONF_CH2_ONLY;
   // Délais :  la lecture se fait entre la carte interface carte
   Sleep( 100 );
  }
 // Echec
 else
  {
   vFQDebug("vFWIMOModbusSaveCh2ToSensor > Send error Ch2 1");
   return;
  }
}

//----------------------------------------------------------------------------//
// Get de la configuration d'un channel dans le capteur
//----------------------------------------------------------------------------//
void vFWIMOModbusGetChannelFromSensor( TUCHAR ucChannel )
{
 // Test intégrité
 if( ucChannel >= ( TUCHAR )3 ) return;

 // Selon la voie utilisée
 switch( ucChannel )
  {
   //----------
   // Voie 0
   //----------
   case( ( TUCHAR )0 ):
    // Sauvegarde de la configuration de la voie
    vFWIMOModbusGetCh0FromSensor();
   break;
   //----------
   // Voie 1
   //----------
   case( ( TUCHAR )1 ):
    // Sauvegarde de la configuration de la voie
    vFWIMOModbusGetCh1FromSensor();
   break;
   //----------
   // Voie 2
   //----------
   case( ( TUCHAR )2 ):
    // Sauvegarde de la configuration de la voie
    vFWIMOModbusGetCh2FromSensor();
   break;
   default:
   break;
  }
}

//----------------------------------------------------------------------------//
// Get du channel 0 dans le capteur
//----------------------------------------------------------------------------//
static void vFWIMOModbusGetCh0FromSensor( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }

 vFQDebug( "== vFWIMOModbusGetCh0FromSensor ==" );

 // Requête
 tModbus.ucReplySize = tModbusReadConf0.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadConf0.ucFunction, ( TUCHAR * )tModbusReadConf0.tucRequest, tModbusReadConf0.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_GET_CONF_CH0_ONLY;
   // Délais :  la lecture se fait entre la carte interface carte
   Sleep( 100 );
   //Form2->cxLabel1->Caption = "Writing channel 0 configuration...";
  }
 // Echec
 else
  {
   qDebug("Send error");
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// Get du channel 1 dans le capteur
//----------------------------------------------------------------------------//
static void vFWIMOModbusGetCh1FromSensor( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }

 vFQDebug( "== vFWIMOModbusGetCh1FromSensor ==" );

 // Requête
 tModbus.ucReplySize = tModbusReadConf1.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadConf1.ucFunction, ( TUCHAR * )tModbusReadConf1.tucRequest, tModbusReadConf1.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_GET_CONF_CH1_ONLY;
   // Délais :  la lecture se fait entre la carte interface carte
   Sleep( 100 );
   //Form2->cxLabel1->Caption = "Writing channel 0 configuration...";
  }
 // Echec
 else
  {
   qDebug("Send error");
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// Get du channel 2 dans le capteur
//----------------------------------------------------------------------------//
static void vFWIMOModbusGetCh2FromSensor( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }

 vFQDebug( "== vFWIMOModbusGetCh2FromSensor ==" );

 // Requête
 tModbus.ucReplySize = tModbusReadConf2.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadConf2.ucFunction, ( TUCHAR * )tModbusReadConf2.tucRequest, tModbusReadConf2.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_GET_CONF_CH2_ONLY;
   // Délais :  la lecture se fait entre la carte interface carte
   Sleep( 100 );
   //Form2->cxLabel1->Caption = "Writing channel 0 configuration...";
  }
 // Echec
 else
  {
   qDebug("Send error");
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// Démarrage mise à jour capteur
//----------------------------------------------------------------------------//
void vFWIMOModbusStartUpdate( QString qUrl )
{
 QByteArray qByteArray;

 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
 // Création de l'handler de fichier
 QFile tFileHandler( qUrl );
 // Ouverture du fichier *.ini
 if( tFileHandler.open( QIODevice::ReadOnly ) )
  {
   //tucWIMOUpdateData
   qByteArray = tFileHandler.readAll();
   tFileHandler.close();
   // Vérification qu'on a le bon nombre d'octet
   if( qByteArray.length() == 65536 )
    {
     // Conversion de donnée
     memcpy( tucWIMOUpdateData, qByteArray.data(), sizeof( tucWIMOUpdateData ) );
     // Requête
     tModbus.ucReplySize = tModbusStartUpdate.ucReplySize;
     if( tModBus.bFSend( ( TUCHAR )0x80, tModbusStartUpdate.ucFunction,
                         ( TUCHAR * )tModbusStartUpdate.tucRequest,
                         tModbusStartUpdate.ucRequestSize ) )
      {
       qDebug("Start eMODBUS_STATE_UPDATE");
       tModbus.eState = eMODBUS_STATE_UPDATE;
       return;
      }
     // Echec
     else
      {
       return;
      }
    }
  }
 else
  {
   qDebug("Can't open"+qUrl.toLatin1());
   return;
  }
}

//----------------------------------------------------------------------------//
// Interface - CMD de passage en mode bootloader
//----------------------------------------------------------------------------//
bool bFWIMOModbusStartUpdateInterfaceCMD( QByteArray &qByteArray )
{
 // Conversion de donnée
 memcpy( tucWIMOUpdateData, qByteArray.data(), sizeof( tucWIMOUpdateData ) );

 // Requête : passage en mode update/bootloader
 tModbus.ucReplySize = tModbusStartInterfaceUpdate.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusStartInterfaceUpdate.ucFunction,
                     ( TUCHAR * )tModbusStartInterfaceUpdate.tucRequest,
                     tModbusStartInterfaceUpdate.ucRequestSize ) )
  {
   // La requête est executée
   return( ( bool )true );
  }
 // Erreur
 return( ( bool )false );
}

//----------------------------------------------------------------------------//
// Reset capteur
//----------------------------------------------------------------------------//
void vFWIMOModbusStartReset( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   return;
  }
 // Requête
 tModbus.ucReplySize = tModbusReset.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80,
                     tModbusReset.ucFunction,
                     ( TUCHAR * )tModbusReset.tucRequest,
                     tModbusReset.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_RESET;
  }
 // Echec
 else
  {
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// O2 - Passage du capteur en mode calibration
//----------------------------------------------------------------------------//
void vFWIMOModbusO2SwitchToCalibrationMode( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   qDebug("vFWIMOModbusO2SwitchToCalibrationMode > tModbus.eState != eMODBUS_STATE_FREE");
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }

 qDebug( "vFWIMOModbusO2SwitchToCalibrationMode > Send request" );
 // Requête 1 - Envoi de la commande pour lecture dans le capteur
 tModbus.ucReplySize = tModbusO2SwitchToCalibrationModeCMD.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusO2SwitchToCalibrationModeCMD.ucFunction, ( TUCHAR * )tModbusO2SwitchToCalibrationModeCMD.tucRequest, tModbusO2SwitchToCalibrationModeCMD.ucRequestSize ) )
  {
   qDebug( "vFWIMOModbusO2SwitchToCalibrationMode > Request sent" );
   tModbus.eState = eMODBUS_STATE_O2_SWITCH_TO_CALIBRATION_MODE_CMD;
   //while( tModbus.eState != eMODBUS_STATE_FREE ) vFMainProcess();
   // Délais
   Sleep( 100 );
  }
 // Echec
 else
  { 
   qDebug( "vFWIMOModbusO2SwitchToCalibrationMode > Request error send" );
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// O2 - Lecture de la structure
//----------------------------------------------------------------------------//
void vFWIMOModbus_O2_ReadConfFromI2CSensor( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }

  vFQDebug( "== vFWIMOModbus_O2_ReadConfFromI2CSensor ==" );


 // Requête
 tModbus.ucReplySize = tModbusReadConfO2.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadConfO2.ucFunction, ( TUCHAR * )tModbusReadConfO2.tucRequest, tModbusReadConfO2.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_GET_CONF_O2;
   // Délais :  la lecture se fait entre la carte interface carte
   Sleep( 100 );
  }
 // Echec
 else
  {
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// O2 - Envoi de la commande de lecture
//----------------------------------------------------------------------------//
void vFWIMOModbus_O2_sendReadCMD( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }

 // Requête 1 - Envoi de la commande pour lecture dans le capteur
 tModbus.ucReplySize = tModbusO2ReadCMD.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusO2ReadCMD.ucFunction, ( TUCHAR * )tModbusO2ReadCMD.tucRequest, tModbusO2ReadCMD.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_SET_O2_READ_CMD;
   // Délais :  la lecture se fait entre la carte interface carte
   Sleep( 100 );
  }
 // Echec
 else
  {
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// O2 - Envoi de la commande de lecture de l'offset en temperature
//----------------------------------------------------------------------------//
void vFWIMOModbus_O2_sendReadTemperatureOffsetCMD( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }

 // Requête 1 - Envoi de la commande pour lecture dans le capteur
 tModbus.ucReplySize = tModbusO2ReadTemperatureOffsetCMD.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusO2ReadTemperatureOffsetCMD.ucFunction, ( TUCHAR * )tModbusO2ReadTemperatureOffsetCMD.tucRequest, tModbusO2ReadTemperatureOffsetCMD.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_SET_O2_READ_TEMPERATURE_OFFSET_CMD;
   // Délais :  la lecture se fait entre la carte interface carte
   Sleep( 100 );
  }
 // Echec
 else
  {
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// O2 - Envoi de la commande d'écriture de l'offset en temperature
//----------------------------------------------------------------------------//
void vFWIMOModbus_O2_sendWriteTemperatureOffsetCMD( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }

 // Requête
 tModbus.ucReplySize = tModbusO2WriteTemperatureOffsetCMD.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80,
                     tModbusO2WriteTemperatureOffsetCMD.ucFunction,
                     ( TUCHAR * )tModbusO2WriteTemperatureOffsetCMD.tucRequest,
                     tModbusO2WriteTemperatureOffsetCMD.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_SET_O2_WRITE_TEMPERATURE_OFFSET_CMD;
   // Délais :  la lecture se fait entre la carte interface carte
   Sleep( 100 );
  }
 // Echec
 else
  {
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// O2 - Ecriture de la structure
//----------------------------------------------------------------------------//
void vFWIMOModbusWriteO2Struct( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }

 // Requête
 tModbus.ucReplySize = tModbusWriteConfO2.ucReplySize;
 memcpy( tModbusWriteConfO2.tucData, &tWIMOPyroO2Configuration, tModbusWriteConfO2.ucRequestSize );
 if( tModBus.bFSend( ( TUCHAR )0x80,
                     tModbusWriteConfO2.ucFunction,
                     ( TUCHAR * )tModbusWriteConfO2.tucRequest,
                     tModbusWriteConfO2.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_SET_CONF_O2;
   // Délais :  la lecture se fait entre la carte interface carte
   Sleep( 100 );
  }
 // Echec
 else
  {
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// O2 - Set point manually
//----------------------------------------------------------------------------//
void vFWIMOModbusSetPointManually( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }

 // Requête 1 - Envoi de la configurationvers l'interface
 tModbus.ucReplySize = tModbusWriteConfO2.ucReplySize;
 memcpy( tModbusWriteConfO2.tucData, &tWIMOPyroO2Configuration, sizeof( TWIMOPyroO2ConfigurationObject ) );
 if( tModBus.bFSend( ( TUCHAR )0x80,
                     tModbusWriteConfO2.ucFunction,
                     ( TUCHAR * )tModbusWriteConfO2.tucRequest,
                     tModbusWriteConfO2.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_SET_CONF_O2;
   while( tModbus.eState != eMODBUS_STATE_FREE ) vFMainProcess();
   // Délais :  la lecture se fait entre la carte interface carte
   Sleep( 100 );
   // Requête
   tModbus.ucReplySize = tModbusO2SetPointManuallyCMD.ucReplySize;
   if( tModBus.bFSend( ( TUCHAR )0x80, tModbusO2SetPointManuallyCMD.ucFunction, ( TUCHAR * )tModbusO2SetPointManuallyCMD.tucRequest, tModbusO2SetPointManuallyCMD.ucRequestSize ) )
    {
     tModbus.eState = eMODBUS_STATE_SET_O2_SET_POINT_MANUALLY_CMD;
     while( tModbus.eState != eMODBUS_STATE_FREE ) vFMainProcess();
     // Délais :  la lecture se fait entre la carte interface carte
     Sleep( 100 );
     // Sauvegarde des paramètres
     // Requête
     tModbus.ucReplySize = tModbusReadConfO2.ucReplySize;
     if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadConfO2.ucFunction, ( TUCHAR * )tModbusReadConfO2.tucRequest, tModbusReadConfO2.ucRequestSize ) )
      {
       tModbus.eState = eMODBUS_STATE_SET_O2_SAVE;
       while( tModbus.eState != eMODBUS_STATE_FREE ) vFMainProcess();
       // Délais :  la lecture se fait entre la carte interface carte
       Sleep( 100 );
       // Requête - Envoi de la commande pour lecture dans le capteur
       tModbus.ucReplySize = tModbusO2ReadCMD.ucReplySize;
       if( tModBus.bFSend( ( TUCHAR )0x80, tModbusO2ReadCMD.ucFunction, ( TUCHAR * )tModbusO2ReadCMD.tucRequest, tModbusO2ReadCMD.ucRequestSize ) )
        {
         tModbus.eState = eMODBUS_STATE_SET_O2_READ_CMD;
         while( tModbus.eState != eMODBUS_STATE_FREE ) vFMainProcess();
         // Délais :  la lecture se fait entre la carte interface carte
         Sleep( 100 );
         // Requête
         tModbus.ucReplySize = tModbusReadConfO2.ucReplySize;
         if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadConfO2.ucFunction, ( TUCHAR * )tModbusReadConfO2.tucRequest, tModbusReadConfO2.ucRequestSize ) )
          {
           tModbus.eState = eMODBUS_STATE_GET_CONF_O2;
           while( tModbus.eState != eMODBUS_STATE_FREE ) vFMainProcess();
           // Délais :  la lecture se fait entre la carte interface carte
           Sleep( 100 );
          }
        }
      }
     // Echec
     else
      {
       //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
       return;
      }
    }
   // Echec
   else
    {
     //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
     return;
    }
  }
 // Echec
 else
  {
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// O2 - Envoi de la configuration O2 vers l'interface I2C
//----------------------------------------------------------------------------//
void vFWIMOModbus_O2_sendConfToI2CSensor( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }

 // Requête 1 - Envoi de la configurationvers l'interface
 tModbus.ucReplySize = tModbusWriteConfO2.ucReplySize;
 memcpy( tModbusWriteConfO2.tucData, &tWIMOPyroO2Configuration, sizeof( TWIMOPyroO2ConfigurationObject ) );
 if( tModBus.bFSend( ( TUCHAR )0x80,
                     tModbusWriteConfO2.ucFunction,
                     ( TUCHAR * )tModbusWriteConfO2.tucRequest,
                     tModbusWriteConfO2.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_SET_CONF_O2;
   // Délais
   Sleep( 100 );
  }
 // Echec
 else
  {
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   qDebug("Communication error ! ");
   return;
  }
}

//----------------------------------------------------------------------------//
// O2 - CMD pour écriture de la configuration capteur initiale
//----------------------------------------------------------------------------//
void vFWIMOModbus_O2_sendConfigWriteCMD( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
 // Requête
 tModbus.ucReplySize = tModbusO2SetSensorSettingsCMD.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusO2SetSensorSettingsCMD.ucFunction, ( TUCHAR * )tModbusO2SetSensorSettingsCMD.tucRequest, tModbusO2SetSensorSettingsCMD.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_SET_O2_SET_SENSOR_SETTINGS_CMD;
   //%%AP - 2021.09.14
   //while( tModbus.eState != eMODBUS_STATE_FREE ) vFMainProcess();
   // Délais :  la lecture se fait entre la carte interface carte
   Sleep( 500 );
  }
 // Echec
 else
  {
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   qDebug("Communication error !");
   return;
  }
}

//----------------------------------------------------------------------------//
// O2 - Set air commande
//----------------------------------------------------------------------------//
void vFWIMOModbusO2SetAir( void )
{
 char tcString[16];

 qDebug( "== vFWIMOModbusO2SetAir ==" );
 qDebug( "%d", (tModbus.eState != eMODBUS_STATE_FREE) );
 qDebug( "tModbus.eState : %d", tModbus.eState );

 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   qDebug("vFWIMOModbusO2SetAir() > Not free exit");
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
 qDebug( "vFWIMOModbusO2SetAir() > Envoi de la commande" );

 // Requête
 tModbus.ucReplySize = tModbusO2Set100.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusO2Set100.ucFunction, ( TUCHAR * )tModbusO2Set100.tucRequest, tModbusO2Set100.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_SET_O2_100;
   //%%AP - 2021.09.14
   //while( tModbus.eState != eMODBUS_STATE_FREE ) vFMainProcess();
   // Délais :  la lecture se fait entre la carte interface carte
   //Sleep( 6000 );
   Sleep( 500 );

  }
 // Echec
 else
  {
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
}

//----------------------------------------------------------------------------//
// O2 - Lecture de la structure 
//----------------------------------------------------------------------------//
void vFWIMOModbusO2Set0( void )
{
 char tcString[16];


 qDebug( "== vFWIMOModbusO2Set0 ==" );
 qDebug( "tModbus.eState != eMODBUS_STATE_FREE : %d", (tModbus.eState != eMODBUS_STATE_FREE) );
 qDebug( "tModbus.eState : %d", tModbus.eState );

 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE )
  {
   qDebug("vFWIMOModbusO2Set0() > Not free exit");
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 );
   return;
  }
 qDebug( "vFWIMOModbusO2Set0() > Envoi de la commande" );

 // Requête
 tModbus.ucReplySize = tModbusO2Set0.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusO2Set0.ucFunction, ( TUCHAR * )tModbusO2Set0.tucRequest, tModbusO2Set0.ucRequestSize ) )
  {
   tModbus.eState = eMODBUS_STATE_SET_O2_0;
   //Sleep( 6000 );
   //%%AP - 2021.09.14
   Sleep( 500 );
  }
 // Echec
 else
  {
   // Erreur
   qDebug("vFWIMOModbusO2Set0 - Error 3");
   tModbus.eState = eMODBUS_STATE_FREE;
   return;
  }
}

//----------------------------------------------------------------------------//
// O2 - Save calibration commande
//----------------------------------------------------------------------------//
void vFWIMOModbusO2Save( void )
{
 // Gestion appui
 if( tModbus.eState != eMODBUS_STATE_FREE ) 
  {
   //MessageDlg( "Action impossible. Another action is already running !", mtError, TMsgDlgButtons() << mbOK, 0 ); 
   return; 
  }
 // Requête
 tModbus.ucReplySize = tModbusO2Save.ucReplySize;
 if( tModBus.bFSend( ( TUCHAR )0x80, tModbusO2Save.ucFunction, ( TUCHAR * )tModbusO2Save.tucRequest, tModbusO2Save.ucRequestSize ) ) 
  {
   tModbus.eState = eMODBUS_STATE_SET_O2_SAVE;
   //while( tModbus.eState != eMODBUS_STATE_FREE ) vFMainProcess();
   //%%AP - 2021.09.14
   // Délais :  la lecture se fait entre la carte interface
   Sleep( 500 );
  }
 // Echec
 else 
  { 
   // Erreur
   qDebug("vFWIMOModbusO2Save - Error 1");
   tModbus.eState = eMODBUS_STATE_FREE;
   //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 ); 
   return; 
  }
}

//----------------------------------------------------------------------------//
// Test si le grafcet est à l'état de repos
//----------------------------------------------------------------------------//
bool bIsWIMOModbusGrafcetBusy( void )
{
 // Test de l'état du grafcet
 return( ( bool )( tModbus.eState != eMODBUS_STATE_FREE ) );
}

// Timeout timer
bool bTimeout = false;
void vFTimeoutTimer(void){
 qDebug("Timeout");
 bTimeout = true;
}

//----------------------------------------------------------------------------//
// Gestion Modbus
//----------------------------------------------------------------------------//
TBOOL bGWIMOModbusManager( void )
{
 TModBusDriverDataObject * ptModBusDriverData;
 TBOOL bActive = ( TBOOL )aFALSE;
 static TULONGINT uliSecond = ( TULONGINT )0;
 static QDateTime tNow;
 TFLOAT tfResult[3];
 TUCHAR ucIndex;
 TUCHAR ucDebugResult = 0;
 TDOUBLE dRaw, dPhy;
 QDateTime qDateTime;
 QString sTemp, sCh0, sCh1, sCh2;
 TDateTimeObject tDateTime;
 TCHAR tcBufferDate[sizeof( "HH:MM:SS JJ/MM/AA" ) + 2 + 1];
 static TUINT uiUpdateIndex;
 QStringList tChannelList;

 char tcBuffer[1024], tcString[64];
 union
  {
   TULONGINT uliValue;
   TUCHAR tucValue[ sizeof( TULONGINT ) ];
  }
 tLong;


 if( tModbus.eState != eMODBUS_STATE_FREE )
  // Gestion Modbus composant
  bActive = tModBus.bGScheduler();

 // Selon l'état Modbus
 switch( tModbus.eState )
  {
   //------
   // Libre
   //------
   case( eMODBUS_STATE_FREE ):
   default :
   break;
   //------
   // Reset
   //------
   case( eMODBUS_STATE_RESET ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      qDebug("eMODBUS_STATE_RESET");
      //Sleep(2000);

      // Affichage
      //Form2->Show(); Form2->cxLabel1->Caption = "Sensor is resetting...";

      //QTimer *timer = new QTimer();
      //timer->setSingleShot(true);
      //QTimer *timer = new QTimer();
      QTimer timer;
      QObject::connect(&timer, &QTimer::timeout, vFTimeoutTimer);

      //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
      timer.start(100);
      // Attente
      for( uiUpdateIndex = 0; uiUpdateIndex < 21; uiUpdateIndex++ )
       {
        //timer = new QTimer();
        //timer->setSingleShot(true);
        //timer->start(80);
        while( !bTimeout )
         {
          QCoreApplication::processEvents();
          QThread::msleep(1);
         }
        bTimeout = false;

        // Affichage loader
        ptFactoryEvent->resetLoaderValue( uiUpdateIndex * 5 );
       }
      timer.stop();
      qDebug("eMODBUS_STATE_RESET END");


      // Terminé
      //Form2->Close();
      tModbus.eState = eMODBUS_STATE_FREE;
     }
    // Echec
    else
     {
      //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
   //------------
   // Mise à jour
   //------------
   case( eMODBUS_STATE_UPDATE ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      // Attente
      Sleep( 2000 );
      // Affichage
      //Form2->cxLabel1->Caption = "Downloading firmware...";
      // Requête
      tModbus.ucReplySize = tModbusUpdateCommand.ucReplySize;
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusUpdateCommand.ucFunction,
                          ( TUCHAR * )tModbusUpdateCommand.tucRequest,
                          tModbusUpdateCommand.ucRequestSize ) )
       {
        qDebug("Start eMODBUS_STATE_UPDATING_1");
        tModbus.eState = eMODBUS_STATE_UPDATING_1;
       }
      // Echec
      else
       {
        qDebug( "Communication error ! 1" );
        //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
        tModbus.eState = eMODBUS_STATE_FREE;
        //Form2->Close();
       }
     }
    // Echec
    else
     {
      qDebug( "Communication error ! 2" );
      //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
   //-----------------
   // UPDATING 1
   //-----------------
   case eMODBUS_STATE_UPDATING_1 :
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      qDebug("eMODBUS_STATE_UPDATING_1");
      // Attente
      Sleep( 10 );
      // Initialisation
      uiUpdateIndex = ( TUINT )0;
      // Requête
      tModbus.ucReplySize = tModbusUpdateData.ucReplySize;
      memcpy( &tModbusUpdateData.tucRequest[5], &tucWIMOUpdateData[ uiUpdateIndex * 246 ], 246 );
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusUpdateData.ucFunction, ( TUCHAR * )tModbusUpdateData.tucRequest, tModbusUpdateData.ucRequestSize ) )
       {
        qDebug("Start eMODBUS_STATE_UPDATING_2");
        tModbus.eState = eMODBUS_STATE_UPDATING_2;
       }
      // Echec
      else
       {
        qDebug( "Communication error ! 3" );
        //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
        tModbus.eState = eMODBUS_STATE_FREE;
        //Form2->Close();
       }
     }
    // Echec
    else
     {
      qDebug( "Communication error ! 4" );
      //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 ); u
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
   //-----------------
   // UPDATING 2
   //-----------------
   case( eMODBUS_STATE_UPDATING_2 ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      qDebug("eMODBUS_STATE_UPDATING_2");
      // Attente
      Sleep( 10 );
      // Terminé
      if( ++uiUpdateIndex > ( TUINT )267 )
       {
        // Requête
        tModbus.ucReplySize = tModbusUpdateStop.ucReplySize;
        if( tModBus.bFSend( ( TUCHAR )0x80, tModbusUpdateStop.ucFunction, ( TUCHAR * )tModbusUpdateStop.tucRequest, tModbusUpdateStop.ucRequestSize ) )
         {
          qDebug("Start eMODBUS_STATE_UPDATING_3");
          tModbus.eState = eMODBUS_STATE_UPDATING_3;
         }
        // Echec
        else
         {
          qDebug( "Communication error ! 5" );
          //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
          tModbus.eState = eMODBUS_STATE_FREE;
          //Form2->Close();
         }
       }
      // On continu
      else
       {
        // Affichage
        //Form2->cxProgressBar1->Position = 100 * uiUpdateIndex / 267; Application->ProcessMessages();
        // Sensor update signal
        emit ptSensorUpdateControler->updateProgressSignal( 95 * uiUpdateIndex / 267 );
        //
        ptFactoryEvent->updateLoaderValue( 95 * uiUpdateIndex / 267 );
        //ptFactoryEvent->updateLoaderValue( 100 * uiUpdateIndex / 267 );
        // Requête
        tModbus.ucReplySize = tModbusUpdateData.ucReplySize;
        memcpy( &tModbusUpdateData.tucRequest[5], &tucWIMOUpdateData[ uiUpdateIndex * 246 ], 246 );
        if( tModBus.bFSend( ( TUCHAR )0x80, tModbusUpdateData.ucFunction, ( TUCHAR * )tModbusUpdateData.tucRequest, tModbusUpdateData.ucRequestSize ) )
         {
          tModbus.eState = eMODBUS_STATE_UPDATING_2;
         }
        // Echec
        else
         {
          qDebug( "Communication error ! 6" );
          //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
          tModbus.eState = eMODBUS_STATE_FREE;
          //Form2->Close();
         }
       }
     }
    // Echec
    else
     {
      qDebug( "Communication error ! 7" );
      //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
   //-----------------
   // UPDATING 3
   //-----------------
   case( eMODBUS_STATE_UPDATING_3 ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      qDebug("eMODBUS_STATE_UPDATING_3 : END");
      // Attente
      Sleep( 10 );
      // Requête
      tModbus.ucReplySize = tModbusReadVersion.ucReplySize;
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadVersion.ucFunction, ( TUCHAR * )tModbusReadVersion.tucRequest, tModbusReadVersion.ucRequestSize ) )
       {
        tModbus.eState = eMODBUS_STATE_GET_VERSION;
        // Debug
        qDebug("tModbus.eState = eMODBUS_STATE_GET_VERSION");
        //Form2->cxLabel1->Caption = "Reading firmware version...";
        //Form2->Show();
       }
      else
       {
        qDebug("Send error");
        tModbus.eState = eMODBUS_STATE_FREE;
        //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
        break;
       }

      // Terminé
      //tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
    // Echec
    else
     {
      qDebug( "Communication error ! 8" );
      //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
   //---------------
   // Attente mesure
   //---------------
   case( eMODBUS_STATE_WAIT ):
    // Tant qu'on est toujours dans la seconde de l'ancienne mesure : pas de mesure
    //if( uliSecond == ( TULONGINT )NowInSec() ) break;
    //%%AP - Modif pour debug
    //if( ( TULONGINT )Now().time().second() > ( uliSecond + 5 ) ) break;

   /*
    qDebug( "uliSecond : %d", uliSecond );
    qDebug( "Now().time().second() : %d", Now().time().second() );
    qDebug("Start mesure");
  */
    vFQDebug("eMODBUS_STATE_WAIT > Start mesure CMD sending");
    // Date de l'interrogation
    tNow = Now();
    uliSecond = ( TULONGINT )tNow.time().second();
    // Nombre d'interrogation flag
    tModbus.uiMeasureTimeoutCpt = 10;
    // Initialisation
    //tModBus.vFInitialization();
    // Requête de démarrage d'une mesure
    tModbus.ucReplySize = tModbusMeasure.ucReplySize;
    if( tModBus.bFSend( ( TUCHAR )0x80, tModbusMeasure.ucFunction, ( TUCHAR * )tModbusMeasure.tucRequest, tModbusMeasure.ucRequestSize ) )
     {
      // Changement d'état
      tModbus.eState = eMODBUS_STATE_MEASURE;
      //Sleep( 500 );
     }
    else
     {
      vFQDebug( "eMODBUS_STATE_WAIT Error 1" );
      // Changement d'état
      tModbus.eState = eMODBUS_STATE_FREE;//eMODBUS_STATE_WAIT;
     }
   break;
   //-----------------
   // Lancement mesure
   //-----------------
   case( eMODBUS_STATE_MEASURE ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      //qDebug( "eMODBUS_STATE_MEASURE" );
      //%%AP - 2020.06.15 - Modification temporaire
      // Attente
      Sleep( 5 );
      //%%AP - 2022.11.09 - Test diminution du timing
      //Sleep( 1 ); // Fonctionne

      qDebug( "eMODBUS_STATE_MEASURE: %d + 200ms (sécu)", tWIMOParametersSensor.tHeader.tSensor.uiAcquisitionTime );
      //Sleep( 200 );
      //qDebug( "tWIMOParametersSensor.tHeader.tSensor.uiAcquisitionTime = %d", tWIMOParametersSensor.tHeader.tSensor.uiAcquisitionTime );

      vFQDebug("eMODBUS_STATE_MEASURE > vFSleepWithProcess start" );
      vFQDebug(QString("eMODBUS_STATE_MEASURE > uiAquisitionTime %1").arg( tWIMOParametersSensor.tHeader.tSensor.uiAcquisitionTime ) );
      //%%AP - 2021.12.20 - DEBUG : Test décalage interrogation
      vFSleepWithProcess( tWIMOParametersSensor.tHeader.tSensor.uiAcquisitionTime );
      //vFSleepWithProcess( 400 ); // Pour debug provocation de bug
      //%%AP - 2022.11.09 - La suppression 20ms fonctionnne
      vFSleepWithProcess( 20 );
      vFQDebug("vFSleepWithProcess stop");
      // Requête
      tModbus.ucReplySize = tModbusMeasuring.ucReplySize;
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusMeasuring.ucFunction, ( TUCHAR * )tModbusMeasuring.tucRequest, tModbusMeasuring.ucRequestSize ) )
       // Changement d'état
       tModbus.eState = eMODBUS_STATE_MEASURING;
      // Echec
      else
       {
        qDebug( "eMODBUS_STATE_MEASURE Error 1" );
        // Changement d'état
        tModbus.eState = eMODBUS_STATE_FREE;//eMODBUS_STATE_WAIT;
       }
     }
    // Echec
    else
     {
      qDebug( "eMODBUS_STATE_MEASURE Error 2" );
      // Changement d'état
      tModbus.eState = eMODBUS_STATE_FREE;//eMODBUS_STATE_WAIT;
     }
   break;
   //----------------
   // Mesure en cours
   //----------------
   case( eMODBUS_STATE_MEASURING ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      vFQDebug(QString("eMODBUS_STATE_MEASURING > Flag %1").arg(ptModBusDriverData->tucData[1]));
      // Test de mesure en cours
      if(  ( ptModBusDriverData->tucData[1] & aWIMO_SENSOR_I2C_STATUS_CONVERTING )
      //%%AP - 2021.12.20 - DEBUG - Si non réponse
        || ( ( ptModBusDriverData->tucData[1] == ( TUCHAR )0 ) && tModbus.uiMeasureTimeoutCpt )
        )
       {
        vFQDebug("eMODBUS_STATE_MEASURING > Mesure non-terminée");
        // Décrement du nombre d'essai
        tModbus.uiMeasureTimeoutCpt--;
        // Mesure non terminée : attente
        Sleep( 100 );
        //Sleep( 20 ); // Timing de debug pour créer plantage
        // Enregistrement du flag capteur
        tModbus.ucSensorFlag = ( TUCHAR )ptModBusDriverData->tucData[1];
        // Requête : Is measuring ?
        tModbus.ucReplySize = tModbusMeasuring.ucReplySize;
        if( tModBus.bFSend( ( TUCHAR )0x80, tModbusMeasuring.ucFunction, ( TUCHAR * )tModbusMeasuring.tucRequest, tModbusMeasuring.ucRequestSize ) )
         tModbus.eState = eMODBUS_STATE_MEASURING;
        // Echec
        else
         {
          qDebug( "eMODBUS_STATE_MEASURING Error 1" );
          tModbus.eState = eMODBUS_STATE_FREE; // eMODBUS_STATE_WAIT;
         }
       }
      // La mesure est terminée (n'est pas en cours)
      else
       {
        vFQDebug("eMODBUS_STATE_MEASURING > La mesure est terminée");

        // Test de mesure en erreur
        if( ptModBusDriverData->tucData[1] & aWIMO_SENSOR_I2C_STATUS_ERROR )
         {
          vFQDebug("eMODBUS_STATE_MEASURING > Mesure error");
          tModbus.eState = eMODBUS_STATE_FREE;
          break;
         }
        // Test de mesure en cours
        if( ptModBusDriverData->tucData[1] & aWIMO_SENSOR_I2C_STATUS_RESTARTING )
         {
          vFQDebug("eMODBUS_STATE_MEASURING > Mesure restarting");
          tModbus.eState = eMODBUS_STATE_FREE;
          break;
         }

        //%%AP - 2021.09.15 - On ralonge
        // Attente
        //Sleep( 50 );
        Sleep( 100 );
        // Requête du resultat de mesure
        tModbus.ucReplySize = tModbusResult.ucReplySize;
        if( tModBus.bFSend( ( TUCHAR )0x80, tModbusResult.ucFunction, ( TUCHAR * )tModbusResult.tucRequest, tModbusResult.ucRequestSize ) )
         tModbus.eState = eMODBUS_STATE_RESULT;
        // Echec
        else
         {
          vFQDebug( "eMODBUS_STATE_MEASURING Error 2" );
          tModbus.eState = eMODBUS_STATE_FREE;//eMODBUS_STATE_WAIT;
         }
       }
     }
    // Echec
    else
     {
      vFQDebug( "eMODBUS_STATE_MEASURING Error 3" );
      tModbus.eState = eMODBUS_STATE_FREE;//eMODBUS_STATE_WAIT;
     }
   break;
   //---------
   // Résultat
   //---------
   case( eMODBUS_STATE_RESULT ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;

    ucDebugResult = tModBus.ucFReadResponse( &ptModBusDriverData );

    //tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED;
    // Traitement réponses
    //if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
    if( ucDebugResult == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      // Gestion des données
      memcpy( tfResult, ( TUCHAR * )&ptModBusDriverData->tucData[1], 3 * sizeof( TFLOAT ) );
      // Debug
      qDebug( "fResult[0]=%f", tfResult[0] );
      qDebug( "fResult[1]=%f", tfResult[1] );
      qDebug( "fResult[2]=%f", tfResult[2] );
      if(  ( tfResult[0] != tfResult[0] )
        || ( tfResult[1] != tfResult[1] )
        || ( tfResult[2] != tfResult[2] ) )
       {
        ptLiveviewObj->setCh0Data("Error");
        ptLiveviewObj->setCh1Data("Error");
        ptLiveviewObj->setCh2Data("Error");
        ptLiveviewObj->setFCh0Data(-1);
        ptLiveviewObj->setFCh1Data(-1);
        ptLiveviewObj->setFCh2Data(-1);
       }
      else
       {
        // Ch0
        if(  ( tfResult[0] > ( float )1  )
          || ( tfResult[0] < ( float )-1 ) )
         sCh0 = QString::asprintf( "%.3f", tfResult[0] );
        else
         sCh0 = QString::asprintf( "%.5f", tfResult[0] );
        //%%AP - 2021.12.09 - Erreur
        // Ch1
        //if(  ( tfResult[0] > ( float )1  )
        //  || ( tfResult[0] < ( float )-1 ) )
        if(  ( tfResult[1] > ( float )1  )
          || ( tfResult[1] < ( float )-1 ) )
         sCh1 = QString::asprintf( "%.3f", tfResult[1] );
        else
         sCh1 = QString::asprintf( "%.5f", tfResult[1] );
        // Ch2
        if(  ( tfResult[2] > ( float )1  )
          || ( tfResult[2] < ( float )-1 ) )
         sCh2 = QString::asprintf( "%.3f", tfResult[2] );
        else
         sCh2 = QString::asprintf( "%.5f", tfResult[2] );
        // Debug
        qDebug() << "CPP sCh0=" << sCh0;
        qDebug() << "CPP sCh1=" << sCh1;
        qDebug() << "CPP sCh2=" << sCh2;
        // Remonté du résultat
        ptLiveviewObj->setCh0Data(sCh0);
        ptLiveviewObj->setCh1Data(sCh1);
        ptLiveviewObj->setCh2Data(sCh2);
        ptDataSensorCh[0]->setSValue(sCh0);
        ptDataSensorCh[1]->setSValue(sCh1);
        ptDataSensorCh[2]->setSValue(sCh2);
        ptLiveviewObj->setFCh0Data(tfResult[0]);
        ptLiveviewObj->setFCh1Data(tfResult[1]);
        ptLiveviewObj->setFCh2Data(tfResult[2]);

        // Mise à jour du recording delay
        if( !tAcquisitionStartTime.isValid() )
         {
          QDateTime tDTNow = QDateTime::currentDateTime();
          ptLiveviewObj->setRecordingSampleDelay( ( int )( 0 ) );
          // Mise à jour du datetime
          tAcquisitionStartTime = tDTNow;
         }
        else
         {
          QDateTime tDTNow = QDateTime::currentDateTime();
          qint64 uliNow = tDTNow.toSecsSinceEpoch();
          qint64 uliAcquisitionStartTime = tAcquisitionStartTime.toSecsSinceEpoch();
          qDebug() << "Recording delay is: " << ( int )( uliNow - uliAcquisitionStartTime );
          ptLiveviewObj->setRecordingSampleDelay( ( int )( uliNow - uliAcquisitionStartTime ) );
          // Mise à jour du datetime
          tAcquisitionStartTime = tDTNow;
         }

        // Enregistrement des échantillons
        ptLiveview->recordMeasureSample(tfResult[0],tfResult[1],tfResult[2]);
        // Transmission résultat à LiveviewConf
        ptLiveviewConfEvent->transmitLiveViewData( tfResult[0], tfResult[1], tfResult[2] );
        // Mesure terminée
        ptLiveviewObj->setMeasureFinished(true);
       }
     }
    // Erreur sur l'échantillon
    else
     {
      ptLiveviewObj->setCh0Data("Error");
      ptLiveviewObj->setCh1Data("Error");
      ptLiveviewObj->setCh2Data("Error");
      ptLiveviewObj->setFCh0Data(tfResult[0]);
      ptLiveviewObj->setFCh1Data(tfResult[1]);
      ptLiveviewObj->setFCh2Data(tfResult[2]);
     }
    vFQDebug("eMODBUS_STATE_RESULT > OK");
    // On repasse en interrogation
    tModbus.eState = eMODBUS_STATE_FREE;
   break;
   //----------
   // Nettoyage
   //----------
   case( eMODBUS_STATE_CLEAN ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      // Attente
      Sleep( 5 );
      // Requête
      tModbus.ucReplySize = tModbusCleaning.ucReplySize;
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusCleaning.ucFunction, ( TUCHAR * )tModbusCleaning.tucRequest, tModbusCleaning.ucRequestSize ) )
       {
        tModbus.eState = eMODBUS_STATE_CLEANING;
        //Form2->cxLabel1->Caption = "Cleaning in progress...";
        //Form2->Show();
       }
      // Echec
      else
       {
        qDebug("Clean error 1");
        //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      qDebug("Clean error 2");
      //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //-------------------
   // Nettoyage en cours
   //--------------------
   case( eMODBUS_STATE_CLEANING ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      // Test de nettoyage en cours
      if( ptModBusDriverData->tucData[1] & 0x20 )
       {
        // Attente
        Sleep( 450 );
        // Affichage
        //Form2->cxProgressBar1->Position += 5; Application->ProcessMessages();
        // Requête
        tModbus.ucReplySize = tModbusCleaning.ucReplySize;
        if( tModBus.bFSend( ( TUCHAR )0x80, tModbusCleaning.ucFunction, ( TUCHAR * )tModbusCleaning.tucRequest, tModbusCleaning.ucRequestSize ) )
         {
          tModbus.eState = eMODBUS_STATE_CLEANING;
         }
        // Echec
        else
         {
          qDebug("Cleaning error 1");
          //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
          tModbus.eState = eMODBUS_STATE_FREE;
          //Form2->Close();
         }
       }
      else
       {
        // On n'est plus en busy
        //bBusyState
        ptWiperObj->setProperty("bBusyState", false);
        // Terminé
        //Form2->Close();
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      qDebug("Cleaning error 2");
      //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
   //--------
   // Version de l'interface/outil
   //--------
   case( eMODBUS_STATE_GET_INTERFACE_VERSION ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      qDebug("eMODBUS_STATE_GET_INTERFACE_VERSION");
      // Attente
      Sleep( 100 );
      // Affichage
      //Form2->cxProgressBar1->Position = 20;
      //Application->ProcessMessages();
      // Mise en forme
      memcpy( tLong.tucValue, &ptModBusDriverData->tucData[1], sizeof( tLong ) );
      tWIMOModbusInterfaceVersion.ucMajor = ( TUCHAR )( ( tLong.uliValue & 0x000000FF ) );
      tWIMOModbusInterfaceVersion.ucMinor = ( TUCHAR )( ( tLong.uliValue & 0x0000FF00 ) >> 8  );
      tWIMOModbusInterfaceVersion.uiBuild = ( TUINT ) ( ( tLong.uliValue & 0xFFFF0000 ) >> 16 );

      // Interface minimale version
      if( !bFWIMOInterfaceCheckMinimalVersion() )
       {
        // Capteur non-autorisé
        QString sStringError;
        sStringError.append("Please update usb tool software version.\r\n");
        //sStringError.append("Go to Factory tab > Check for update.\r\n");
        sStringError.append( QString("\r\nCurrent usb tool version: \t"+ QString::asprintf( "V%u.%u.%u", tWIMOModbusInterfaceVersion.ucMajor, tWIMOModbusInterfaceVersion.ucMinor, tWIMOModbusInterfaceVersion.uiBuild )) );
        sStringError.append( QString("\r\nMin required version: \t"+sInterfaceMinimalVersion) );

        // Affichage du message d'erreur
        ptMainEvent->vFMainWindowErrorOpen( sStringError );
        // Modification de l'état de la variable
        ptMainData->setErrorSensorNeedUpdate( true );
        // Réinit de l'état
        //tModbus.eState = eMODBUS_STATE_FREE;
        // On quitte
        //break;
       }
      else
       {
        ptMainData->setErrorSensorNeedUpdate( false );
       }
      // Numéro de version mise à jour
      emit ptModbusControler->requestInterfaceVersionNumberEndSignal();
      // State free
      tModbus.eState = eMODBUS_STATE_FREE;
     }
    // Echec
    else
     {
      // Debug !
      //tModBus.ucFReadResponse( &ptModBusDriverData );
      qDebug( QTime::currentTime().toString().toLatin1() );
      qDebug("Communication error !");

      // Erreur
      //ptFactoryEvent->updateInterfaceVersion( "Error" );

      //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
   //--------
   // Modbus 9600 - Reset measure period
   //--------
   case( eMODBUS_STATE_RESET_CONF_MEASURE_PERIOD ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      vFQDebug("eMODBUS_STATE_RESET_CONF_MEASURE_PERIOD OK");
      Sleep(800);
      // Requête
      tModbus.ucReplySize = tModbusReadVersion.ucReplySize;
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadVersion.ucFunction,
                          ( TUCHAR * )tModbusReadVersion.tucRequest,
                          tModbusReadVersion.ucRequestSize ) )
       {
        tModbus.eState = eMODBUS_STATE_GET_VERSION;
        qDebug("eMODBUS_STATE_RESET_CONF_MEASURE_PERIOD sent");
        //Form2->cxLabel1->Caption = "Reading firmware version...";
        //Form2->Show();
       }
      // Echec
      else
       {
        qDebug("eMODBUS_STATE_GET_VERSION > Send error");
        ptMainEvent->vFMainWindowErrorOpen("Communication error");
        //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
       }
     }
    // Echec
    else
     {
      vFQDebug("Communication error !");
      ptMainData->setError(true);
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //--------
   // Version
   //--------
   case( eMODBUS_STATE_GET_VERSION ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      vFQDebug("eMODBUS_STATE_GET_VERSION OK");
      // Attente
      Sleep( 100 );
      // Affichage
      //Form2->cxProgressBar1->Position = 20;
      // Sauvegarde du numéro de version
      memcpy( &tSensorVersion, &ptModBusDriverData->tucData[1], sizeof( tSensorVersion ) );
      //Application->ProcessMessages();
      memcpy( tLong.tucValue, &ptModBusDriverData->tucData[1], sizeof( tLong ) );
      sprintf( tcString,
               "V%d.%d.%d",
               ( TUINT )( ( tLong.uliValue & 0x000000FF ) ),
               ( TUINT )( ( tLong.uliValue & 0x0000FF00 ) >> 8 ),
               ( TUINT )( ( tLong.uliValue & 0xFFFF0000 ) >> 16 ) );
      qDebug() << "Version : " << tcString;
      // Sauvegarde du numero de version
      sSensorSoftware = QString( tcString );
      // Sensor update signal
      emit ptSensorUpdateControler->updateProgressSignal( 96 );
      // En cas de mise à jour
      ptFactoryEvent->updateLoaderValue( 96 );
      // Attente
      Sleep( ((ptMainData->getCOMSlowMode())?500:5) );
      vFQDebug("eMODBUS_STATE_GET_CONF_PRODUCT sending");
      // Requête
      tModbus.ucReplySize = tModbusReadConfP.ucReplySize;
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadConfP.ucFunction, ( TUCHAR * )tModbusReadConfP.tucRequest, tModbusReadConfP.ucRequestSize ) )
       {
        tModbus.eState = eMODBUS_STATE_GET_CONF_PRODUCT;
        //Form2->cxLabel1->Caption = "Reading product configuration...";
       }
      // Echec
      else
       {
        vFQDebug("Communication error ! 2686");
        ptMainData->setError(true);
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      vFQDebug("Communication error ! 2694");
      ptMainData->setError(true);
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //----------------------
   // Configuration produit
   //----------------------
   case( eMODBUS_STATE_GET_CONF_PRODUCT ):
   case( eMODBUS_STATE_GET_CONF_PRODUCT_ONLY ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    //
    qDebug("eMODBUS_STATE_GET_CONF_PRODUCT IN");
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      vFQDebug("eMODBUS_STATE_GET_CONF_PRODUCT OK");
      // Réinit du paramètre avant mesure
      tModbus.b9600Init = ( TBOOL )aFALSE;
      // Attente
      Sleep( 100 );
      // Récupération de la donnée dans une structure temporaire
      memcpy( &tWIMOParamCheckIntegrity.tHeader, &ptModBusDriverData->tucData[1], sizeof( TWIMOParametersHeaderObject ) );

      //%%AP - 2020.11.08 - à vérifier et duppliquer pour les voies de mesure
      // Mise à jour des paramètres avec controle
      if( !bFWIMOParameterConfProductCheckIntegrity() )
       {
        // Paramètres invalides
        ptMainEvent->vFMainWindowErrorOpen("Read configuration error");
        ptMainData->setError(true);
        // On libère l'automate
        tModbus.eState = eMODBUS_STATE_FREE;
        break;
       }
      // Copie de la structure temporaire vers la structure d'utilisation
      memcpy( &tWIMOParametersSensor.tHeader, &tWIMOParamCheckIntegrity.tHeader, sizeof( TWIMOParametersHeaderObject ) );
      // Mise à jour QML
      vFWIMOParameterConfProductQMLUpdate();

      // Si c'est un capteur
      if( tWIMOParametersSensor.tHeader.tProduct.cType == ( TCHAR )'S' )
       {
        // Sensor update signal
        emit ptSensorUpdateControler->updateProgressSignal( 97 );
        // En cas de mise à jour : pourcentage d'avancement
        ptFactoryEvent->updateLoaderValue( 97 );
       }
      // Si c'est un wiper
      if( tWIMOParametersSensor.tHeader.tProduct.cType == ( TCHAR )'W' )
       {
        // En cas de mise à jour : pourcentage d'avancement
        ptFactoryEvent->updateLoaderValue( 100 );
        // Déclenchement de l'affichage des onglets
        ptDashboardControler->vFDashboardHandleEndOfRefresh();
        ptMainData->setError(true);
        // On libère
        tModbus.eState = eMODBUS_STATE_FREE;
        break;
       }

      // Si en mode lecture CH0 seul ou balai
      if(  ( tModbus.eState == eMODBUS_STATE_GET_CONF_PRODUCT_ONLY )
        || ( tWIMOParametersSensor.tHeader.tProduct.cType == ( TCHAR )'W' ) )
       {
        // On libère
        tModbus.eState = eMODBUS_STATE_FREE;
        vFQDebug("eMODBUS_STATE_GET_CONF_PRODUCT_ONLY > Error W");
        ptMainData->setError(true);
        break;
       }
      // Attente
      //Sleep( 100 );
      Sleep( ((ptMainData->getCOMSlowMode())?500:100) );
      vFQDebug( "eMODBUS_STATE_GET_CONF_CH0 > Sending" );
      // Requête
      //tModbus.ucReplySize = tModbusReadConf1.ucReplySize;
      //if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadConf1.ucFunction, ( TUCHAR * )tModbusReadConf1.tucRequest, tModbusReadConf1.ucRequestSize ) )
      tModbus.ucReplySize = tModbusReadConf0.ucReplySize;
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadConf0.ucFunction, ( TUCHAR * )tModbusReadConf0.tucRequest, tModbusReadConf0.ucRequestSize ) )
       {
        tModbus.eState = eMODBUS_STATE_GET_CONF_CH0;
        //Form2->cxLabel1->Caption = "Reading channel 0 configuration...";
       }
      // Echec
      else
       {
        ptMainEvent->vFMainWindowErrorOpen( "Communication error" );
        ptMainData->setError(true);
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      vFQDebug("Communication error ! 2790");
      ptMainData->setError(true);
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //---------------------
   // Configuration voie 0
   //---------------------
   case( eMODBUS_STATE_GET_CONF_CH0 ):
   case( eMODBUS_STATE_GET_CONF_CH0_ONLY ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      vFQDebug("eMODBUS_STATE_GET_CONF_CH0 OK");
      // Attente
      Sleep( 100 );
      // En cas de mise à jour : Mise à jour du pourcentage
      ptFactoryEvent->updateLoaderValue( 98 );
      // Sensor update signal
      emit ptSensorUpdateControler->updateProgressSignal( 98 );
      // Récupération
      memcpy( &tWIMOParametersSensor.ttChannel[0], &ptModBusDriverData->tucData[1], sizeof( TWIMOParametersChannelObject ) );
      //----------------------
      // Controle si capteur autorisé
      //----------------------
      if(  ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_TEMPERATURE          ) // S03
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_CONDUCTIVITY         ) // S04
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_TURBIDITY            ) // S05
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_OXYGEN_CONCENTRATION ) // S06
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_OXYGEN_SATURATION    ) // S07
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_PH                   ) // S08
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_TURNER_CHLA          ) // S09
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_TURNER_PHYCOC        ) // S10
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_TURNER_PHYCOE        ) // s11
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_TURNER_CDOM          ) // S12
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_FLUO_CHLA            ) // S13
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_PAR                  ) // S14
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_FLUO_PHYCOC          ) // S15
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_FLUO_PHYCOE          ) // S16
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_FLUO_CDOM            ) // S17
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_FLUO_TRYPTO          ) // S18
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_TURNER_TRYPTO        ) // S19
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_S20_REDOX            ) // S20
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_S21_AMMONIUM         ) // S21
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_S22_POTASSIUM        ) // S22
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_S23_NITRATE          ) // S23
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_S24                  ) // S24
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_S25_O2_TEMPERATURE   ) // S25
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_S26_CRUDE_OIL_TURNER ) // S26
        && ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_S27_CRUDE_OIL_NKE    ) // S27
        && ( !ptMainData->getNkeMode() ) ) // Mode nke => Tous les capteurs autorisés
       {
        // Capteur non-autorisé
        QString sStringError;
        sStringError.append("Sensor not supported by current software version.\r\n");
        sStringError.append( QString("\r\nSoftware version: \tV"+sSoftwareVersionFull) );
        sStringError.append( QString("\r\nSensor version: \t"+sSensorSoftware) );
        sStringError.append( QString("\r\nSerial number: \t"+sSerialNumber) );

        sStringError.append( QString("\r\nID: \t\tS%1").arg( tWIMOParametersSensor.ttChannel[0].ucParameter ) );
        sStringError.append( QString("\r\nIdentifier: \t%1 %2").arg( tWIMOParametersSensor.tHeader.tProduct.tcIdentifier, tWIMOParametersSensor.tHeader.tProduct.tcVersion ) );
        sStringError.append( QString("\r\nManufacturer: \t%1").arg( tWIMOParametersSensor.tHeader.tProduct.tcManufacturer ) );
        sStringError.append( QString("\r\nFirst channel: \t%1").arg( tWIMOParametersSensor.ttChannel[0].tcName ) );
        // Affichage du message d'erreur
        ptMainEvent->vFMainWindowErrorOpen( sStringError );
        ptMainData->setError(true);
        // Réinit de l'état
        tModbus.eState = eMODBUS_STATE_FREE;
        // On quitte
        break;
       }
      //---------------------------------------------------
      //%%AP - 2021.09.22 - Restriction numéro de version
      //---------------------------------------------------
      if( !bFWIMOSensorCheckMinimalVersion() )
       {
        // Capteur non-autorisé
        QString sStringError;
        sStringError.append("Please update sensor software version.\r\n");
        //sStringError.append("Go to Factory tab > Check for update.\r\n");
        sStringError.append( QString("\r\nCurrent sensor version: \t"+sSensorSoftware) );
        sStringError.append( QString("\r\nMin required sensor version: \t"+sSensorMinimalVersion) );
        sStringError.append( QString("\r\nSerial number: \t"+sSerialNumber) );

        sStringError.append( QString("\r\nID: \t\tS%1").arg( tWIMOParametersSensor.ttChannel[0].ucParameter ) );
        sStringError.append( QString("\r\nIdentifier: \t%1 %2").arg( tWIMOParametersSensor.tHeader.tProduct.tcIdentifier, tWIMOParametersSensor.tHeader.tProduct.tcVersion ) );
        sStringError.append( QString("\r\nManufacturer: \t%1").arg( tWIMOParametersSensor.tHeader.tProduct.tcManufacturer ) );
        sStringError.append( QString("\r\nFirst channel: \t%1").arg( tWIMOParametersSensor.ttChannel[0].tcName ) );
        // Affichage du message d'erreur
        ptMainEvent->vFMainWindowErrorOpen( sStringError );
        //%%AP - 2022.03.08 - V1.2.5.3
        // Levé de la restriction du numéro de version logiciel pour les O2 productions
        // Si pas en mode nke : on arrête
        if( !ptMainData->getNkeMode() && !ptMainData->getProdMode() )
         {
          // Modification de l'état de la variable
          ptMainData->setErrorSensorNeedUpdate( true );
          ptMainData->setError(true);
          // Réinit de l'état
          tModbus.eState = eMODBUS_STATE_FREE;
          // On quitte
          break;
         }
       }
      else
       {
        ptMainData->setErrorSensorNeedUpdate( false );
       }

      // Selon le type de capteur
      if(  ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_TURBIDITY )
        || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_FLUO_CHLA )
        || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_FLUO_PHYCOC )
        || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_FLUO_PHYCOE )
        || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_FLUO_CDOM )
        || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_FLUO_TRYPTO ) )
       {
        // On affiche Range/Average/Integration Time
        ptFactoryEvent->configSettingsDisplayEvosens();
       }

      // Capteur classique
      if(  ( tWIMOParametersSensor.tHeader.tProduct.cType == ( TCHAR )'S' )
        && ( tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber >= ( TUCHAR )1 ) )
       {
        // Remplissage donnée capteur
        vFWIMOParameterUpdateSensorChannel(0);

        // Si capteur CT - Voie Conductivité
        if( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )4 )
         {
          qDebug( "tWIMOParametersSensor.ttChannel[0].ttPointTheo[0].fRaw = %f",      tWIMOParametersSensor.ttChannel[0].ttPointTheo[0].fRaw );
          qDebug( "tWIMOParametersSensor.ttChannel[0].ttPointTheo[0].fPhysical = %f", tWIMOParametersSensor.ttChannel[0].ttPointTheo[0].fPhysical );
          qDebug( "tWIMOParametersSensor.ttChannel[0].ttPointTheo[1].fRaw = %f",      tWIMOParametersSensor.ttChannel[0].ttPointTheo[1].fRaw );
          qDebug( "tWIMOParametersSensor.ttChannel[0].ttPointTheo[1].fPhysical = %f", tWIMOParametersSensor.ttChannel[0].ttPointTheo[1].fPhysical );

          ptCalibration->setCalibTheoRaw1(tWIMOParametersSensor.ttChannel[0].ttPointTheo[0].fRaw);
          ptCalibration->setCalibTheoPhy1(tWIMOParametersSensor.ttChannel[0].ttPointTheo[0].fPhysical);
          ptCalibration->setCalibTheoRaw2(tWIMOParametersSensor.ttChannel[0].ttPointTheo[1].fRaw);
          ptCalibration->setCalibTheoPhy2(tWIMOParametersSensor.ttChannel[0].ttPointTheo[1].fPhysical);

          // On affiche la calibration théorique dans FACTORY
          sprintf( tcString, "%.6f", tWIMOParametersSensor.tHeader.tCT.tCompensation.fA1 );
          ptDataSensorCT->setTCompensationA( tcString );
          sprintf( tcString, "%.6f", tWIMOParametersSensor.tHeader.tCT.tCompensation.fA2 );
          ptDataSensorCT->setTCompensationB( tcString );
          sprintf( tcString, "%.6f", tWIMOParametersSensor.tHeader.tCT.tCompensation.fA3 );
          ptDataSensorCT->setTCompensationC( tcString );
          sprintf( tcString, "%.6f", tWIMOParametersSensor.tHeader.tCT.tCompensation.fRaw20 );
          ptDataSensorCT->setTCompensationRaw20( tcString );
         }
        // Remplissage des points de calibration - voie 0
        vFCalibrationFillPointToQML( 0 );
        // Reset point de calibration
        vFCalibrationResetPointToQmlWithChannel(0);
        // On définit la voie 0 comme voie courante
        ptCalibration->setCurrentChannel(0);
       }
      // Si en mode lecture CH0 seul
      if( tModbus.eState == eMODBUS_STATE_GET_CONF_CH0_ONLY )
       {
        // On libère
        tModbus.eState = eMODBUS_STATE_FREE;
        break;
       }
      // Attente
      Sleep( ((ptMainData->getCOMSlowMode())?500:5) );
      vFQDebug( "eMODBUS_STATE_GET_CONF_CH1 > Sending" );
      // Requête
      tModbus.ucReplySize = tModbusReadConf1.ucReplySize;
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadConf1.ucFunction, ( TUCHAR * )tModbusReadConf1.tucRequest, tModbusReadConf1.ucRequestSize ) )
       {
        tModbus.eState = eMODBUS_STATE_GET_CONF_CH1;
       }
      // Echec
      else
       {
        qDebug( "eMODBUS_STATE_GET_CONF_CH1 > Sending Communication error !" );
        ptMainEvent->vFMainWindowErrorOpen( "Communication error");
        ptMainData->setError(true);
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      qDebug( "Communication error !" );
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      ptMainData->setError(true);
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //---------------------
   // Configuration voie 1
   //---------------------
   case( eMODBUS_STATE_GET_CONF_CH1 ):
   case( eMODBUS_STATE_GET_CONF_CH1_ONLY ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      // Attente
      Sleep( 100 );
      // En cas de mise à jour
      ptFactoryEvent->updateLoaderValue( 99 );
      // Sensor update signal
      emit ptSensorUpdateControler->updateProgressSignal( 99 );
      //
      vFQDebug("eMODBUS_STATE_GET_CONF_CH1");
      // Récupération
      memcpy( &tWIMOParametersSensor.ttChannel[1], &ptModBusDriverData->tucData[1], sizeof( TWIMOParametersChannelObject ) );
      // Si capteur 2 voies ou plus
      if(  ( tWIMOParametersSensor.tHeader.tProduct.cType          == ( TCHAR )'S' )
        && ( tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber >= ( TUCHAR )2  ) )
       {
        // Remplissage donnée capteur
        vFWIMOParameterUpdateSensorChannel(1);
        // Ch1 - Parameter
        qDebug("tWIMOParametersSensor.ttChannel[1].ucParameter = "+tWIMOParametersSensor.ttChannel[1].ucParameter);

        // Remplissage des points de calibration - voie 1
        vFCalibrationFillPointToQML( 1 );
        //%%AP - 2020.11.04 - AJOUT
        // Reset point de calibration
        //vFCalibrationResetPointToQmlWithChannel(1);
       }
      // Si en mode lecture CH1 seul
      if( tModbus.eState == eMODBUS_STATE_GET_CONF_CH1_ONLY )
       {
        //%%AP - 2020.11.04 - Retiré
        // Reset point de calibration
        vFCalibrationResetPointToQmlWithChannel(1);
        // On définit la voie 0 comme voie courante
        ptCalibration->setCurrentChannel(1);
        ptMainData->setError(true);
        vFQDebug("eMODBUS_STATE_GET_CONF_CH1_ONLY > Error");
        // On libère
        tModbus.eState = eMODBUS_STATE_FREE;
        break;
       }
      // Attente
      //Sleep( 5 );
      Sleep( ((ptMainData->getCOMSlowMode())?500:5) );
      vFQDebug( "eMODBUS_STATE_GET_CONF_CH2 > Sending" );
      // Requête
      tModbus.ucReplySize = tModbusReadConf2.ucReplySize;
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusReadConf2.ucFunction, ( TUCHAR * )tModbusReadConf2.tucRequest, tModbusReadConf2.ucRequestSize ) )
       {
        tModbus.eState = eMODBUS_STATE_GET_CONF_CH2;
        //Form2->cxLabel1->Caption = "Reading channel 2 configuration...";
       }
      // Echec
      else
       {
        ptMainData->setError(true);
        vFQDebug( "eMODBUS_STATE_GET_CONF_CH2 > Sending Error" );
        ptMainEvent->vFMainWindowErrorOpen( "Sending error");
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      ptMainData->setError(true);
      vFQDebug( "eMODBUS_STATE_GET_CONF_CH1 > Communication error" );
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //---------------------
   // Configuration voie 2
   //---------------------
   case( eMODBUS_STATE_GET_CONF_CH2 ):
   case( eMODBUS_STATE_GET_CONF_CH2_ONLY ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      // Attente
      Sleep( 100 );
      // En cas de mise à jour
      ptFactoryEvent->updateLoaderValue( 100 );
      // Sensor update signal
      emit ptSensorUpdateControler->updateProgressSignal( 100 );
      // Sensor update signal
      emit ptSensorUpdateControler->updateEndSignal();
      //
      vFQDebug("eMODBUS_STATE_GET_CONF_CH2");  // Test si on est en écriture exclusive
      // Récupération
      memcpy( &tWIMOParametersSensor.ttChannel[2], &ptModBusDriverData->tucData[1], sizeof( TWIMOParametersChannelObject ) );
      // Dashboard - Channel name liste
      vFWIMOParameterQMLUpdateDashboardChannelName();
      // Voie de mesure
      if( tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber == 1 )
       ptCalibration->setChannelList( QStringList({QString(tWIMOParametersSensor.ttChannel[0].tcName)}));
      else if( tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber == 2 )
       ptCalibration->setChannelList( QStringList({QString(tWIMOParametersSensor.ttChannel[0].tcName), QString(tWIMOParametersSensor.ttChannel[1].tcName)}));
      else if( tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber == 3 )
       ptCalibration->setChannelList( QStringList({QString(tWIMOParametersSensor.ttChannel[0].tcName), QString(tWIMOParametersSensor.ttChannel[1].tcName), QString(tWIMOParametersSensor.ttChannel[2].tcName)}));
      // Test si O2 mode nke ou métro
      if(  ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_OXYGEN_CONCENTRATION )
        && ( ptMainData->getNkeMode() || ptMainData->getMetroMode() ) )
       ptCalibration->setChannelList( QStringList({QString(tWIMOParametersSensor.ttChannel[0].tcName), QString(tWIMOParametersSensor.ttChannel[1].tcName), QString(tWIMOParametersSensor.ttChannel[2].tcName)}));


      // Affichage
      if(  (  ( tWIMOParametersSensor.tHeader.tProduct.cType == ( TCHAR )'S' )
           && ( tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber >= ( TUCHAR )3 ) )
        || (  ( tWIMOParametersSensor.tHeader.tProduct.cType == ( TCHAR )'S' )
           && ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_OXYGEN_CONCENTRATION )
           && ( ptMainData->getMetroMode() || ptMainData->getNkeMode() ) )
        )
       {
        vFQDebug("3 channels used");
        // Remplissage donnée capteur
        vFWIMOParameterUpdateSensorChannel(2);
        // Nombre de voie de mesure du capteur
        ptLiveviewObj->setChannelNumber(3);

        // Remplissage des points de calibration - voie 2
        vFCalibrationFillPointToQML( 2 );
        // Si c'est la voie température O2
        if( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_S25_O2_TEMPERATURE )
         {
          // Reset point de calibration
          vFCalibrationResetPointToQmlWithChannel(2);
          // On définit la voie 0 comme voie courante
          ptCalibration->setCurrentChannel(2);
         }
       }
      // Test si c'est un refresh conf 2 uniquement
      if( tModbus.eState == eMODBUS_STATE_GET_CONF_CH2_ONLY )
       {
        // Reset point de calibration
        vFCalibrationResetPointToQmlWithChannel(2);
        // On définit la voie 2 comme voie courante
        ptCalibration->setCurrentChannel(2);
        //%%AP - WTF ?
        //ptMainData->setError(true);
        //vFQDebug("eMODBUS_STATE_GET_CONF_CH2_ONLY > Error");
        //%%AP - 2020/11/04 - Ajout
        // On libère
        tModbus.eState = eMODBUS_STATE_FREE;
        break;
       }
      // Attente
      Sleep( 5 );
      //%%AP - 2021.09.07 - Fin mise en commentaire
      // Test si c'est un refresh général
      if(  ( tModbus.eState == eMODBUS_STATE_GET_CONF_CH2 )
        //|| ( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_OXYGEN_CONCENTRATION )
        )
       {
        // Fin du refresh des données
        ptFactoryEvent->endOfRefreshData();
        // Si pas un oxygène
        if( tWIMOParametersSensor.ttChannel[0].ucParameter != ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_OXYGEN_CONCENTRATION )
         // Déclenchement de l'affichage des onglets
         ptDashboardControler->vFDashboardHandleEndOfRefresh();
        // Liveviewconf display
        ptLiveviewConfEvent->displayLiveViewConf();
        // Sauvegarde de la configuration
        sprintf( tcString,
                 "%02u-%05u_config.bin",
                 tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY,
                 tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN );
        //ptFactoryEvent->startSaveConfig( QString("save.bin"), QString("../Config/") );
        ptFactoryEvent->bFStartSaveConfig( tcString, QString("../Config/") );
       }
      vFQDebug("eMODBUS_STATE_GET_CONF_CH2 > tModbus.eState = eMODBUS_STATE_FREE;");
      // Terminé
      tModbus.eState = eMODBUS_STATE_FREE;
     }
    // Echec
    else
     {
      ptMainData->setError(true);
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      vFQDebug("Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;

   //-----------------
   // Configuration O2
   //-----------------
   case( eMODBUS_STATE_GET_CONF_O2 ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      vFQDebug("eMODBUS_STATE_GET_CONF_O2");
      vFQDebug("eMODBUS_STATE_GET_CONF_O2 > Test sensor flag");
      // Test flag erreur
      if( tModbus.ucSensorFlag & aWIMO_SENSOR_I2C_STATUS_ERROR )
       {
        // Si en mode nke : on laisse l'affichage
        if( ptMainData->getNkeMode() )
         {
          // Déclenchement de l'affichage des onglets
          ptDashboardControler->vFDashboardHandleEndOfRefresh();
         }
        // On arrête là avec message d'erreur
        else
         {
          // Debug
          vFQDebug("eMODBUS_STATE_GET_CONF_O2 > Sensor flag error");
          // Oxygen read failled
          ptMainEvent->vFMainWindowErrorOpen("Oxygen sensor read Failed");
          //%%TODO
          // Effacement des données capteurs
         }
       }
      else
       {
        // Sensor flag OK
        vFQDebug("eMODBUS_STATE_GET_CONF_O2 > Sensor flag OK");
        // Récupération
        memcpy( &tWIMOPyroO2Configuration, &ptModBusDriverData->tucData[1], sizeof( TWIMOPyroO2ConfigurationObject ) );
        // Lancement d'une fonction d'affichage des données
        ptO2Event->configO2Display();
        // Déclenchement de l'affichage des onglets
        ptDashboardControler->vFDashboardHandleEndOfRefresh();
        //%%AP - 2020.11.30 - Ajout du reste du boulot
        // Liveviewconf display
        ptLiveviewConfEvent->displayLiveViewConf();
        // Sauvegarde de la configuration
        sprintf( tcString,
                 "%02u-%05u_config.bin",
                 tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY,
                 tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN );
        //ptFactoryEvent->startSaveConfig( QString("save.bin"), QString("../Config/") );
        ptFactoryEvent->bFStartSaveConfig( tcString, QString("../Config/") );
       }
      // Attente
      Sleep( 100 );
      tModbus.eState = eMODBUS_STATE_FREE;
     }
    // Echec
    else
     {
      vFQDebug("eMODBUS_STATE_GET_CONF_O2 Failed");
      //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //----------------------
   // Configuration cadence mesure mode modbus 9600
   //----------------------
   case( eMODBUS_STATE_SET_CONF_MEASURE_PERIOD ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      vFQDebug("eMODBUS_STATE_SET_CONF_MEASURE_PERIOD > OK");
      // Attente
      Sleep( 500 );
      //tModbus.eState = eMODBUS_STATE_FREE;
      // Cadence initialisée
      tModbus.b9600Init = ( TBOOL )aTRUE;
      // Start mesure
      tModbus.eState = eMODBUS_STATE_GET_MEASURE_DATA_9600_CMD;
     }
    // Echec
    else
     {
      vFQDebug("eMODBUS_STATE_SET_CONF_MEASURE_PERIOD > Send error");
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;

   //----------------------
   // Réultat mesure 9600
   //----------------------
   case( eMODBUS_STATE_GET_MEASURE_DATA_9600_CMD ):
    vFQDebug( "eMODBUS_STATE_GET_MEASURE_DATA_9600_CMD OK" );
    // Attente
    //Sleep( 100 );
    // Requête du resultat de mesure
    tModbus.ucReplySize = tModbusResult9600.ucReplySize;
    if( tModBus.bFSend( ( TUCHAR )0x80, tModbusResult9600.ucFunction, ( TUCHAR * )tModbusResult9600.tucRequest, tModbusResult9600.ucRequestSize ) )
     {
      vFQDebug( "eMODBUS_STATE_GET_MEASURE_DATA_9600_CMD > Sent" );
      // Attente
      Sleep( 500 );
      //Sleep( 500 );
      // Passage dans le mode
      tModbus.eState = eMODBUS_STATE_GET_MEASURE_DATA_9600;
     }
    // Echec
    else
     {
      vFQDebug( "eMODBUS_STATE_GET_MEASURE_DATA_9600 > Sending error" );
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //----------------------
   // Réultat mesure 9600
   //----------------------
   case( eMODBUS_STATE_GET_MEASURE_DATA_9600 ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      vFQDebug( "eMODBUS_STATE_GET_MEASURE_DATA_9600 > OK" );
      // Gestion des données
      memcpy( tfResult, ( TUCHAR * )&ptModBusDriverData->tucData[1], 3 * sizeof( TFLOAT ) );
      // Debug
      qDebug( "fResult[0]=%f", tfResult[0] );
      qDebug( "fResult[1]=%f", tfResult[1] );
      qDebug( "fResult[2]=%f", tfResult[2] );

      // Ch0
      if(  ( tfResult[0] > ( float )1  )
        || ( tfResult[0] < ( float )-1 ) )
       sCh0 = QString::asprintf( "%.3f", tfResult[0] );
      else
       sCh0 = QString::asprintf( "%.5f", tfResult[0] );
      // Ch1
      if(  ( tfResult[0] > ( float )1  )
        || ( tfResult[0] < ( float )-1 ) )
       sCh1 = QString::asprintf( "%.3f", tfResult[1] );
      else
       sCh1 = QString::asprintf( "%.5f", tfResult[1] );
      // Ch2
      if(  ( tfResult[2] > ( float )1  )
        || ( tfResult[2] < ( float )-1 ) )
       sCh2 = QString::asprintf( "%.3f", tfResult[2] );
      else
       sCh2 = QString::asprintf( "%.5f", tfResult[2] );

      // Remonté du résultat
      ptLiveviewObj->setCh0Data(sCh0);
      ptLiveviewObj->setCh1Data(sCh1);
      ptLiveviewObj->setCh2Data(sCh2);
      ptLiveviewObj->setFCh0Data(tfResult[0]);
      ptLiveviewObj->setFCh1Data(tfResult[1]);
      ptLiveviewObj->setFCh2Data(tfResult[2]);
      // Enregistrement des échantillons
      ptLiveview->recordMeasureSample(tfResult[0],tfResult[1],tfResult[2]);
      // Transmission résultat à LiveviewConf
      ptLiveviewConfEvent->transmitLiveViewData( tfResult[0], tfResult[1], tfResult[2] );
      // Mesure terminée
      ptLiveviewObj->setMeasureFinished(true);
     }
    // Erreur sur l'échantillon
    else
     {
      vFQDebug( "eMODBUS_STATE_GET_MEASURE_DATA_9600 > NOK" );
      ptLiveviewObj->setCh0Data("Error");
      ptLiveviewObj->setCh1Data("Error");
      ptLiveviewObj->setCh2Data("Error");
      ptLiveviewObj->setFCh0Data(tfResult[0]);
      ptLiveviewObj->setFCh1Data(tfResult[1]);
      ptLiveviewObj->setFCh2Data(tfResult[2]);
     }
    // On repasse en interrogation
    tModbus.eState = eMODBUS_STATE_FREE;
   break;
   //----------------------
   // Configuration produit
   //----------------------
   case( eMODBUS_STATE_SET_CONF_PRODUCT ):
   case( eMODBUS_STATE_SET_CONF_PRODUCT_ONLY ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      qDebug("eMODBUS_STATE_SET_CONF_PRODUCT");
      // Test si on est en écriture exclusive
      if( tModbus.eState == eMODBUS_STATE_SET_CONF_PRODUCT_ONLY )
       {
        // Busy state
        ptWiperObj->setProperty("bBusyState", false);
        ptFactoryEvent->endOfSNSubmit();
        // On libère
        tModbus.eState = eMODBUS_STATE_FREE;
        break;
       }
      qDebug("eMODBUS_STATE_SET_CONF_PRODUCT");
      // Affichage
      //Form2->cxProgressBar1->Position = 25; Application->ProcessMessages();
      // Attente
      //Sleep( 250 );
      Sleep( 100 );
      // Requête
      tModbus.ucReplySize = tModbusWriteConf0.ucReplySize;
      memcpy( tModbusWriteConf0.tucData, tWIMOParametersSensor.ttChannel[0].tucBuffer, sizeof( TWIMOParametersChannelObject ) );
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusWriteConf0.ucFunction, ( TUCHAR * )tModbusWriteConf0.tucRequest, tModbusWriteConf0.ucRequestSize ) )
       {
        //%%AP - BUG ?
        //tModbus.eState = eMODBUS_STATE_GET_CONF_CH0;
        tModbus.eState = eMODBUS_STATE_SET_CONF_CH0;
        //Form2->cxLabel1->Caption = "Writing channel 0 configuration...";
       }
      // Echec
      else
       {
        //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
        tModbus.eState = eMODBUS_STATE_FREE;
        //Form2->Close();
       }
     }
    // Echec
    else
     {
      //MessageDlg( "Communication error !", mtError, TMsgDlgButtons() << mbOK, 0 );
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
   //---------------------
   // Configuration voie 0
   //---------------------
   case( eMODBUS_STATE_SET_CONF_CH0 ):
   case( eMODBUS_STATE_SET_CONF_CH0_ONLY ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      // Délais :  la lecture se fait entre la carte interface carte
      Sleep( 100 );
      // Test si on est en écriture exclusive
      if( tModbus.eState == eMODBUS_STATE_SET_CONF_CH0_ONLY )
       {
        vFQDebug("eMODBUS_STATE_SET_CONF_CH0_ONLY");
        // On libère
        tModbus.eState = eMODBUS_STATE_FREE;
        break;
       }
      vFQDebug("eMODBUS_STATE_SET_CONF_CH0");
      // Affichage
      //Form2->cxProgressBar1->Position = 50;
      //Application->ProcessMessages();
      // Attente
      Sleep( 5 );
      // Requête
      tModbus.ucReplySize = tModbusWriteConf1.ucReplySize;
      memcpy( tModbusWriteConf1.tucData, tWIMOParametersSensor.ttChannel[2].tucBuffer, sizeof( TWIMOParametersChannelObject ) );
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusWriteConf1.ucFunction, ( TUCHAR * )tModbusWriteConf1.tucRequest, tModbusWriteConf1.ucRequestSize ) )
       {
        tModbus.eState = eMODBUS_STATE_SET_CONF_CH1;
        //Form2->cxLabel1->Caption = "Writing channel 1 configuration...";
       }
      // Echec
      else
       {
        qDebug("Communication error !");
        ptMainEvent->vFMainWindowErrorOpen( "Communication error");
        tModbus.eState = eMODBUS_STATE_FREE;
        //Form2->Close();
       }
     }
    // Echec
    else
     {
      vFQDebug("Communication error !");
      vFWIMOModbusHandleRequestError();
     }
   break;
   //---------------------
   // Configuration voie 1
   //---------------------
   case( eMODBUS_STATE_SET_CONF_CH1 ):
   case( eMODBUS_STATE_SET_CONF_CH1_ONLY ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      // Délais :  la lecture se fait entre la carte interface carte
      Sleep( 100 );
      // Test si on est en écriture exclusive
      if( tModbus.eState == eMODBUS_STATE_SET_CONF_CH1_ONLY )
       {
        vFQDebug("eMODBUS_STATE_SET_CONF_CH1_ONLY");
        // On libère
        tModbus.eState = eMODBUS_STATE_FREE;
        break;
       }
      qDebug("eMODBUS_STATE_SET_CONF_CH1");
      // Affichage
      //Form2->cxProgressBar1->Position = 75; Application->ProcessMessages();
      // Attente
      Sleep( 5 );
      // Requête
      tModbus.ucReplySize = tModbusWriteConf2.ucReplySize;
      memcpy( tModbusWriteConf2.tucData, tWIMOParametersSensor.ttChannel[2].tucBuffer, sizeof( TWIMOParametersChannelObject ) );
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusWriteConf2.ucFunction, ( TUCHAR * )tModbusWriteConf2.tucRequest, tModbusWriteConf2.ucRequestSize ) )
       {
        tModbus.eState = eMODBUS_STATE_SET_CONF_CH2;
        //Form2->cxLabel1->Caption = "Writing channel 2 configuration...";
       }
      // Echec
      else
       {
        vFQDebug("Communication error !");
        ptMainEvent->vFMainWindowErrorOpen( "Communication error");
        tModbus.eState = eMODBUS_STATE_FREE;
        //Form2->Close();
       }
     }
    // Echec
    else
     {
      vFQDebug("Communication error !");
      vFWIMOModbusHandleRequestError();
     }
   break;
   //---------------------
   // Configuration voie 2
   //---------------------
   case( eMODBUS_STATE_SET_CONF_CH2 ):
   case( eMODBUS_STATE_SET_CONF_CH2_ONLY ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      // Délais :  la lecture se fait entre la carte interface carte
      Sleep( 100 );
      vFQDebug("eMODBUS_STATE_SET_CONF_CH2");
      // Test si on est en écriture exclusive
      if( tModbus.eState == eMODBUS_STATE_SET_CONF_CH2_ONLY )
       {
        // CH2 Only
        vFQDebug("eMODBUS_STATE_SET_CONF_CH2_ONLY");
        // On libère
        tModbus.eState = eMODBUS_STATE_FREE;
        break;
       }
      // Debug
      qDebug("eMODBUS_STATE_SET_CONF_CH2");
      // Affichage
      //Form2->cxProgressBar1->Position = 100;
      //Application->ProcessMessages();
      // Attente
      Sleep( 5 );
      // Terminé
      tModbus.eState = eMODBUS_STATE_FREE;
      // Factory refresh
      if( bFactoryDataRefresh )
       {
        // Réinit
        bFactoryDataRefresh = ( TBOOL )aFALSE;
        // Attente
        Sleep( 100 );
        // Il faut faire un refresh des données
        vFWIMOModbusGetVersionNumber();
       }
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      vFWIMOModbusHandleRequestError();
     }
   break;

   //-----------------
   // O2 - Set configuration
   //-----------------
   case( eMODBUS_STATE_SET_CONF_O2 ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      vFQDebug("eMODBUS_STATE_SET_CONF_O2");
      // Attente
      Sleep( 100 );
      // Requête : Is busy ? (measuring)
      tModbus.ucReplySize = tModbusMeasuring.ucReplySize;
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusMeasuring.ucFunction, ( TUCHAR * )tModbusMeasuring.tucRequest, tModbusMeasuring.ucRequestSize ) )
       {
        tModbus.eState = eMODBUS_STATE_SET_CONF_O2_WAIT;
       }
      // Echec
      else
       {
        qDebug( "eMODBUS_STATE_SET_CONF_O2 Error 1" );
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //-----------------
   // O2 - Set configuration wait
   //-----------------
   case( eMODBUS_STATE_SET_CONF_O2_WAIT ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      vFQDebug("eMODBUS_STATE_SET_CONF_O2_WAIT");
      // Test de mesure en cours
      if( ptModBusDriverData->tucData[1] & ( TUCHAR )aWIMO_SENSOR_I2C_STATUS_CONVERTING )
       {
        qDebug("O2 Set conf not finished");
        // Mesure non terminée : attente
        Sleep( 100 );
        // Requête : Is measuring ?
        tModbus.ucReplySize = tModbusMeasuring.ucReplySize;
        if( tModBus.bFSend( ( TUCHAR )0x80, tModbusMeasuring.ucFunction, ( TUCHAR * )tModbusMeasuring.tucRequest, tModbusMeasuring.ucRequestSize ) )
         {
          tModbus.eState = eMODBUS_STATE_SET_CONF_O2_WAIT;
         }
        // Echec
        else
         {
          qDebug( "eMODBUS_STATE_MEASURING Error 1" );
          tModbus.eState = eMODBUS_STATE_FREE;
         }
       }
      else
       {
        // Terminée
        Sleep( 100 );
        qDebug("O2 Set conf finished");
        // Enregistrement du flag d'erreur
        tModbus.ucSensorFlag = ( TUCHAR )ptModBusDriverData->tucData[1];
        qDebug("Flag state: %d", tModbus.ucSensorFlag );
        //%%AP - TODO - Analyse du flag

        // Terminée
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;

   //-----------------
   // O2 - Set Air (100%)
   //-----------------
   case( eMODBUS_STATE_SET_O2_100 ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      // On trace
      vFQDebug("eMODBUS_STATE_SET_O2_100");
      // Non terminée : attente
      Sleep( 100 );
      // Commande terminée ?
      tModbus.eState = eMODBUS_STATE_SET_O2_100_IS_CMD_FINISHED_REQUEST;
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //----------------------------
   // Envoi de la requête de commande terminée
   //----------------------------
   case( eMODBUS_STATE_SET_O2_100_IS_CMD_FINISHED_REQUEST ):
    // On trace
    vFQDebug("eMODBUS_STATE_SET_O2_100_IS_CMD_FINISHED_REQUEST");
    // Requête : Is busy ? (measuring)
    tModbus.ucReplySize = tModbusMeasuring.ucReplySize;
    if( tModBus.bFSend( ( TUCHAR )0x80, tModbusMeasuring.ucFunction, ( TUCHAR * )tModbusMeasuring.tucRequest, tModbusMeasuring.ucRequestSize ) )
     {
      tModbus.eState = eMODBUS_STATE_SET_O2_100_IS_CMD_FINISHED;
      Sleep( 100 );
     }
    // Echec
    else
     {
      qDebug( "eMODBUS_STATE_SET_CONF_O2 Error 1" );
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //-----------------
   // O2 - Set Air (100%)
   //-----------------
   case( eMODBUS_STATE_SET_O2_100_IS_CMD_FINISHED ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      vFQDebug("eMODBUS_STATE_SET_O2_100_IS_CMD_FINISHED");
      // Test de mesure en cours
      if( ptModBusDriverData->tucData[1] & ( TUCHAR )aWIMO_SENSOR_I2C_STATUS_CONVERTING )
       {
        qDebug("O2 Set Air not finished");
        // Non terminée : attente
        Sleep( 100 );
        // Relance Is finished request ?
        tModbus.eState = eMODBUS_STATE_SET_O2_100_IS_CMD_FINISHED_REQUEST;
       }
      else
       {
        qDebug("O2 Set Air finished");
        // Enregistrement du flag d'erreur
        tModbus.ucSensorFlag = ( TUCHAR )ptModBusDriverData->tucData[1];
        qDebug("Flag state: %d", tModbus.ucSensorFlag );
        //%%AP - TODO - Analyse du flag

        // Terminée
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
   

   //-----------------
   // O2 - Set 0%
   //-----------------
   case( eMODBUS_STATE_SET_O2_0 ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      // On trace
      vFQDebug("eMODBUS_STATE_SET_O2_0");
      // Non terminée : attente
      Sleep( 100 );
      // Commande terminée ?
      tModbus.eState = eMODBUS_STATE_SET_O2_0_IS_CMD_FINISHED_REQUEST;
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //-----------------
   // O2 - Set 0%
   //-----------------
   case( eMODBUS_STATE_SET_O2_0_IS_CMD_FINISHED_REQUEST ):
    // On trace
    vFQDebug("eMODBUS_STATE_SET_O2_0_IS_CMD_FINISHED_REQUEST");
    // Requête : Is busy ? (measuring)
    tModbus.ucReplySize = tModbusMeasuring.ucReplySize;
    if( tModBus.bFSend( ( TUCHAR )0x80, tModbusMeasuring.ucFunction, ( TUCHAR * )tModbusMeasuring.tucRequest, tModbusMeasuring.ucRequestSize ) )
     {
      tModbus.eState = eMODBUS_STATE_SET_O2_0_IS_CMD_FINISHED;
      Sleep( 100 );
     }
    // Echec
    else
     {
      qDebug( "eMODBUS_STATE_SET_O2_0_IS_CMD_FINISHED_REQUEST Error 1" );
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //-----------------
   // O2 - Set 0%
   //-----------------
   case( eMODBUS_STATE_SET_O2_0_IS_CMD_FINISHED ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      vFQDebug("eMODBUS_STATE_SET_O2_0_IS_CMD_FINISHED");
      Sleep( 100 );
      // Test de mesure en cours
      if( ptModBusDriverData->tucData[1] & ( TUCHAR )aWIMO_SENSOR_I2C_STATUS_CONVERTING )
       {
        qDebug("O2 Set 0% not finished");
        // Non terminée : attente
        //Sleep( 100 );
        // Relance Is finished request ?
        tModbus.eState = eMODBUS_STATE_SET_O2_0_IS_CMD_FINISHED_REQUEST;
       }
      else
       {
        qDebug("O2 Set 0% finished");
        // Enregistrement du flag d'erreur
        tModbus.ucSensorFlag = ( TUCHAR )ptModBusDriverData->tucData[1];
        qDebug("Flag state: %d", tModbus.ucSensorFlag );
        //%%AP - TODO - Analyse du flag

        // Terminée
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
   
   //-----------------
   // O2 - SVS commande : sauvegarde dans le capteur
   //-----------------
   case( eMODBUS_STATE_SET_O2_SAVE ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      // On trace
      vFQDebug("eMODBUS_STATE_SET_O2_SAVE");
      // Attente
      Sleep( 100 );
      // Requête : Is busy ? (measuring)
      tModbus.ucReplySize = tModbusMeasuring.ucReplySize;
      if( tModBus.bFSend( ( TUCHAR )0x80, tModbusMeasuring.ucFunction, ( TUCHAR * )tModbusMeasuring.tucRequest, tModbusMeasuring.ucRequestSize ) )
       {
        tModbus.eState = eMODBUS_STATE_SET_O2_SAVE_WAIT;
       }
      // Echec
      else
       {
        qDebug( "eMODBUS_STATE_SET_CONF_O2 Error 1" );
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else 
     { 
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE; 
      //Form2->Close(); 
     }
   break;
   //-----------------
   // O2 - SVS commande : sauvegarde dans le capteur wait
   //-----------------
   case( eMODBUS_STATE_SET_O2_SAVE_WAIT ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      vFQDebug("eMODBUS_STATE_SET_O2_SAVE_WAIT");
      // Test de mesure en cours
      if( ptModBusDriverData->tucData[1] & ( TUCHAR )aWIMO_SENSOR_I2C_STATUS_CONVERTING )
       {
        qDebug("O2 Save conf not finished");
        // Mesure non terminée : attente
        Sleep( 100 );
        // Requête : Is measuring ?
        tModbus.ucReplySize = tModbusMeasuring.ucReplySize;
        if( tModBus.bFSend( ( TUCHAR )0x80, tModbusMeasuring.ucFunction, ( TUCHAR * )tModbusMeasuring.tucRequest, tModbusMeasuring.ucRequestSize ) )
         {
          tModbus.eState = eMODBUS_STATE_SET_O2_SAVE_WAIT;
         }
        // Echec
        else
         {
          qDebug( "eMODBUS_STATE_SET_O2_SAVE_WAIT Error 1" );
          tModbus.eState = eMODBUS_STATE_FREE;
         }
       }
      else
       {
        qDebug("O2 Save finished");
        // Enregistrement du flag d'erreur
        tModbus.ucSensorFlag = ( TUCHAR )ptModBusDriverData->tucData[1];
        qDebug("Flag state: %d", tModbus.ucSensorFlag );
        //%%AP - TODO - Analyse du flag
        // Si flag d'erreur
        if( tModbus.ucSensorFlag & aWIMO_SENSOR_I2C_STATUS_ERROR )
         {
          // Debug
          qDebug("Sensor flag error");
          // Oxygen read failled
          ptMainEvent->vFMainWindowErrorOpen("Oxygen sensor read Failed");
         }
        // Terminée
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;

   //-----------------
   // O2 - Read commande : lecture dans le capteur des données
   //-----------------
   case( eMODBUS_STATE_SET_O2_READ_CMD ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      Sleep( 100 );
      // On trace
      vFQDebug("eMODBUS_STATE_SET_O2_READ_CMD END");
      // Terminé
      //MessageDlg( "Action successfully completed !", mtInformation, TMsgDlgButtons() << mbOK, 0 );
      //%%AP - 2020.11.20 - Modification
      //tModbus.eState = eMODBUS_STATE_FREE;
      // Modification de l'état
      tModbus.eState = eMODBUS_STATE_SET_O2_IS_READ_CMD_FINISHED_REQUEST;
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
   //-----------------
   // O2 - Read commande : Interrogation si le capteur a terminée
   //-----------------
   case( eMODBUS_STATE_SET_O2_IS_READ_CMD_FINISHED_REQUEST ):
    // On trace
    vFQDebug("eMODBUS_STATE_SET_O2_IS_READ_CMD_FINISHED_REQUEST");
    // Requête : Is measuring ?
    tModbus.ucReplySize = tModbusMeasuring.ucReplySize;
    if( tModBus.bFSend( ( TUCHAR )0x80, tModbusMeasuring.ucFunction, ( TUCHAR * )tModbusMeasuring.tucRequest, tModbusMeasuring.ucRequestSize ) )
     tModbus.eState = eMODBUS_STATE_SET_O2_IS_READ_CMD_FINISHED;
    // Echec
    else
     {
      qDebug( "eMODBUS_STATE_MEASURING Error 1" );
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //%%AP - 2020.11.20 - Interrogation si le capteur a terminée
   //-----------------
   // O2 - Read commande : Interrogation si le capteur a terminée
   //-----------------
   case( eMODBUS_STATE_SET_O2_IS_READ_CMD_FINISHED ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      Sleep( 100 );
      vFQDebug("eMODBUS_STATE_SET_O2_IS_READ_CMD_FINISHED");
      // Test de mesure en cours
      if( ptModBusDriverData->tucData[1] & ( TUCHAR )aWIMO_SENSOR_I2C_STATUS_CONVERTING )
       {
        vFQDebug("O2 Read not finished");
        // Mesure non terminée : attente
        Sleep( 350 );
        // Relance d'une interrogation
        tModbus.eState = eMODBUS_STATE_SET_O2_IS_READ_CMD_FINISHED_REQUEST;
       }
      else
       {
        vFQDebug("O2 Read finished");
        // Enregistrement du flag d'erreur
        tModbus.ucSensorFlag = ( TUCHAR )ptModBusDriverData->tucData[1];
        qDebug("Flag state: %d", tModbus.ucSensorFlag );
        // Terminée
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;

   //-----------------
   // O2 - Read commande : lecture dans le capteur du temperature offset
   //-----------------
   case( eMODBUS_STATE_SET_O2_READ_TEMPERATURE_OFFSET_CMD ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      Sleep( 100 );
      // On trace
      vFQDebug("eMODBUS_STATE_SET_O2_READ_TEMPERATURE_OFFSET_CMD END");
      // Terminé
      //MessageDlg( "Action successfully completed !", mtInformation, TMsgDlgButtons() << mbOK, 0 );
      //%%AP - 2020.11.20 - Modification
      //tModbus.eState = eMODBUS_STATE_FREE;
      // Modification de l'état
      tModbus.eState = eMODBUS_STATE_SET_O2_IS_READ_TEMPERATURE_OFFSET_CMD_FINISHED_REQUEST;
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
   //-----------------
   // O2 - Read commande : Interrogation si le capteur a terminée
   //-----------------
   case( eMODBUS_STATE_SET_O2_IS_READ_TEMPERATURE_OFFSET_CMD_FINISHED_REQUEST ):
    // On trace
    vFQDebug("eMODBUS_STATE_SET_O2_IS_READ_TEMPERATURE_OFFSET_CMD_FINISHED_REQUEST");
    // Requête : Is measuring ?
    tModbus.ucReplySize = tModbusMeasuring.ucReplySize;
    if( tModBus.bFSend( ( TUCHAR )0x80, tModbusMeasuring.ucFunction, ( TUCHAR * )tModbusMeasuring.tucRequest, tModbusMeasuring.ucRequestSize ) )
     tModbus.eState = eMODBUS_STATE_SET_O2_IS_READ_TEMPERATURE_OFFSET_CMD_FINISHED;
    // Echec
    else
     {
      qDebug( "eMODBUS_STATE_MEASURING Error 1" );
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //%%AP - 2020.11.20 - Interrogation si le capteur a terminée
   //-----------------
   // O2 - Read commande : Interrogation si le capteur a terminée
   //-----------------
   case( eMODBUS_STATE_SET_O2_IS_READ_TEMPERATURE_OFFSET_CMD_FINISHED ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      Sleep( 100 );
      vFQDebug("eMODBUS_STATE_SET_O2_IS_READ_TEMPERATURE_OFFSET_CMD_FINISHED");
      // Test de mesure en cours
      if( ptModBusDriverData->tucData[1] & ( TUCHAR )aWIMO_SENSOR_I2C_STATUS_CONVERTING )
       {
        vFQDebug("O2 Read temperature offset not finished");
        // Mesure non terminée : attente
        Sleep( 350 );
        // Relance d'une interrogation
        tModbus.eState = eMODBUS_STATE_SET_O2_IS_READ_TEMPERATURE_OFFSET_CMD_FINISHED_REQUEST;
       }
      else
       {
        vFQDebug("O2 Read temperature offset finished");
        // Enregistrement du flag d'erreur
        tModbus.ucSensorFlag = ( TUCHAR )ptModBusDriverData->tucData[1];
        qDebug("Flag state: %d", tModbus.ucSensorFlag );
        // Terminée
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;

   //-----------------
   // O2 - Write commande : Ecriture dans le capteur de l'offset en temperature
   //-----------------
   case( eMODBUS_STATE_SET_O2_WRITE_TEMPERATURE_OFFSET_CMD ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      Sleep( 100 );
      // On trace
      vFQDebug("eMODBUS_STATE_SET_O2_WRITE_TEMPERATURE_OFFSET_CMD END");
      // Terminé
      //MessageDlg( "Action successfully completed !", mtInformation, TMsgDlgButtons() << mbOK, 0 );
      //%%AP - 2020.11.20 - Modification
      //tModbus.eState = eMODBUS_STATE_FREE;
      // Modification de l'état
      tModbus.eState = eMODBUS_STATE_SET_O2_IS_WRITE_TEMPERATURE_OFFSET_CMD_FINISHED_REQUEST;
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
   //-----------------
   // O2 - Read commande : Interrogation si le capteur a terminée
   //-----------------
   case( eMODBUS_STATE_SET_O2_IS_WRITE_TEMPERATURE_OFFSET_CMD_FINISHED_REQUEST ):
    // On trace
    vFQDebug("eMODBUS_STATE_SET_O2_IS_WRITE_TEMPERATURE_OFFSET_CMD_FINISHED_REQUEST");
    // Requête : Is measuring ?
    tModbus.ucReplySize = tModbusMeasuring.ucReplySize;
    if( tModBus.bFSend( ( TUCHAR )0x80, tModbusMeasuring.ucFunction, ( TUCHAR * )tModbusMeasuring.tucRequest, tModbusMeasuring.ucRequestSize ) )
     tModbus.eState = eMODBUS_STATE_SET_O2_IS_READ_CMD_FINISHED;
    // Echec
    else
     {
      qDebug( "eMODBUS_STATE_MEASURING Error 1" );
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //%%AP - 2020.11.20 - Interrogation si le capteur a terminée
   //-----------------
   // O2 - Read commande : Interrogation si le capteur a terminée
   //-----------------
   case( eMODBUS_STATE_SET_O2_IS_WRITE_TEMPERATURE_OFFSET_CMD_FINISHED ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      Sleep( 100 );
      vFQDebug("eMODBUS_STATE_SET_O2_IS_WRITE_TEMPERATURE_OFFSET_CMD_FINISHED");
      // Test de mesure en cours
      if( ptModBusDriverData->tucData[1] & ( TUCHAR )aWIMO_SENSOR_I2C_STATUS_CONVERTING )
       {
        vFQDebug("O2 write offset not finished");
        // Mesure non terminée : attente
        Sleep( 350 );
        // Relance d'une interrogation
        tModbus.eState = eMODBUS_STATE_SET_O2_IS_WRITE_TEMPERATURE_OFFSET_CMD_FINISHED_REQUEST;
       }
      else
       {
        vFQDebug("O2 write offset finished");
        // Enregistrement du flag d'erreur
        tModbus.ucSensorFlag = ( TUCHAR )ptModBusDriverData->tucData[1];
        qDebug("Flag state: %d", tModbus.ucSensorFlag );
        // Terminée
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;

   //-----------------
   // O2 - CMD pour écriture des points dph0 et dphi100 manuellement
   //-----------------
   case( eMODBUS_STATE_SET_O2_SET_POINT_MANUALLY_CMD ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      // Terminé
      //MessageDlg( "Action successfully completed !", mtInformation, TMsgDlgButtons() << mbOK, 0 );
      tModbus.eState = eMODBUS_STATE_FREE;
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;


   //-----------------
   // O2 - CMD pour écriture de la configuration capteur initiale
   //-----------------
   case( eMODBUS_STATE_SET_O2_SET_SENSOR_SETTINGS_CMD ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      // Attente
      Sleep( 100 );
      // Debug
      vFQDebug( "eMODBUS_STATE_SET_O2_SET_SENSOR_SETTINGS_CMD END" );
      // Terminé
      //MessageDlg( "Action successfully completed !", mtInformation, TMsgDlgButtons() << mbOK, 0 );
      tModbus.eState = eMODBUS_STATE_O2_SET_SENSOR_SETTINGS_CMD_IS_FINISHED_REQUEST;
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
   //-----------------
   // O2 - CMD pour écriture de la configuration capteur initiale
   //-----------------
   case( eMODBUS_STATE_O2_SET_SENSOR_SETTINGS_CMD_IS_FINISHED_REQUEST ):
    // On trace
    vFQDebug("eMODBUS_STATE_O2_SET_SENSOR_SETTINGS_CMD_IS_FINISHED_REQUEST");
    // Requête : Is measuring ?
    tModbus.ucReplySize = tModbusMeasuring.ucReplySize;
    if( tModBus.bFSend( ( TUCHAR )0x80, tModbusMeasuring.ucFunction, ( TUCHAR * )tModbusMeasuring.tucRequest, tModbusMeasuring.ucRequestSize ) )
     tModbus.eState = eMODBUS_STATE_O2_SET_SENSOR_SETTINGS_CMD_IS_FINISHED;
    // Echec
    else
     {
      qDebug( "eMODBUS_STATE_O2_SET_SENSOR_SETTINGS_CMD_IS_FINISHED_REQUEST Error 1" );
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //-----------------
   // O2 - CMD pour écriture de la configuration capteur initiale
   //-----------------
   case( eMODBUS_STATE_O2_SET_SENSOR_SETTINGS_CMD_IS_FINISHED ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      Sleep( 100 );
      vFQDebug("eMODBUS_STATE_O2_SET_SENSOR_SETTINGS_CMD_IS_FINISHED");
      // Test de mesure en cours
      if( ptModBusDriverData->tucData[1] & ( TUCHAR )aWIMO_SENSOR_I2C_STATUS_CONVERTING )
       {
        qDebug("O2 Write conf not finished");
        // Mesure non terminée : attente
        Sleep( 350 );
        // Relance d'une interrogation
        tModbus.eState = eMODBUS_STATE_O2_SET_SENSOR_SETTINGS_CMD_IS_FINISHED_REQUEST;
       }
      else
       {
        // Attente
        Sleep( 100 );
        qDebug("O2 Write conf finished");
        // Enregistrement du flag d'erreur
        tModbus.ucSensorFlag = ( TUCHAR )ptModBusDriverData->tucData[1];
        qDebug("Flag state: %d", tModbus.ucSensorFlag );
        // Terminée
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      vFQDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;

   //-----------------
   // O2 - CMD pour reset de la calibration avec membrane
   //-----------------
   case( eMODBUS_STATE_SET_O2_START_CALIBRATION_CMD ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      Sleep( 100 );
      vFQDebug( "eMODBUS_STATE_SET_O2_START_CALIBRATION_CMD END" );
      // Terminé
      //MessageDlg( "Action successfully completed !", mtInformation, TMsgDlgButtons() << mbOK, 0 );
      tModbus.eState = eMODBUS_STATE_FREE;
     }
    // Echec
    else
     {
      vFQDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;

   //-----------------
   // O2 - CMD pour switch en mode calibration du capteur
   //-----------------
   case( eMODBUS_STATE_O2_SWITCH_TO_CALIBRATION_MODE_CMD ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      vFQDebug( "eMODBUS_STATE_O2_SWITCH_TO_CALIBRATION_MODE_CMD END" );
      // Changement de mode : attente
      Sleep( 350 );
      // Requête : Is measuring ?
      tModbus.eState = eMODBUS_STATE_O2_IS_SWITCH_CALIBRATION_FINISHED_CMD_SEND_CMD;
     }
    // Echec
    else
     {
      vFQDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //-----------------
   // O2 - Envoi de la commande
   //-----------------
   case( eMODBUS_STATE_O2_IS_SWITCH_CALIBRATION_FINISHED_CMD_SEND_CMD ):
    // Requête : Is measuring ?
    tModbus.ucReplySize = tModbusMeasuring.ucReplySize;
    if( tModBus.bFSend( ( TUCHAR )0x80, tModbusMeasuring.ucFunction, ( TUCHAR * )tModbusMeasuring.tucRequest, tModbusMeasuring.ucRequestSize ) )
     {
      Sleep( 100 );
      tModbus.eState = eMODBUS_STATE_SET_O2_IS_SWITCH_CALIBRATION_CMD_FINISHED;
     }
    // Echec
    else
     {
      qDebug( "eMODBUS_STATE_MEASURING Error 1" );
      tModbus.eState = eMODBUS_STATE_FREE;
     }
   break;
   //%%AP - 2020.11.20 - Interrogation si le capteur a terminée
   //-----------------
   // O2 - Read commande : Interrogation si le capteur a terminée
   //-----------------
   case( eMODBUS_STATE_SET_O2_IS_SWITCH_CALIBRATION_CMD_FINISHED ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      Sleep( 100 );
      vFQDebug("eMODBUS_STATE_SET_O2_IS_SWITCH_CALIBRATION_CMD_FINISHED");
      // Test de mesure en cours
      //if( ptModBusDriverData->tucData[1] & 0x10 )
      if( ptModBusDriverData->tucData[1] & ( TUCHAR )aWIMO_SENSOR_I2C_STATUS_CONVERTING )
       {
        vFQDebug("O2 Switch calibration mode not finished");
        // Changement de mode : attente
        Sleep( 350 );
        // Relance d'une commande
        tModbus.eState = eMODBUS_STATE_O2_IS_SWITCH_CALIBRATION_FINISHED_CMD_SEND_CMD;
       }
      else
       {
        vFQDebug("O2 Switch calibration mode finished");
        // Enregistrement du flag d'erreur
        tModbus.ucSensorFlag = ( TUCHAR )ptModBusDriverData->tucData[1];
        qDebug("Flag state: %d", tModbus.ucSensorFlag );
        //%%AP - TODO
        // Test si flag d'erreur

        // Terminée
        tModbus.eState = eMODBUS_STATE_FREE;
       }
     }
    // Echec
    else
     {
      vFQDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;

   //-----------------
   // CMD Demande de mise à jour de l'interface/outil
   //-----------------
   case( eMODBUS_STATE_UPDATE_INTERFACE_CMD ):
    // Attente réponse
    if( !tModBus.bFResponse() ) break;
    // Traitement réponses
    if( tModBus.ucFReadResponse( &ptModBusDriverData ) == aMODBUS_CLIENT_DATA_RECEIVED )
     {
      qDebug( "eMODBUS_STATE_UPDATE_INTERFACE_CMD END" );
      // Terminé
      //MessageDlg( "Action successfully completed !", mtInformation, TMsgDlgButtons() << mbOK, 0 );
      tModbus.eState = eMODBUS_STATE_FREE;
     }
    // Echec
    else
     {
      qDebug("Communication error !");
      ptMainEvent->vFMainWindowErrorOpen( "Communication error");
      tModbus.eState = eMODBUS_STATE_FREE;
      //Form2->Close();
     }
   break;
  }


 // On retourne l'état d'activité du grafcet
 return( ( TBOOL )bActive );
}

//----------------------------------------------------------------------------//
// Gestion des erreurs d'écriture du grafcet
//----------------------------------------------------------------------------//
void vFWIMOModbusHandleRequestError( void )
{
 TEWIMOModbusState eLastState = tModbus.eState;

 // Test si on a atteint le nombre de relance max
 if( tModbus.ucRequestRetry > ( TUCHAR )3 )
  {
   // Abandon de la requête
   tModbus.ucRequestRetry = ( TUCHAR )0;
   tModbus.eState = eMODBUS_STATE_FREE;
   // Notification de l'erreur
   qDebug("Communication error !");
   ptMainEvent->vFMainWindowErrorOpen( "Communication error: please check sensor configuration");
   //!!!!!!!!!
   //! On peut imaginer une fermeture du COM
   //! et clear de la fenêtre avec retour sur le dashboard
   return;
  }
 // Incrément du compteur de reprise
 tModbus.ucRequestRetry++;
 // Délais de reprise
 Sleep( 100 );
 // Selon l'état courant du grafcet
 switch( tModbus.eState )
  {
   //---------------
   // Set Ch0
   //---------------
   case( eMODBUS_STATE_SET_CONF_CH0 ):
   case( eMODBUS_STATE_SET_CONF_CH0_ONLY ):
    // Sauvegarde de la voie 0
    tModbus.eState = eMODBUS_STATE_FREE;
    vFWIMOModbusSaveCh0ToSensor();
    tModbus.eState = eLastState;
   break;
   //---------------
   // Set Ch1
   //---------------
   case( eMODBUS_STATE_SET_CONF_CH1 ):
   case( eMODBUS_STATE_SET_CONF_CH1_ONLY ):
    // Sauvegarde de la voie 1
    tModbus.eState = eMODBUS_STATE_FREE;
    vFWIMOModbusSaveCh1ToSensor();
    tModbus.eState = eLastState;
   break;
   //---------------
   // Set Ch2
   //---------------
   case( eMODBUS_STATE_SET_CONF_CH2 ):
   case( eMODBUS_STATE_SET_CONF_CH2_ONLY ):
    // Sauvegarde de la voie 2
    tModbus.eState = eMODBUS_STATE_FREE;
    vFWIMOModbusSaveCh2ToSensor();
    tModbus.eState = eLastState;
   break;
   //---------------
   // Default
   //---------------
   default:
    tModbus.eState = eMODBUS_STATE_FREE;
   break;
  }
}

#endif
