//----------------------------------------------------------------------------//
// Fichier .........: "TypeC.h"
// Auteur ..........: Stéphane Dutertre
// Date de création : 14/02/05
//----------------------------------------------------------------------------//
/* Description :
    Cette interface définie les types standards "MICREL". 
    Ces types sont communs à tous les projets "MICREL".
*/

#ifndef TYPEC_H
#define TYPEC_H

//----------------------------------------------------------------------------//
// Inclusion des librairies
//----------------------------------------------------------------------------//
#include <TypeMicro.h>
//#pragma pack(1)

//----------------------------------------------------------------------------//
// Déclaration des types pour 8051 :: Compilateur "KEIL"
//----------------------------------------------------------------------------//
#ifdef aMC_8051
 // Définition du stockage des variables
 #define aBIG_ENDIAN
 // Définition d'un booléen (Taille = 1 bit)
 typedef bit               TBOOL;
 // Définition d'un octet signé (Taille = 1 octet)
 typedef signed char       TCHAR;
 // Définition d'un octet non signé (Taille = 1 octet)
 typedef unsigned char     TUCHAR;
 // Définition d'un mot signé (Taille = 2 octets)
 typedef signed int        TINT;
 // Définition d'un mot non signé (Taille = 2 octets)
 typedef unsigned int      TUINT;
 // Définition d'un mot long signé (Taille = 4 octets)
 typedef signed long int   TLONGINT;
 // Définition d'un mot long non signé (Taille = 4 octets)
 typedef unsigned long int TULONGINT;
 // Définition d'un mot long signé (Taille = 8 octets)
 typedef signed long long int   TLONG2INT;
 // Définition d'un mot long non signé (Taille = 8 octets)
 typedef unsigned long long int TULONG2INT;
 // Définition d'un flottant (Taille = 4 octets)
 typedef float             TFLOAT;
 // Définition d'un double (Taille = 8 octets)
 typedef double            TDOUBLE;
 // Définition d'un long double (Taille = 12 octets)
 typedef long double       TLONGDOUBLE;
 //--- SPECIAL 8051 ---//
 // Les types xdata existe sur 8051
 //--- SPECIAL M16C ---//
 #define near
 #define far
 #define huge
 //--- Le type constant ---//
 #define TCONST code
#endif

//----------------------------------------------------------------------------//
// Déclaration des types pour MICROCHIP :: Compilateur "CCS"
//----------------------------------------------------------------------------//
#ifdef aMC_PIC
 // Définition du stockage des variables
 #define aLITTLE_ENDIAN
 // Définition d'un booléen (Taille = 1 octet)
 typedef int1           TBOOL;
 // Définition d'un octet signé (Taille = 1 octet)
 typedef signed char    TCHAR;
 // Définition d'un octet non signé (Taille = 1 octet)
 typedef unsigned char  TUCHAR;
 // Définition d'un mot signé (Taille = 2 octets)
 typedef signed int16   TINT;
 // Définition d'un mot non signé (Taille = 2 octets)
 typedef unsigned int16 TUINT;
 // Définition d'un mot long signé (Taille = 4 octets)
 typedef signed int32   TLONGINT;
 // Définition d'un mot long non signé (Taille = 4 octets)
 typedef unsigned int32 TULONGINT;
 // Définition d'un flottant (Taille = 4 octets)
 typedef float TFLOAT;
 // Définition d'un double (Taille = 8 octets) Le type double n'existe pas
 //typedef double TDOUBLE;
 // Définition d'un long double (Taille = 12 octets)
 // typedef long double TLONGDOUBLE;
 // Définition du type
 //--- SPECIAL 8051 ---//
 // Les types xdata n'existe pas sur microchip
 #define xdata
 #define idata
 //--- SPECIAL M16C ---//
 #define near
 #define far
 #define huge
 //--- Le type constant ---//
 #define TCONST const
#endif

//----------------------------------------------------------------------------//
// Déclaration des types pour RENESAS :: Compilateur "IAR"
//----------------------------------------------------------------------------//
#ifdef aMC_R8C
 //---------------------------------------------------------------------------//
 // Inclusion des librairies
 //---------------------------------------------------------------------------//
 #include <intrinsics.h>
 #include <ior8c24_25.h>

// Définition du stockage des variables
 #define aLITTLE_ENDIAN
 // Définition d'un booléen (Taille = 1 octet)
 typedef unsigned char      TBOOL;
 // Définition d'un octet signé (Taille = 1 octet)
 typedef signed char        TCHAR;
 // Définition d'un octet non signé (Taille = 1 octet)
 typedef unsigned char      TUCHAR;
 // Définition d'un mot signé (Taille = 2 octets)
 typedef signed short int   TINT;
 // Définition d'un mot non signé (Taille = 2 octets)
 typedef unsigned short int TUINT;
 // Définition d'un mot long signé (Taille = 4 octets)
 typedef signed long int    TLONGINT;
 // Définition d'un mot long non signé (Taille = 4 octets)
 typedef unsigned long int  TULONGINT;
 // Définition d'un mot long signé (Taille = 8 octets)
 typedef signed long long int   TLONG2INT;
 // Définition d'un mot long non signé (Taille = 8 octets)
 typedef unsigned long long int TULONG2INT;
 // Définition d'un flottant (Taille = 4 octets)
 typedef float              TFLOAT;
 // Définition d'un double (Taille = 8 octets)
 typedef double             TDOUBLE;
 // Définition d'un long double (Taille = 12 octets)
 typedef long double        TLONGDOUBLE;
 //--- SPECIAL 8051 ---//
 // Les types xdata n'existent pas sur mitsubishi
 #define xdata
 #define idata
 //--- SPECIAL M16C ---//
 #define near
 #define far
 #define huge
 //--- Le type constant ---//
 #define TCONST const
#endif

//----------------------------------------------------------------------------//
// Déclaration des types pour RENESAS :: Compilateur "IAR"
//----------------------------------------------------------------------------//
#ifdef aMC_M16C
 #include <IOM16C56.h>
 #include <INTRINSICS.H>
 // Définition du stockage des variables
 #define aLITTLE_ENDIAN
 // Définition d'un booléen (Taille = 1 octet)
 typedef unsigned char      TBOOL;
 // Définition d'un octet signé (Taille = 1 octet)
 typedef signed char        TCHAR;
 // Définition d'un octet non signé (Taille = 1 octet)
 typedef unsigned char      TUCHAR;
 // Définition d'un mot signé (Taille = 2 octets)
 typedef signed short int   TINT;
 // Définition d'un mot non signé (Taille = 2 octets)
 typedef unsigned short int TUINT;
 // Définition d'un mot long signé (Taille = 4 octets)
 typedef signed long int    TLONGINT;
 // Définition d'un mot long non signé (Taille = 4 octets)
 typedef unsigned long int  TULONGINT;
 // Définition d'un mot long signé (Taille = 8 octets)
 typedef signed long long int   TLONG2INT;
 // Définition d'un mot long non signé (Taille = 8 octets)
 typedef unsigned long long int TULONG2INT;
 // Définition d'un flottant (Taille = 4 octets)
 typedef float              TFLOAT;
 // Définition d'un double (Taille = 8 octets) 
 typedef double             TDOUBLE;
 // Définition d'un long double (Taille = 12 octets)
 typedef long double        TLONGDOUBLE;
 //--- SPECIAL 8051 ---//
 // Les types xdata n'existent pas sur mitsubishi
 #define xdata
 #define idata
 //--- SPECIAL M16C ---//
 //--- Le type constant ---//
 #define TCONST const
#endif

//----------------------------------------------------------------------------//
// Déclaration des types pour RENESAS :: Compilateur "IAR"
//----------------------------------------------------------------------------//
#ifdef aMC_M32C
 // Définition du stockage des variables
 #define aLITTLE_ENDIAN
 // Définition d'un booléen (Taille = 1 octet)
 typedef unsigned char      TBOOL;
 // Définition d'un octet signé (Taille = 1 octet)
 typedef signed char        TCHAR;
 // Définition d'un octet non signé (Taille = 1 octet)
 typedef unsigned char      TUCHAR;
 // Définition d'un mot signé (Taille = 2 octets)
 typedef signed short int   TINT;
 // Définition d'un mot non signé (Taille = 2 octets)
 typedef unsigned short int TUINT;
 // Définition d'un mot long signé (Taille = 4 octets)
 typedef signed long int    TLONGINT;
 // Définition d'un mot long non signé (Taille = 4 octets)
 typedef unsigned long int  TULONGINT;
 // Définition d'un mot long signé (Taille = 8 octets)
 typedef signed long long int   TLONG2INT;
 // Définition d'un mot long non signé (Taille = 8 octets)
 typedef unsigned long long int TULONG2INT;
 // Définition d'un flottant (Taille = 4 octets)
 typedef float              TFLOAT;
 // Définition d'un double (Taille = 8 octets) 
 typedef double             TDOUBLE;
 // Définition d'un long double (Taille = 12 octets)
 typedef long double        TLONGDOUBLE;
 //--- SPECIAL 8051 ---//
 // Les types xdata n'existent pas sur mitsubishi
 #define xdata
 #define idata
 //--- SPECIAL M16C ---//
 //--- Le type constant ---//
 #define TCONST const
#endif

//----------------------------------------------------------------------------//
// Déclaration des types pour TEXAS :: Compilateur "Cross Studio"
//----------------------------------------------------------------------------//
#ifdef aMC_MSP430
 // Définition du stockage des variables
 #define aLITTLE_ENDIAN
 // Définition d'un booléen (Taille = 1 octet)
 typedef unsigned char      TBOOL;
 // Définition d'un octet signé (Taille = 1 octet)
 typedef signed char        TCHAR;
 // Définition d'un octet non signé (Taille = 1 octet)
 typedef unsigned char      TUCHAR;
 // Définition d'un mot signé (Taille = 2 octets)
 typedef signed short int   TINT;
 // Définition d'un mot non signé (Taille = 2 octets)
 typedef unsigned short int TUINT;
 // Définition d'un mot long signé (Taille = 4 octets)
 typedef signed long int    TLONGINT;
 // Définition d'un mot long non signé (Taille = 4 octets)
 typedef unsigned long int  TULONGINT;
 // Définition d'un mot long signé (Taille = 8 octets)
 typedef signed long long int   TLONG2INT;
 // Définition d'un mot long non signé (Taille = 8 octets)
 typedef unsigned long long int TULONG2INT;
 // Définition d'un flottant (Taille = 4 octets)
 typedef float              TFLOAT;
 // Définition d'un double (Taille = 8 octets) 
 typedef double             TDOUBLE;
 // Définition d'un long double (Taille = 12 octets)
 typedef long double        TLONGDOUBLE;
 //--- SPECIAL 8051 ---//
 // Les types xdata n'existent pas sur mitsubishi
 #define xdata
 #define idata
 //--- SPECIAL M16C ---//
 //--- SPECIAL M16C ---//
 #define near
 #define far
 #define huge
 //--- Le type constant ---//
 #define TCONST const
#endif

//----------------------------------------------------------------------------//
// Déclaration des types pour RENESAS :: Compilateur "IAR"
//----------------------------------------------------------------------------//
#ifdef aMC_R32C
 // Définition du stockage des variables
 #define aLITTLE_ENDIAN
 // Définition d'un booléen (Taille = 1 octet)
 typedef unsigned char      TBOOL;
 // Définition d'un octet signé (Taille = 1 octet)
 typedef signed char        TCHAR;
 // Définition d'un octet non signé (Taille = 1 octet)
 typedef unsigned char      TUCHAR;
 // Définition d'un mot signé (Taille = 2 octets)
 typedef signed short int   TINT;
 // Définition d'un mot non signé (Taille = 2 octets)
 typedef unsigned short int TUINT;
 // Définition d'un mot long signé (Taille = 4 octets)
 typedef signed long int    TLONGINT;
 // Définition d'un mot long non signé (Taille = 4 octets)
 typedef unsigned long int  TULONGINT;
 // Définition d'un flottant (Taille = 4 octets)
 typedef float              TFLOAT;
 // Définition d'un double (Taille = 8 octets) 
 typedef double             TDOUBLE;
 // Définition d'un long double (Taille = 12 octets)
 typedef long double        TLONGDOUBLE;
 //--- SPECIAL 8051 ---//
 // Les types xdata n'existent pas sur mitsubishi
 #define xdata
 #define idata
 //--- SPECIAL M16C ---//
 //--- Le type constant ---//
 #define TCONST const
#endif

//----------------------------------------------------------------------------//
// Déclaration des types pour ARM :: Compilateur gcc
//----------------------------------------------------------------------------//
#ifdef aMC_STM32
 // Définition du stockage des variables
 #define aLITTLE_ENDIAN
  // Définition d'un booleen
 typedef unsigned char      TBOOL;
 // Définition d'un octet signé
 typedef signed char        TCHAR;
 // Définition d'un octet non signé
 typedef unsigned char      TUCHAR;
 // Définition d'un mot signé (2 octets)
 typedef signed short int   TINT;
 // Définition d'un mot non signé (2 octets)
 typedef unsigned short int TUINT;
 // Définition d'un mot long signé (4 octets)
 typedef signed long int    TLONGINT;
 // Définition d'un mot long non signé (4 octets)
 typedef unsigned long int  TULONGINT;
 // Définition d'un mot long signé (Taille = 8 octets)
 typedef signed long long int   TLONG2INT;
 // Définition d'un mot long non signé (Taille = 8 octets)
 typedef unsigned long long int TULONG2INT;
 // Définition d'un flottant (4 octets)
 typedef float              TFLOAT;
 // Définition d'un double (8 octets)
 typedef double             TDOUBLE;
 // Définition d'un long double (Taille = 12 octets)
 typedef long double        TLONGDOUBLE;
 //--- SPECIAL 8051 ---//
 // Les types xdata n'existent pas sur mitsubishi
 #define xdata
 #define idata
 //--- SPECIAL M16C ---//
 #define far
 #define huge
 //--- SPECIAL BUILDER ---//
 //--- Le type constant ---//
 #define TCONST const
#endif

//----------------------------------------------------------------------------//
// Déclaration des types pour ARM / KL17 :: Compilateur IAR
//----------------------------------------------------------------------------//
#ifdef aMC_KL17
 // Définition du stockage des variables
 #define aLITTLE_ENDIAN
  // Définition d'un booleen
 typedef unsigned char      TBOOL;
 // Définition d'un octet signé
 typedef signed char        TCHAR;
 // Définition d'un octet non signé
 typedef unsigned char      TUCHAR;
 // Définition d'un mot signé (2 octets)
 typedef signed short int   TINT;
 // Définition d'un mot non signé (2 octets)
 typedef unsigned short int TUINT;
 // Définition d'un mot long signé (4 octets)
 typedef signed long int    TLONGINT;
 // Définition d'un mot long non signé (4 octets)
 typedef unsigned long int  TULONGINT;
 // Définition d'un mot long signé (Taille = 8 octets)
 typedef signed long long int   TLONG2INT;
 // Définition d'un mot long non signé (Taille = 8 octets)
 typedef unsigned long long int TULONG2INT;
 // Définition d'un flottant (4 octets)
 typedef float              TFLOAT;
 // Définition d'un double (8 octets)
 typedef double             TDOUBLE;
 // Définition d'un long double (Taille = 12 octets)
 typedef long double        TLONGDOUBLE;
 //--- SPECIAL 8051 ---//
 // Les types xdata n'existent pas sur mitsubishi
 #define xdata
 #define idata
 //--- SPECIAL M16C ---//
 #define far
 #define huge
 //--- SPECIAL BUILDER ---//
 //--- Le type constant ---//
 #define TCONST const
#endif

//----------------------------------------------------------------------------//
// Déclaration des types pour PC
//----------------------------------------------------------------------------//
#ifdef aMC_BUILDER
 // Définition du stockage des variables
 #define aLITTLE_ENDIAN     1
 // Définition d'un booléen (Taille = 1 octet)
 typedef bool               TBOOL;
 // Définition d'un octet signé
 //%%SD déjà définit dans "WinNT.H"
 #ifndef _TCHAR_DEFINED
 #define _TCHAR_DEFINED
 #define TCHAR TTCHAR
 typedef char               TCHAR;
 #endif
 // Définition d'un octet non signé
 typedef unsigned char      TUCHAR;
 // Définition d'un mot signé (2 octets)
 typedef signed short int   TINT;
 // Définition d'un mot non signé (2 octets)
 typedef unsigned short int TUINT;
 // Définition d'un mot long signé (4 octets)
 typedef signed long int    TLONGINT;
 // Définition d'un mot long non signé (4 octets)
 typedef unsigned long int  TULONGINT;
 // Définition d'un mot long signé (Taille = 8 octets)
 typedef signed long long int   TLONG2INT;
 // Définition d'un mot long non signé (Taille = 8 octets)
 typedef unsigned long long int TULONG2INT;
 // Définition d'un flottant (4 octets)
 typedef float              TFLOAT;
 // Définition d'un double (8 octets)
 typedef double             TDOUBLE;
 // Définition d'un long double (Taille = 12 octets)
 typedef long double        TLONGDOUBLE;
 //--- SPECIAL 8051 ---//
 #define xdata
 #define idata
 //--- SPECIAL M16C ---//
 #define far
 #define huge
 //--- SPECIAL BUILDER ---//
 //--- Le type constant ---//
 #define TCONST const
#endif

#endif
