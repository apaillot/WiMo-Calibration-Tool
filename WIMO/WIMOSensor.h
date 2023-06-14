//============================================================================//
// Fichier .........: "WIMOSensor.h"
// Auteur ..........: Alexandre Paillot
// Date de création : 2021.09.22
//----------------------------------------------------------------------------//
/* Description :
    Gestion des paramètres de WIMO.
*/
//============================================================================//

#ifndef WIMO_SENSOR_H
#define WIMO_SENSOR_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeCMMN.h>
#include <TypeC.h>
#include <QString>
#include <SYSTEMAcquisition.h>
#include <PILETypes_1_20.h>
#include <DATETIMELib_4_05.h>
#include "WIMOParameters.h"
#include <main.h>

//============================================================================//
// Déclaration des constantes
//============================================================================//

//============================================================================//
// Déclaration des types
//============================================================================//

//============================================================================//
// Déclaration des variables exportées
//============================================================================//
// Version minimal du capteur en cours
extern QString sSensorMinimalVersion;
// Version
extern QString sInterfaceMinimalVersion;

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des fonctions exportées
//============================================================================//
//----------------------------------------------------------------------------//
// Vérification de la version capteur minial requise
//----------------------------------------------------------------------------//
// Retourne :
//   - aTRUE si paramétrage semble correct, aFALSE sinon.
//----------------------------------------------------------------------------//
extern TBOOL bFWIMOSensorCheckMinimalVersion( void );
//----------------------------------------------------------------------------//
// Vérification de la version interface minimale requise
//----------------------------------------------------------------------------//
// Retourne :
//   - aTRUE si paramétrage semble correct, aFALSE sinon.
//----------------------------------------------------------------------------//
extern TBOOL bFWIMOInterfaceCheckMinimalVersion( void );

#endif

