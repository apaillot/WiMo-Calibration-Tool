//============================================================================//
// Fichier .........: "WIMOParameter.c"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2020/01/28
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef WIMO_PARAMETER_C
#define WIMO_PARAMETER_C

//============================================================================//
// Inclusion des librairies
//============================================================================//
// Type nke
#include <TypeC.h>
#include <TypeCMMN.h>
// Lib standart
#include <string.h>
#include <stdio.h>
#include <stddef.h>
// Composant nke
#include <ModBusData_2_04.h>
#include <ModBusDriver_2_04.h>
#include <Endian_1_00.h>
// User
#include "WIMOClientModbus.h">
#include "WIMOParameters.h"
#include "TARGETHardware.h"
#include "DataSensorChannel.h"

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
// Variable de configuration capteur
TWIMOParametersSensorObject tWIMOParametersSensor;
// Variable de configuration capteur pour vérification
TWIMOParametersSensorObject tWIMOParamCheckIntegrity;
// Pour manipulation O2
TWIMOPyroO2ConfigurationObject tWIMOPyroO2Configuration;
// Version
TVersionObject tSensorVersion;
// SN
QString sSerialNumber;

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

//----------------------------------------------------------------------------//
// Initialisation
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Vérification de la configuration générale capteur (1ere partie structure)
//----------------------------------------------------------------------------//
// Retourne :
//   - aTRUE si paramétrage semble correct, aFALSE sinon.
//----------------------------------------------------------------------------//
TBOOL bFWIMOParameterConfProductCheckIntegrity( void )
{
 char tcBuffer[1024];
 char tcString[64];
 TWIMOParametersSensorObject * ptSensor = &tWIMOParamCheckIntegrity;

 //---------------------------------
 // Test si le paramétrage est cohérent
 //---------------------------------
 //-------------
 //-- Product --
 //-------------
 // Activation du capteur
 if(  ( ptSensor->tHeader.tProduct.cActive != ( TCHAR )'0' )
   && ( ptSensor->tHeader.tProduct.cActive != ( TCHAR )'1' ) ) return( ( TBOOL )aFALSE );
 // Identifier
 if(  ( ptSensor->tHeader.tProduct.tcIdentifier[0] == ( TCHAR )0xFF )
   || ( ptSensor->tHeader.tProduct.tcIdentifier[0] == ( TCHAR )'\0' ) ) return( ( TBOOL )aFALSE );
 // Fabricant
 if(  ( ptSensor->tHeader.tProduct.tcManufacturer[0] == ( TCHAR )0xFF )
   || ( ptSensor->tHeader.tProduct.tcManufacturer[0] == ( TCHAR )'\0' ) ) return( ( TBOOL )aFALSE );
 //OPTIONNAL : Version
 //if(  ( ptSensor->tHeader.tProduct.tcVersion[0] == ( TCHAR )0xFF )
 //  || ( ptSensor->tHeader.tProduct.tcVersion[0] == ( TCHAR )'\0' ) ) return( ( TBOOL )aFALSE );
 // Type
 if(  ( ptSensor->tHeader.tProduct.cType != ( TCHAR )aWIMO_PARAMETERS_SENSOR_TYPE_SENSOR     )
   && ( ptSensor->tHeader.tProduct.cType != ( TCHAR )aWIMO_PARAMETERS_SENSOR_TYPE_WIPER      )
   && ( ptSensor->tHeader.tProduct.cType != ( TCHAR )aWIMO_PARAMETERS_SENSOR_TYPE_CLEANER_UV ) ) return( ( TBOOL )aFALSE );
 //-------------
 //-- Sensors --
 //-------------
 // Si c'est un capteur
 if( ptSensor->tHeader.tProduct.cType == ( TCHAR )'S' )
  {
   // Nombre de voie du capteur
   if(  ( ptSensor->tHeader.tSensor.ucChannelNumber == ( TUCHAR )0 )
     || ( ptSensor->tHeader.tSensor.ucChannelNumber >  ( TUCHAR )3 ) ) return( ( TBOOL )aFALSE );
  }
 //-----------
 //-- Wiper --
 //-----------
 // Si c'est un wiper
 if( ptSensor->tHeader.tProduct.cType == ( TCHAR )'W' )
  {
   // Détection angle abérant balai
   if( ptSensor->tHeader.tWiper.uiClockwiseAngle > ( TUINT )270 ) return( ( TBOOL )aFALSE );
  }

 // OK paramétrage valide
 return( ( TBOOL )aTRUE );
}

//----------------------------------------------------------------------------//
// Mise à jour QML configuration générale capteur (1ere partie structure)
//----------------------------------------------------------------------------//
void vFWIMOParameterConfProductQMLUpdate( void )
{
 char tcString[64];

 //---------------------------------
 // Mise à jour variable QML
 //---------------------------------
 // SN
 sprintf( tcString, "%02d-%05d", tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY, tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN );
 sSerialNumber = QString( tcString );
 ptDataSensor->setSN(tcString);
 ptDataSensor->setSN_YY(tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY);
 ptDataSensor->setSN_No(tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN);
 // Identifier
 sprintf( tcString, "%s"       , tWIMOParametersSensor.tHeader.tProduct.tcIdentifier );
 ptDataSensor->setIdentifier(tcString);
 // Manufacturer
 sprintf( tcString, "%s"       , tWIMOParametersSensor.tHeader.tProduct.tcManufacturer );
 ptDataSensor->setManufacturer(tcString);
 // Version
 sprintf( tcString, "%s"       , tWIMOParametersSensor.tHeader.tProduct.tcVersion );
 // Nombre de voie
 ptDataSensor->setChannelNb(tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber);
 // Firmware calibration
 sprintf( tcString, "V%u.%u.%u",
          tSensorVersion.ucMajor,
          tSensorVersion.ucMinor,
          tSensorVersion.uiBuild );
 ptDataSensor->setFirmwareVersion( tcString );
 //
 ptFactoryEvent->configSettingsDisplay();
 // Capteur
 if( tWIMOParametersSensor.tHeader.tProduct.cType == ( TCHAR )'S' )
  {
   // Pour test
   qDebug("MODBUS tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber = %d", tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber );
   // Force 3 voies de mesure sur le capteur
   if(  ( tWIMOParametersSensor.tHeader.tProduct.cType == ( TCHAR )'S' )
     && ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_OXYGEN_CONCENTRATION )
     && ( ptMainData->getMetroMode() || ptMainData->getNkeMode() ) )
    //ptLiveviewObj->setProperty("channelNumber", 3);
    ptLiveviewObj->setChannelNumber(3);
   else
    //ptLiveviewObj->setProperty("channelNumber", tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber);
    ptLiveviewObj->setChannelNumber(tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber);
  }
 // Wiper
 if( tWIMOParametersSensor.tHeader.tProduct.cType == ( TCHAR )'W' )
  {
   // Debug
   qDebug( "Refresh CPP Clockwise: %d", tWIMOParametersSensor.tHeader.tWiper.uiClockwiseAngle );
   qDebug( "Refresh CPP Clockwise: %d", tWIMOParametersSensor.tHeader.tWiper.uiCounterClockwiseAngle );
   // Angle capteur
   ptWiperObj->setProperty("uiClockwise",     tWIMOParametersSensor.tHeader.tWiper.uiClockwiseAngle);
   ptWiperObj->setProperty("uiAnticlockwise", tWIMOParametersSensor.tHeader.tWiper.uiCounterClockwiseAngle);
   ptWiperObj->setProperty("uiOffset",        tWIMOParametersSensor.tHeader.tWiper.uiOffset);
   ptWiperObj->setProperty("uiMin",           tWIMOParametersSensor.tHeader.tWiper.uiMinValue);
   ptWiperObj->setProperty("uiMax",           tWIMOParametersSensor.tHeader.tWiper.uiMaxValue);
  }
}

//----------------------------------------------------------------------------//
// Mise à jour QML channel name
//----------------------------------------------------------------------------//
void vFWIMOParameterQMLUpdateDashboardChannelName( void )
{
 // Voie de mesure
 if( tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber == 1 )
  ptDataSensor->setChannelList( QStringList({QString(tWIMOParametersSensor.ttChannel[0].tcName)}));
 else if( tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber == 2 )
  ptDataSensor->setChannelList( QStringList({QString(tWIMOParametersSensor.ttChannel[0].tcName),
                                              QString(tWIMOParametersSensor.ttChannel[1].tcName)}));
 else if( tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber == 3 )
  ptDataSensor->setChannelList( QStringList({QString(tWIMOParametersSensor.ttChannel[0].tcName),
                                             QString(tWIMOParametersSensor.ttChannel[1].tcName),
                                             QString(tWIMOParametersSensor.ttChannel[2].tcName)}));
 // Channel liste
 //tDataSensor.setChannelList({"Conductivity","Temperature","Fluorescence"});
}


//----------------------------------------------------------------------------//
// Mise à jour sensor channel
//----------------------------------------------------------------------------//
void vFWIMOParameterUpdateSensorChannel( TUCHAR ucChannel )
{
 char tcString[64];
 QString sTemp;

 ptDataSensorCh[ucChannel]->setActive(true);
 ptDataSensorCh[ucChannel]->setName(tWIMOParametersSensor.ttChannel[ucChannel].tcName);
 ptDataSensorCh[ucChannel]->setID(tWIMOParametersSensor.ttChannel[ucChannel].ucParameter);
 if( tWIMOParametersSensor.ttChannel[ucChannel].ucParameter < 10 )
  sprintf( tcString, "S0%u", tWIMOParametersSensor.ttChannel[ucChannel].ucParameter );
 else
  sprintf( tcString, "S%u", tWIMOParametersSensor.ttChannel[ucChannel].ucParameter );
 ptDataSensorCh[ucChannel]->setSID(tcString);
 ptDataSensorCh[ucChannel]->setUnit(tWIMOParametersSensor.ttChannel[ucChannel].tcUnit);
 ptDataSensorCh[ucChannel]->setUnitList( { tWIMOParametersSensor.ttChannel[ucChannel].tcUnit } );

 //ptDataSensorCh[ucChannel]->setCalibrationType("None");
 //ptDataSensorCh[ucChannel]->setCalibrationList({"None", "Multipoint"});
 ptDataSensorCh[ucChannel]->setAverage(tWIMOParametersSensor.ttChannel[ucChannel].uiAverage);
 sTemp = sFWIMOModbusFormatCalibrationFormula( ucChannel, false );
 ptDataSensorCh[ucChannel]->setFormula(sTemp);
 sTemp = sFWIMOModbusGetCalibrationDateTime( ucChannel );
 ptDataSensorCh[ucChannel]->setCalibrationDate(sTemp);
 ptDataSensorCh[ucChannel]->setIndex(tWIMOParametersSensor.ttChannel[ucChannel].ucParameter);
 ptDataSensorCh[ucChannel]->setValue(0);

 //------------
 // Anciens paramètres
 //------------
 // Selon la voie
 switch( ucChannel )
  {
   //---------------
   // Voie 0
   //---------------
   case( ( TUCHAR )0 ):
    // Ch0 - Parameter
    ptLiveviewObj->setCh0Parameter(tWIMOParametersSensor.ttChannel[0].ucParameter);
    // Ch0 - Name
    ptLiveviewObj->setCh0Name(tWIMOParametersSensor.ttChannel[0].tcName);
    // Ch0 - Unité
    ptLiveviewObj->setCh0Unit(tWIMOParametersSensor.ttChannel[0].tcUnit);
    // Ch0 - SN
    sprintf( tcString,
             "%02u-%05u",
             tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY,
             tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN );
    ptLiveviewObj->setCh0SN( tcString );
    // Ch0 - Name
    ptCalibration->setCh0Name(tWIMOParametersSensor.ttChannel[0].tcName);
    // Ch0 - Unit
    ptCalibration->setCh0Unit(tWIMOParametersSensor.ttChannel[0].tcUnit);
    // Ch0 - ID
    ptCalibration->setCh0ID(tWIMOParametersSensor.ttChannel[0].ucParameter);
    // Ch0 - Type
    ptCalibration->setCh0Type(QString(tWIMOParametersSensor.ttChannel[0].cCalibration));
    // Ch0 - Average
    ptCalibration->setCh0Average(tWIMOParametersSensor.ttChannel[0].uiAverage);
    // Ch0 - Formula
    sTemp = sFWIMOModbusFormatCalibrationFormula( 0, false );
    ptCalibration->setCh0Formula(sTemp);
    // Ch0 - DateTime
    sTemp = sFWIMOModbusGetCalibrationDateTime( 0 );
    ptCalibration->setCh0DateTime(sTemp);
   break;
   //---------------
   // Voie 1
   //---------------
   case( ( TUCHAR )1 ):
    // Ch1 - Parameter
    ptLiveviewObj->setCh1Parameter(tWIMOParametersSensor.ttChannel[1].ucParameter);
    // Ch1 - Name
    ptLiveviewObj->setCh1Name(tWIMOParametersSensor.ttChannel[1].tcName);
    // Ch1 - Unité
    ptLiveviewObj->setCh1Unit(tWIMOParametersSensor.ttChannel[1].tcUnit);
    // Ch1 - SN
    sprintf( tcString,
             "%02u-%05u",
             tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY,
             tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN );
    ptLiveviewObj->setCh1SN( tcString );

    // Ch1 - Name
    ptCalibration->setCh1Name(tWIMOParametersSensor.ttChannel[1].tcName);
    // Ch1 - Unit
    ptCalibration->setCh1Unit(tWIMOParametersSensor.ttChannel[1].tcUnit);
    // Ch1 - ID
    ptCalibration->setCh1ID(tWIMOParametersSensor.ttChannel[1].ucParameter);
    // Ch1 - Type
    ptCalibration->setCh1Type(QString(tWIMOParametersSensor.ttChannel[1].cCalibration));
    // Ch1 - Average
    ptCalibration->setCh1Average(tWIMOParametersSensor.ttChannel[1].uiAverage);
    // Ch1 - Formula
    sTemp = sFWIMOModbusFormatCalibrationFormula( 1, false );
    ptCalibration->setCh1Formula(sTemp);
    // Ch1 - DateTime
    sTemp = sFWIMOModbusGetCalibrationDateTime( 1 );
    ptCalibration->setCh1DateTime(sTemp);
   break;
   //---------------
   // Voie 2
   //---------------
   case( ( TUCHAR )2 ):
    // Ch2 - Parameter
    ptLiveviewObj->setCh2Parameter(tWIMOParametersSensor.ttChannel[2].ucParameter);
    // Ch2 - Name
    ptLiveviewObj->setCh2Name(tWIMOParametersSensor.ttChannel[2].tcName);
    // Ch2 - Unité
    ptLiveviewObj->setCh2Unit(tWIMOParametersSensor.ttChannel[2].tcUnit);
    // Ch2 - SN
    sprintf( tcString,
             "%02u-%05u",
             tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY,
             tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN );
    ptLiveviewObj->setCh2SN( tcString );

    // Ch2 - Name
    ptCalibration->setCh2Name(tWIMOParametersSensor.ttChannel[2].tcName);
    // Ch2 - Unit
    ptCalibration->setCh2Unit(tWIMOParametersSensor.ttChannel[2].tcUnit);
    // Ch2 - ID
    ptCalibration->setCh2ID(tWIMOParametersSensor.ttChannel[2].ucParameter);
    // Ch2 - Type
    ptCalibration->setCh2Type(QString(tWIMOParametersSensor.ttChannel[2].cCalibration));
    // Ch2 - Average
    ptCalibration->setCh2Average(tWIMOParametersSensor.ttChannel[2].uiAverage);
    // Ch2 - Formula
    sTemp = sFWIMOModbusFormatCalibrationFormula( 2, false );
    ptCalibration->setCh2Formula(sTemp);
    // Ch2 - DateTime
    sTemp = sFWIMOModbusGetCalibrationDateTime( 2 );
    ptCalibration->setCh2DateTime(sTemp);
   break;
   //---------------
   // errur
   //---------------
   default:return;
  }
}

#endif
