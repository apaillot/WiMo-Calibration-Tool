//============================================================================//
// Fichier .........: "SYSTEMAcquisition.h"
// Auteur ..........: Christophe SCHAEFFER
// Date de création : 20/02/2015
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef SYSTEM_ACQUISITION_H
#define SYSTEM_ACQUISITION_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeC.h>
#include <SYSTEMSensorBus.h>

//============================================================================//
// Déclaration des constantes
//============================================================================//

// Liste des voies utilisables
#define aSYSTEM_ACQUISITION_CHANNEL_0                0
#define aSYSTEM_ACQUISITION_CHANNEL_1                1
#define aSYSTEM_ACQUISITION_CHANNEL_2                2
#define aSYSTEM_ACQUISITION_CHANNEL_NUMBER           3
// Etats d'acquisition
#define aSYSTEM_ACQUISITION_NONE                     0
#define aSYSTEM_ACQUISITION_RUNNING                  1
#define aSYSTEM_ACQUISITION_RESULT_OK                2
#define aSYSTEM_ACQUISITION_RESULT_FAIL              3

//============================================================================//
// Déclaration des types
//============================================================================//

//============================================================================//
// Déclaration des variables exportées
//============================================================================//

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des fonctions exportées
//============================================================================//
/*
//----------------------------------------------------------------------------//
// Initialisation
extern void vFSYSTEMAcquisitionInitialization( void );

//----------------------------------------------------------------------------//
// Chargement d'un canal
// Paramètre :
// - Numéro de voie
extern void vFSYSTEMAcquisitionLoad( int );
extern void vFSYSTEMAcquisitionUnLoad( int );

//----------------------------------------------------------------------------//
// Etat acquisition
// Paramètre :
// - Numéro de voie (optionnel)
// Résultat :
// - aTRUE si actif
// - aFALSE sinon
extern TBOOL bFSYSTEMAcquisitionIsRunningSensor( int );
extern TBOOL bFSYSTEMAcquisitionIsRunningAll( void );

//----------------------------------------------------------------------------//
// Récupération d'information selon paramètre
// Paramètres :
// - Numéro de paramètre
// Résultat :
// - Index capteur
extern TCHAR cFSYSTEMAcquisitionGetSensorIndex1(  TUCHAR );
extern TCHAR cFSYSTEMAcquisitionGetChannelIndex1( TUCHAR );

//----------------------------------------------------------------------------//
// Récupération d'information selon paramètre
// Paramètres :
// - Chaine d'identification du paramètre
// Résultat :
// - Index capteur/voie
extern TCHAR cFSYSTEMAcquisitionGetSensorIndex2(  TCHAR * );
extern TCHAR cFSYSTEMAcquisitionGetChannelIndex2( TCHAR * );

//----------------------------------------------------------------------------//
// Démarrage d'acquisition
// Paramètre :
// - Numéro de voie (optionnel)
// Résultat :
// - aTRUE si accepté
// - aFALSE sinon
extern TBOOL bFSYSTEMAcquisitionStartAcquisitionSensor( int );
extern TBOOL bFSYSTEMAcquisitionStartAcquisitionAll( void );

//----------------------------------------------------------------------------//
// Base de temps
extern void vFSYSTEMAcquisitionTick10ms( void );

//----------------------------------------------------------------------------//
// Automate de gestion
// Résultat :
// - aTRUE si actif
// - aFALSE sinon
extern TBOOL bGSYSTEMAcquisitionManager( void );

//----------------------------------------------------------------------------//
// Arrêt des acquisitions
extern void vFSYSTEMAcquisitionStop( void );

//----------------------------------------------------------------------------//
// Calcul de la durée d'acquisition totale
// Résultat :
// - Durée d'acquisition totale
extern TUINT uiSYSTEMAcquisitionComputeAcquisitionTime( void );
*/

#endif
