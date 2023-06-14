//============================================================================//
// Fichier .........: "ENDIAN_1_00.h"
// Auteur ..........: Stéphane Dutertre
// Date de création : 26/04/09
//----------------------------------------------------------------------------//
/* Description :
    Gestion du big et du little endian.
*/
//============================================================================//

#ifndef ENDIAN_1_00_H
#define ENDIAN_1_00_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeC.h>

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

//------------------------------------------------------------------------------
// Les entiers
//------------------------------------------------------------------------------
// Les entiers non signés
#ifdef aLITTLE_ENDIAN
 // Macros vers little endian rien à faire
 #define uiMEndianLittle( uiData )  ( TUINT )( uiData )
 // Macros vers big endian :: décalage
 #define uiMEndianBig( uiData )     ( ( TUINT )( ( TUINT )uiData << 8 ) | ( TUINT )( ( TUINT )uiData >> 8 ) )
#else
 #ifdef aBIG_ENDIAN
  // Macros vers little endian rien à faire
  #define uiMEndianLittle( uiData ) ( ( TUINT )( ( TUINT )uiData << 8 ) | ( TUINT )( ( TUINT )uiData >> 8 ) )
  // Macros vers big endian :: décalage
  #define uiMEndianBig( uiData )    ( TUINT )( uiData )
 #endif
#endif

// Les entiers signés
#ifdef aLITTLE_ENDIAN
 // Macros vers little endian rien à faire
 #define iMEndianLittle( iData )  ( TINT )( iData )
 // Macros vers big endian :: décalage
 #define iMEndianBig( iData )     ( TINT )( uiMEndianBig( ( TUINT )iData ) )
#else
 #ifdef aBIG_ENDIAN
  // Macros vers little endian rien à faire
  #define iMEndianLittle( iData ) ( TINT )( uiMEndianLittle( ( TUINT )iData ) )
  // Macros vers big endian :: décalage
  #define iMEndianBig( iData )    ( TINT )( iData )
 #endif
#endif

//------------------------------------------------------------------------------
// Les longs entiers
//------------------------------------------------------------------------------
// Les longs non signés
#ifdef aLITTLE_ENDIAN
 // Macros vers little endian rien à faire
 #define uliMEndianLittle( uliData )  ( TULONGINT )( uliData )
 // Macros vers big endian :: décalage
 #define uliMEndianBig( uliData )     ( ( ( ( TULONGINT )uiMEndianBig( ( TUINT )uliData ) ) << 16 ) | ( ( TULONGINT )uiMEndianBig( ( TUINT )( uliData >> 16 ) ) ) )
#else
 #ifdef aBIG_ENDIAN
  // Macros vers little endian rien à faire
  #define uliMEndianLittle( uliData ) ( ( ( ( TULONGINT )uiMEndianLittle( ( TUINT )uliData ) ) << 16 ) | ( ( TULONGINT )uiMEndianLittle( ( TUINT )( uliData >> 16 ) ) ) )
  // Macros vers big endian :: décalage
  #define uliMEndianBig( uliData )    ( TULONGINT )( uliData )
 #endif
#endif

// Les longs signés
#ifdef aLITTLE_ENDIAN
 // Macros vers little endian rien à faire
 #define liMEndianLittle( liData )  ( TLONGINT )( liData )
 // Macros vers big endian :: décalage
 #define liMEndianBig( liData )     ( ( ( ( TLONGINT )uiMEndianBig( ( TUINT )liData ) ) << 16 ) | ( ( TLONGINT )uiMEndianBig( ( TUINT )( liData >> 16 ) ) ) )
#else
 #ifdef aBIG_ENDIAN
  // Macros vers little endian rien à faire
  #define liMEndianLittle( liData ) ( ( ( ( TLONGINT )uiMEndianLittle( ( TUINT )liData ) ) << 16 ) | ( ( TLONGINT )uiMEndianLittle( ( TUINT )( liData >> 16 ) ) ) )
  // Macros vers big endian :: décalage
  #define liMEndianBig( liData )    ( TLONGINT )( liData )
 #endif
#endif

//============================================================================//
// Déclaration des fonctions exportées
//============================================================================//

#endif

