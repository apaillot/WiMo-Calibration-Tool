//============================================================================//
// Fichier .........: "FIFODriverConfiguration_4_01.h"
// Auteur ..........: Stéphane Dutertre
// Date de création : 21/11/05
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//
// Evolution : 4.02
// Auteur ...: Stéphane DUTERTRE
// Date .....: 24/01/10
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Intégration du composant "BUG Profiler"
*/
//============================================================================//

#ifndef FIFO_DRIVER_CONFIGURATION_4_02_H
#define FIFO_DRIVER_CONFIGURATION_4_02_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeC.h>
//#include <TARGETHardware.h>

//============================================================================//
// Définition des fonctions utilisées (1 = utilisée, 0 = supprimée)
//============================================================================//
//----------------------------------------------------------------------------//
/// Initialisation de la FIFO
#define __vFFIFOInitialization__ 1
//------------------------------------------------------------------------------
/// On stocke un octet dans la FIFO
#define __vFFIFOPush__           1
//----------------------------------------------------------------------------//
/// Lecture d'un octet dans la FIFO
#define __ucFFIFOPop__           1
//----------------------------------------------------------------------------//
/// Lecture du nombre d'octet dans la FIFO
#define __uxFFIFONumber__        0
//----------------------------------------------------------------------------//
/// Lecture de l'espace libre dans la FIFO
#define __uxFFIFOFree__          0
//----------------------------------------------------------------------------//
/// Indique s'il y a au moins une donnée dans la FIFO
#define __bFFIFOData__           1
//----------------------------------------------------------------------------//
/// Indique s'il n'y a plus de place dans la FIFO
#define __bFFIFOFull__           0

//============================================================================//
// Déclaration des constantes
//============================================================================//
/// Définition de la taille de la FIFO
#define aFIFO_SIZE               32

/// Empêche l'écrasement des données les plus anciennes lorsque la FIFO est pleine
#define aFIFO_NO_DATA_LOST       0

//============================================================================//
// Déclaration des types
//============================================================================//
/// Adaptation automatique du type à la compilation
#if aFIFO_SIZE <= 256
 #define TUXTYPE_FIFO TUCHAR
#elif aFIFO_SIZE <= 65536
 #define TUXTYPE_FIFO TUINT
#else
 #define TUXTYPE_FIFO TULONGINT
#endif

//============================================================================//
// Déclaration des variables exportées
//============================================================================//

//============================================================================//
// Déclaration des macros
//============================================================================//
/// Définition de la MACRO d'activation de l'IRQ liée à la FIFO
//#define vMFIFOEnableInterrupt()  vMTARGETEnableInterrupt()
/// Définition de la MACRO de désactivation de l'IRQ liée à la FIFO
//#define vMFIFODisableInterrupt() vMTARGETDisableInterrupt()
/// Définition de la MACRO d'activation de l'IRQ liée à la FIFO
#define vMFIFOEnableInterrupt()  {}
/// Définition de la MACRO de désactivation de l'IRQ liée à la FIFO
#define vMFIFODisableInterrupt() {}

//============================================================================//
// Déclaration des fonctions exportées
//============================================================================//

#endif


