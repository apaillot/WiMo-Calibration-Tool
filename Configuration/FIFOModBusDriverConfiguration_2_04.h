//============================================================================//
// Fichier .........: "ModBusDriverConfiguration_2_01.h"
// Auteur ..........: Stéphane DUTERTRE
// Date de création : 21/12/06
//----------------------------------------------------------------------------//
/* Description :
    Configuration de la FIFO de communication ModBus.
*/
//============================================================================//
// Evolution :
// Auteur ...:
// Date .....:
//----------------------------------------------------------------------------//
/* Description de l'évolution :
*/
//============================================================================//
// Evolution : 2.02
// Auteur ...: Stéphane DUTERTRE
// Date .....: 18/10/10
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Ajout du fichier de Data.h
*/
//============================================================================//
// Evolution : 2.03
// Auteur ...: Stéphane DUTERTRE
// Date .....: 04/05/11
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Changement de l'adresse du ModBus sur un type variable
*/
//============================================================================//
// Evolution : 2.04
// Auteur ...: Stéphane DUTERTRE
// Date .....: 28/06/13
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Changement d'une directive du composant ModBus (réception de caratère) afin d'être indépendant de la FIFO.
*/
//============================================================================//

#ifndef FIFO_MODBUS_DRIVER_CONFIGURATION_2_04_H
#define FIFO_MODBUS_DRIVER_CONFIGURATION_2_04_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeC.h>

//============================================================================//
// Définition des fonctions utilisées (1 = utilisée, 0 = supprimée)
//============================================================================//

//----------------------------------------------------------------------------//
// Initialisation de la FIFO
#define __vFFIFOInitialization__ 1
//------------------------------------------------------------------------------
// On stocke un octet dans la FIFO
#define __vFFIFOPush__           1
//----------------------------------------------------------------------------//
// Lecture d'un octet dans la FIFO
#define __ucFFIFOPop__           1
//----------------------------------------------------------------------------//
// Lecture du nombre d'octet dans la FIFO
#define __xFFIFONumber__         0
//----------------------------------------------------------------------------//
// Lecture du nombre d'octet dans la FIFO
#define __xFFIFOFree__           0
//----------------------------------------------------------------------------//
// Indique s'il y a au moins une donnée dans la FIFO
#define __bFFIFOData__           1
//----------------------------------------------------------------------------//
// Indique s'il y a au moins une donnée dans la FIFO
#define __bFFIFOFull__           0

//============================================================================//
// Déclaration des constantes
//============================================================================//
#define aFIFO_SIZE               1024
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
#define vMFIFOEnableInterrupt()
#define vMFIFODisableInterrupt()

// BUG cré un warning "Empty declaration"
#define vMFIFOModBusDriverPushVirtualFunction() static void vFFIFOModBusDriverPush( TUCHAR ucData ) { tFIFOModBusDriver.vFPush( ucData ); }
// Suivant qu'il s'agit d'une compilation sous builder ou IAR (Buggé !!!)
//#define vMFIFOModBusDriverPush tFIFOModBusDriver.vFPush
#define vMFIFOModBusDriverPush vFFIFOModBusDriverPush

//============================================================================//
// Déclaration des fonctions exportées
//============================================================================//

#endif

