//============================================================================//
// Fichier .........: "WIMOParameters.h"
// Auteur ..........: Christophe SCHAEFFER
// Date de création : 13/02/2014
//----------------------------------------------------------------------------//
/* Description :
    Gestion des paramètres de WIMO.
*/
//============================================================================//

#ifndef WIMO_PARAMETER_H
#define WIMO_PARAMETER_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeCMMN.h>
#include <TypeC.h>
#include <SYSTEMAcquisition.h>
#include <PILETypes_1_20.h>
#include <DATETIMELib_4_05.h>
#include <main.h>

//#pragma pack(1)
#pragma pack(push, 1)

#include "WIMOMath.h"

//============================================================================//
// Déclaration des constantes
//============================================================================//
// Liste des status
#define aWIMO_SENSOR_I2C_STATUS_INIT            ( TUCHAR )0x00
#define aWIMO_SENSOR_I2C_STATUS_DATA_0_VALID    ( TUCHAR )aBIT0
#define aWIMO_SENSOR_I2C_STATUS_DATA_1_VALID    ( TUCHAR )aBIT1
#define aWIMO_SENSOR_I2C_STATUS_DATA_2_VALID    ( TUCHAR )aBIT2
//...                                           ( TUCHAR )aBIT3
#define aWIMO_SENSOR_I2C_STATUS_CONVERTING      ( TUCHAR )aBIT4
#define aWIMO_SENSOR_I2C_STATUS_CLEANING        ( TUCHAR )aBIT5
//%%AP - Ajout d'un bit de communication avec le master
#define aWIMO_SENSOR_I2C_STATUS_RESTARTING      ( TUCHAR )aBIT6
#define aWIMO_SENSOR_I2C_STATUS_ERROR           ( TUCHAR )aBIT7

// Types de capteurs
#define aWIMO_PARAMETERS_SENSOR_TYPE_CONSTANT          ( TCHAR )'C'
#define aWIMO_PARAMETERS_SENSOR_TYPE_SENSOR            ( TCHAR )'S'
#define aWIMO_PARAMETERS_SENSOR_TYPE_WIPER             ( TCHAR )'W'
#define aWIMO_PARAMETERS_SENSOR_TYPE_CLEANER_UV        ( TCHAR )'U'
#define aWIMO_PARAMETERS_SENSOR_TYPE_VIRTUAL           ( TCHAR )'V'
// Voies de mesure
#define aWIMO_PARAMETERS_CHANNEL_PRESSURE_STATIC       ( TUCHAR )0
#define aWIMO_PARAMETERS_CHANNEL_PRESSURE_KELLER       ( TUCHAR )1
#define aWIMO_PARAMETERS_CHANNEL_TEMPERATURE_KELLER    ( TUCHAR )2
#define aWIMO_PARAMETERS_CHANNEL_TEMPERATURE           ( TUCHAR )3
#define aWIMO_PARAMETERS_CHANNEL_CONDUCTIVITY          ( TUCHAR )4
#define aWIMO_PARAMETERS_CHANNEL_TURBIDITY             ( TUCHAR )5
#define aWIMO_PARAMETERS_CHANNEL_OXYGEN_CONCENTRATION  ( TUCHAR )6
#define aWIMO_PARAMETERS_CHANNEL_OXYGEN_SATURATION     ( TUCHAR )7
#define aWIMO_PARAMETERS_CHANNEL_PH                    ( TUCHAR )8
#define aWIMO_PARAMETERS_CHANNEL_TURNER_CHLA           ( TUCHAR )9
#define aWIMO_PARAMETERS_CHANNEL_TURNER_PHYCOC         ( TUCHAR )10
#define aWIMO_PARAMETERS_CHANNEL_TURNER_PHYCOE         ( TUCHAR )11
#define aWIMO_PARAMETERS_CHANNEL_TURNER_CDOM           ( TUCHAR )12
#define aWIMO_PARAMETERS_CHANNEL_FLUO_CHLA             ( TUCHAR )13
#define aWIMO_PARAMETERS_CHANNEL_PAR                   ( TUCHAR )14
#define aWIMO_PARAMETERS_CHANNEL_FLUO_PHYCOC           ( TUCHAR )15
#define aWIMO_PARAMETERS_CHANNEL_FLUO_PHYCOE           ( TUCHAR )16
#define aWIMO_PARAMETERS_CHANNEL_FLUO_CDOM             ( TUCHAR )17
#define aWIMO_PARAMETERS_CHANNEL_FLUO_TRYPTO           ( TUCHAR )18
#define aWIMO_PARAMETERS_CHANNEL_TURNER_TRYPTO         ( TUCHAR )19
#define aWIMO_PARAMETERS_CHANNEL_S20_REDOX             ( TUCHAR )20
#define aWIMO_PARAMETERS_CHANNEL_S21_AMMONIUM          ( TUCHAR )21
#define aWIMO_PARAMETERS_CHANNEL_S22_POTASSIUM         ( TUCHAR )22
#define aWIMO_PARAMETERS_CHANNEL_S23_NITRATE           ( TUCHAR )23
#define aWIMO_PARAMETERS_CHANNEL_S24_CHLORIDE          ( TUCHAR )24
#define aWIMO_PARAMETERS_CHANNEL_S25_O2_TEMPERATURE    ( TUCHAR )25
#define aWIMO_PARAMETERS_CHANNEL_S26_CRUDE_OIL_TURNER  ( TUCHAR )26
#define aWIMO_PARAMETERS_CHANNEL_S27_CRUDE_OIL_NKE     ( TUCHAR )27
//...
#define aWIMO_PARAMETERS_CHANNEL_SALINITY              ( TUCHAR )30
#define aWIMO_PARAMETERS_CHANNEL_SOUND_VELOCITY        ( TUCHAR )31
#define aWIMO_PARAMETERS_CHANNEL_CONDUCTIVITY_25       ( TUCHAR )32
#define aWIMO_PARAMETERS_CHANNEL_DISSOLVED_SOLIDS      ( TUCHAR )33
#define aWIMO_PARAMETERS_CHANNEL_CHLORIDE              ( TUCHAR )34
#define aWIMO_PARAMETERS_CHANNEL_DEPTH                 ( TUCHAR )35
#define aWIMO_PARAMETERS_CHANNEL_OXYGEN_COMPENSATED    ( TUCHAR )36
//...
#define aWIMO_PARAMETERS_CHANNEL_TEST                  ( TUCHAR )60
// Voies locales
#define aWIMO_PARAMETERS_3_CHANNEL_PRESSURE_STATIC     ( TUCHAR )0
#define aWIMO_PARAMETERS_3_CHANNEL_PRESSURE_KELLER     ( TUCHAR )1
#define aWIMO_PARAMETERS_3_CHANNEL_TEMPERATURE_KELLER  ( TUCHAR )2
#define aWIMO_PARAMETERS_3_CHANNEL_SALINITY            ( TUCHAR )3
#define aWIMO_PARAMETERS_3_CHANNEL_SOUND_VELOCITY      ( TUCHAR )4
#define aWIMO_PARAMETERS_3_CHANNEL_CONDUCTIVITY_25     ( TUCHAR )5
#define aWIMO_PARAMETERS_3_CHANNEL_DISSOLVED_SOLIDS    ( TUCHAR )6
#define aWIMO_PARAMETERS_3_CHANNEL_CHLORIDE            ( TUCHAR )7
#define aWIMO_PARAMETERS_3_CHANNEL_DEPTH               ( TUCHAR )8
#define aWIMO_PARAMETERS_3_CHANNEL_OXYGEN_COMPENSATED  ( TUCHAR )9
//...
#define aWIMO_PARAMETERS_3_CHANNEL_NUMBER              ( TUCHAR )12
// Types de calibration
#define aWIMO_PARAMETERS_CALIBRATION_NONE              ( TCHAR )'N'
#define aWIMO_PARAMETERS_CALIBRATION_MULTIPOINT        ( TCHAR )'M'
#define aWIMO_PARAMETERS_CALIBRATION_LINEAR            ( TCHAR )'L'
#define aWIMO_PARAMETERS_CALIBRATION_STEINHART         ( TCHAR )'S'
#define aWIMO_PARAMETERS_CALIBRATION_BENNETT           ( TCHAR )'B'
#define aWIMO_PARAMETERS_CALIBRATION_POLYNOMIAL2       ( TCHAR )'2'
#define aWIMO_PARAMETERS_CALIBRATION_POLYNOMIAL3       ( TCHAR )'3'
// Types d'interface Modbus
#define aWIMO_PARAMETERS_INTERFACE_RS485               ( TCHAR )'5'
#define aWIMO_PARAMETERS_INTERFACE_RS232               ( TCHAR )'2'
// Gestion de modems
#define aWIMO_PARAMETERS_EMAP_TYPE_HOST                ( TCHAR )'H'
#define aWIMO_PARAMETERS_EMAP_TYPE_MODEM_3G            ( TCHAR )'3'
#define aWIMO_PARAMETERS_EMAP_TYPE_MODEM_RUDICS        ( TCHAR )'R'
#define aWIMO_PARAMETERS_EMAP_TYPE_GPS                 ( TCHAR )'G'
// Gestion du concentrateur
#define aWIMO_PARAMETERS_CONCENTRATEUR_NKE             ( TCHAR )'N'
// Gestion Buzzer
#define aWIMO_PARAMETERS_BUZZER_NONE                   ( TUCHAR )0x00
#define aWIMO_PARAMETERS_BUZZER_ON_POWER_ON            ( TUCHAR )aBIT0
#define aWIMO_PARAMETERS_BUZZER_ON_WIFI_ACTIVATION     ( TUCHAR )aBIT1
#define aWIMO_PARAMETERS_BUZZER_ON_WIFI_ON             ( TUCHAR )aBIT2
// Gestion balai
#define aWIMO_PARAMETERS_WIPER_COMPATIBILITY_CLEAN_ON  ( TCHAR )'C'
#define aWIMO_PARAMETERS_WIPER_COMPATIBILITY_CLEAN_OFF ( TCHAR )'N'
#define aWIMO_PARAMETERS_WIPER_COMPATIBILITY_AVOID     ( TCHAR )'A'
// Gestion alertes/alarmes
#define aWIMO_PARAMETERS_ALARM_NOT_USED                ( TCHAR )'N'
#define aWIMO_PARAMETERS_ALARM_ALERT                   ( TCHAR )'E'
#define aWIMO_PARAMETERS_ALARM_ALARM                   ( TCHAR )'A'
// Gestion des conditions de test
#define aWIMO_PARAMETERS_TEST_SIGN_SUPERIOR            ( TCHAR )'S'
#define aWIMO_PARAMETERS_TEST_SIGN_INFERIOR            ( TCHAR )'I'
// Liste des modes de marche
#define aWIMO_PARAMETERS_START_MODE_MANUAL             ( TCHAR )'M'
#define aWIMO_PARAMETERS_START_MODE_DATE               ( TCHAR )'D'
#define aWIMO_PARAMETERS_START_MODE_CONDITION          ( TCHAR )'C'
// Liste des modes d'arrêt
#define aWIMO_PARAMETERS_STOP_MODE_MANUAL              ( TCHAR )'M'
#define aWIMO_PARAMETERS_STOP_MODE_DATE                ( TCHAR )'D'
#define aWIMO_PARAMETERS_STOP_MODE_CONDITION           ( TCHAR )'C'
// Liste des modes d'échantillonnage
#define aWIMO_PARAMETERS_SAMPLING_MODE_CONTINUOUS      ( TCHAR )'C'
#define aWIMO_PARAMETERS_SAMPLING_MODE_BURST           ( TCHAR )'B'
#define aWIMO_PARAMETERS_SAMPLING_MODE_TIDE            ( TCHAR )'T'
#define aWIMO_PARAMETERS_SAMPLING_MODE_WAVE            ( TCHAR )'W'
// Liste des types d'échantillonnage
#define aWIMO_PARAMETERS_SAMPLING_TYPE_PERIOD          ( TCHAR )'P'
#define aWIMO_PARAMETERS_SAMPLING_TYPE_RATE            ( TCHAR )'R'
// Gestion format d'export
#define aWIMO_PARAMETERS_FILE_FORMAT_CSV               ( TCHAR )'C'
#define aWIMO_PARAMETERS_FILE_FORMAT_TOPKAPI           ( TCHAR )'T'
// Gestion format d'enregistrement
#define aWIMO_PARAMETERS_STORE_FORMAT_NONE             ( TCHAR )'N'
#define aWIMO_PARAMETERS_STORE_FORMAT_FLOAT_0          ( TCHAR )'0'
#define aWIMO_PARAMETERS_STORE_FORMAT_FLOAT_1          ( TCHAR )'1'
#define aWIMO_PARAMETERS_STORE_FORMAT_FLOAT_2          ( TCHAR )'2'
#define aWIMO_PARAMETERS_STORE_FORMAT_FLOAT_3          ( TCHAR )'3'
#define aWIMO_PARAMETERS_STORE_FORMAT_FLOAT_4          ( TCHAR )'4'
#define aWIMO_PARAMETERS_STORE_FORMAT_FLOAT_5          ( TCHAR )'5'
// Nombre de points étalonnage
#define aWIMO_PARAMETERS_LINEAR_POINTS_NUMBER_MAX      ( TUCHAR )14
// Etat d'activation
#define aWIMO_PARAMETERS_FUNCTION_ACTIVE               ( TCHAR )'1'
#define aWIMO_PARAMETERS_FUNCTION_INACTIVE             ( TCHAR )'0'
// Types de batteries
#define aWIMO_PARAMETERS_BATTERY_TYPE_ALKALINE         ( TCHAR )'A'
#define aWIMO_PARAMETERS_BATTERY_TYPE_NIMH             ( TCHAR )'N'
// Gestion des actions alarmes
#define aWIMO_PARAMETERS_ALARM_ACTION_NONE             ( TCHAR )'N'
#define aWIMO_PARAMETERS_ALARM_ACTION_FILE             ( TCHAR )'F'

//============================================================================//
// Déclaration des types
//============================================================================//
//-------------------------
// Point étalonnage
//-------------------------

typedef struct
{
 TFLOAT fPhysical;
 TFLOAT fRaw;
}
TWIMOMathPointObject;
/*
typedef struct
{
 TDOUBLE dCoeffA;
 TDOUBLE dCoeffB;
 TDOUBLE dCoeffC;
 TDOUBLE dCoeffD;
}
TWIMOMathCTNObject;

typedef struct
{
 TDOUBLE dCoeffA;
 TDOUBLE dCoeffB;
 TDOUBLE dCoeffC;
}
TWIMOMathPolynome2Object;

typedef struct
{
 TFLOAT fCoeffA;
 TFLOAT fCoeffB;
}
TWIMOMathLinearObject;
*/

// Configuration sensor principale
typedef struct
{
 union
  {
   TUCHAR tucBuffer[128];
   struct
    {
     struct
      {
       TCHAR cActive;
       TCHAR tcIdentifier[ 8 ];
       TCHAR tcManufacturer[ 16 ];
       TCHAR tcVersion[ 16 ];
       TCHAR cType;
       TCHAR cWiperCompatibility;
       TUCHAR ucSerialNumberY;
       TUINT uiSerialNumberN;
      }
     tProduct;
     union
      {
       TUCHAR tucBufferSensor[ 32 ];
       struct
        {
         TUCHAR ucChannelNumber;
         TUINT uiPowerOnTime;
         TUINT uiAcquisitionTime;
         union
          {
           // Paramètre spécifique capteur
           // Nombre d'échantillons usines
           TUINT  uiAverageSamples;
           // Paramètre spécifique capteur
           struct
            {
             TUINT  uiIntegrationTime;
             TUCHAR ucRange;
             TUINT  uiAverageSamples;
             TUINT  uiIntegrationSum;
             TUCHAR ucTestMode;
             TUINT  uiPowerOn;
            } tEvosens;
          };
        }
       tSensor;
       struct
        {
         TUINT uiClockwiseAngle;
         TUINT uiCounterClockwiseAngle;
         TCHAR cPeriod;
         TUINT uiParkingAngle;
        }
       tWiper;
       struct
        {
         TCHAR cPeriod;
         TCHAR cDuration;
        }
       tCleanerUV;
      };
     struct
      {
       TFLOAT fSleep_mA;
       TFLOAT fRunning_mA;
      }
     tConsumption;
     union
      {
       TUCHAR tucBuffer2[ 2 ];
       // Eau douce ou eau de mer ?
       TUCHAR ucModel;
      };
     union
      {
       // Paramètre spécifique capteur CT
       struct
        {
         // Point de compensation
         struct
          {
           union
            {
             TFLOAT fA1;
             TULONGINT uliA1;
            };
           union
            {
             TFLOAT fA2;
             TULONGINT uliA2;
            };
           union
            {
             TFLOAT fA3;
             TULONGINT uliA3;
            };
           union
            {
             TFLOAT fRaw20;
             TULONGINT uliRaw20;
            };
          }
         tCompensation;
        }
       tCT;
      };
    };
  };
}
TWIMOParametersHeaderObject;


//#pragma pack(1)
// Configuration voie de mesure
typedef struct
{
 union
  {
   TUCHAR tucBuffer[246];
   struct
    {
     TCHAR cActive;
     TUCHAR ucParameter;
     TUINT uiAverage;
     TCHAR tcName[54];
     TCHAR tcUnit[16];
     TCHAR cFormat;
     /*
     struct
      {
       TFLOAT fThresholdLow;
       TFLOAT fThresholdHigh;
      }
     tWarning;
     struct
      {
       TFLOAT fThresholdLow;
       TFLOAT fThresholdHigh;
      }
     tAlert;
     */
     TWIMOMathPointObject ttPointTheo[2];
     TCHAR cCalibration;
     TWIMOMathPointObject ttPoints[ 14 ];
     union
      {
       TWIMOMathCTNObject tWIMOMathCTN;
       TWIMOMathPolynome2Object tWIMOMathPolynome2;
       TWIMOMathPolynome3Object tWIMOMathPolynome3;
       TWIMOMathLinearObject tWIMOMathLinear;
       union
        {
         TFLOAT fValue;
         TFLOAT fOffset;
         TFLOAT fFactor;
         TFLOAT fLatitude;
        };
      };
     TULONGINT uliDateTimeCalibration;
     union
      {
       struct
        {
         // Temperature de calibration
         TFLOAT  fCalibrationTemperature;
         // Temps d'acquisition pour la voie (interne au capteur : pas dans la WiMo sonde)
         TUINT uiAcquisitionTime;
        } tPH;
       // Temps d'acquisition pour la voie (interne au capteur : pas dans la WiMo sonde)
       TUINT uiAcquisitionTime;
      };
    };
  };
}
TWIMOParametersChannelObject;


typedef struct
{
 TWIMOParametersHeaderObject tHeader;
 TWIMOParametersChannelObject ttChannel[3];
}
TWIMOParametersSensorObject;
/*
typedef struct
{
 TCHAR tcCode[ sizeof( "SC7541206" ) - 1 ];
 TBOOL bExternalTemperature;
 TBOOL bExternalTemperature0;
 TBOOL bExternalTemperature100;
 TFLOAT fTemperature;
 TFLOAT fTemperature0;
 TFLOAT fTemperature100;
 TFLOAT fPressure;
 TFLOAT fPressure0;
 TFLOAT fPressure100;
}*/
typedef struct
{
 TBOOL  bExternalTemperature;
 TLONGINT liTemperature;
 TULONGINT uliPressure;
 // SC7-540-200
 TUCHAR ucSensorType;
 TUCHAR ucIntensity;
 TUCHAR ucAmp;
 // Phase shift with sensor code
 TULONGINT uliCodeDphi0;
 TULONGINT uliCodeDphi100;
 // Temperature offset
 TLONGINT  liTemperatureOffset;
 TULONGINT uliDummy;
 // Etat de la calibration
 TBOOL  bCalibrated;
 // Calibration
 TBOOL     bExternalTemperature0;
 TLONGINT  liTemperature0;
 TBOOL     bExternalTemperature100;
 TLONGINT  liTemperature100;
 TULONGINT uliPressure100;
}
TWIMOPyroO2ConfigurationObject;

// Version
typedef struct
 {
  TUCHAR ucMajor;
  TUCHAR ucMinor;
  TUINT  uiBuild;
 }
TVersionObject;

//============================================================================//
// Déclaration des variables exportées
//============================================================================//
// Structure de configution du capteur
extern TWIMOParametersSensorObject tWIMOParametersSensor;
// Variable de configuration capteur pour vérification
extern TWIMOParametersSensorObject tWIMOParamCheckIntegrity;
// Structure de configuration du capteur O2
extern TWIMOPyroO2ConfigurationObject tWIMOPyroO2Configuration;
// Version
extern TVersionObject tSensorVersion;

#pragma pack(pop)

// SN
extern QString sSerialNumber;

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des fonctions exportées
//============================================================================//
//----------------------------------------------------------------------------//
// Vérification de la configuration générale capteur (1ere partie structure)
//----------------------------------------------------------------------------//
// Retourne :
//   - aTRUE si paramétrage semble correct, aFALSE sinon.
//----------------------------------------------------------------------------//
extern TBOOL bFWIMOParameterConfProductCheckIntegrity( void );
//----------------------------------------------------------------------------//
// Mise à jour QML configuration générale capteur (1ere partie structure)
//----------------------------------------------------------------------------//
extern void vFWIMOParameterConfProductQMLUpdate( void );
//----------------------------------------------------------------------------//
// Mise à jour QML channel name
//----------------------------------------------------------------------------//
extern void vFWIMOParameterQMLUpdateDashboardChannelName( void );
//----------------------------------------------------------------------------//
// Mise à jour sensor channel
//----------------------------------------------------------------------------//
extern void vFWIMOParameterUpdateSensorChannel( TUCHAR );

#endif

