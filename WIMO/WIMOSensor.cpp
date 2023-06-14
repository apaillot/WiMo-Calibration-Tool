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
// Qt
#include <QString>
// Composant nke
#include <ModBusData_2_04.h>
#include <ModBusDriver_2_04.h>
#include <Endian_1_00.h>
// User
#include "WIMOClientModbus.h">
#include "TARGETHardware.h"
#include "WIMOSensor.h"

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
QString sSensorMinimalVersion = "";
QString sInterfaceMinimalVersion = "";

//============================================================================//
// Déclaration des variables non exportées
//============================================================================//
// Version minimal capteur requis
const TVersionObject tSensorVersionMinimalRequire[] =
{
 { 1, 2, 0 }, // S00 - Dummy
 { 1, 2, 0 }, // S01 - Dummy
 { 1, 2, 0 }, // S02 - Dummy
 { 1, 2, 0 }, // S03 - Temperature
 { 1, 2, 0 }, // S04 - Conductivity
 { 1, 2, 0 }, // S05 - Turbidity
//%%AP - 2022.03.08 - Levé de la restriction pour les O2 pour production
// { 1, 2, 6 }, // S06 - Oxygen concentration
// { 1, 2, 6 }, // S07 - Oxygen saturation
 { 1, 4, 0 }, // S06 - Oxygen concentration
 { 1, 4, 0 }, // S07 - Oxygen saturation
 { 1, 2, 0 }, // S08 - pH
 { 1, 2, 0 }, // S09 - Turner Chla
 { 1, 2, 0 }, // S10 - Turner PhycoC
 { 1, 2, 0 }, // S11 - Turner PhycoE
 { 1, 2, 0 }, // S12 - Turner CDOM
 { 1, 2, 0 }, // S13 - Fluo Chla
 { 1, 2, 0 }, // S14 - PAR
 { 1, 2, 0 }, // S15 - Fluo PhycoC
 { 1, 2, 0 }, // S16 - Fluo PhycoE
 { 1, 2, 0 }, // S17 - Fluo CDOM
 { 1, 2, 0 }, // S18 - Fluo Trypto
 { 1, 2, 0 }, // S19 - Turner Trypto
 { 1, 2, 0 }, // S20 - Redox
 { 1, 2, 0 }, // S21 - Ammonium
 { 1, 2, 0 }, // S22 - Potassium
 { 1, 2, 0 }, // S23 - Nitrate
 { 1, 2, 0 }, // S24 - ISE
 { 1, 2, 0 }, // S25 - O2 temperature
 { 1, 2, 0 }, // S26 - Crude Oil
 { 1, 2, 0 }, // S27 - Crude Oil
};
// Version minimale interface requis
const TVersionObject tInterfaceVersionMinimalRequire = { 1, 0, 4 }; // S00 - Dummy

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
// Vérification de la version capteur minial requise
//----------------------------------------------------------------------------//
// Retourne :
//   - aTRUE si paramétrage semble correct, aFALSE sinon.
//----------------------------------------------------------------------------//
TBOOL bFWIMOSensorCheckMinimalVersion( void )
{
 TUCHAR ucSensorIndex = tWIMOParametersSensor.ttChannel[0].ucParameter;
 // Test d'intégrité
 if( ucSensorIndex >= sizeof( tSensorVersionMinimalRequire ) )
  {
   return( ( TBOOL )aFALSE );
  }

 // Numéro de version du capteur
 TUCHAR ucSensorMajor = tSensorVersion.ucMajor;
 TUCHAR ucSensorMinor = tSensorVersion.ucMinor;
 TUINT  uiSensorBuild = tSensorVersion.uiBuild;
 // Numéro de version du capteur
 TUCHAR ucMinimalMajor = tSensorVersionMinimalRequire[ ucSensorIndex ].ucMajor;
 TUCHAR ucMinimalMinor = tSensorVersionMinimalRequire[ ucSensorIndex ].ucMinor;
 TUINT  uiMinimalBuild = tSensorVersionMinimalRequire[ ucSensorIndex ].uiBuild;

 // Generation de la version minimal requise
 sSensorMinimalVersion = QString::asprintf( "%u.%u.%u",
                                            ucMinimalMajor,
                                            ucMinimalMinor,
                                            uiMinimalBuild );

 // Test si version capteur minimale requise est OK
 if(  ( ucSensorMajor > ucMinimalMajor )
   || (  ( ucSensorMajor == ucMinimalMajor )
      && ( ucSensorMinor > ucMinimalMinor  ) )
   || (  ( ucSensorMajor == ucMinimalMajor )
      && ( ucSensorMinor == ucMinimalMinor )
      && ( uiSensorBuild >= uiMinimalBuild ) ) )
  {
   // OK
   return( ( TBOOL )aTRUE );
  }
 // NOK
 return( ( TBOOL )aFALSE );
}

//----------------------------------------------------------------------------//
// Vérification de la version interface minimale requise
//----------------------------------------------------------------------------//
// Retourne :
//   - aTRUE si paramétrage semble correct, aFALSE sinon.
//----------------------------------------------------------------------------//
TBOOL bFWIMOInterfaceCheckMinimalVersion( void )
{
 // Numéro de version du capteur
 TUCHAR ucSensorMajor = tWIMOModbusInterfaceVersion.ucMajor;
 TUCHAR ucSensorMinor = tWIMOModbusInterfaceVersion.ucMinor;
 TUINT  uiSensorBuild = tWIMOModbusInterfaceVersion.uiBuild;
 // Numéro de version du capteur
 TUCHAR ucMinimalMajor = tInterfaceVersionMinimalRequire.ucMajor;
 TUCHAR ucMinimalMinor = tInterfaceVersionMinimalRequire.ucMinor;
 TUINT  uiMinimalBuild = tInterfaceVersionMinimalRequire.uiBuild;

 // Generation de la version minimal requise
 sInterfaceMinimalVersion = QString::asprintf( "%u.%u.%u",
                                            ucMinimalMajor,
                                            ucMinimalMinor,
                                            uiMinimalBuild );

 // Test si version capteur minimale requise est OK
 if(  ( ucSensorMajor > ucMinimalMajor )
   || (  ( ucSensorMajor == ucMinimalMajor )
      && ( ucSensorMinor > ucMinimalMinor ) )
   || (  ( ucSensorMajor == ucMinimalMajor )
      && ( ucSensorMinor == ucMinimalMinor )
      && ( uiSensorBuild >=  uiMinimalBuild ) ) )
  {
   // OK
   return( ( TBOOL )aTRUE );
  }
 // NOK
 return( ( TBOOL )aFALSE );
}

#endif
