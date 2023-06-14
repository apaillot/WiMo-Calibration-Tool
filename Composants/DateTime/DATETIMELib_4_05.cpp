//============================================================================//
// Fichier .........: "DATETIMELib_4_04.c"
// Auteur ..........: Stéphane Dutertre
// Date de création : 21/11/05
//----------------------------------------------------------------------------//
/*!
    \mainpage
    Description :
     Routines de gestion de la date et de l'heure.
    \author Stéphane DUTERTRE
    \date 21/11/05
    \version 3.00
*/
//============================================================================//
// Evolution : 3.01
// Auteur ...: Stéphane DUTERTRE
// Date .....: 30/11/06
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Ajout de 2 fonctions de test de validité de la date  et de l'heure
*/
//============================================================================//
// Evolution : 4.00
// Auteur ...: Stéphane DUTERTRE
// Date .....: 30/11/06
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Gestion de l'heure locale + autres fonctions de suppression d'heure et de 
    jour.
*/
//============================================================================//
// Evolution : 4.01
// Auteur ...: Stéphane DUTERTRE
// Date .....: 19/07/07
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Ajout du define "aDATETIME_SECOND_SINCE_2000" dans les fonctions :
    - uliFDATETIMEDateTimeToSec
    - vFDATETIMESecToDateTime
*/
//============================================================================//
// Evolution : 4.03
// Auteur ...: Stéphane DUTERTRE
// Date .....: 30/08/07
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Ajout des fonctions :
    - bFDATETIMEAddTimeToTime : Ajout d'une heure à une heure
    - bFDATETIMEDecTimeToTime : Soustrait une heure à une heure
*/
//============================================================================//
// Evolution : 4.03 Correctif
// Auteur ...: Stéphane DUTERTRE
// Date .....: 14/09/07
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    - Correction bug compilateur IAR sur la fonction "uliFDATETIMEDateTimeToSec" :
    Le compilateur renvoie une valeur incohérente sur manipulation de 
    "tucDayOfMonth".
    - Correction bug sur la fonction "uliFDATETIMEDateTimeToSec" ne renvoie pas
    le bon nombre de seconde si l'année en cours est bisextile et si l'on est
    le 29 février ou mois supérieur (il manque 86400 secondes à la valeur 
    retournée).  
*/
//============================================================================//
// Evolution : 4.03 Correctif
// Auteur ...: Stéphane DUTERTRE
// Date .....: 04/10/07
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    - Correction bug compilateur IAR sur fonction "bFDATETIMEIsValid", etc... :
    Le compilateur renvoie une valeur incohérente sur manipulation de
    "tucDayOfMonth".
*/
//============================================================================//
// Evolution : 4.03 Correctif
// Auteur ...: Stéphane DUTERTRE
// Date .....: 14/12/07
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    - Correction sur la fonction "vFDATETIMESecToDateTime" ne gère pas
    correctement les années bisextiles (voir le 29 février).
*/
//============================================================================//
// Evolution : 4.03 Correctif
// Auteur ...: SD + OG
// Date .....: 03/01/08
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    - BUG vu par OG. Correction sur la fonction "bFDATETIMESuperior" qui ne renvoie pas 
     True en cas d'égalité des dates testées : Uniquement avec le test en Comparaison
      directe.
*/
//============================================================================//
// Evolution : 4.03 Correctif
// Auteur ...: Stéphane DUTERTRE
// Date .....: 05/03/08
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    - Dans le compteur de seconde, il manque 86400 secondes soit le 29 février 
    de l'an 2000.
*/
//============================================================================//
// Evolution : 4.04
// Auteur ...: Stéphane DUTERTRE
// Date .....: 06/03/08
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    - Accélération de la fonction de transtypage "uliFDATETIMEDateTimeToSec".
*/
//============================================================================//
// Evolution : 4.04 Correctif
// Auteur ...: Stéphane DUTERTRE
// Date .....: 05/03/08
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    - La fonction de décrémentation du compteur de jour ou de seconde 
    alors on limite la date\time à XX:XX:XX 01/01/00.
    De même, l'ncrémentation de jour ou de seconde se limite à XX:XX:XX 31/12/255.
*/
//============================================================================//
// Evolution : 4.04 Correctif
// Auteur ...: Stéphane DUTERTRE
// Date .....: 12/03/08
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    - Correction de la fonction "DayOfWeek" qui ne fonctionnait que jusqu'à 2099.
*/
//============================================================================//
// Evolution : 4.04 
// Auteur ...: Stéphane DUTERTRE
// Date .....: 14/04/08
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    - Ajout de la fonction "bFDATETIMEDecTimeToTime".
*/
//============================================================================//
// Evolution : 4.04 Correctif
// Auteur ...: Stéphane DUTERTRE
// Date .....: 14/04/08
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    - Ajout de la fonction "uliFDATETIMEDateTimeToSec" dans la fonction bFStrToDateTime.
*/
//============================================================================//
// Evolution : 4.04 Correctif
// Auteur ...: Stéphane DUTERTRE
// Date .....: 13/10/10
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    - Correctif : Erreur de comparaison dans la fonction "bFDATETIMEDecTimeToTime", 
    il ne fallait pas tester à > 60 mais >= 60.
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

#ifndef DATETIME_LIB_4_05_C
#define DATETIME_LIB_4_05_C

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeCMMN.h>
#include <TypeC.h>
#include <DATETIMELib_4_05.h>
#include <DATETIMELibConfiguration_4_05.h>
#if aDATETIME_FORMATTED
 #include <String.h>
 #include <Stdio.h>
#endif

//============================================================================//
// Déclaration des constantes
//============================================================================//
// Dans le cas du compteur de seconde
#define aDATETIME_YEAR_MAX ( 136 )

//============================================================================//
// Déclaration des types
//============================================================================//

//============================================================================//
// Déclaration des variables non exportées
//============================================================================//
/// Monbre de jours des mois
static TCONST TUCHAR far tucDayOfMonth[ 12 ] =
{
 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/*#if aDATETIME_FORMATTED
 /// Chaîne de formattage de binaire en ASCII
 static TCONST TCHAR far tcStringDateTimeFormat[] = "%02u:%02u:%02u %02u/%02u/%02u";
#endif*/

// Permet d'accélérer les calculs
#if __uliFDATETIMEDateTimeToSec__
// Nombre de secondes dans le mois
#define aSEC_IN_JAN  ( ( TULONGINT )86400 * ( TULONGINT )31 )
#define aSEC_IN_FEB  ( ( TULONGINT )86400 * ( TULONGINT )28 )
#define aSEC_IN_MAR  ( ( TULONGINT )86400 * ( TULONGINT )31 )
#define aSEC_IN_APR  ( ( TULONGINT )86400 * ( TULONGINT )30 )
#define aSEC_IN_MAY  ( ( TULONGINT )86400 * ( TULONGINT )31 )
#define aSEC_IN_JUN  ( ( TULONGINT )86400 * ( TULONGINT )30 )
#define aSEC_IN_JUL  ( ( TULONGINT )86400 * ( TULONGINT )31 )
#define aSEC_IN_AUG  ( ( TULONGINT )86400 * ( TULONGINT )31 )
#define aSEC_IN_SEP  ( ( TULONGINT )86400 * ( TULONGINT )30 )
#define aSEC_IN_OCT  ( ( TULONGINT )86400 * ( TULONGINT )31 )
#define aSEC_IN_NOV  ( ( TULONGINT )86400 * ( TULONGINT )30 )
#define aSEC_IN_DEC  ( ( TULONGINT )86400 * ( TULONGINT )31 )
// On calcule les secondes cumulées
static TCONST TULONGINT far tuliSecInYear[ 12 ] =
{
 0,
 aSEC_IN_JAN,
 aSEC_IN_JAN + aSEC_IN_FEB,
 aSEC_IN_JAN + aSEC_IN_FEB + aSEC_IN_MAR,
 aSEC_IN_JAN + aSEC_IN_FEB + aSEC_IN_MAR + aSEC_IN_APR,
 aSEC_IN_JAN + aSEC_IN_FEB + aSEC_IN_MAR + aSEC_IN_APR + aSEC_IN_MAY,
 aSEC_IN_JAN + aSEC_IN_FEB + aSEC_IN_MAR + aSEC_IN_APR + aSEC_IN_MAY + aSEC_IN_JUN,
 aSEC_IN_JAN + aSEC_IN_FEB + aSEC_IN_MAR + aSEC_IN_APR + aSEC_IN_MAY + aSEC_IN_JUN + 
 aSEC_IN_JUL,
 aSEC_IN_JAN + aSEC_IN_FEB + aSEC_IN_MAR + aSEC_IN_APR + aSEC_IN_MAY + aSEC_IN_JUN + 
 aSEC_IN_JUL + aSEC_IN_AUG,
 aSEC_IN_JAN + aSEC_IN_FEB + aSEC_IN_MAR + aSEC_IN_APR + aSEC_IN_MAY + aSEC_IN_JUN + 
 aSEC_IN_JUL + aSEC_IN_AUG + aSEC_IN_SEP,
 aSEC_IN_JAN + aSEC_IN_FEB + aSEC_IN_MAR + aSEC_IN_APR + aSEC_IN_MAY + aSEC_IN_JUN +
 aSEC_IN_JUL + aSEC_IN_AUG + aSEC_IN_SEP + aSEC_IN_OCT,
 aSEC_IN_JAN + aSEC_IN_FEB + aSEC_IN_MAR + aSEC_IN_APR + aSEC_IN_MAY + aSEC_IN_JUN + 
 aSEC_IN_JUL + aSEC_IN_AUG + aSEC_IN_SEP + aSEC_IN_OCT + aSEC_IN_NOV
};
#endif

//============================================================================//
// Déclaration des variables exportées
//============================================================================//

//============================================================================//
// Déclaration des macros
//============================================================================//
/// Test si l'année est bissextile
#define bMIS_BISSEXTILE( ucYear ) ( TBOOL )( !ucYear || ( ( ( ucYear % 4 ) == 0 ) && ( ( ucYear % 100 ) != 0 ) ) )

//============================================================================//
// Déclaration des fonctions exportées
//============================================================================//

//------------------------------------------------------------------------------
/// \brief Test la validité d'une date\time
/// \param ptDateTime : Pointeur sur un objet T_DateTime (Destination)
/// \return Retourne aTRUE si ptDateTime est valide et aFALSE sinon
//------------------------------------------------------------------------------
#if __bFDATETIMEIsValid__
TBOOL bFDATETIMEIsValid( TCONST TDateTimeObject * ptDateTime )
{
 TUCHAR ucDayOfMonth;
 // Test des secondes
 if( ptDateTime->tTime.ucSecond >= ( TUCHAR )60 ) return( ( TBOOL )aFALSE );
 // Test des minutes
 if( ptDateTime->tTime.ucMinute >= ( TUCHAR )60 ) return( ( TBOOL )aFALSE );
 // Test des secondes
 if( ptDateTime->tTime.ucHour   >= ( TUCHAR )24 ) return( ( TBOOL )aFALSE );
 #if aDATETIME_SECOND_SINCE_2000
  // Test des années
  if( ptDateTime->tDate.ucYear   >= ( TUCHAR )aDATETIME_YEAR_MAX ) return( ( TBOOL )aFALSE );
 #endif
 // On vérifie que le mois est valide
 if( !ptDateTime->tDate.ucMonth || (ptDateTime->tDate.ucMonth > ( TUCHAR )aDECE) ) return( ( TBOOL )aFALSE );
 // Gestion des jours
 ucDayOfMonth = tucDayOfMonth[ ( TUCHAR )( ptDateTime->tDate.ucMonth - 1 ) ];
 if( (ptDateTime->tDate.ucMonth == ( TUCHAR )aFEVR) && bMIS_BISSEXTILE( ptDateTime->tDate.ucYear ) ) ucDayOfMonth++;
 // On vérifie le jour
 if( !ptDateTime->tDate.ucDay || (ptDateTime->tDate.ucDay > ucDayOfMonth) ) return( ( TBOOL )aFALSE );
 // On teste le jour
 #if aDATETIME_DAY_OF_WEEK
  if( ptDateTime->tDate.ucDayOfWeek != ucFDayOfWeek( &ptDateTime->tDate ) ) return( ( TBOOL )aFALSE );
 #endif
 // OK tout est correcte
 return( ( TBOOL )aTRUE );
}
#endif

//------------------------------------------------------------------------------
/// \brief Test la validité d'une date
/// \param ptDate : Pointeur sur un objet TDateObject (Destination)
/// \return Retourne aTRUE si ptDate est valide et aFALSE sinon
//------------------------------------------------------------------------------
#if __bFDATETIMEIsDateValid__
TBOOL bFDATETIMEIsDateValid( TCONST TDateObject * ptDate )
{
 TUCHAR ucDayOfMonth;

 #if aDATETIME_SECOND_SINCE_2000
  // Test des années
  if( ptDate->ucYear >= ( TUCHAR )aDATETIME_YEAR_MAX ) return( ( TBOOL )aFALSE );
 #endif 
 // On vérifie que le mois est valide
 if( !ptDate->ucMonth || (ptDate->ucMonth > ( TUCHAR )aDECE) ) return( ( TBOOL )aFALSE );
 // Gestion des jours
 ucDayOfMonth = tucDayOfMonth[ ( TUCHAR )( ptDate->ucMonth - 1 ) ];
 if( (ptDate->ucMonth == ( TUCHAR )aFEVR) && bMIS_BISSEXTILE( ptDate->ucYear ) ) ucDayOfMonth++;
 // On vérifie le jour
 if( !ptDate->ucDay || (ptDate->ucDay > ucDayOfMonth) ) return( ( TBOOL )aFALSE );
 // On teste le jour
 #if aDATETIME_DAY_OF_WEEK
  if( ptDate->ucDayOfWeek != ucFDayOfWeek( ptDate ) ) return( ( TBOOL )aFALSE );
 #endif
 // OK tout est correcte
 return( ( TBOOL )aTRUE );
}
#endif

//------------------------------------------------------------------------------
/// \brief Test la validité d'une heure
/// \param ptDate : Pointeur sur un objet TTimeObject (Destination)
/// \return Retourne aTRUE si pT_Time est valide et aFALSE sinon
//------------------------------------------------------------------------------
#if __bFDATETIMEIsTimeValid__
TBOOL bFDATETIMEIsTimeValid( TCONST TTimeObject * ptTime )
{
 // Test des secondes
 if( ptTime->ucSecond >= ( TUCHAR )60 ) return( ( TBOOL )aFALSE );
 // Test des minutes
 if( ptTime->ucMinute >= ( TUCHAR )60 ) return( ( TBOOL )aFALSE );
 // Test des secondes
 if( ptTime->ucHour   >= ( TUCHAR )24 ) return( ( TBOOL )aFALSE );
 // OK tout est correcte
 return( ( TBOOL )aTRUE );
}
#endif

//------------------------------------------------------------------------------
/// \brief Comparaison de 2 dates
/// \param ptDateTime1 : Pointeur sur un objet TDateTimeObject
/// \param ptDateTime2 : Pointeur sur un objet TDateTimeObject
/// \return Retourne aTRUE si ptDateTime1 est supérieure ou égale à ptDateTime2 
//------------------------------------------------------------------------------
#if __bFDATETIMESuperior__
TBOOL bFDATETIMESuperior( TCONST TDateTimeObject * ptDateTime1, TCONST TDateTimeObject * ptDateTime2 )
{
 #if !aDATETIME_SECOND_SINCE_2000
  // On teste la supériorité de la date
  if( ptDateTime1->tDate.ucYear   >  ptDateTime2->tDate.ucYear   ) return( ( TBOOL )aTRUE );
  if( ptDateTime1->tDate.ucYear   <  ptDateTime2->tDate.ucYear   ) return( ( TBOOL )aFALSE );
  if( ptDateTime1->tDate.ucMonth  >  ptDateTime2->tDate.ucMonth  ) return( ( TBOOL )aTRUE );
  if( ptDateTime1->tDate.ucMonth  <  ptDateTime2->tDate.ucMonth  ) return( ( TBOOL )aFALSE );
  if( ptDateTime1->tDate.ucDay    >  ptDateTime2->tDate.ucDay    ) return( ( TBOOL )aTRUE );
  if( ptDateTime1->tDate.ucDay    <  ptDateTime2->tDate.ucDay    ) return( ( TBOOL )aFALSE );
  if( ptDateTime1->tTime.ucHour   >  ptDateTime2->tTime.ucHour   ) return( ( TBOOL )aTRUE );
  if( ptDateTime1->tTime.ucHour   <  ptDateTime2->tTime.ucHour   ) return( ( TBOOL )aFALSE );
  if( ptDateTime1->tTime.ucMinute >  ptDateTime2->tTime.ucMinute ) return( ( TBOOL )aTRUE );
  if( ptDateTime1->tTime.ucMinute <  ptDateTime2->tTime.ucMinute ) return( ( TBOOL )aFALSE );
  if( ptDateTime1->tTime.ucSecond >= ptDateTime2->tTime.ucSecond ) return( ( TBOOL )aTRUE );
  // Inférieur
  return( ( TBOOL )aFALSE );
 #else
  // Comparaison directe
  return( ( TBOOL )(ptDateTime1->uliSecond >= ptDateTime2->uliSecond) );
 #endif
}
#endif

//------------------------------------------------------------------------------
/// \brief Comparaison de 2 heures
/// \param ptTime1 : Pointeur sur un objet TTimeObject
/// \param ptTime2 : Pointeur sur un objet TTimeObject
/// \return Retourne aTRUE si ptTime1 est supérieure ou égale à ptTime2 
//------------------------------------------------------------------------------
#if __bFDATETIMEIsTimeSuperior__
TBOOL bFDATETIMEIsTimeSuperior( TCONST TTimeObject * ptTime1, TCONST TTimeObject * ptTime2 )
{
 // On teste la supériorité de la date
 if( ptTime1->ucHour   >  ptTime2->ucHour   ) return( ( TBOOL )aTRUE );
 if( ptTime1->ucHour   <  ptTime2->ucHour   ) return( ( TBOOL )aFALSE );
 if( ptTime1->ucMinute >  ptTime2->ucMinute ) return( ( TBOOL )aTRUE );
 if( ptTime1->ucMinute <  ptTime2->ucMinute ) return( ( TBOOL )aFALSE );
 if( ptTime1->ucSecond >= ptTime2->ucSecond ) return( ( TBOOL )aTRUE );
 // Inférieure
 return( ( TBOOL )aFALSE );
}
#endif

//------------------------------------------------------------------------------
/// \brief Comparaison de 2 dates
/// \param ptDateTime1 : Pointeur sur un objet TDateObject
/// \param ptDateTime2 : Pointeur sur un objet TDateObject
/// \return Retourne aTRUE si ptDate1 est supérieure ou égale à ptDate2 
//------------------------------------------------------------------------------
#if __bFDATETIMEIsDateSuperior__
TBOOL bFDATETIMEIsDateSuperior( TCONST TDateObject * ptDate1, TCONST TDateObject * ptDate2 )
{
 // On teste la supériorité de la date
 if( ptDate1->ucYear   >  ptDate2->ucYear  ) return( ( TBOOL )aTRUE );
 if( ptDate1->ucYear   <  ptDate2->ucYear  ) return( ( TBOOL )aFALSE );
 if( ptDate1->ucMonth  >  ptDate2->ucMonth ) return( ( TBOOL )aTRUE );
 if( ptDate1->ucMonth  <  ptDate2->ucMonth ) return( ( TBOOL )aFALSE );
 if( ptDate1->ucDay    >= ptDate2->ucDay   ) return( ( TBOOL )aTRUE );
 // Inférieure
 return( ( TBOOL )aFALSE );
}
#endif

//------------------------------------------------------------------------------
/// \brief Convertion de secondes en TTimeObject
/// \param pT_Time     : Pointeur de destination de la convertion sur un objet TTimeObject
/// \param uliSecTime : Nombre de seconde à convertir en heure
/// \return Retourne aTRUE si uliSecTime < 86400 secondes
//------------------------------------------------------------------------------
#if __bFDATETIMESecondsToTime__
TBOOL bFDATETIMESecondsToTime( TTimeObject * ptTime, TULONGINT uliSecTime )
{
 TUINT uiSecTime = ( TUINT )(uliSecTime / ( TULONGINT )60);
 // On récupère les secondes
 ptTime->ucSecond = ( TUCHAR )(uliSecTime % ( TULONGINT )60);
 // On récupère les minutes
 ptTime->ucMinute = ( TUCHAR )(uiSecTime % ( TUINT )60);
 // On récupère les heures
 ptTime->ucHour = ( TUCHAR )(uiSecTime / ( TUINT )60);
 // On gère les débordements de valeur
 if( ptTime->ucHour < ( TUCHAR )24 ) return( ( TBOOL )aTRUE );
 // intégrité de la données
 ptTime->ucHour %= ( TUCHAR )24;
 // Erreur sur les sécondes
 return( ( TBOOL )aFALSE );
}
#endif

//------------------------------------------------------------------------------
/// \brief Ajout d'une heure à une heure
/// \param ptTime1     : Pointeur de destination de l'heure à modifier
/// \param ptTime2    : Pointeur de source de l'heure à ajouter
/// \return Aucun
//------------------------------------------------------------------------------
#if __bFDATETIMEAddTimeToTime__
TBOOL bFDATETIMEAddTimeToTime( TTimeObject * ptTime1, TCONST TTimeObject * ptTime2 )
{
 // Test des secondes
 if( ptTime1->ucSecond >= ( TUCHAR )60 ) return( ( TBOOL )aFALSE );
 // Test des minutes
 if( ptTime1->ucMinute >= ( TUCHAR )60 ) return( ( TBOOL )aFALSE );
 // Test des heures
 if( ptTime1->ucHour   >= ( TUCHAR )24 ) return( ( TBOOL )aFALSE );
 // Test des secondes
 if( ptTime2->ucSecond >= ( TUCHAR )60 ) return( ( TBOOL )aFALSE );
 // Test des minutes
 if( ptTime2->ucMinute >= ( TUCHAR )60 ) return( ( TBOOL )aFALSE );
 // Test des heures
 if( ptTime2->ucHour   >= ( TUCHAR )24 ) return( ( TBOOL )aFALSE );
 // On ajoute les secondes
 ptTime1->ucSecond += ptTime2->ucSecond;
 // On teste l'overflow
 if( ptTime1->ucSecond >= ( TUCHAR )60 ) 
  {
   // On soustrait les secondes
   ptTime1->ucSecond -= ( TUCHAR )60;
   // On ajoute une minute
   ptTime1->ucMinute++;
  }
 // On ajoute les minutes
 ptTime1->ucMinute += ptTime2->ucMinute;
 // On teste l'overflow
 if( ptTime1->ucMinute >= ( TUCHAR )60 ) 
  {
   // On soustrait les minutes
   ptTime1->ucMinute -= ( TUCHAR )60;
   // On ajoute une heure
   ptTime1->ucHour++;
  }
 // On ajoute les heures
 ptTime1->ucHour += ptTime2->ucHour;
 // On teste l'overflow
 if( ptTime1->ucHour >= ( TUCHAR )24 ) ptTime1->ucHour -= ( TUCHAR )24;
 // Ok
 return( ( TBOOL )aTRUE );
}
#endif

//------------------------------------------------------------------------------
/// \brief Soustrait d'une heure à une heure
/// \param ptTime1     : Pointeur de destination de l'heure à modifier
/// \param ptTime2    : Pointeur de source de l'heure à ajouter
/// \return Aucun
//------------------------------------------------------------------------------
#if __bFDATETIMEDecTimeToTime__
TBOOL bFDATETIMEDecTimeToTime( TTimeObject * ptTime1, TCONST TTimeObject * ptTime2 )
{
 // Test des secondes
 if( ptTime1->ucSecond >= ( TUCHAR )60 ) return( ( TBOOL )aFALSE );
 // Test des minutes
 if( ptTime1->ucMinute >= ( TUCHAR )60 ) return( ( TBOOL )aFALSE );
 // Test des heures
 if( ptTime1->ucHour   >= ( TUCHAR )24 ) return( ( TBOOL )aFALSE );
 // Test des secondes
 if( ptTime2->ucSecond >= ( TUCHAR )60 ) return( ( TBOOL )aFALSE );
 // Test des minutes
 if( ptTime2->ucMinute >= ( TUCHAR )60 ) return( ( TBOOL )aFALSE );
 // Test des heures
 if( ptTime2->ucHour   >= ( TUCHAR )24 ) return( ( TBOOL )aFALSE );
 // On ajoute les secondes
 ptTime1->ucSecond -= ptTime2->ucSecond;
 // On teste l'overflow
 if( ptTime1->ucSecond >= ( TUCHAR )60 ) 
  {
   // On soustrait les secondes
   ptTime1->ucSecond += ( TUCHAR )60;
   // On ajoute une minute
   ptTime1->ucMinute--;
  }
 // On ajoute les minutes
 ptTime1->ucMinute -= ptTime2->ucMinute;
 // On teste l'overflow
 if( ptTime1->ucMinute >= ( TUCHAR )60 ) 
  {
   // On soustrait les minutes
   ptTime1->ucMinute += ( TUCHAR )60;
   // On ajoute une heure
   ptTime1->ucHour--;
  }
 // On ajoute les heures
 ptTime1->ucHour -= ptTime2->ucHour;
 // On teste l'overflow
 if( ptTime1->ucHour >= ( TUCHAR )24 ) ptTime1->ucHour += ( TUCHAR )24;
 // Ok
 return( ( TBOOL )aTRUE );
}
#endif

//------------------------------------------------------------------------------
/// \brief Ajout de jours à une TDateObject
/// \param ptDate : Pointeur sur un objet TDateObject (Destination)
/// \param uliDays : Nombre de jours à ajouter à ptDateTime
/// \return Aucun
//------------------------------------------------------------------------------
#if __bFDATETIMEAddDays__
TBOOL bFDATETIMEAddDays( TDateTimeObject * ptDateTime, TULONGINT uliDays )
{
 TUCHAR    ucDayOfMonth;

 // On teste la validité de la date
 if( !bFDATETIMEIsDateValid( &ptDateTime->tDate ) ) return( ( TBOOL )aFALSE );
 // On ajoute le nombre de jours
 while( uliDays-- )
  {
   // On incrémente les jours
   ptDateTime->tDate.ucDay++;
   // Gestion des jours
   ucDayOfMonth = tucDayOfMonth[ ( TUCHAR )( ptDateTime->tDate.ucMonth - 1 ) ];
   if( (ptDateTime->tDate.ucMonth == ( TUCHAR )aFEVR) && bMIS_BISSEXTILE( ptDateTime->tDate.ucYear ) ) ucDayOfMonth++;
   if( ptDateTime->tDate.ucDay > ucDayOfMonth )
    {
     // On efface les jours
     ptDateTime->tDate.ucDay = ( TUCHAR )1;
     // On teste le mois
     if( ++ptDateTime->tDate.ucMonth > ( TUCHAR )aDECE )
      {
       // On regarde si pas de débordement de date à 2135 ou 2255
       #if aDATETIME_SECOND_SINCE_2000
        if( ++ptDateTime->tDate.ucYear >= ( TUCHAR )aDATETIME_YEAR_MAX )
       #else
        if( !( TUCHAR )( ++ptDateTime->tDate.ucYear ) )
       #endif
        {
         // On reste sur le 31/12/255
         ptDateTime->tDate.ucDay   = ( TUCHAR )31;
         ptDateTime->tDate.ucMonth = ( TUCHAR )aDECE;
         #if aDATETIME_SECOND_SINCE_2000
          ptDateTime->tDate.ucYear = ( TUCHAR )aDATETIME_YEAR_MAX - 1;
         #else
          ptDateTime->tDate.ucYear = ( TUCHAR )255;
         #endif
         break;
        }
       // On repasse le premier janvier
       ptDateTime->tDate.ucMonth = ( TUCHAR )aJANV;
      }
    }
   #if aDATETIME_DAY_OF_WEEK
    if( ++ptDateTime->tDate.ucDayOfWeek > ( TUCHAR )aDI ) ptDateTime->tDate.ucDayOfWeek = ( TUCHAR )aLU;
   #endif
   // Si le compteur de secondes est utilisé
   #if aDATETIME_SECOND_SINCE_2000
    // On incrémente le compteur de secondes
    ptDateTime->uliSecond += ( TULONGINT )24 * ( TULONGINT )60 * ( TULONGINT )60;
   #endif
  }
 // Ok
 return( ( TBOOL )aTRUE );
}
#endif

//------------------------------------------------------------------------------
/// \brief Soustrait de jours à une TDateObject
/// \param ptDate : Pointeur sur un objet TDateObject (Destination)
/// \param uliDays : Nombre de jours à soustraire à ptDateTime
/// \return Aucun
//------------------------------------------------------------------------------
#if __bFDATETIMEDecDays__
TBOOL bFDATETIMEDecDays( TDateTimeObject * ptDateTime, TULONGINT uliDays )
{
 // On teste la validité de la date
 if( !bFDATETIMEIsDateValid( &ptDateTime->tDate ) ) return( ( TBOOL )aFALSE );
 // On soustrait le nombre de jours
 while( uliDays-- )
  {
   // On décrémente les jours
   if( !(--ptDateTime->tDate.ucDay) )
    {
     // On décrémente les mois
     if( !(--ptDateTime->tDate.ucMonth) )
      {
       // On passe au mois de décembre
       ptDateTime->tDate.ucMonth = ( TUCHAR )aDECE;
       // On dérémente les jours
       if( !(ptDateTime->tDate.ucYear--) )
        {
         // On limite à l'an 01/01/2000
         ptDateTime->tDate.ucDay   = ( TUCHAR )1;
         ptDateTime->tDate.ucMonth = ( TUCHAR )aJANV;
         ptDateTime->tDate.ucYear  = ( TUCHAR )0;
         break;
        }
      }
     // Gestion des jours
     ptDateTime->tDate.ucDay = tucDayOfMonth[ ( TUCHAR )( ptDateTime->tDate.ucMonth - 1 ) ];
     if( (ptDateTime->tDate.ucMonth == ( TUCHAR )aFEVR) && bMIS_BISSEXTILE( ptDateTime->tDate.ucYear ) ) ptDateTime->tDate.ucDay++;
    }
   #if aDATETIME_DAY_OF_WEEK
    if( ( TCHAR )(--ptDateTime->tDate.ucDayOfWeek) < ( TCHAR )aLU ) ptDateTime->tDate.ucDayOfWeek = ( TUCHAR )aDI;
   #endif
   // Si le compteur de secondes est utilisé
   #if aDATETIME_SECOND_SINCE_2000
    // On décrémente le compteur de secondes
    ptDateTime->uliSecond -= ( TULONGINT )24 * ( TULONGINT )60 * ( TULONGINT )60;
   #endif
  }
 // Ok
 return( ( TBOOL )aTRUE );
}
#endif

//------------------------------------------------------------------------------
/// \brief Ajout de secondes à une TDateTimeObject
/// \param ptDateTime : Pointeur sur un objet TDateTimeObject (Destination)
/// \param uliSecTime : Nombre de seconde à ajouter à ptDateTime
/// \return Aucun
//------------------------------------------------------------------------------
#if (__bFDATETIMEAddSeconds__ && __bFDATETIMEAddTime__ && __bFDATETIMEAddDays__)
TBOOL bFDATETIMEAddSeconds( TDateTimeObject * ptDateTime, TULONGINT uliSecTime )
{
 TUINT uiNumberOfDay = ( TUINT )(uliSecTime / (( TULONGINT )24 * ( TULONGINT )60 * ( TULONGINT )60));
 TTimeObject tTime;

 // on ajoute l'heure en cours à la date
 if( !bFDATETIMESecondsToTime( &tTime, uliSecTime % (( TULONGINT )24 * ( TULONGINT )60 * ( TULONGINT )60) ) ) return( ( TBOOL )aFALSE );
 // On le fait 2 fois car 12H / 2 = 1 jour
 if( !bFDATETIMEAddTime( ptDateTime, &tTime ) ) return( ( TBOOL )aFALSE );
 // On ajoute le nombre de jour
 if( !bFDATETIMEAddDays( ptDateTime, ( TULONGINT )uiNumberOfDay ) ) return( ( TBOOL )aFALSE );
 // Ok
 return( ( TBOOL )aTRUE );
}
#endif

//------------------------------------------------------------------------------
/// \brief Soustraction de secondes à une TDateTimeObject
/// \param ptDateTime : Pointeur sur un objet TDateTimeObject (Destination)
/// \param uliSecTime : Nombre de seconde à soustraire à ptDateTime
/// \return Aucun
//------------------------------------------------------------------------------
#if (__bFDATETIMEDecSeconds__ && __bFDATETIMEDecTime__ && __bFDATETIMEDecDays__)
TBOOL bFDATETIMEDecSeconds( TDateTimeObject * ptDateTime, TULONGINT uliSecTime )
{
 TUINT uiNumberOfDay = ( TUINT )(uliSecTime / (( TULONGINT )24 * ( TULONGINT )60 * ( TULONGINT )60));
 TTimeObject tTime;

 // on ajoute l'heure en cours à la date
 if( !bFDATETIMESecondsToTime( &tTime, uliSecTime % (( TULONGINT )24 * ( TULONGINT )60 * ( TULONGINT )60) )  ) return( ( TBOOL )aFALSE );
 // On le fait 2 fois car 12H / 2 = 1 jour
 if( !bFDATETIMEDecTime( ptDateTime, &tTime ) ) return( ( TBOOL )aFALSE );
 // On ajoute le nombre de jour
 if( !bFDATETIMEDecDays( ptDateTime, ( TULONGINT )uiNumberOfDay ) ) return( ( TBOOL )aFALSE );
 // Ok
 return( ( TBOOL )aTRUE );
}
#endif

//------------------------------------------------------------------------------
/// \brief Ajout d'une heure à une date
/// \param ptDateTime : Pointeur sur un objet TDateTimeObject (Destination)
/// \param pT_Time     : Nombre d'heures, minutes, secondes à ajouter à la ptDateTime
/// \return Aucun
//------------------------------------------------------------------------------
#if __bFDATETIMEAddTime__
TBOOL bFDATETIMEAddTime( TDateTimeObject * ptDateTime, TCONST TTimeObject * ptTime )
{
 TUCHAR ucDayOfMonth;

 // Pour intégrité :: On teste la validité de l'heure
 if( !bFDATETIMEIsTimeValid( ptTime ) ) return( ( TBOOL )aFALSE );
 // On teste la validité de la date
 if( !bFDATETIMEIsValid( ptDateTime ) ) return( ( TBOOL )aFALSE );
 // Ajout des secondes
 ptDateTime->tTime.ucSecond += ptTime->ucSecond;
 // on test le résultat
 if( ptDateTime->tTime.ucSecond >= ( TUCHAR )60 )
  {
   // On ajoute les minutes
   ptDateTime->tTime.ucMinute++;
   // On soustrait le nombre de seconde
   ptDateTime->tTime.ucSecond -= ( TUCHAR )60;
  }
 // Ajout des minutes
 ptDateTime->tTime.ucMinute += ptTime->ucMinute;
 if( ptDateTime->tTime.ucMinute >= ( TUCHAR )60 )
  {
   // On ajoute les minutes
   ptDateTime->tTime.ucHour++;
   // On soustrait le nombre de seconde
   ptDateTime->tTime.ucMinute -= ( TUCHAR )60;
  }
 // Ajout des heures
 ptDateTime->tTime.ucHour += ptTime->ucHour;
 // Gestion d'une exception simple
 do
  {
   if( ptDateTime->tTime.ucHour >= ( TUCHAR )24 )
    {
     // On soustrait le nombre de seconde
     ptDateTime->tTime.ucHour -= ( TUCHAR )24;
     // On ajoute les minutes
     ptDateTime->tDate.ucDay++;
     // Gestion des jours
     ucDayOfMonth = tucDayOfMonth[ ( TUCHAR )( ptDateTime->tDate.ucMonth - 1 ) ];
     if( (ptDateTime->tDate.ucMonth == ( TUCHAR )aFEVR) && bMIS_BISSEXTILE( ptDateTime->tDate.ucYear ) ) ucDayOfMonth++;
     if( ptDateTime->tDate.ucDay > ucDayOfMonth )
      {
       // On efface les jours
       ptDateTime->tDate.ucDay =  ( TUCHAR )1;
       // On teste le mois
       if( ++ptDateTime->tDate.ucMonth > ( TUCHAR )aDECE )
        {
         // On regarde si pas de débordement de date à 2255
         #if aDATETIME_SECOND_SINCE_2000
          if( ++ptDateTime->tDate.ucYear >= ( TUCHAR )aDATETIME_YEAR_MAX )
         #else
          if( !( TUCHAR )( ++ptDateTime->tDate.ucYear ) )
         #endif
          {
           // On reste sur le 31/12/255
           ptDateTime->tDate.ucDay   = ( TUCHAR )31;
           ptDateTime->tDate.ucMonth = ( TUCHAR )aDECE;
           #if aDATETIME_SECOND_SINCE_2000
            ptDateTime->tDate.ucYear = ( TUCHAR )aDATETIME_YEAR_MAX - 1;
            // On gère le débordement en soustrayant un jour
            ptDateTime->uliSecond    -= ( TULONGINT )24 * ( TULONGINT )60 * ( TULONGINT )60;
           #else
            ptDateTime->tDate.ucYear = ( TUCHAR )255;
           #endif
           // Déclenchement de l'exception
           break;
          }
         // On est le premier janvier
         ptDateTime->tDate.ucMonth = ( TUCHAR )aJANV;
        }
      }
     // Gestion du jour de la semaine
     #if aDATETIME_DAY_OF_WEEK
      if( ++ptDateTime->tDate.ucDayOfWeek > ( TUCHAR )aDI ) ptDateTime->tDate.ucDayOfWeek = ( TUCHAR )aLU;
     #endif
    }
  }
 while( ( TBOOL )aFALSE );
 #if aDATETIME_SECOND_SINCE_2000
  //incrémente le nombres de secondes
  //ajout des secondes
  ptDateTime->uliSecond += ptTime->ucSecond;
  //ajout des minutes
  ptDateTime->uliSecond += ptTime->ucMinute * ( TULONGINT )60;
  //ajout des heures
  ptDateTime->uliSecond += ptTime->ucHour * ( TULONGINT )60 * ( TULONGINT )60;
 #endif
 // Ok
 return( ( TBOOL )aTRUE );
}
#endif

//------------------------------------------------------------------------------
/// \brief Soustrait une heure à une date
/// \param ptDateTime : Pointeur sur un objet TDateTimeObject (Destination)
/// \param pT_Time     : Nombre d'heures, minutes, secondes à soustraire à la ptDateTime
/// \return Aucun
//------------------------------------------------------------------------------
#if __bFDATETIMEDecTime__
TBOOL bFDATETIMEDecTime( TDateTimeObject * ptDateTime, TCONST TTimeObject * ptTime )
{
 // Pour intégrité :: On teste la validité de l'heure
 if( !bFDATETIMEIsTimeValid( ptTime ) ) return( ( TBOOL )aFALSE );
 // On teste la validité de la date
 if( !bFDATETIMEIsValid( ptDateTime ) ) return( ( TBOOL )aFALSE );
 // Soustraction des secondes
 ptDateTime->tTime.ucSecond -= ptTime->ucSecond;
 // On teste le résultat
 if( ( TCHAR )ptDateTime->tTime.ucSecond < ( TCHAR )0 )
  {
   // On ajoute les minutes
   ptDateTime->tTime.ucMinute--;
   // On soustrait le nombre de seconde
   ptDateTime->tTime.ucSecond += ( TUCHAR )60;
  }
 // Ajout des minutes
 ptDateTime->tTime.ucMinute -= ptTime->ucMinute;
 if( ( TCHAR )ptDateTime->tTime.ucMinute < ( TCHAR )0 )
  {
   // On ajoute les minutes
   ptDateTime->tTime.ucHour--;
   // On soustrait le nombre de seconde
   ptDateTime->tTime.ucMinute += ( TUCHAR )60;
  }
 // Ajout des heures
 ptDateTime->tTime.ucHour -= ptTime->ucHour;
 // Gestion d'une exception simple
 do
  {
   if( ( TCHAR )ptDateTime->tTime.ucHour < ( TCHAR )0 )
    {
     // On soustrait le nombre de seconde
     ptDateTime->tTime.ucHour += ( TUCHAR )24;
     // On décrémente les jours
     if( !(--ptDateTime->tDate.ucDay) )
      {
       // On cécrémente de mois
       if( !(--ptDateTime->tDate.ucMonth) )
        {
         // On passe au mois de décembre
         ptDateTime->tDate.ucMonth = ( TUCHAR )aDECE;
         if( !(ptDateTime->tDate.ucYear--) )
          {
           // On limite à l'an 01/01/2000
           ptDateTime->tDate.ucDay   = ( TUCHAR )1;
           ptDateTime->tDate.ucMonth = ( TUCHAR )aJANV;
           ptDateTime->tDate.ucYear  = ( TUCHAR )0;
           #if aDATETIME_SECOND_SINCE_2000   
            // On gère le débordement en rajoutant un jour
            ptDateTime->uliSecond    += ( TULONGINT )24 * ( TULONGINT )60 * ( TULONGINT )60;
           #endif
           // Déclenchement de l'exception
           break;
          }
        }
       // Gestion des jours
       ptDateTime->tDate.ucDay = tucDayOfMonth[ ( TUCHAR )( ptDateTime->tDate.ucMonth - 1 ) ];
       if( ( ptDateTime->tDate.ucMonth == ( TUCHAR )aFEVR ) && bMIS_BISSEXTILE( ptDateTime->tDate.ucYear ) ) ptDateTime->tDate.ucDay++;
      }
     #if aDATETIME_DAY_OF_WEEK
      if( ( ( TCHAR )--ptDateTime->tDate.ucDayOfWeek ) < ( TCHAR )aLU ) ptDateTime->tDate.ucDayOfWeek = ( TUCHAR )aDI;
     #endif
    }
  }
 while( ( TBOOL )aFALSE );
 #if aDATETIME_SECOND_SINCE_2000
  //incrémente le nombres de secondes :: ajout des secondes
  ptDateTime->uliSecond -= ptTime->ucSecond;
  //ajout des minutes
  ptDateTime->uliSecond -= ptTime->ucMinute * ( TULONGINT )60;
  //ajout des heures
  ptDateTime->uliSecond -= ptTime->ucHour * ( TULONGINT )60 * ( TULONGINT )60;
 #endif
 // Ok
 return( ( TBOOL )aTRUE );
}
#endif

//------------------------------------------------------------------------------
/// \brief Ajout d'une seconde à une DateTime
/// \param ptDateTime : Pointeur sur un objet TDateTimeObject (Destination)
/// \return Aucun
//------------------------------------------------------------------------------
#if __vFDATETIMEAddSecondTick__
void vFDATETIMEAddSecondTick( TDateTimeObject * ptDateTime )
{
 TUCHAR ucDayOfMonth;
 #if aSUMMER_WINTER_DATETIME
 static TBOOL bWinterTime = ( TBOOL )aFALSE;
 #endif

 // Ajout des secondes
 if( ++ptDateTime->tTime.ucSecond >= ( TUCHAR )60 )
  {
   // On soustrait le nombre de seconde
   ptDateTime->tTime.ucSecond = 0;
   // Ajout des minutes
   if( ++ptDateTime->tTime.ucMinute >= ( TUCHAR )60 )
    {
     // On soustrait le nombre de seconde
     ptDateTime->tTime.ucMinute = 0;
     // Ajout des heures
     if( ++ptDateTime->tTime.ucHour >= ( TUCHAR )24 )
      {
       // On soustrait le nombre de seconde
       ptDateTime->tTime.ucHour = 0;
       // On ajoute les minutes
       ptDateTime->tDate.ucDay++;
       // Gestion des jours
       ucDayOfMonth = tucDayOfMonth[ ( TUCHAR )( ptDateTime->tDate.ucMonth - 1 ) ];
       if( (ptDateTime->tDate.ucMonth == ( TUCHAR )aFEVR) && bMIS_BISSEXTILE( ptDateTime->tDate.ucYear ) ) ucDayOfMonth++;
       if( ptDateTime->tDate.ucDay > ucDayOfMonth )
        {
         // On efface les jours
         ptDateTime->tDate.ucDay = ( TUCHAR )1;
         // On teste le mois
         if( ++ptDateTime->tDate.ucMonth > ( TUCHAR )aDECE )
          {
           ptDateTime->tDate.ucMonth = ( TUCHAR )1;
           // On regarde si pas de débordement de date à 2255
           #if aDATETIME_SECOND_SINCE_2000
            if( ++ptDateTime->tDate.ucYear >= ( TUCHAR )aDATETIME_YEAR_MAX )
           #else
            if( !( TUCHAR )( ++ptDateTime->tDate.ucYear ) )
           #endif
            {
             // On reste à 23:59:59
             ptDateTime->tTime.ucSecond = ( TUCHAR )59;
             ptDateTime->tTime.ucMinute = ( TUCHAR )59;
             ptDateTime->tTime.ucHour   = ( TUCHAR )23;
             // On reste sur le 31/12/255
             ptDateTime->tDate.ucDay   = ( TUCHAR )31;
             ptDateTime->tDate.ucMonth = ( TUCHAR )aDECE;
             #if aDATETIME_SECOND_SINCE_2000
              ptDateTime->tDate.ucYear  = ( TUCHAR )aDATETIME_YEAR_MAX - 1;
             #else
              ptDateTime->tDate.ucYear  = ( TUCHAR )255;
             #endif
             // On incrémente pas le compteur de seconde non plus
             return;
            }
          }
        }
       #if aDATETIME_DAY_OF_WEEK
        if( ++ptDateTime->tDate.ucDayOfWeek > ( TUCHAR )aDI ) ptDateTime->tDate.ucDayOfWeek = ( TUCHAR )aLU;
       #endif
      }
     #if aSUMMER_WINTER_DATETIME
     // On cherche la fin des mois utiles
     if( (ptDateTime->tDate.ucDayOfWeek == ( TUCHAR )aDI) && ( (tucDayOfMonth[ ( TUCHAR )( ptDateTime->tDate.ucMonth - 1 ) ] - ptDateTime->tDate.ucDay) < ( TUCHAR )7 ) )
      {
       // Changement ete --> hiver
       if( (ptDateTime->tDate.ucMonth == ( TUCHAR )aSUMMER_MONTH) && (ptDateTime->tTime.ucHour == ( TUCHAR )(aCHANGING_HOUR + aWINTER_TIME)) )
        {
         // On ajoute le nombre d'heure souhaité
         ptDateTime->tTime.ucHour += ( TUCHAR )(aSUMMER_TIME - aWINTER_TIME);
        }
       // Changement hiver --> ete
       if( !bWinterTime && (ptDateTime->tDate.ucMonth == ( TUCHAR )aWINTER_MONTH) && (ptDateTime->tTime.ucHour == ( TUCHAR )(aCHANGING_HOUR + aSUMMER_TIME)) )
        {
         // On ajoute le nombre d'heure souhaité
         ptDateTime->tTime.ucHour -= ( TUCHAR )(aSUMMER_TIME - aWINTER_TIME);
         // On mémorise l'événement
         bWinterTime = ( TBOOL )aTRUE;
        }
       else bWinterTime = ( TBOOL )aFALSE;
      }
     #endif 
    }
  }
 #if aDATETIME_SECOND_SINCE_2000
  //incrémente le nombres de secondes
  ptDateTime->uliSecond++;
 #endif
}
#endif


//------------------------------------------------------------------------------
/// \brief Calcul le jour de la semaine en fonction de la date (depuis 1582)
/// \param ptDateTime : Pointeur sur un objet TDateTimeObject
/// \return Retourne le jour de la semaine
//------------------------------------------------------------------------------
#if __ucFDayOfWeek__
TUCHAR ucFDayOfWeek( TCONST TDateObject * ptDate )
{
 TINT   iDayOfWeek;
 TUCHAR ucMonth;
 TUCHAR ucYear;
 // On commence en 2000
 TUCHAR ucCentury = ( TUCHAR )20 + ( ptDate->ucYear / ( TUCHAR )100 );

 // Précalcule avant formule
 ucMonth = ptDate->ucMonth - ( TUCHAR )2;
 ucYear  = ptDate->ucYear % ( TUCHAR )100;
 // On teste la cohérence
 if( ( TCHAR )ucMonth <= 0 )
  {
   // On ajoute le nombre de mois nécessaires
   ucMonth += ( TUCHAR )12;
   // On teste le cohérence
   if( ( TCHAR )(--ucYear) < 0 )
    {
     // On fixe les années et siècle correspondant
     ucYear = ( TUCHAR )99;
     ucCentury--;
    }
  }
 // La formule est iJour = cQuantieme+(2.6*cMois-0.2)+(cAnnee+cAnnee/4)+(cSiecle/4-2*cSiecle)
 iDayOfWeek = ptDate->ucDay + (ucMonth * ( TUCHAR )13 - ( TUCHAR )1) / ( TUCHAR )5 + (ucYear + ucYear / ( TUCHAR )4) + (ucCentury / ( TUCHAR )4 - ucCentury * ( TUCHAR )2);
 // On recherche le jour
 while( iDayOfWeek < 0 ) iDayOfWeek += ( TINT )7;
 // On rammène le calcule en jour de LU à DI
 if( --iDayOfWeek < 0 ) iDayOfWeek = ( TINT )aDI;
 // Retourne le jour de la semaine
 return ( TUCHAR )( iDayOfWeek % ( TINT )7 );
}
#endif

//------------------------------------------------------------------------------
/// \brief Donne la date et l'heure locale en foncton de l'heure GMT
/// \param ptDateTime : Pointeur sur un objet TDateTimeObject (Destination)
/// \return ptLocalDateTime : Pointeur sur l'heure et la date locale
//------------------------------------------------------------------------------
#if __vFDATETIMEGetLocalTime__ && aDATETIME_DAY_OF_WEEK && aSUMMER_WINTER_DATETIME
void vFDATETIMEGetLocalTime( TDateTimeObject * ptLocalDateTime, TCONST TDateTimeObject * ptGMTDateTime )
{
 TUCHAR ucRestOfDaysInMonth;
 TBOOL bSummerTime = ( TBOOL )aFALSE;
 TTimeObject tLocalTime;

 // On copie les dates
 *ptLocalDateTime = *ptGMTDateTime;
 // On vérifie les bornes ( l'été )
 if( (ptGMTDateTime->tDate.ucMonth >= ( TUCHAR )aSUMMER_MONTH) && (ptGMTDateTime->tDate.ucMonth <= ( TUCHAR )aWINTER_MONTH) )
  {
   // Gestion d'une exception simple
   do
    {
     // Référence c'est l'été
     if( ptGMTDateTime->tDate.ucMonth < ( TUCHAR )aWINTER_MONTH )
      {
       if( ptGMTDateTime->tDate.ucMonth > ( TUCHAR )aSUMMER_MONTH ) { bSummerTime = ( TBOOL )aTRUE; break; }
       // On vérifie que l'on est vers la fin du mois
       // On regarde si l'on est le dernier dimanche du mois
       if( ( ucRestOfDaysInMonth = (tucDayOfMonth[ ( TUCHAR )( ptGMTDateTime->tDate.ucMonth - 1 ) ] - ptGMTDateTime->tDate.ucDay) ) < ( TUCHAR )7 )
        {
         if( (ptGMTDateTime->tDate.ucDayOfWeek != ( TUCHAR )aDI) && ((ptGMTDateTime->tDate.ucDayOfWeek + ucRestOfDaysInMonth) >= ( TUCHAR )aDI) ) break;
         if( (ptGMTDateTime->tDate.ucDayOfWeek == ( TUCHAR )aDI) && (ptGMTDateTime->tTime.ucHour < ( TUCHAR )aCHANGING_HOUR) ) break;
         bSummerTime = ( TBOOL )aTRUE;
        }
       break;
      }
     // On vérifie que l'on est vers la fin du mois
     // On regarde si l'on est le dernier dimanche du mois
     if( ( ucRestOfDaysInMonth = (tucDayOfMonth[ ( TUCHAR )( ptGMTDateTime->tDate.ucMonth - 1 ) ] - ptGMTDateTime->tDate.ucDay) ) < ( TUCHAR )7 )
      {
       if( (ptGMTDateTime->tDate.ucDayOfWeek != ( TUCHAR )aDI) && ((ptGMTDateTime->tDate.ucDayOfWeek + ucRestOfDaysInMonth) >= ( TUCHAR )aDI) )
        { bSummerTime = ( TBOOL )aTRUE; break; }
       if( (ptGMTDateTime->tDate.ucDayOfWeek == ( TUCHAR )aDI) && (ptGMTDateTime->tTime.ucHour < ( TUCHAR )aCHANGING_HOUR) )
        { bSummerTime = ( TBOOL )aTRUE; break; }
      }
     else bSummerTime = ( TBOOL )aTRUE;
    }
   // Fin de l'exception
   while( aFALSE );
  }
 tLocalTime.ucSecond = tLocalTime.ucMinute = 0;
 tLocalTime.ucHour = ( TUCHAR )aWINTER_TIME;
 if( bSummerTime ) tLocalTime.ucHour = ( TUCHAR )aSUMMER_TIME;
 // On incrémente de ce qu(il faut
 bFDATETIMEAddTime( ptLocalDateTime, &tLocalTime );
}
#endif

//------------------------------------------------------------------------------
/// \brief Donne la date et l'heure GMT en foncton de l'heure locale
/// \param ptDateTime : Pointeur sur un objet TDateTimeObject (Destination)
/// \return ptGMTDateTime : Pointeur sur l'heure et la date GMT
//------------------------------------------------------------------------------
#if __vFDATETIMEGetGMTTime__ && aDATETIME_DAY_OF_WEEK && aSUMMER_WINTER_DATETIME
void vFDATETIMEGetGMTTime( TDateTimeObject * ptGMTDateTime, TCONST TDateTimeObject * ptLocalDateTime )
{
 TUCHAR ucRestOfDaysInMonth;
 TBOOL bSummerTime = ( TBOOL )aFALSE;
 TTimeObject tLocalTime;
 
 // On copie les dates
 *ptGMTDateTime = *ptLocalDateTime;
 // On vérifie les bornes ( l'été )
 if( (ptLocalDateTime->tDate.ucMonth >= ( TUCHAR )aSUMMER_MONTH) && (ptLocalDateTime->tDate.ucMonth <= ( TUCHAR )aWINTER_MONTH) )
  {
   // Gestion d'une exception simple
   do
    {
     // Référence c'est l'été
     if( ptLocalDateTime->tDate.ucMonth < ( TUCHAR )aWINTER_MONTH )
      {
       if( ptLocalDateTime->tDate.ucMonth > ( TUCHAR )aSUMMER_MONTH ) { bSummerTime = ( TBOOL )aTRUE; break; }
       // On vérifie que l'on est vers la fin du mois
       // On regarde si l'on est le dernier dimanche du mois
       if( ( ucRestOfDaysInMonth = (tucDayOfMonth[ ( TUCHAR )( ptLocalDateTime->tDate.ucMonth - 1 ) ] - ptLocalDateTime->tDate.ucDay) ) < ( TUCHAR )7 )
        {
         if( (ptLocalDateTime->tDate.ucDayOfWeek != ( TUCHAR )aDI) && ((ptLocalDateTime->tDate.ucDayOfWeek + ucRestOfDaysInMonth) >= ( TUCHAR )aDI) ) break;
         if( (ptLocalDateTime->tDate.ucDayOfWeek == ( TUCHAR )aDI) && ((ptLocalDateTime->tTime.ucHour - ( TUCHAR )aSUMMER_TIME) < ( TUCHAR )aCHANGING_HOUR) ) break;
         bSummerTime = ( TBOOL )aTRUE;
        }
       break;
      }
     // On vérifie que l'on est vers la fin du mois
     // On regarde si l'on est le dernier dimanche du mois
     if( ( ucRestOfDaysInMonth = (tucDayOfMonth[ ( TUCHAR )( ptLocalDateTime->tDate.ucMonth - 1 ) ] - ptLocalDateTime->tDate.ucDay) ) < ( TUCHAR )7 )
      {
       if( (ptLocalDateTime->tDate.ucDayOfWeek != ( TUCHAR )aDI) && ((ptLocalDateTime->tDate.ucDayOfWeek + ucRestOfDaysInMonth) >= ( TUCHAR )aDI) )
        { bSummerTime = ( TBOOL )aTRUE; break; }
       if( (ptLocalDateTime->tDate.ucDayOfWeek == ( TUCHAR )aDI) && ((ptLocalDateTime->tTime.ucHour - ( TUCHAR )aWINTER_TIME) < ( TUCHAR )aCHANGING_HOUR) )
        { bSummerTime = ( TBOOL )aTRUE; break; }
      }
     else bSummerTime = ( TBOOL )aTRUE;
    }
   // Fin de l'exception
   while( aFALSE );
  }
 tLocalTime.ucSecond = tLocalTime.ucMinute = 0;
 tLocalTime.ucHour = ( TUCHAR )aWINTER_TIME;
 if( bSummerTime ) tLocalTime.ucHour = ( TUCHAR )aSUMMER_TIME;
 // On incrémente de ce qu(il faut
 bFDATETIMEDecTime( ptGMTDateTime, &tLocalTime );
}
#endif

//------------------------------------------------------------------------------
/// \brief On initialise un objet Date   Time avec une chaîne (Ne gère pas le jour de la semaine)
/// Format "HH:MM:SS JJ/MM/AA"
/// \param ptDateTime : Pointeur sur un objet TDateTimeObject (Destination)
/// \param pcStr      : Chaîne contenant la Date et l'Heure
/// \return Retourne aTRUE si la convertion est possible aFALSE sinon
//------------------------------------------------------------------------------
#if __bFStrToDateTime__
TBOOL bFStrToDateTime( TDateTimeObject * ptDateTime, TCONST TCHAR far * pcStr )
{
 // ICI Le "int" est obligatoire car dépend de la plateforme 
 unsigned int uiSecond, uiMinute, uiHour, uiDay, uiMonth, uiYear;

 // Conversion
 if( sscanf( pcStr, aDATE_TIME_STR_FORMAT, aDATE_TIME_STR_PARAMETER_LIST ) == 
            ( sizeof( uiHour ) + sizeof( uiMinute ) + sizeof( uiSecond ) + sizeof( uiDay ) + sizeof( uiMonth ) + sizeof( uiYear ) ) / sizeof( unsigned int ) )
  {
   // Test d'intégrité de l'heure
   if(   ( uiHour   > ( unsigned int )23 ) 
      || ( uiMinute > ( unsigned int )59 ) 
      || ( uiSecond > ( unsigned int )59 ) ) return( ( TBOOL )aFALSE );                                                                                                                                            
   // Test d'intégrité de la date
   if(   !uiMonth 
      || ( uiMonth > ( unsigned int )aDECE ) 
      || !uiDay 
      || (   ( uiDay > tucDayOfMonth[ ( TUCHAR )( uiMonth - 1 ) ] ) 
          && (   ( uiMonth != ( unsigned int )aFEVR ) 
              || ( uiYear  %  ( unsigned int )4     ) 
              || !( uiYear %  ( unsigned int )100   ) 
              || ( ( uiDay - 1 ) > tucDayOfMonth[ ( TUCHAR )( uiMonth - 1 ) ] ) ) ) )
    // Erreur
    return( ( TBOOL )aFALSE );
   // On recopie les champs
   ptDateTime->tTime.ucHour   = ( TUCHAR )uiHour;
   ptDateTime->tTime.ucMinute = ( TUCHAR )uiMinute;
   ptDateTime->tTime.ucSecond = ( TUCHAR )uiSecond;
   ptDateTime->tDate.ucDay    = ( TUCHAR )uiDay;
   ptDateTime->tDate.ucMonth  = ( TUCHAR )uiMonth;
   ptDateTime->tDate.ucYear   = ( TUCHAR )uiYear;
   #if( aDATETIME_DAY_OF_WEEK )
    // Si gestion du jour de la semaine
    ptDateTime->tDate.ucDayOfWeek = ucFDayOfWeek( &ptDateTime->tDate );
   #endif
   #if( aDATETIME_SECOND_SINCE_2000 )
    // On affecte le résultat
    uliFDATETIMEDateTimeToSec( ptDateTime );
   #endif
   return( ( TBOOL )aTRUE );
  }
 // Erreur
 return( ( TBOOL )aFALSE );
}
#endif

//------------------------------------------------------------------------------
/// \brief On initialise une chaîne avec un DateTime (Ne gère pas le jour de la semaine)
/// Format "HH:MM:SS JJ/MM/AA"
/// \param pcStr      : Chaîne récupérant la Date et l'Heure (Destination)
/// \param ptDateTime : Pointeur sur un objet TDateTimeObject
/// \return Retourne aTRUE si la convertion est possible aFALSE sinon
//------------------------------------------------------------------------------
#if( __bFDateTimeToStr__ )
TBOOL bFDateTimeToStr( TCHAR * pcStr, TCONST TDateTimeObject * ptDateTime )
{
 // Conversion
 return( ( TBOOL )( sprintf( ( TCHAR * )pcStr, aDATE_TIME_STR_FORMAT, aDATE_TIME_STR_OBJECT_LIST ) == ( sizeof( aDATE_TIME_STR_OUTPUT ) - 1 ) ) );
}
#endif

//------------------------------------------------------------------------------
/// \brief Mise à jour de la date à partir du nombre de secondes depuis 2000
/// \param ptDateTime : Pointeur sur un objet TDateTimeObject (Destination)
/// \param uliSecond  : Nombre de seconde depuis l'an 2000
/// \return Aucun
//------------------------------------------------------------------------------
#if( __vFDATETIMESecToDateTime__ )
void vFDATETIMESecToDateTime( TDateTimeObject * ptDateTime , TULONGINT uliSecond )
{
 TULONGINT uliDays, uliSecondsOfTheDay;
 TUINT uiDayMinutes, uiOneYear;
 TUCHAR ucSECCpt, ucDaysInMonth, ucYear;
 
 // On analyse le résultat
 // On récupère le nombre de seconds du jour
 uliSecondsOfTheDay = uliSecond % ( TULONGINT )86400;
 // On affecte à TClock les secondes
 ptDateTime->tTime.ucSecond = uliSecondsOfTheDay % ( TULONGINT )60;
 // On récupère les minutes du jour
 uiDayMinutes = uliSecondsOfTheDay / ( TULONGINT )60;
 // On affecte à TClock les minutes
 ptDateTime->tTime.ucMinute = uiDayMinutes % ( TUINT )60;
 // Et les Heures ...
 ptDateTime->tTime.ucHour = uiDayMinutes / ( TUINT )60;
 // On recherche l'année en cours en partant de 2000
 ucYear = 0;
 // On récupère le nombre de jour (même var que uliSeconds)
 uliDays = uliSecond / ( TULONGINT )86400;
 // On boucle tant que vrai
 while( ( TBOOL )aTRUE )
  {
   uiOneYear = ( TUINT )365;
   // On regarde s'il s'agit d'une année bisextile
   if( bMIS_BISSEXTILE( ucYear ) )
    // On rajoute un jour
    uiOneYear++;
   // On décrémente du nombre de jours
   if( uiOneYear > uliDays ) break;
   // On enlève les jours utilisés
   uliDays -= uiOneYear;
   // On incrémente l'année
   ucYear++;
  }
 // Ici on a l'année et on affecte TClock
 ptDateTime->tDate.ucYear = ucYear;
 // On cherche le mois
 for( ucSECCpt = 0; ucSECCpt < sizeof(tucDayOfMonth); ucSECCpt++ )
  {
   // On prend le nombre de jour du mois
   ucDaysInMonth = tucDayOfMonth[ ucSECCpt ] - ( TUCHAR )1;
   // Le cas de l'année bisextile
   if( ( ucSECCpt == ( TUCHAR )( aFEVR - 1 ) ) && ( uiOneYear == ( TUINT )366 ) )
    // pour février les années bissextiles on ajoutes un jour
    ucDaysInMonth++;
   // On cherche si le nombre de jour à atteindre est > au nombre de jour du mois en cours
   if( uliDays <= ucDaysInMonth ) break;
   // On soutrait le nombre de jour du mois
   uliDays -= ucDaysInMonth + ( TUCHAR )1;
  }
 // On affecte à DateTime le mois pour Janvier (1)
 ptDateTime->tDate.ucMonth = ucSECCpt + ( TUCHAR )1;
 // Et les jours + 1 pour le 1er
 ptDateTime->tDate.ucDay = uliDays + ( TUCHAR )1;
 // On récupère le jour de la semaine
 #if aDATETIME_DAY_OF_WEEK
  ptDateTime->tDate.ucDayOfWeek = ucFDayOfWeek( &ptDateTime->tDate );
 #endif
 #if aDATETIME_SECOND_SINCE_2000
  // On affecte le résultat
 ptDateTime->uliSecond = uliSecond;
 #endif
}
#endif

//------------------------------------------------------------------------------
/// \brief Mise à jour du nombre de secondes depuis 2000 à partir de la date
/// \param ptDateTime : Pointeur sur un objet TDateTimeObject
/// \return Le nombre de seconde depuis l'an 2000
//------------------------------------------------------------------------------
#if __uliFDATETIMEDateTimeToSec__
TULONGINT uliFDATETIMEDateTimeToSec( TDateTimeObject * ptDateTime )
{
 register TULONGINT uliSecond;

 // Test d'intégrité
 if( !bFDATETIMEIsValid( ptDateTime ) ) return( ( TULONGINT )0 );
 //secondes le nombre de secondes depuis 2000
 uliSecond = ( ptDateTime->tDate.ucYear * ( TULONGINT )365 + ( ( TUCHAR )1 + ptDateTime->tDate.ucYear / ( TUINT )4 ) - ( ptDateTime->tDate.ucYear / ( TUINT )100 ) ) * ( TULONGINT )24 * ( TULONGINT )60 * ( TULONGINT )60;
 //ajout des jours de l'année en cours
 uliSecond += tuliSecInYear[ ptDateTime->tDate.ucMonth - ( TUCHAR )1 ];
 // On regarde si l'année est bissextile
 if( ptDateTime->tDate.ucMonth <= ( TUCHAR )aFEVR )
  {
   // si l'année est bisextile le mois de février à 29 Jours
   if( bMIS_BISSEXTILE( ptDateTime->tDate.ucYear ) )
    uliSecond -= ( TULONGINT )24 * ( TULONGINT )60 * ( TULONGINT )60;
  }
 // Ajoute les secondes du mois en cours
 uliSecond += ( ptDateTime->tDate.ucDay - ( TUCHAR )1 ) * ( TULONGINT )24 * ( TULONGINT )60 * ( TULONGINT )60;
 uliSecond += ptDateTime->tTime.ucHour   * ( TULONGINT )60 * ( TULONGINT )60;
 uliSecond += ptDateTime->tTime.ucMinute * ( TULONGINT )60;
 uliSecond += ptDateTime->tTime.ucSecond;
 #if aDATETIME_SECOND_SINCE_2000
  // On affecte le résultat
  ptDateTime->uliSecond = uliSecond;
 #endif
 // On retourne le résultat
 return( uliSecond );
}
#endif

#endif

