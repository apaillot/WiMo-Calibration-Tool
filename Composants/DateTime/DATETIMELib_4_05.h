//============================================================================//
// Fichier .........: "DATETIMELib_4_04.h"
// Auteur ..........: Stéphane Dutertre
// Date de création : 25/11/08
//----------------------------------------------------------------------------//
/* Description :
    Monipulation de date.
*/
//============================================================================//
// Evolution : 4.05
// Auteur ...: Stéphane DUTERTRE
// Date .....: 21/07/11
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Formatage paramétrable de la chaîne bFDateTimeToStr et bFStrToDateTime
*/
//============================================================================//

#ifndef DATETIME_LIB_4_05_H
#define DATETIME_LIB_4_05_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeC.h>
#include <DATETIMELibConfiguration_4_05.h>

//============================================================================//
// Déclaration des constantes
//============================================================================//
// Les jours
#define aLU  0
#define aMA  1
#define aME  2
#define aJE  3
#define aVE  4
#define aSA  5
#define aDI  6

// Les mois
#define aJANV 1
#define aFEVR 2
#define aMARS 3
#define aAVRI 4
#define aMAI  5
#define aJUIN 6
#define aJUIL 7
#define aAOUT 8
#define aSEPT 9
#define aOCTO 10
#define aNOVE 11
#define aDECE 12

//============================================================================//
// Déclaration des types
//============================================================================//
/// Objet de gestion de l'heure
typedef struct
{
 TUCHAR ucSecond;
 TUCHAR ucMinute;
 TUCHAR ucHour;
}
TTimeObject;

/// Objet de gestion de la date
typedef struct
{
 #if aDATETIME_DAY_OF_WEEK
  TUCHAR ucDayOfWeek;
 #endif
 TUCHAR ucDay;
 TUCHAR ucMonth;
 TUCHAR ucYear;
}
TDateObject;

/// Objet de gestion de la date et de l'heure
typedef struct
{
 TTimeObject tTime;
 TDateObject tDate;
 #if aDATETIME_SECOND_SINCE_2000
  TULONGINT uliSecond;
 #endif
}
TDateTimeObject;

//============================================================================//
// Déclaration des variables exportées
//============================================================================//

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des fonctions exportées
//============================================================================//

//------------------------------------------------------------------------------
/// Test la validité d'une date\Time
// P.E. :
//   - Pointeur sur l'objet DateTime à tester
// P.S. :
//   - aTRUE si valide et aFALSE sinon
#if __bFDATETIMEIsValid__
extern TBOOL bFDATETIMEIsValid( TCONST TDateTimeObject * );
#endif

//------------------------------------------------------------------------------
/// Test la validité d'une date
// P.E. :
//   - Pointeur sur l'objet Date à tester
// P.S. :
//   - aTRUE si valide et aFALSE sinon
#if __bFDATETIMEIsDateValid__
extern TBOOL bFDATETIMEIsDateValid( TCONST TDateObject * );
#endif

//------------------------------------------------------------------------------
/// Test la validité d'une heure
// P.E. :
//   - Pointeur sur l'objet Time à tester
// P.S. :
//   - aTRUE si valide et aFALSE sinon
#if __bFDATETIMEIsTimeValid__
extern TBOOL bFDATETIMEIsTimeValid( TCONST TTimeObject * );
#endif

//------------------------------------------------------------------------------
/// Comparaison de 2 dates
// P.E. :
//   - Pointeur sur un objet DateTime à comparer ( A )
//   - Pointeur sur un objet DateTime de référence ( B )
// P.S. :
//   - aTRUE si A >= B et aFALSE sinon
#if __bFDATETIMESuperior__
extern TBOOL bFDATETIMESuperior( TCONST TDateTimeObject * , TCONST TDateTimeObject * );
#endif

//------------------------------------------------------------------------------
/// \brief Comparaison de 2 heures
/// \param ptTime1 : Pointeur sur un objet TTimeObject
/// \param ptTime2 : Pointeur sur un objet TTimeObject
/// \return Retourne aTRUE si ptTime1 est supérieure ou égale à ptTime2 
#if __bFDATETIMEIsTimeSuperior__
extern TBOOL bFDATETIMEIsTimeSuperior( TCONST TTimeObject * , TCONST TTimeObject * );
#endif

//------------------------------------------------------------------------------
/// \brief Comparaison de 2 dates
/// \param ptDateTime1 : Pointeur sur un objet TDateObject
/// \param ptDateTime2 : Pointeur sur un objet TDateObject
/// \return Retourne aTRUE si ptDate1 est supérieure ou égale à ptDate2 
#if __bFDATETIMEIsDateSuperior__
extern TBOOL bFDATETIMEIsDateSuperior( TCONST TDateObject * , TCONST TDateObject * );
#endif

//------------------------------------------------------------------------------
///  Ajout d'une heure à une heure
// P.E. :
//   - Pointeur sur un objet Time à modifier (A)
//   - Pointeur sur un objet Time  (B)
// P.S. :
//   - aTRUE si l'opération A = A + B est effectuée
#if __bFDATETIMEAddTimeToTime__
extern TBOOL bFDATETIMEAddTimeToTime( TTimeObject * , TCONST TTimeObject * );
#endif

//------------------------------------------------------------------------------
/// Soustrait une heure à une heure
// P.E. :
//   - Pointeur sur un objet Time à modifier (A)
//   - Pointeur sur un objet Time (B)
// P.S. :
//   - aTRUE si l'opération A = A - B est effectuée
#if __bFDATETIMEDecTimeToTime__
extern TBOOL bFDATETIMEDecTimeToTime( TTimeObject * , TCONST TTimeObject * );
#endif

//------------------------------------------------------------------------------
/// Convertion de secondes en TTimeObject
// P.E. :
//   - Pointeur sur un objet Time à modifier (A)
//   - Un nombre de seconde  à ajouter(B)
// P.S. :
//   - aTRUE si l'opération A = A + B est effectuée
#if __bFDATETIMESecondsToTime__
extern TBOOL bFDATETIMESecondsToTime( TTimeObject * , TULONGINT );
#endif

//------------------------------------------------------------------------------
/// Ajout d'une heure à une date
// P.E. :
//   - Pointeur sur un objet DateTime à modifier (A)
//   - Pointeur sur un objet Time  (B)
// P.S. :
//   - aTRUE si l'opération A = A + B est effectuée
#if __bFDATETIMEAddTime__
extern TBOOL bFDATETIMEAddTime( TDateTimeObject * , TCONST TTimeObject * );
#endif

//------------------------------------------------------------------------------
/// Soustrait une heure à une date
// P.E. :
//   - Pointeur sur un objet DateTime à modifier (A)
//   - Pointeur sur un objet Time  (B)
// P.S. :
//   - aTRUE si l'opération A = A - B est effectuée
#if __bFDATETIMEDecTime__
extern TBOOL bFDATETIMEDecTime( TDateTimeObject * , TCONST TTimeObject * );
#endif

//------------------------------------------------------------------------------
/// \brief Ajout de jours à une TDateObject
// P.E. :
//   - Pointeur sur un objet DateTime à modifier (A)
//   -Nombre de jour  à ajouter (B)
// P.S. :
//   - aTRUE si l'opération A = A + B est effectuée
#if __bFDATETIMEAddDays__
extern TBOOL bFDATETIMEAddDays( TDateTimeObject * , TULONGINT );
#endif

//------------------------------------------------------------------------------
/// \brief Soustrait des jours à une TDateObject
// P.E. :
//   - Pointeur sur un objet DateTime à modifier (A)
//   - Nombre de jour  à soustraire(B)
// P.S. :
//   - aTRUE si l'opération A = A - B est effectuée
#if __bFDATETIMEDecDays__
extern TBOOL bFDATETIMEDecDays( TDateTimeObject * , TULONGINT );
#endif

//------------------------------------------------------------------------------
/// Ajout de secondes à une TDateTimeObject
// P.E. :
//   - Pointeur sur un objet DateTime à modifier (A)
//   - Nombre de seconde  à ajouter (B)
// P.S. :
//   - aTRUE si l'opération A = A + B est effectuée
#if __bFDATETIMEAddSeconds__
extern TBOOL bFDATETIMEAddSeconds( TDateTimeObject * , TULONGINT );
#endif

//------------------------------------------------------------------------------
/// Soustrait des secondes à une DateTime
// P.E. :
//   - Pointeur sur un objet DateTime à modifier (A)
//   - Nombre de seconde  à soustraire (B)
// P.S. :
//   - aTRUE si l'opération A = A - B est effectuée
#if __bFDATETIMEDecSeconds__
extern TBOOL bFDATETIMEDecSeconds( TDateTimeObject * , TULONGINT );
#endif

//------------------------------------------------------------------------------
/// ajout une seconde à une DateTime
// P.E. :
//   - Pointeur sur un objet DateTime à modifier
// P.S. :
//   - Aucun
#if __vFDATETIMEAddSecondTick__
extern void vFDATETIMEAddSecondTick( TDateTimeObject * );
#endif

//------------------------------------------------------------------------------
/// Calcul le jour de la semaine en fonction de la date (depuis 1582)
// P.E. : Pointeur sur TDateTimeObject
// P.S. : Jour de la semaine
#if __ucFDayOfWeek__
extern TUCHAR ucFDayOfWeek( TCONST TDateObject * );
#endif

//------------------------------------------------------------------------------
/// \brief Donne la date et l'heure locale en foncton de l'heure GMT
/// \param pT_DateTime : Pointeur sur un objet TDateTimeObject (Destination)
/// \return ptLocalDateTime : Pointeur sur l'heure et la date locale
#if __vFDATETIMEGetLocalTime__ && aDATETIME_DAY_OF_WEEK && aSUMMER_WINTER_DATETIME
extern void vFDATETIMEGetLocalTime( TDateTimeObject * , TCONST TDateTimeObject * );
#endif

//------------------------------------------------------------------------------
/// \brief Donne la date et l'heure GMT en foncton de l'heure locale
/// \param ptDateTime : Pointeur sur un objet TDateTimeObject (Destination)
/// \return ptGMTDateTime : Pointeur sur l'heure et la date GMT
//------------------------------------------------------------------------------
#if __vFDATETIMEGetGMTTime__ && aDATETIME_DAY_OF_WEEK && aSUMMER_WINTER_DATETIME
extern void vFDATETIMEGetGMTTime( TDateTimeObject * , TCONST TDateTimeObject * );
#endif

//------------------------------------------------------------------------------
/// On initialise un objet DateTime avec une chaîne (Ne gère pas le jour de la semaine)
/// Format "HH:MM:SS JJ/MM/AA"
// P.E. :
//   - Pointeur sur un objet DateTime (destination
//   - Pointeur sur chaîne contenant la date/time
// P.S. :
//   - aTRUE si la conversion est effectuée
#if __bFStrToDateTime__
extern TBOOL bFStrToDateTime( TDateTimeObject * , TCONST TCHAR far * );
#endif

//------------------------------------------------------------------------------
/// On initialise une chaîne avec un DateTime (Ne gère pas le jour de la semaine)
/// Format "HH:MM:SS JJ/MM/AA"
// P.E. :
//   - Pointeur sur chaîne de destination
//   - Pointeur sur un objet DateTime
// P.S. :
//   - aTRUE si la conversion est effectuée
#if __bFDateTimeToStr__
extern TBOOL bFDateTimeToStr( TCHAR * , TCONST TDateTimeObject * );
#endif

//------------------------------------------------------------------------------
/// Mise à jour de la date à partir du nombre de secondes depuis 2000 
// P.E. :
//   - Pointeur sur un objet DateTime de destination
//   - Nombre de seconde contenant la date à convertir
// P.S. :
//   - Aucun
#if __vFDATETIMESecToDateTime__
extern void vFDATETIMESecToDateTime( TDateTimeObject * , TULONGINT );
#endif

//------------------------------------------------------------------------------
/// Mise à jour du nombre de secondes depuis 2000 à partir de la date
// P.E. :
//   - Pointeur sur un objet DateTime
// P.S. :
//   - Nombre de seconde depuis l'an 2000
#if __uliFDATETIMEDateTimeToSec__
extern TULONGINT uliFDATETIMEDateTimeToSec( TDateTimeObject * );
#endif

#endif
