//============================================================================//
// Fichier .........: "ENDIAN_1_00.h"
// Auteur ..........: St�phane Dutertre
// Date de cr�ation : 26/04/09
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
// D�claration des constantes
//============================================================================//

//============================================================================//
// D�claration des types
//============================================================================//

//============================================================================//
// D�claration des variables export�es
//============================================================================//

//============================================================================//
// D�claration des macros
//============================================================================//

//------------------------------------------------------------------------------
// Les entiers
//------------------------------------------------------------------------------
// Les entiers non sign�s
#ifdef aLITTLE_ENDIAN
 // Macros vers little endian rien � faire
 #define uiMEndianLittle( uiData )  ( TUINT )( uiData )
 // Macros vers big endian :: d�calage
 #define uiMEndianBig( uiData )     ( ( TUINT )( ( TUINT )uiData << 8 ) | ( TUINT )( ( TUINT )uiData >> 8 ) )
#else
 #ifdef aBIG_ENDIAN
  // Macros vers little endian rien � faire
  #define uiMEndianLittle( uiData ) ( ( TUINT )( ( TUINT )uiData << 8 ) | ( TUINT )( ( TUINT )uiData >> 8 ) )
  // Macros vers big endian :: d�calage
  #define uiMEndianBig( uiData )    ( TUINT )( uiData )
 #endif
#endif

// Les entiers sign�s
#ifdef aLITTLE_ENDIAN
 // Macros vers little endian rien � faire
 #define iMEndianLittle( iData )  ( TINT )( iData )
 // Macros vers big endian :: d�calage
 #define iMEndianBig( iData )     ( TINT )( uiMEndianBig( ( TUINT )iData ) )
#else
 #ifdef aBIG_ENDIAN
  // Macros vers little endian rien � faire
  #define iMEndianLittle( iData ) ( TINT )( uiMEndianLittle( ( TUINT )iData ) )
  // Macros vers big endian :: d�calage
  #define iMEndianBig( iData )    ( TINT )( iData )
 #endif
#endif

//------------------------------------------------------------------------------
// Les longs entiers
//------------------------------------------------------------------------------
// Les longs non sign�s
#ifdef aLITTLE_ENDIAN
 // Macros vers little endian rien � faire
 #define uliMEndianLittle( uliData )  ( TULONGINT )( uliData )
 // Macros vers big endian :: d�calage
 #define uliMEndianBig( uliData )     ( ( ( ( TULONGINT )uiMEndianBig( ( TUINT )uliData ) ) << 16 ) | ( ( TULONGINT )uiMEndianBig( ( TUINT )( uliData >> 16 ) ) ) )
#else
 #ifdef aBIG_ENDIAN
  // Macros vers little endian rien � faire
  #define uliMEndianLittle( uliData ) ( ( ( ( TULONGINT )uiMEndianLittle( ( TUINT )uliData ) ) << 16 ) | ( ( TULONGINT )uiMEndianLittle( ( TUINT )( uliData >> 16 ) ) ) )
  // Macros vers big endian :: d�calage
  #define uliMEndianBig( uliData )    ( TULONGINT )( uliData )
 #endif
#endif

// Les longs sign�s
#ifdef aLITTLE_ENDIAN
 // Macros vers little endian rien � faire
 #define liMEndianLittle( liData )  ( TLONGINT )( liData )
 // Macros vers big endian :: d�calage
 #define liMEndianBig( liData )     ( ( ( ( TLONGINT )uiMEndianBig( ( TUINT )liData ) ) << 16 ) | ( ( TLONGINT )uiMEndianBig( ( TUINT )( liData >> 16 ) ) ) )
#else
 #ifdef aBIG_ENDIAN
  // Macros vers little endian rien � faire
  #define liMEndianLittle( liData ) ( ( ( ( TLONGINT )uiMEndianLittle( ( TUINT )liData ) ) << 16 ) | ( ( TLONGINT )uiMEndianLittle( ( TUINT )( liData >> 16 ) ) ) )
  // Macros vers big endian :: d�calage
  #define liMEndianBig( liData )    ( TLONGINT )( liData )
 #endif
#endif

//============================================================================//
// D�claration des fonctions export�es
//============================================================================//

#endif

