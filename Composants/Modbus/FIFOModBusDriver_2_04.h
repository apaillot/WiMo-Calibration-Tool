//============================================================================//
// Fichier .........: "FIFOModBusDriver_2_01.h"
// Auteur ..........: Stéphane DUTERTRE
// Date de création : 26/12/06
//----------------------------------------------------------------------------//
/* Description :
    Gestion de la FIFO de communication ModBus.
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

#ifndef FIFO_MODBUS_DRIVER_2_04_H
#define FIFO_MODBUS_DRIVER_2_04_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
// On redéfinie l'objet
#define aFIFO_REDEFINITION 1
// Inclusion de la configuration (Paramètre de redéfinition)
#include <FIFOModBusDriverConfiguration_2_04.h>

//============================================================================//
// Déclaration des constantes
//============================================================================//

//============================================================================//
// Déclaration des types
//============================================================================//

//============================================================================//
// Déclaration des variables exportées
//============================================================================//

//============================================================================//
// Déclaration des macros
//============================================================================//
/// Redéfinition de l'objet tFIFO
#ifdef FIFO_DRIVER_4_02_H
 #undef FIFO_DRIVER_4_02_H
#endif

/// Redefinition de l'objet
#ifdef tFIFO
 #undef tFIFO
#endif
#define tFIFO tFIFOModBusDriver

/// Redefinition de l'objet
#ifdef TFIFOObject
 #undef TFIFOObject
#endif
#define TFIFOObject TFIFOModBusDriverObject

// On ouvre l'inbterface
#include <FIFODriver_4_02.h>

//============================================================================//
// Déclaration des prototypes de fonctions exportées
//============================================================================//

#endif

