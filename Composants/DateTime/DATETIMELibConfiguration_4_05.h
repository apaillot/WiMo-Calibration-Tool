//============================================================================//
// Fichier .........: "DATETIMELibConfiguration_4_05.h"
// Auteur ..........: Stéphane Dutertre
// Date de création : 21/11/05
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef DATETIME_LIB_CONFIGURATION_4_05_H
#define DATETIME_LIB_CONFIGURATION_4_05_H

//============================================================================//
// Inclusion des librairies
//============================================================================//

//============================================================================//
/// Définition des fonctions utilisées (1 = utilisée, 0 = supprimée)
//============================================================================//
//------------------------------------------------------------------------------
/// Test la validité d'une date\Time
#define __bFDATETIMEIsValid__          1
//------------------------------------------------------------------------------
/// Test la validité d'une date
#define __bFDATETIMEIsDateValid__      1
//------------------------------------------------------------------------------
/// Test la validité d'une date
#define __bFDATETIMEIsTimeValid__      1
//------------------------------------------------------------------------------
/// Comparaison de 2 dates
#define __bFDATETIMESuperior__         1
//------------------------------------------------------------------------------
/// Test de 2 heures
#define __bFDATETIMEIsTimeSuperior__   1
//------------------------------------------------------------------------------
/// Test de 2 dates
#define __bFDATETIMEIsDateSuperior__   1
//------------------------------------------------------------------------------
///  Ajout d'une heure à une heure
#define __bFDATETIMEAddTimeToTime__    1
//------------------------------------------------------------------------------
///  Soustrait une heure à une heure
#define __bFDATETIMEDecTimeToTime__    1
//------------------------------------------------------------------------------
/// Convertion de secondes en TTime
#define __bFDATETIMESecondsToTime__    1
//------------------------------------------------------------------------------
/// Ajout d'une heure à une date
#define __bFDATETIMEAddTime__          1
//------------------------------------------------------------------------------
/// Soustrait d'une heure à une date
#define __bFDATETIMEDecTime__          1
//------------------------------------------------------------------------------
/// Ajout de jours à une TDate
#define __bFDATETIMEAddDays__          1
//------------------------------------------------------------------------------
/// Soustrait des jours à une TDate
#define __bFDATETIMEDecDays__          1
//------------------------------------------------------------------------------
/// Ajout de secondes à une TDateTime
#define __bFDATETIMEAddSeconds__       1
//------------------------------------------------------------------------------
/// Soustrait de secondes à une TDateTime
#define __bFDATETIMEDecSeconds__       1
//------------------------------------------------------------------------------
/// Ajout d'une seconde à une DateTime
#define __vFDATETIMEAddSecondTick__    1
//------------------------------------------------------------------------------
/// Calcul le jour de la semaine en fonction de la date (depuis 1582)
#define __ucFDayOfWeek__               1
//------------------------------------------------------------------------------
/// Donne la date et l'heure locale en foncton de l'heure GMT
#define __vFDATETIMEGetLocalTime__     0
//------------------------------------------------------------------------------
/// Donne la date et l'heure GMT en foncton de l'heure locale
#define __vFDATETIMEGetGMTTime__       0
//------------------------------------------------------------------------------
/// On initialise un objet DateTime avec une chaîne
#define __bFStrToDateTime__            1
//------------------------------------------------------------------------------
/// On initialise une chaîne avec un DateTime
#define __bFDateTimeToStr__            1
//------------------------------------------------------------------------------
/// Mise à jour de la date à partir du nombre de secondes depuis 2000
#define __vFDATETIMESecToDateTime__    1
//------------------------------------------------------------------------------
/// Mise à jour du nombre de secondes depuis 2000 à partir de la date
#define __uliFDATETIMEDateTimeToSec__  1

//============================================================================//
// Déclaration des constantes
//============================================================================//
/// Active ou non la gestion intrinsèque du jour de la semaine
#define aDATETIME_DAY_OF_WEEK          0
/// Active ou non le formattage de chaîne de caractères
#define aDATETIME_FORMATTED            (__bFDateTimeToStr__ || __bFStrToDateTime__)
/// Active ou on la gestion intrinsèque des secondes depuis 2000
#define aDATETIME_SECOND_SINCE_2000    0
// Gestion été/hiver
#define aSUMMER_WINTER_DATETIME        0 && aDATETIME_DAY_OF_WEEK
// Définition du mois de passage à l'heure d'été
#define aSUMMER_MONTH                  aMARS
// Définition du mois de passage à l'heure d'été
#define aWINTER_MONTH                  aOCTO
// Heure de décalage
#define aCHANGING_HOUR                 (TUCHAR)1
// Décalage pour l'heure d'hiver
#define aWINTER_TIME                   (TUCHAR)1
// Décalage pour l'heure d'été
#define aSUMMER_TIME                   (TUCHAR)2
/// Chaîne de formattage
#define aDATE_TIME_STR_FORMAT          "%02u-%02u-%02u %02u:%02u:%02u"
/// Les paramètres de conversion DateTime to String : doit être en concordance avec la macro aDATE_TIME_STR_FORMAT >>> Toujours utiliser ptDateTime->tTime et ptDateTime->tDate
#define aDATE_TIME_STR_OBJECT_LIST     ptDateTime->tDate.ucYear % ( TUCHAR )100, ptDateTime->tDate.ucMonth  % ( TUCHAR )13, ptDateTime->tDate.ucDay % ( TUCHAR )32, ptDateTime->tTime.ucHour % ( TUCHAR )24, ptDateTime->tTime.ucMinute % ( TUCHAR )60, ptDateTime->tTime.ucSecond % ( TUCHAR )60
/// Les paramètres de conversion String To DateTime : doit être en concordance avec la macro aDATE_TIME_STR_FORMAT >>> Toujours utiliser uiHour, uiMinute, uiSecond, uiDay, uiMonth, uiYear
#define aDATE_TIME_STR_PARAMETER_LIST  &uiYear, &uiMonth, &uiDay, &uiHour, &uiMinute, &uiSecond
/// Format de la date de sortie
#define aDATE_TIME_STR_OUTPUT          "YY-MM-DD HH:MM:SS"

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

#endif


