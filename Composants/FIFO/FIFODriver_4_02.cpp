//============================================================================//
// Fichier .........: "FIFODriver_4_01.c"
// Auteur ..........: Stéphane Dutertre
// Date de création : 21/11/05
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//
// Evolution : 3.11
// Auteur ...: Stéphane Dutertre
// Date .....: 27/07/06
//----------------------------------------------------------------------------//
/*! \mainpage
     Description de l'évolution :
     \n Modification des commentaires pour utilisation de DOxygen + possibilité
     d'avoir un buffer de FIFO entre 2^0 et 2^32 octets + ajout de la fonction
     "bFFIFOFull" [Vrai si FIFO pleine].
     \n\n
     \author Stéphane DUTERTRE
     \date 27/07/06
     \version version 4.00
*/
//============================================================================//
// Evolution : 4.00
// Auteur ...: Stéphane Dutertre
// Date .....: 07/09/06
//----------------------------------------------------------------------------//
/*! \mainpage
     Description de l'évolution :
     \n Modification de la représentation du composant. On utilise la notation
     pointée pour effectuer le traitement. Ceci permet d'avoir un objet avec
     des fonctions membres du type C++ (Meilleur encapsulation) et facilite
     la redifition de l'objet.
     \n\n
     \author Stéphane DUTERTRE
     \date 27/07/06
     \version version 4.00
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

#ifndef FIFO_DRIVER_4_02_C
#define FIFO_DRIVER_4_02_C

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include "TypeC.h"
// Si on doit redéfinir l'objet
#if( !aFIFO_REDEFINITION )
 // Inclusion de la configuration
 #include "FIFODriver_4_02.h"
 //---------------------------------------------------------------------------//
 // Message à destination du BUG Profiler
 //---------------------------------------------------------------------------//
 #define aBUG_PROFILER_MESSAGE_FIFO_INITIALIZATION ( eBUG_PROFILER_SOURCE_COMPONENT | eBUG_PROFILER_LEVEL_0 | 0x0000 )
 #define aBUG_PROFILER_MESSAGE_FIFO_DATA_LOST      ( eBUG_PROFILER_SOURCE_COMPONENT | eBUG_PROFILER_LEVEL_2 | 0x0009 )
 #define aBUG_PROFILER_MESSAGE_FIFO_OVERFLOW       ( eBUG_PROFILER_SOURCE_COMPONENT | eBUG_PROFILER_LEVEL_2 | 0x0000 )
#endif
#if( aBUG_PROFILER_IN_USE )
 // Pistage de bugs
 #include <BUGProfiler_1_00.h>
#endif
// Inclusion des librairies standard
#include <String.h>

//============================================================================//
// Déclaration des constantes
//============================================================================//

//============================================================================//
// Déclaration des types
//============================================================================//
/// Structure de la FIFO
typedef struct
{
 TUXTYPE_FIFO uxWrite;
 TUXTYPE_FIFO uxRead;
 TUCHAR       tucData[ aFIFO_SIZE ];
}
TFIFOData;

//============================================================================//
// Déclaration des prototypes de fonctions non exportées
//============================================================================//
#if( __vFFIFOInitialization__ )
 static void vFFIFOInitialization( void );
#endif
#if( __vFFIFOPush__ )
 static void vFFIFOPush( TUCHAR );
#endif
#if( __ucFFIFOPop__ )
 static TUCHAR ucFFIFOPop( void );
#endif
#if( __uxFFIFONumber__ )
 static TUXTYPE_FIFO uxFFIFONumber( void );
#endif
#if( __uxFFIFOFree__ )
 static TUXTYPE_FIFO uxFFIFOFree( void );
#endif
#if( __bFFIFOData__ )
 static TBOOL bFFIFOData( void );
#endif
#if( __bFFIFOFull__ )
 static TBOOL bFFIFOFull( void );
#endif

//============================================================================//
// Déclaration des variables exportées
//============================================================================//
/// Fonctions membres de l'objet tFIFO
TCONST TFIFOObject tFIFO =
{
 //------------------------------------------------------------------------------
 /// \brief On stocke un octet dans la FIFO
 /// \param Stockage de "ucData " dans la FIFO
 /// \return Aucun
 //------------------------------------------------------------------------------
 #if( __vFFIFOPush__ )
  vFFIFOPush,
 #endif
 //----------------------------------------------------------------------------//
 /// \brief Lecture d'un octet dans la FIFO
 /// \param Aucun
 /// \return Renvoie la première donnée stockée
 //----------------------------------------------------------------------------//
 #if( __ucFFIFOPop__ )
  ucFFIFOPop,
 #endif
 //----------------------------------------------------------------------------//
 /// \brief  Lecture du nombre d'octet dans la FIFO
 /// \param Aucun
 /// \return Retourne le nombre d'octets stockés
 //----------------------------------------------------------------------------//
 #if( __uxFFIFONumber__ )
  uxFFIFONumber,
 #endif
 //----------------------------------------------------------------------------//
 /// \brief Lecture de l'espace libre dans la FIFO
 /// \param Aucun
 /// \return Retourne le nombre d'octets disponibles
 //----------------------------------------------------------------------------//
 #if( __uxFFIFOFree__ )
  uxFFIFOFree,
 #endif
 //----------------------------------------------------------------------------//
 /// \brief Indique s'il y a au moins une donnée dans la FIFO
 /// \param Aucun
 /// \return Retourne "aTRUE" s'il y a au moins une donnée et "aFALSE" sinon.
 //----------------------------------------------------------------------------//
 #if( __bFFIFOData__ )
  bFFIFOData,
 #endif
 //----------------------------------------------------------------------------//
 /// \brief Indique s'il n'y a plus de place dans la FIFO
 /// \param Aucun
 /// \return Retourne "aTRUE" s'il n'y a plus de place de libre et "aFALSE" sinon.
 //----------------------------------------------------------------------------//
 #if( __bFFIFOFull__ )
  bFFIFOFull,
 #endif
 //----------------------------------------------------------------------------//
 /// \brief Initialisation de la FIFO
 /// \param Aucun
 /// \return Aucun
 //----------------------------------------------------------------------------//
 #if( __vFFIFOInitialization__ )
  vFFIFOInitialization
 #endif
};
//============================================================================//
// Déclaration des variables non exportées
//============================================================================//
/// Variable de gestion de la FIFO
static TFIFOData tFIFOData;

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
/// \brief Initialisation de la FIFO
/// \param Aucun
/// \return Aucun
//----------------------------------------------------------------------------//
#if( __vFFIFOInitialization__ )
static void vFFIFOInitialization( void )
{
 // On désactive les IRQs
 vMFIFODisableInterrupt();
 // Initialisation de la FIFO (Pointeur d'écriture)
 tFIFOData.uxWrite = ( TUXTYPE_FIFO )0;
 // On réactive les IRQs
 vMFIFOEnableInterrupt();
 // Initialisation de la FIFO (Pointeur de lecture)
 tFIFOData.uxRead = ( TUXTYPE_FIFO )0;
 #if( aBUG_PROFILER_IN_USE )
  // On informe de l'initialisation du composant
  vFBUGProfiler( aBUG_PROFILER_MESSAGE_FIFO_INITIALIZATION );
 #endif
}
#endif

//------------------------------------------------------------------------------
/// \brief On stocke un octet dans la FIFO
/// \param Stockage de "ucData " dans la FIFO
/// \return Aucun
//------------------------------------------------------------------------------
#if( __vFFIFOPush__ )
static void vFFIFOPush( TUCHAR ucData )
{
 #if( aFIFO_NO_DATA_LOST )
  // On regarde si l'on peut-écrire la donnée ou pas
  if( ( ( tFIFOData.uxWrite + 1 ) % sizeof( tFIFOData.tucData ) ) == tFIFOData.uxRead )
   {
    // On informe de l'initialisation du composant
    vFBUGProfiler( aBUG_PROFILER_MESSAGE_FIFO_DATA_LOST );
    // Il y a rebouclage on sort :: La donnée est perdue
    return;
   }
 #endif
 // Stockage d'un Octet dans la FIFO
 tFIFOData.tucData[ tFIFOData.uxWrite ] = ucData;
 // On fait une FIFO circulaire
 if( ++tFIFOData.uxWrite >= sizeof( tFIFOData.tucData ) ) tFIFOData.uxWrite = ( TUXTYPE_FIFO )0;
 // Si l'on est en debug
 #if( aBUG_PROFILER_IN_USE )
  // On vérifie qu'il n'y a pas d'overflow
  if( tFIFOData.uxWrite == tFIFOData.uxRead )
   // Overflow
   vFBUGProfiler( aBUG_PROFILER_MESSAGE_FIFO_OVERFLOW );
 #endif
}
#endif

//----------------------------------------------------------------------------//
/// \brief Lecture d'un octet dans la FIFO
/// \param Aucun
/// \return Renvoie la première donnée stockée
//----------------------------------------------------------------------------//
#if( __ucFFIFOPop__ )
static TUCHAR ucFFIFOPop( void )
{
 TUCHAR ucData;

 // Lecture d'un octet dans la FIFO
 ucData = tFIFOData.tucData[ tFIFOData.uxRead ];
 // On avance l'indice de lecture
 if( ++tFIFOData.uxRead >= sizeof( tFIFOData.tucData ) ) tFIFOData.uxRead = ( TUXTYPE_FIFO )0;
 // on retourne l'octet lu
 return( ucData );
}
#endif

//----------------------------------------------------------------------------//
/// \brief  Lecture du nombre d'octet dans la FIFO
/// \param Aucun
/// \return Retourne le nombre d'octets stockés
//----------------------------------------------------------------------------//
#if( __uxFFIFONumber__ )
static TUXTYPE_FIFO uxFFIFONumber( void )
{
 TUXTYPE_FIFO uxSize;

 // On désactive les IRQs
 vMFIFODisableInterrupt();
 // On récupère le nombre d'octet stocké dans la FIFO
 uxSize = ( TUXTYPE_FIFO )( ( sizeof( tFIFOData.tucData ) + tFIFOData.uxWrite - tFIFOData.uxRead ) % sizeof( tFIFOData.tucData ) );
 // On réactive les IRQs
 vMFIFOEnableInterrupt();
 // On retourne le résultat
 return( uxSize );
}
#endif

//----------------------------------------------------------------------------//
/// \brief Lecture de l'espace libre dans la FIFO
/// \param Aucun
/// \return Retourne le nombre d'octets disponibles
//----------------------------------------------------------------------------//
#if( __uxFFIFOFree__ )
static TUXTYPE_FIFO uxFFIFOFree( void )
{
 TUXTYPE_FIFO uxSize;

 // On désactive les IRQs
 vMFIFODisableInterrupt();
 // On récupère le nombre d'octet stocké dans la FIFO
 uxSize = ( TUXTYPE_FIFO )( sizeof( tFIFOData.tucData ) - 1 - ( sizeof( tFIFOData.tucData ) + tFIFOData.uxWrite - tFIFOData.uxRead ) % sizeof( tFIFOData.tucData ) );
 // On réactive les IRQs
 vMFIFOEnableInterrupt();
 // On retourne le résultat
 return( uxSize );
}
#endif

//----------------------------------------------------------------------------//
/// \brief Indique s'il y a au moins une donnée dans la FIFO
/// \param Aucun
/// \return Retourne "aTRUE" s'il y a au moins une donnée et "aFALSE" sinon.
//----------------------------------------------------------------------------//
#if( __bFFIFOData__ )
static TBOOL bFFIFOData( void )
{
 TBOOL bResult;

 // On désactive les IRQs
 vMFIFODisableInterrupt();
 // On récupère le nombre d'octet stocké dans la FIFO
 bResult = ( TBOOL )( tFIFOData.uxWrite != tFIFOData.uxRead );
 // On réactive les IRQs
 vMFIFOEnableInterrupt();
 // On retourne le résultat
 return( bResult );
}
#endif

//----------------------------------------------------------------------------//
/// \brief Indique s'il n'y a plus de place dans la FIFO
/// \param Aucun
/// \return Retourne "aTRUE" s'il n'y a plus de place de libre et "aFALSE" sinon.
//----------------------------------------------------------------------------//
#if( __bFFIFOFull__ )
static TBOOL bFFIFOFull( void )
{
 TBOOL bResult;

 // On désactive les IRQs
 vMFIFODisableInterrupt();
 // On vérifie que le pointeur d'écriture ne va pas dépasser le pointeur de lecture
 bResult = ( TBOOL )( ( ( tFIFOData.uxWrite + 1 ) % sizeof( tFIFOData.tucData ) ) == tFIFOData.uxRead );
 // On réactive les IRQs
 vMFIFOEnableInterrupt();
 // On retourne le résultat
 return( bResult );
}
#endif

#undef TUXTYPE_FIFO

#endif
