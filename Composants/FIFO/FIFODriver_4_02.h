//============================================================================//
// Fichier .........: "FIFODriver_4_01.h"
// Auteur ..........: Stéphane Dutertre
// Date de création : 21/11/05
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//
// Evolution : 4.01
// Auteur ...: Stéphane Dutertre
// Date .....: 04/05/09
//----------------------------------------------------------------------------//
/* Description :
    Changement de la macro TUXTYPE en TUXTYPE_FIFO.
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

#ifndef FIFO_DRIVER_4_02_H
#define FIFO_DRIVER_4_02_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeC.h>
// Si on doit redéfinir l'objet
#if( !aFIFO_REDEFINITION )
 // Inclusion de la configuration
 #include "FIFODriverConfiguration_4_02.h"
#endif

//============================================================================//
// Déclaration des constantes
//============================================================================//

//============================================================================//
// Déclaration des types
//============================================================================//
/// Gestion de l'objet FIFO
typedef struct
{
 //------------------------------------------------------------------------------
 /// \brief On stocke un octet dans la FIFO
 /// \param Stockage de "ucData " dans la FIFO
 /// \return Aucun
 //------------------------------------------------------------------------------
 #if( __vFFIFOPush__ )
  void ( * vFPush )( TUCHAR );
 #endif
 //----------------------------------------------------------------------------//
 /// \brief Lecture d'un octet dans la FIFO
 /// \param Aucun
 /// \return Renvoie la première donnée stockée
 //----------------------------------------------------------------------------//
 #if( __ucFFIFOPop__ )
  TUCHAR ( * ucFPop )( void );
 #endif
 //----------------------------------------------------------------------------//
 /// \brief  Lecture du nombre d'octet dans la FIFO
 /// \param Aucun
 /// \return Retourne le nombre d'octets stockés
 //----------------------------------------------------------------------------//
 #if( __uxFFIFONumber__ )
  TUXTYPE_FIFO ( * uxFNumber )( void );
 #endif
 //----------------------------------------------------------------------------//
 /// \brief Lecture de l'espace libre dans la FIFO
 /// \param Aucun
 /// \return Retourne le nombre d'octets disponibles
 //----------------------------------------------------------------------------//
 #if( __uxFFIFOFree__ )
  TUXTYPE_FIFO ( * uxFFree )( void );
 #endif
 //----------------------------------------------------------------------------//
 /// \brief Indique s'il y a au moins une donnée dans la FIFO
 /// \param Aucun
 /// \return Retourne "aTRUE" s'il y a au moins une donnée et "aFALSE" sinon.
 //----------------------------------------------------------------------------//
 #if( __bFFIFOData__ )
  TBOOL ( * bFData )( void );
 #endif
 //----------------------------------------------------------------------------//
 /// \brief Indique s'il n'y a plus de place dans la FIFO
 /// \param Aucun
 /// \return Retourne "aTRUE" s'il n'y a plus de place de libre et "aFALSE" sinon.
 //----------------------------------------------------------------------------//
 #if( __bFFIFOFull__ )
  TBOOL ( * bFFull )( void );
 #endif
 //----------------------------------------------------------------------------//
 /// \brief Initialisation de la FIFO
 /// \param Aucun
 /// \return Aucun
 //----------------------------------------------------------------------------//
 #if( __vFFIFOInitialization__ )
  void ( * vFInitialization )( void );
 #endif
} TFIFOObject;

//============================================================================//
// Déclaration des variables exportées
//============================================================================//
extern TCONST TFIFOObject tFIFO;

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des fonctions exportées
//============================================================================//

#endif
