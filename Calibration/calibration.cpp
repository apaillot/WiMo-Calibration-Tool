//============================================================================//
// Fichier .........: "calibration.cpp"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2020/01/31
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef CALIBRATION_CPP
#define CALIBRATION_CPP

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include "calibration.h"
#include "WIMOClientModBus.h"
#include <calibpoint.h>
#include <main.h>
#include <WIMOParameters.h>
#include <mainevent.h>
// Qt
#include <QDebug>
#include <QQmlContext>
#include <QDateTime>

//%%AP - 2020.07.10 - Ajout printer
#include <QWidget>
#include <QPrinter>
#include <QTextDocument>
#include <QDir>

// Type nke
#include <TypeC.h>
#include <TypeCMMN.h>

//============================================================================//
// Déclaration des constantes
//============================================================================//

//============================================================================//
// Déclaration des prototypes de fonctions non exportées
//============================================================================//

//============================================================================//
// Déclaration des types
//============================================================================//

//============================================================================//
// Déclaration des variables exportées
//============================================================================//
// Point de calibration
extern CalibPointModel * ptCalibPoint;
// Context de l'application qml
extern QQmlContext * ptContext;

//============================================================================//
// Déclaration des variables non exportées
//============================================================================//

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des prototypes de fonctions exportées
//============================================================================//

//****************************************************************************//
// Le code des fonctions
//****************************************************************************//

//----------------------------------------------------------
// Constructeur de la classe
//----------------------------------------------------------
Calibration::Calibration(QObject *parent)
 : QObject(parent)
{

}

//----------------------------------------------------------
// Nombre de voie
//----------------------------------------------------------
int Calibration::getChannelNumber()
{
 return m_uiChannelNumber;
}
void Calibration::setChannelNumber(int n)
{
 m_uiChannelNumber = n;
 //emit currentChannelChanged(m_uiChannelNumber);
 emit channelNumberChanged();
}

//----------------------------------------------------------
// Voie utilisée
//----------------------------------------------------------
int Calibration::getCurrentChannel()
{
 return m_uiCurrentChannel;
}
void Calibration::setCurrentChannel(int n)
{
 m_uiCurrentChannel = n;
 //emit currentChannelChanged(m_uiCurrentChannel);
 emit currentChannelChanged();
}

//----------------------------------------------------------
// Parameter ID
//----------------------------------------------------------
int Calibration::getCurrentChannelID()
{
 return m_iCurrentChannelID;
}
void Calibration::setCurrentChannelID(int n)
{
 m_iCurrentChannelID = n;
 //emit currentChannelChanged(m_uiCurrentChannel);
 emit currentChannelIDChanged();
}

//----------------------------------------------------------
// Busy state
//----------------------------------------------------------
bool Calibration::getBusyState()
{
 return m_bBusyState;
}
void Calibration::setBusyState(bool n)
{
 m_bBusyState = n;
 //emit currentChannelChanged(m_uiCurrentChannel);
 emit busyStateChanged();
}

//----------------------------------------------------------
// Error state
//----------------------------------------------------------
bool Calibration::getErrorState()
{
 return m_bErrorState;
}
void Calibration::setErrorState(bool n)
{
 m_bErrorState = n;
 emit errorStateChanged();
}

//----------------------------------------------------------
// Channel 0 : Name
//----------------------------------------------------------
QString Calibration::getCh0Name() const
{
 return m_sCh0Name;
}
void Calibration::setCh0Name(const QString &n)
{
 m_sCh0Name = n;
 emit ch0NameChanged( m_sCh0Name );
}
//----------------------------------------------------------
// Channel 0 : Unit
//----------------------------------------------------------
QString Calibration::getCh0Unit() const
{
 return m_sCh0Unit;
}
void Calibration::setCh0Unit(const QString &n)
{
 m_sCh0Unit = n;
 emit ch0UnitChanged( m_sCh0Unit );
}
//----------------------------------------------------------
// Channel 0 : ID
//----------------------------------------------------------
int Calibration::getCh0ID() const
{
 return m_iCh0ID;
}
void Calibration::setCh0ID(int n)
{
 m_iCh0ID = n;
 emit ch0IDChanged( m_iCh0ID );
}
//----------------------------------------------------------
// Channel 0 : Type
//----------------------------------------------------------
QString Calibration::getCh0Type() const
{
 return m_sCh0Type;
}
void Calibration::setCh0Type(const QString &n)
{
 m_sCh0Type = n;
 emit ch0TypeChanged( m_sCh0Type );
}
//----------------------------------------------------------
// Channel 0 : Average
//----------------------------------------------------------
int Calibration::getCh0Average() const
{
 return m_iCh0Average;
}
void Calibration::setCh0Average(int n)
{
 m_iCh0Average = n;
 emit ch0AverageChanged( m_iCh0Average );
}
//----------------------------------------------------------
// Channel 0 : Type
//----------------------------------------------------------
QString Calibration::getCh0Formula() const
{
 return m_sCh0Formula;
}
void Calibration::setCh0Formula(const QString &n)
{
 m_sCh0Formula = n;
 emit ch0FormulaChanged( m_sCh0Formula );
}
//----------------------------------------------------------
// Channel 0 : DateTime
//----------------------------------------------------------
QString Calibration::getCh0DateTime() const
{
 return m_sCh0DateTime;
}
void Calibration::setCh0DateTime(const QString &n)
{
 m_sCh0DateTime = n;
 emit ch0DateTimeChanged( m_sCh0DateTime );
}
/*
//----------------------------------------------------------
// Channel 0 : A1
//----------------------------------------------------------
float Calibration::getCh0A1() const
{
 return m_fCh0A1;
}
void Calibration::setCh0A1(const float n)
{
 m_fCh0A1 = n;
 emit ch0A1Changed( m_fCh0A1 );
}
//----------------------------------------------------------
// Channel 0 : A2
//----------------------------------------------------------
float Calibration::getCh0A2() const
{
 return m_fCh0A2;
}
void Calibration::setCh0A2(const float n)
{
 m_fCh0A2 = n;
 emit ch0A2Changed( m_fCh0A2 );
}
//----------------------------------------------------------
// Channel 0 : A3
//----------------------------------------------------------
float Calibration::getCh0A3() const
{
 return m_fCh0A3;
}
void Calibration::setCh0A3(const float n)
{
 m_fCh0A3 = n;
 emit ch0A3Changed( m_fCh0A3 );
}
//----------------------------------------------------------
// Channel 0 : A3
//----------------------------------------------------------
float Calibration::getRaw20() const
{
 return m_fRaw20;
}
void Calibration::setRaw20(const float n)
{
 m_fRaw20 = n;
 emit raw20Changed( m_fRaw20 );
}
*/
//----------------------------------------------------------
// Channel 0 : m_fRaw1
//----------------------------------------------------------
float Calibration::getCalibTheoRaw1() const
{
 return m_fRaw1;
}
void Calibration::setCalibTheoRaw1(const float n)
{
 m_fRaw1 = n;
 emit calibTheoRaw1Changed( m_fRaw1 );
}
//----------------------------------------------------------
// Channel 0 : m_fRaw2
//----------------------------------------------------------
float Calibration::getCalibTheoRaw2() const
{
 return m_fRaw2;
}
void Calibration::setCalibTheoRaw2(const float n)
{
 m_fRaw2 = n;
 emit calibTheoRaw2Changed( m_fRaw2 );
}
//----------------------------------------------------------
// Channel 0 : m_fPhy1
//----------------------------------------------------------
float Calibration::getCalibTheoPhy1() const
{
 return m_fPhy1;
}
void Calibration::setCalibTheoPhy1(const float n)
{
 m_fPhy1 = n;
 emit calibTheoPhy1Changed( m_fPhy1 );
}
//----------------------------------------------------------
// Channel 0 : m_fPhy2
//----------------------------------------------------------
float Calibration::getCalibTheoPhy2() const
{
 return m_fPhy2;
}
void Calibration::setCalibTheoPhy2(const float n)
{
 m_fPhy2 = n;
 emit calibTheoPhy2Changed( m_fPhy2 );
}

//----------------------------------------------------------
// Channel 1 : Name
//----------------------------------------------------------
QString Calibration::getCh1Name() const
{
 return m_sCh1Name;
}
void Calibration::setCh1Name(const QString &n)
{
 m_sCh1Name = n;
 emit ch1NameChanged( m_sCh1Name );
}
//----------------------------------------------------------
// Channel 1 : Unit
//----------------------------------------------------------
QString Calibration::getCh1Unit() const
{
 return m_sCh1Unit;
}
void Calibration::setCh1Unit(const QString &n)
{
 m_sCh1Unit = n;
 emit ch1UnitChanged( m_sCh1Unit );
}
//----------------------------------------------------------
// Channel 1 : ID
//----------------------------------------------------------
int Calibration::getCh1ID() const
{
 return m_iCh1ID;
}
void Calibration::setCh1ID(int n)
{
 m_iCh1ID = n;
 emit ch1IDChanged( m_iCh1ID );
}
//----------------------------------------------------------
// Channel 1 : Type
//----------------------------------------------------------
QString Calibration::getCh1Type() const
{
 return m_sCh1Type;
}
void Calibration::setCh1Type(const QString &n)
{
 m_sCh1Type = n;
 emit ch1TypeChanged( m_sCh1Type );
}
//----------------------------------------------------------
// Channel 1 : Average
//----------------------------------------------------------
int Calibration::getCh1Average() const
{
 return m_iCh1Average;
}
void Calibration::setCh1Average(int n)
{
 m_iCh1Average = n;
 emit ch1AverageChanged( m_iCh1Average );
}
//----------------------------------------------------------
// Channel 1 : Type
//----------------------------------------------------------
QString Calibration::getCh1Formula() const
{
 return m_sCh1Formula;
}
void Calibration::setCh1Formula(const QString &n)
{
 m_sCh1Formula = n;
 emit ch1FormulaChanged( m_sCh1Formula );
}
//----------------------------------------------------------
// Channel 1 : DateTime
//----------------------------------------------------------
QString Calibration::getCh1DateTime() const
{
 return m_sCh1DateTime;
}
void Calibration::setCh1DateTime(const QString &n)
{
 m_sCh1DateTime = n;
 emit ch1DateTimeChanged( m_sCh1DateTime );
}
//----------------------------------------------------------
// Channel 2 : Name
//----------------------------------------------------------
QString Calibration::getCh2Name() const
{
 return m_sCh2Name;
}
void Calibration::setCh2Name(const QString &n)
{
 m_sCh2Name = n;
 emit ch2NameChanged( m_sCh2Name );
}
//----------------------------------------------------------
// Channel 2 : Unit
//----------------------------------------------------------
QString Calibration::getCh2Unit() const
{
 return m_sCh2Unit;
}
void Calibration::setCh2Unit(const QString &n)
{
 m_sCh2Unit = n;
 emit ch2UnitChanged( m_sCh2Unit );
}
//----------------------------------------------------------
// Channel 2 : ID
//----------------------------------------------------------
int Calibration::getCh2ID() const
{
 return m_iCh2ID;
}
void Calibration::setCh2ID(int n)
{
 m_iCh2ID = n;
 emit ch2IDChanged( m_iCh2ID );
}
//----------------------------------------------------------
// Channel 2 : Type
//----------------------------------------------------------
QString Calibration::getCh2Type() const
{
 return m_sCh2Type;
}
void Calibration::setCh2Type(const QString &n)
{
 m_sCh2Type = n;
 emit ch2TypeChanged( m_sCh2Type );
}
//----------------------------------------------------------
// Channel 2 : Average
//----------------------------------------------------------
int Calibration::getCh2Average() const
{
 return m_iCh2Average;
}
void Calibration::setCh2Average(int n)
{
 m_iCh2Average = n;
 emit ch2AverageChanged( m_iCh2Average );
}
//----------------------------------------------------------
// Channel 2 : Type
//----------------------------------------------------------
QString Calibration::getCh2Formula() const
{
 return m_sCh2Formula;
}
void Calibration::setCh2Formula(const QString &n)
{
 m_sCh2Formula = n;
 emit ch2FormulaChanged( m_sCh2Formula );
}
//----------------------------------------------------------
// Channel 2 : DateTime
//----------------------------------------------------------
QString Calibration::getCh2DateTime() const
{
 return m_sCh2DateTime;
}
void Calibration::setCh2DateTime(const QString &n)
{
 m_sCh2DateTime = n;
 emit ch2DateTimeChanged( m_sCh2DateTime );
}

//----------------------------------------------------------
// Reset des points de calibration dans objet QML
//----------------------------------------------------------
void vFCalibrationResetPointToQmlWithChannel( TUCHAR ucChannel )
{
 CalibPointModel * ptCalibPt;

 // Mise à jour préliminaire du context avec structure vide
 ptContext->setContextProperty("calibPoint", &tCalibPointEmpty);
 // Selon la voie en cours
 switch( ucChannel )
  {
   // Voie 0
   case( ( TUCHAR )0 ): ptCalibPt = &tCalibPointCh0; break;
   // Voie 1
   case( ( TUCHAR )1 ): ptCalibPt = &tCalibPointCh1; break;
   // Voie 2
   case( ( TUCHAR )2 ): ptCalibPt = &tCalibPointCh2; break;
   // Default
   default: return;
  }
 // Mise à jour préliminaire du context avec structure choisi
 ptContext->setContextProperty("calibPoint", ptCalibPt);
}

//----------------------------------------------------------
// Remplissage des points de calibration dans objet QML
//----------------------------------------------------------
void vFCalibrationFillPointToQML( TUINT uiChannel )
{
 TDOUBLE dRaw, dPhy;
 QString sRaw, sPhy;
 TUCHAR ucIndex;
 CalibPointModel * ptCalibPt;

 // Selon la voie en cours
 switch( uiChannel )
  {
   // Voie 0
   case( ( TUINT )0 ): ptCalibPt = &tCalibPointCh0; break;
   // Voie 1
   case( ( TUINT )1 ): ptCalibPt = &tCalibPointCh1; break;
   // Voie 2
   case( ( TUINT )2 ): ptCalibPt = &tCalibPointCh2; break;
   // Default
   default: return;
  }

 // Effacement de tous les points précedent
 ptCalibPt->removeAll();

 // Remplissage des points de calibration
 for( ucIndex = ( TUCHAR )0; ucIndex < 14; ucIndex++ )
  {
   dRaw = ( TDOUBLE )tWIMOParametersSensor.ttChannel[uiChannel].ttPoints[ucIndex].fRaw;
   dPhy = ( TDOUBLE )tWIMOParametersSensor.ttChannel[uiChannel].ttPoints[ucIndex].fPhysical;
   //sRaw.clear();
   sRaw = QString::number(tWIMOParametersSensor.ttChannel[uiChannel].ttPoints[ucIndex].fRaw);
   sPhy = QString::number(tWIMOParametersSensor.ttChannel[uiChannel].ttPoints[ucIndex].fPhysical);

   if( dRaw < ( TDOUBLE )262143 )
    {
     //qDebug("Nombre %lf", dRaw);
     //qDebug("Nombre %lf", dPhy);
     // Si O2
     if(  ( tWIMOParametersSensor.ttChannel[uiChannel].ucParameter == 6 )
      ||  ( tWIMOParametersSensor.ttChannel[uiChannel].ucParameter == 7 )
      /*||  ( tWIMOParametersSensor.ttChannel[uiChannel].ucParameter == 25 )*/ )
      {
       // Inversion d'écriture
       ptCalibPt->addPoint(CalibPoint(ucIndex+1, sRaw, sPhy ));
      }
     else
      {
       ptCalibPt->addPoint(CalibPoint(ucIndex+1, sPhy, sRaw ));
      }
     //ptCalibPt->addPoint(CalibPoint(ucIndex+1, dPhy, dRaw ));
    }
  }
}

//----------------------------------------------------------
// Ajout d'un point de calibration
//----------------------------------------------------------
void vFCalibrationAddPoint( TUINT uiCurrentChannel,
                            TFLOAT fPhysical,
                            TFLOAT fRaw )
{
 TUINT uiIndex1, uiIndex2;
 TWIMOMathPointObject * ptTab = tWIMOParametersSensor.ttChannel[uiCurrentChannel].ttPoints;
 TFLOAT fRawTemp, fPhysicalTemp;

 // Test si on dépasse le nombre de voie de mesure
 if(  ( uiCurrentChannel > ( TUINT )2 )
 // Si on a dejà le maximum de point de calibration
   || ( ptTab[aWIMO_PARAMETERS_LINEAR_POINTS_NUMBER_MAX-1].fRaw < ( TFLOAT )262143 ) )
  // On quitte
  return;

 // Ajout dans le tableau
 for( uiIndex1 = ( TUINT )0; uiIndex1 < ( TUINT )aWIMO_PARAMETERS_LINEAR_POINTS_NUMBER_MAX; uiIndex1++ )
  {
   // Test si pas de point de calibration
   if( ptTab[uiIndex1].fRaw == ( TFLOAT )262143.0 )
    {
     // On insert la nouvelle valeur
     ptTab[uiIndex1].fRaw      = fRaw;
     ptTab[uiIndex1].fPhysical = fPhysical;
     // On sort
     break;
    }
   // Test si pas de point de calibration au-dessus
   if( ptTab[uiIndex1+1].fRaw == ( TFLOAT )262143.0 )
    {
     // On insert la nouvelle valeur
     ptTab[uiIndex1+1].fRaw      = fRaw;
     ptTab[uiIndex1+1].fPhysical = fPhysical;
     // On sort
     break;
    }
  }

 // Trie
 for( uiIndex1 = ( TUINT )0; uiIndex1 < ( TUINT )aWIMO_PARAMETERS_LINEAR_POINTS_NUMBER_MAX; uiIndex1++ )
  {
   for( uiIndex2 = ( TUINT )0; uiIndex2 < ( TUINT )( aWIMO_PARAMETERS_LINEAR_POINTS_NUMBER_MAX - uiIndex1 ); uiIndex2++ )
    {
     //%%AP - 2020.09.15 - Correction de bug validé et fonctionnel
     // Controle de la borne extrême
     if( uiIndex2 == ( TUINT )( aWIMO_PARAMETERS_LINEAR_POINTS_NUMBER_MAX - 1 ) )
      {
       break;
      }
     // Test si on doit déplacer le point
     if( ptTab[uiIndex2].fRaw > ptTab[uiIndex2+1].fRaw )
      {
       fRawTemp      = ptTab[uiIndex2].fRaw;
       fPhysicalTemp = ptTab[uiIndex2].fPhysical;
       ptTab[uiIndex2].fRaw        = ptTab[uiIndex2+1].fRaw;
       ptTab[uiIndex2].fPhysical   = ptTab[uiIndex2+1].fPhysical;
       ptTab[uiIndex2+1].fRaw      = fRawTemp;
       ptTab[uiIndex2+1].fPhysical = fPhysicalTemp;
      }
    }
  }
}

//----------------------------------------------------------
// Suppression d'un point de calibration
//----------------------------------------------------------
static void vFCalibrationRemovePoint( unsigned int uiCurrentChannel, unsigned int uiIndexRemovedRow )
{
 TUINT uiIndex1, uiIndex2;
 TUCHAR ucPointNumber = ( TUCHAR )0;

 if(  ( uiCurrentChannel > ( TUINT )2 )
   || ( uiIndexRemovedRow > ( TUINT )14 ) )
  // On quitte
  return;

 TWIMOMathPointObject * ptTab = tWIMOParametersSensor.ttChannel[uiCurrentChannel].ttPoints;

 for(uiIndex1=0; uiIndex1 < ( TUINT )aWIMO_PARAMETERS_LINEAR_POINTS_NUMBER_MAX; uiIndex1++)
  {
   // Si on est à la valeur d'index de suppression
   if( uiIndex1 == uiIndexRemovedRow )
    {
     for( uiIndex2 = uiIndex1; uiIndex2 < ( TUINT )( aWIMO_PARAMETERS_LINEAR_POINTS_NUMBER_MAX - 1 ); uiIndex2++)
      {
       // Décalage
       ptTab[uiIndex2].fRaw      = ptTab[uiIndex2+1].fRaw;
       ptTab[uiIndex2].fPhysical = ptTab[uiIndex2+1].fPhysical;
      }
     ptTab[uiIndex2].fRaw      = ( TFLOAT )262143;
     ptTab[uiIndex2].fPhysical = ( TFLOAT )0;
     break;
    }
  }

 // Parcourt des points
 for(uiIndex1=0; uiIndex1 < ( TUINT )aWIMO_PARAMETERS_LINEAR_POINTS_NUMBER_MAX; uiIndex1++)
  {
   if( ptTab[uiIndex1].fRaw == ( TFLOAT )262143 ) break;
   ucPointNumber++;
  }

 // Selon le calibration
 switch( tWIMOParametersSensor.ttChannel[uiCurrentChannel].cCalibration )
  {
   //--------------
   // Multipoint
   //--------------
   case( 'M' ):
    // Si moins de 1 point
    if( ucPointNumber < ( TUCHAR )1 )
     // On force la calibration en None
     tWIMOParametersSensor.ttChannel[ uiCurrentChannel ].cCalibration = ( TCHAR )'N';
   break;
   //--------------
   // Linéaire
   //--------------
   case( 'L' ):
    // Si moins de 1 point
    if( ucPointNumber < ( TUCHAR )2 )
     // On force la calibration en None
     tWIMOParametersSensor.ttChannel[ uiCurrentChannel ].cCalibration = ( TCHAR )'N';
   break;
   //--------------
   // 2nd degré
   //--------------
   case( '2' ):
    // Si moins de 1 point
    if( ucPointNumber < ( TUCHAR )3 )
     // On force la calibration en None
     tWIMOParametersSensor.ttChannel[ uiCurrentChannel ].cCalibration = ( TCHAR )'N';
   break;
   //--------------
   // 3rd degré
   //--------------
   case( '3' ):
    // Si moins de 1 point
    if( ucPointNumber < ( TUCHAR )4 )
     // On force la calibration en None
     tWIMOParametersSensor.ttChannel[ uiCurrentChannel ].cCalibration = ( TCHAR )'N';
   break;
   // Autre calibration :
   // on ne fait rien
   default:
   break;
  }
}

//----------------------------------------------------------
// Suppression d'un point de calibration
//----------------------------------------------------------
void Calibration::removeCalibPoint( int iIndex, int uiChannel )
{
 qDebug("Calibration::removeCalibPoint");
 qDebug() << "iIndex : " << iIndex;
 qDebug() << "uiChannel : " << uiChannel;

 // Suppression du point
 //ptCalibPoint->removePoint(iIndex);

 // Selon la voie en cours
 switch( uiChannel )
  {
   // Voie 0
   case 0:
    qDebug("Index 0");
    // On met à jour le contexte
    tCalibPointCh0.removePoint(iIndex);
   break;
   // Voie 1
   case 1:
    // On met à jour le contexte
    tCalibPointCh1.removePoint(iIndex);
   break;
   // Voie 2
   case 2:
    // On met à jour le contexte
    tCalibPointCh2.removePoint(iIndex);
   break;
   default:break;
  }

 // On met à jour la structure de configuration
 vFCalibrationRemovePoint( uiChannel, ( TUINT )iIndex );
}

//----------------------------------------------------------
// Suppression d'un point de calibration
//----------------------------------------------------------
static void vFCalibrationRemoveAllPoint( int uiCurrentChannel )
{
 TWIMOMathPointObject * ptTab = tWIMOParametersSensor.ttChannel[uiCurrentChannel].ttPoints;
 TUINT uiIndex1;

 for( uiIndex1 = ( TUINT )0; uiIndex1 < ( TUINT )aWIMO_PARAMETERS_LINEAR_POINTS_NUMBER_MAX; uiIndex1++ )
  {
   ptTab[uiIndex1].fRaw      = ( TFLOAT )262143;
   ptTab[uiIndex1].fPhysical = ( TFLOAT )0;
  }
}

//----------------------------------------------------------
// Suppression d'un point de calibration
//----------------------------------------------------------
void Calibration::removeAllCalibPoint( void )
{
 qDebug("removeAllCalibPoint");
 // Suppression du point
 ptCalibPoint->removeAll();

 // Selon la voie en cours
 switch( m_uiCurrentChannel )
  {
   // Voie 0
   case 0:
    qDebug("Index 0");
    // On met à jour le contexte
    tCalibPointCh0.removeAll();
   break;
   // Voie 1
   case 1:
    // On met à jour le contexte
    tCalibPointCh1.removeAll();
   break;
   // Voie 2
   case 2:
    // On met à jour le contexte
    tCalibPointCh2.removeAll();
   break;
   // Default
   default:
   break;
  }

 // On met à jour la structure de configuration
 vFCalibrationRemoveAllPoint( m_uiCurrentChannel );
 // On force la calibration en None
 tWIMOParametersSensor.ttChannel[ m_uiCurrentChannel ].cCalibration = ( TCHAR )'N';

 //%%AP - 2020.09.09 - Suppression car fait après
 // Ecriture capteur
 //vFWIMOModbusSaveChannelToSensor( ( TUCHAR )m_uiCurrentChannel );
 // Mise à jour préliminaire du context avec structure vide
 ptContext->setContextProperty("calibPoint", &tCalibPointEmpty);
 // Selon la voie en cours
 switch( m_uiCurrentChannel )
  {
   // Voie 0
   case 0:
    qDebug("Index 0");
    // On met à jour le contexte
    ptContext->setContextProperty("calibPoint", &tCalibPointCh0);
   break;
   // Voie 1
   case 1:
    // On met à jour le contexte
    ptContext->setContextProperty("calibPoint", &tCalibPointCh1);
   break;
   // Voie 2
   case 2:
    // On met à jour le contexte
    ptContext->setContextProperty("calibPoint", &tCalibPointCh2);
   break;
  }
}

//----------------------------------------------------------
// Mise à jour du tableau des points de calibration
//----------------------------------------------------------
void Calibration::updateCalibPoint( void )
{
 qDebug("updateCalibPoint: %d", m_uiCurrentChannel);

 // Réinit avec un vierge
 ptContext->setContextProperty("calibPoint", &tCalibPointEmpty);
/*
 ptContext->setContextProperty("calibPoint", ptCalibPoint);
 ptContext->setContextProperty("calibPointCh0", &tCalibPointCh0);
 ptContext->setContextProperty("calibPointCh1", &tCalibPointCh1);
 ptContext->setContextProperty("calibPointCh2", &tCalibPointCh2);
*/

 // Selon la voie en cours
 switch( m_uiCurrentChannel )
  {
   // Voie 0
   case 0:
    qDebug("Index 0");
    // On met à jour le contexte
    ptContext->setContextProperty("calibPoint", &tCalibPointCh0);
   break;
   // Voie 1
   case 1:
    // On met à jour le contexte
    ptContext->setContextProperty("calibPoint", &tCalibPointCh1);
   break;
   // Voie 2
   case 2:
    // On met à jour le contexte
    ptContext->setContextProperty("calibPoint", &tCalibPointCh2);
   break;
  }
}

//----------------------------------------------------------
// Mise à jour du tableau des points de calibration
//----------------------------------------------------------
Q_INVOKABLE void Calibration::submitCalibSettings( unsigned int  uiChannelSettings,
                                                   QString       sUnit,
                                                   unsigned char ucCalibType,
                                                   unsigned int  uiAverage )
{
 qDebug("==Calibration::submitCalibSettings==");
 qDebug("uiChannelSettings = %d",uiChannelSettings);
 qDebug("ucCalibTYpe = %s ",sUnit.toLatin1().data());
 qDebug("ucCalibTYpe = %c ",ucCalibType);
 qDebug("uiAverage = %d",uiAverage);
 // Test d'intégrité
 if(  ( ucCalibType > 128 )
   || ( uiAverage == 0    )
   || ( uiAverage > 1000  )
   || ( uiChannelSettings > 2 )
   || ( sUnit.length() > 15 ) ) return;

 // Mise à jour des variables de la structure
 memcpy( tWIMOParametersSensor.ttChannel[uiChannelSettings].tcUnit, sUnit.toLatin1().data(), sUnit.length() );
 tWIMOParametersSensor.ttChannel[uiChannelSettings].tcUnit[sUnit.length()] = ( char )'\0';
 tWIMOParametersSensor.ttChannel[uiChannelSettings].cCalibration = ( char )ucCalibType;
 tWIMOParametersSensor.ttChannel[uiChannelSettings].uiAverage    = ( unsigned short int )uiAverage;
 // Ecriture capteur
 //vFWIMOModbusSaveChannelToSensor( uiChannelSettings );
 //vFWIMOModbusSaveConfigToSensor();
}
//----------------------------------------------------------
// Mise à jour des coefficients de compensation pour CT
//----------------------------------------------------------
Q_INVOKABLE void Calibration::submitCTCompensationFactor( float fY1,
                                                          float fY2,
                                                          float fX1,
                                                          float fX2 )
{
 qDebug("==Calibration::submitCTCompensationFactor==");

 tWIMOParametersSensor.ttChannel[0].ttPointTheo[0].fPhysical = fY1;
 tWIMOParametersSensor.ttChannel[0].ttPointTheo[1].fPhysical = fY2;
 tWIMOParametersSensor.ttChannel[0].ttPointTheo[0].fRaw      = fX1;
 tWIMOParametersSensor.ttChannel[0].ttPointTheo[1].fRaw      = fX2;
}

//----------------------------------------------------------
// Ajout d'un point de calibration
//----------------------------------------------------------
Q_INVOKABLE void Calibration::addCalibPoint( unsigned int uiChannel,
                                             float fPhy,
                                             float fRaw )
{
 qDebug("==Calibration::addCalibPoint==");
 qDebug("uiChannel = %d",uiChannel);

//%%AP - 2021.10.27 - Remis les points
 //%%AP - 2020.11.30 - Ajout d'une exception pour l'O2
 if(  ( tWIMOParametersSensor.ttChannel[uiChannel].ucParameter == ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_OXYGEN_CONCENTRATION )  // S06
   || ( tWIMOParametersSensor.ttChannel[uiChannel].ucParameter == ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_OXYGEN_SATURATION    )  // S07
   /*|| ( tWIMOParametersSensor.ttChannel[uiChannel].ucParameter == ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_S25_O2_TEMPERATURE   )*/ ) // S25
  {
   // Ajout des points dans la structure avec trie préalable
   vFCalibrationAddPoint( ( TUCHAR )uiChannel, fRaw, fPhy );
  }
 else
  {
   // Ajout des points dans la structure avec trie préalable
   vFCalibrationAddPoint( ( TUCHAR )uiChannel, fPhy, fRaw );
  }

 // Remplissage des points de calibration - voie
 vFCalibrationFillPointToQML( ( TUCHAR )uiChannel );
 // Reset point de calibration
 vFCalibrationResetPointToQmlWithChannel( ( TUCHAR )uiChannel );
}

//----------------------------------------------------------------------------//
// Sauvegarde du datetime de la calibration
//----------------------------------------------------------------------------//
Q_INVOKABLE QString Calibration::sSaveCurrentCalibrationDateTime( unsigned int uiChannel )
{
 QDateTime qDateTime;
 TULONGINT uliDateTime;
 TDateTimeObject tDateTime;
 //TCHAR m_tcSaveDateTime[sizeof( "HH:MM:SS JJ/MM/AA" ) + 2 + 1];

 // Test d'intégrité
 if( uiChannel > ( TUINT )3 ) return( "" );

 // Get du datetime courant
 qDateTime = QDateTime::currentDateTime();
 // Conversion au format datetime nke
 tDateTime.tDate.ucYear   = ( TUCHAR )( qDateTime.date().year() - ( TUINT )2000 );
 tDateTime.tDate.ucMonth  = ( TUCHAR )qDateTime.date().month();
 tDateTime.tDate.ucDay    = ( TUCHAR )qDateTime.date().day();
 tDateTime.tTime.ucHour   = ( TUCHAR )qDateTime.time().hour();
 tDateTime.tTime.ucMinute = ( TUCHAR )qDateTime.time().minute();
 tDateTime.tTime.ucSecond = ( TUCHAR )qDateTime.time().second();
 // Récupération du résultat
 uliDateTime = uliFDATETIMEDateTimeToSec( &tDateTime );
 // Sauvegarde du datetime dans la structure capteur
 tWIMOParametersSensor.ttChannel[uiChannel].uliDateTimeCalibration = uliDateTime;

 // Récupération du DateTime au format string
 vFWIMOModbusGetCalibrationDateTime( ( TUCHAR )uiChannel, m_tcSaveDateTime );

 this->m_sSaveDateTime.clear();
 this->m_sSaveDateTime.append( m_tcSaveDateTime );
 // Mise à jour dans la variable affichée
 ptDataSensorCh[uiChannel]->setCalibrationDate(m_tcSaveDateTime);

 // Retour du résultat
 return( m_tcSaveDateTime );
}

//-----------------------------------------------------
// Retourne le nom de la calibration par rapport au calibration type CPP
//-----------------------------------------------------
static QString sFWIMOCalibrationGetCalibTypeString( TCHAR cCalibType )
{
 QString sReturnCalibrationType;

 // Selon le type de calibration de la voie selectionnée
 switch( cCalibType )
  {
   // N : None
   case( ( TCHAR )'N' ):
    sReturnCalibrationType = "None";
   break;
   // M : Multipoint
   case( ( TCHAR )'M' ):
    // Si O2 le terme corrected sera utilisé
    if(  ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )6 )
      || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )7 ) )
     sReturnCalibrationType = "Corrected";
    else
     sReturnCalibrationType = "Multipoint";
   break;
   // S : Steinhart
   case( ( TCHAR )'S' ):
    sReturnCalibrationType = "Steinhart";
   break;
   // B : Bennet
   case( ( TCHAR )'B' ):
    sReturnCalibrationType = "Bennet";
   break;
   // L : Linear
   case( ( TCHAR )'L' ):
    sReturnCalibrationType = "Linear";
   break;
   // 2 : 2nd degree polynomial
   case( ( TCHAR )'2' ):
    sReturnCalibrationType = "2nd degree polynomial";
   break;
   // 3 : 3rd degree polynomial
   case( ( TCHAR )'3' ):
    sReturnCalibrationType = "3rd degree polynomial";
   break;
   default:
    sReturnCalibrationType = "None";
  }
 return sReturnCalibrationType;
}

//----------------------------------------------------------------------------//
// Test si le rapport de calibration existe déjà
//----------------------------------------------------------------------------//
Q_INVOKABLE bool Calibration::bFIsCalibrationReportExist( QString sFilenamePath )
{
 //qDebug() << QFile::exists(sFilenamePath);
 // Retour
 return( QFile::exists( sFilenamePath ) );
}

//----------------------------------------------------------------------------//
// Génération du nom du rapport de calibration
//----------------------------------------------------------------------------//
Q_INVOKABLE QString Calibration::sFGenerateCalibReportFilename( void )
{
 QString sFilename;
 char tcBuffer[ 64 ]={0};
 QDateTime qDateTime;

 // Get du datetime courant
 qDateTime = QDateTime::currentDateTime();

 snprintf( tcBuffer,
           sizeof( tcBuffer ),
           "Sensor WiMo %s %02d %05d_Calibration report %d-%02d-%02d",
           tWIMOParametersSensor.tHeader.tProduct.tcIdentifier,
           tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY,
           tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN,
           qDateTime.date().year(),
           qDateTime.date().month(),
           qDateTime.date().day() );

 sFilename.append( tcBuffer );

 // Retour
 return( sFilename );
}

//----------------------------------------------------------------------------//
// Test de l'état du grafcet modbus
//----------------------------------------------------------------------------//
/*
Q_INVOKABLE bool Calibration::isModbusBusyState( void )
{
 // Retourne
 return( bIsWIMOModbusGrafcetBusy() );
}
*/

//----------------------------------------------------------------------------//
// Renvoi le dossier courant
//----------------------------------------------------------------------------//
Q_INVOKABLE QString Calibration::getCurrentPathFile( void )
{
 QDir dir("../Pdf");

 QString sPath = dir.absolutePath();

 qDebug( sPath.toLatin1().data() );
 return( sPath );
}

//----------------------------------------------------------------------------//
// Sauvegarde de la calibration au format PDF
//----------------------------------------------------------------------------//
Q_INVOKABLE void Calibration::exportCalibrationToPdf( QString sFilePath )
{
 QString sDoc;
 char tcBuffer[ 1024 ]={0};
 char tcIdentifier[ 64 ]={0};
 QDateTime qDateTime;
 char tcDateTime[32] = {0};
 TUCHAR ucCpt;

 QPrinter printer(QPrinter::PrinterResolution);
 printer.setOutputFormat(QPrinter::PdfFormat);
 printer.setPaperSize(QPrinter::A4);
 //printer.setOutputFileName("test_qt.pdf");
 printer.setOutputFileName( sFilePath );
 printer.setPageMargins(15,8,15,8,QPrinter::Millimeter);

 QTextDocument doc;
 //QFile file(":/pdfStylesheet.css");
 QFile file("Models/pdfStylesheet.css");
 if( file.open( QFile::ReadOnly | QFile::Text ) )
  {
   qDebug() << "Opened CSS";
  }
 else
  {
   qDebug() << "Failed to open CSS:" << file.errorString();
  }
 doc.setDefaultStyleSheet(file.readAll());
 file.close();

 //%%AP - 2020.11.05 - Ajout
 //sDoc.append( "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>" );
 //sDoc.append( "<body>" );

 //-------------------
 // Titre
 //-------------------
 //%%AP - 2022.08.25 - Suppression du logo en en-tête
 //sDoc.append("<img src=\"Models/nke-Instrumentation-logo.png\"");
 //sDoc.append("width=\"150\" height=\"71\" align=left/>");
 //sDoc.append("<h1 align=\"center\">Calibration report</h1><br/><hr/>");
 sDoc.append("<h1 align=\"center\">Calibration report</h1><hr/>");

 //-------------------
 // En-tête
 //-------------------
 // Get du datetime courant
 qDateTime = QDateTime::currentDateTime();
 snprintf( tcDateTime,
           sizeof( tcDateTime ),
           "%04d-%02d-%02d %02d:%02d:%02d",
           qDateTime.date().year(), qDateTime.date().month(), qDateTime.date().day(),
           qDateTime.time().hour(), qDateTime.time().minute(), qDateTime.time().second() );

 if(  ( tWIMOParametersSensor.tHeader.tProduct.tcIdentifier[0] != ( TCHAR )0xFF )
   && ( tWIMOParametersSensor.tHeader.tProduct.tcIdentifier[0] != ( TCHAR )'\0' ) )
  {
   strncpy( tcIdentifier,
            tWIMOParametersSensor.tHeader.tProduct.tcIdentifier,
            strlen( tWIMOParametersSensor.tHeader.tProduct.tcIdentifier ) );
  }

 if(  ( tWIMOParametersSensor.tHeader.tProduct.tcVersion[0] != ( TCHAR )0xFF )
   && ( tWIMOParametersSensor.tHeader.tProduct.tcVersion[0] != ( TCHAR )'\0' ) )
  {
   strncpy( &tcIdentifier[ strlen( tcIdentifier ) ], " (", sizeof( TCHAR ) * 2 );
   strncpy( &tcIdentifier[ strlen( tcIdentifier ) ],
            tWIMOParametersSensor.tHeader.tProduct.tcVersion,
            strlen( tWIMOParametersSensor.tHeader.tProduct.tcVersion ) );
  // Si pas de manufacturer
  if(  ( tWIMOParametersSensor.tHeader.tProduct.tcManufacturer[0] == ( TCHAR )0xFF )
    || ( tWIMOParametersSensor.tHeader.tProduct.tcManufacturer[0] == ( TCHAR )'\0' ) )
    {
     strncpy( &tcIdentifier[ strlen( tcIdentifier ) ], ")", sizeof( TCHAR ) );
    }
  }

 if(  ( tWIMOParametersSensor.tHeader.tProduct.tcManufacturer[0] != ( TCHAR )0xFF )
   && ( tWIMOParametersSensor.tHeader.tProduct.tcManufacturer[0] != ( TCHAR )'\0' ) )
  {
   if(  ( tWIMOParametersSensor.tHeader.tProduct.tcVersion[0] != ( TCHAR )0xFF )
     && ( tWIMOParametersSensor.tHeader.tProduct.tcVersion[0] != ( TCHAR )'\0' ) )
    strncpy( &tcIdentifier[ strlen( tcIdentifier ) ], " ", sizeof( TCHAR ) );
   else
    strncpy( &tcIdentifier[ strlen( tcIdentifier ) ], " (", sizeof( TCHAR ) );
   strncpy( &tcIdentifier[ strlen( tcIdentifier ) ],
            tWIMOParametersSensor.tHeader.tProduct.tcManufacturer,
            strlen( tWIMOParametersSensor.tHeader.tProduct.tcManufacturer ) );
   strncpy( &tcIdentifier[ strlen( tcIdentifier ) ], ")", sizeof( TCHAR ) );
  }

 snprintf( tcBuffer,
           sizeof( tcBuffer ),
          "<table>"
           "<tr>"
             "<td><strong>Date: </strong></td>"
             "<td><span>%s</span></td>"
           "</tr>"
           "<tr>"
            "<td><strong>Identifier: </strong></td>"
            "<td><span>%s</span></td>"
           "</tr>"
           "<tr>"
            "<td><strong>Serial number: </strong></td>"
            "<td>%02d-%05d</td>"
           "</tr>"
           "<tr>"
            "<td><strong>Firmware version: </strong></td>"
            "<td>V%d.%d.%d</td>"
           "</tr>"
          "</table>",
          tcDateTime,
          tcIdentifier,
          tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY,
          tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN,
          tSensorVersion.ucMajor,
          tSensorVersion.ucMinor,
          tSensorVersion.uiBuild );
 sDoc.append(tcBuffer);

 sDoc.append("<hr/>");


 //-------------------
 // Voie de calibration
 //-------------------
 TUCHAR ucChannelCpt = ( TUCHAR )0;
 TUCHAR ucPointCpt = ( TUCHAR )0;
 QString sTemp;
 double dRaw, dPhy;

 TCHAR tcBufferDate[sizeof( "HH:MM:SS JJ/MM/AA" ) + 2 + 1];

 // Boucle sur le nombre de voie d'acquisition
 for( ucChannelCpt = ( TUCHAR )0; ucChannelCpt < tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber; ucChannelCpt++ )
  {
   // Si voie > 1
   if( ucChannelCpt > 0 )
    {
     sDoc.append( "<hr/>" );
    }

   memset( tcBuffer, ( TCHAR )'\0', sizeof( tcBuffer ) );
   snprintf( tcBuffer,
            sizeof( tcBuffer ),
            "<div class=\"h3-channel-name\">"
            "<h3 align=center>Channel %d - %s</h3>"
            "</div>",
            ucChannelCpt,
            tWIMOParametersSensor.ttChannel[ucChannelCpt].tcName );
   sDoc.append( tcBuffer );


   snprintf( tcBuffer,
             sizeof( tcBuffer ),
             "<div class=\"channel-info-c\">"
              "<table>"
              "<tr>"
                "<td><strong>Channel name: </strong></td>"
                "<td><span>%s</span></td>"
              "</tr>"
              "<tr>"
               "<td><strong>Unit: </strong></td>"
               "<td><span>%s</span></td>"
              "</tr>"
            ,
            tWIMOParametersSensor.ttChannel[ucChannelCpt].tcName,
            tWIMOParametersSensor.ttChannel[ucChannelCpt].tcUnit );

   //-------------
   // O2 concentration - saturation
   //-------------
   if(  ( tWIMOParametersSensor.ttChannel[ucChannelCpt].ucParameter == ( TUCHAR )6  )
     || ( tWIMOParametersSensor.ttChannel[ucChannelCpt].ucParameter == ( TUCHAR )7  ) )
    {
     //
     //if()
     // Pressure
     snprintf( &tcBuffer[ strlen( tcBuffer ) ],
               sizeof( tcBuffer ),
               "<tr>"
                "<td><strong>Pressure: </strong></td>"
                "<td>%.3f</td>"
               "</tr>",
               tWIMOPyroO2Configuration.uliPressure );

     // Fermeture du tableau
     strncpy( &tcBuffer[ strlen( tcBuffer ) ], "</table></div>", strlen( "</table></div>" ) );
     // Ajout au document
     sDoc.append(tcBuffer);
     sDoc.append( "<h4>Calibration details:</h4>" );
     // Reprise à 0
     memset( tcBuffer, ( TCHAR )'\0', sizeof( tcBuffer ) );


     if( tWIMOParametersSensor.ttChannel[ucChannelCpt].ucParameter == ( TUCHAR )7  )
      {
       sDoc.append( "<p>Same as channel 0.</p>" );
       continue;
      }
     // Sensor code
     snprintf( &tcBuffer[ strlen( tcBuffer ) ],
               sizeof( tcBuffer ),
               "<div class=\"specific-param-c\">"
                "<table>"
                 "<tr>"
                  "<td><strong>Sensor code: </strong></td>"
                  "<td>S%c%d-%u-%u</td>"
                 "</tr>",
               tWIMOPyroO2Configuration.ucIntensity,
               tWIMOPyroO2Configuration.ucAmp,
               ( tWIMOPyroO2Configuration.uliCodeDphi0 / 100 ),
               ( tWIMOPyroO2Configuration.uliCodeDphi100 / 100 ) );
     // Dphi100
     snprintf( &tcBuffer[ strlen( tcBuffer ) ],
               sizeof( tcBuffer ),
               "<tr>"
                "<td><strong>Dphi100: </strong></td>"
                "<td>%.3f</td>"
               "</tr>",
               ( TFLOAT )( ( TFLOAT )tWIMOPyroO2Configuration.uliCodeDphi100 / 1000 ) );
     // Temperature100
     snprintf( &tcBuffer[ strlen( tcBuffer ) ],
               sizeof( tcBuffer ),
               "<tr>"
                "<td><strong>Temperature100: </strong></td>"
                "<td>%.3f</td>"
               "</tr>",
               ( TFLOAT )( ( TFLOAT )tWIMOPyroO2Configuration.liTemperature100 / 1000 ) );
     // Pressure100
     snprintf( &tcBuffer[ strlen( tcBuffer ) ],
               sizeof( tcBuffer ),
               "<tr>"
                "<td><strong>Pressure100: </strong></td>"
                "<td>%.3f</td>"
               "</tr>",
               ( TFLOAT )( ( TFLOAT )tWIMOPyroO2Configuration.uliPressure100 / 1000 ) );
     // Dphi0
     snprintf( &tcBuffer[ strlen( tcBuffer ) ],
               sizeof( tcBuffer ),
               "<tr>"
                "<td><strong>Dphi0: </strong></td>"
                "<td>%.3f</td>"
               "</tr>",
               ( TFLOAT )( ( TFLOAT )tWIMOPyroO2Configuration.uliCodeDphi0 / 1000 ) );
     // Temperature0
     snprintf( &tcBuffer[ strlen( tcBuffer ) ],
               sizeof( tcBuffer ),
               "<tr>"
                "<td><strong>Temperature0: </strong></td>"
                "<td>%.3f</td>"
               "</tr>",
               ( TFLOAT )( ( TFLOAT )tWIMOPyroO2Configuration.liTemperature0 / 1000 ) );
     // Fermeture du tableau
     strncpy( &tcBuffer[ strlen( tcBuffer ) ], "</table></div>", strlen( "</table></div>" ) );
     // Ajout au document
     sDoc.append(tcBuffer);
     // Passage à la voie suivante
     continue;
    }

   // Ch0 - DateTime
   vFWIMOModbusGetCalibrationDateTime( ucChannelCpt, tcBufferDate );
   snprintf( &tcBuffer[ strlen( tcBuffer ) ],
             sizeof( tcBuffer ),
             "<tr>"
              "<td><strong>Calibration date: </strong></td>"
              "<td>%s</td>"
             "</tr>",
            tcBufferDate );

   // Ch0 - Calibration type
   vFWIMOModbusGetCalibrationDateTime( ucChannelCpt, tcBufferDate );
   snprintf( &tcBuffer[ strlen( tcBuffer ) ],
             sizeof( tcBuffer ),
             "<tr>"
              "<td><strong>Calibration type: </strong></td>"
              "<td>%s</td>"
             "</tr>",
            sFWIMOCalibrationGetCalibTypeString( tWIMOParametersSensor.ttChannel[ucChannelCpt].cCalibration ).toLatin1().data() );

   // Ch0 - Formula
   sTemp = sFWIMOModbusFormatCalibrationFormula( ucChannelCpt, true );
   snprintf( &tcBuffer[ strlen( tcBuffer ) ],
             sizeof( tcBuffer ),
             "<tr>"
              "<td><strong>Formula: </strong></td>"
              "<td>%s</td>"
             "</tr>",
            sTemp.toLatin1().data() );
   //-------------
   // O2 Temperature - temperature offset
   //-------------
   if( tWIMOParametersSensor.ttChannel[ucChannelCpt].ucParameter == ( TUCHAR )25  )
    {
     // Temperature offset
     snprintf( &tcBuffer[ strlen( tcBuffer ) ],
               sizeof( tcBuffer ),
               "<tr>"
                "<td><strong>Temperature offset: </strong></td>"
                "<td>%.3f</td>"
               "</tr>",
               ( TFLOAT )( ( TFLOAT )tWIMOPyroO2Configuration.liTemperatureOffset / 1000 ) );
    }

   strncpy( &tcBuffer[ strlen( tcBuffer ) ], "</table></div>", strlen( "</table></div>" ) );
   sDoc.append(tcBuffer);

   //---------------------------------------------
   // Paramètre spécifique
   //---------------------------------------------
   //-------------
   // Tbd / Fluo evosens
   //-------------
   if(  ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )5  )
     || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )13 )
     || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )15 )
     || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )16 )
     || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )17 )
     || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )18 ) )
    {
     memset( tcBuffer, ( TCHAR )'\0', sizeof( tcBuffer ) );
     strncpy( tcBuffer, "<h4>Specific parameters</h4>", strlen( "<h4>Specific parameters</h4>" ) );
     snprintf( &tcBuffer[ strlen( tcBuffer ) ],
               sizeof( tcBuffer ) - strlen( tcBuffer ),
               "<div class=\"specific-param-c\">"
                "<table>"
                 "<tr>"
                  "<td><strong>Integration time: </strong></td>"
                  "<td>%d</td>"
                 "</tr>"
                 "<tr>"
                 "<tr>"
                  "<td><strong>Integration sum: </strong></td>"
                  "<td>%d</td>"
                 "</tr>"
                 "<tr>"
                  "<td><strong>Range: </strong></td>"
                  "<td>%d</td>"
                 "</tr>"
                 "<tr>"
                  "<td><strong>Average samples: </strong></td>"
                  "<td>%d</td>"
                 "</tr>"
                "</table>"
               "</div>",
               tWIMOParametersSensor.tHeader.tSensor.tEvosens.uiIntegrationTime,
               tWIMOParametersSensor.tHeader.tSensor.tEvosens.uiIntegrationSum,
               tWIMOParametersSensor.tHeader.tSensor.tEvosens.ucRange,
               tWIMOParametersSensor.tHeader.tSensor.tEvosens.uiAverageSamples );
     sDoc.append( tcBuffer );
    }
   //-------------
   // pH
   //-------------
   if( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )8 )
    {
     memset( tcBuffer, ( TCHAR )'\0', sizeof( tcBuffer ) );
     strncpy( tcBuffer, "<h4>Specific parameters</h4>", strlen( "<h4>Specific parameters</h4>" ) );
     snprintf( &tcBuffer[ strlen( tcBuffer ) ],
               sizeof( tcBuffer ) - strlen( tcBuffer ),
               "<div class=\"specific-param-c\">"
                "<table>"
                 "<tr>"
                  "<td><strong>Calibration temperature: </strong></td>"
                  "<td>%.2f</td>"
                 "</tr>"
                "</table>"
               "</div>",
               tWIMOParametersSensor.ttChannel[0].tPH.fCalibrationTemperature );
     sDoc.append( tcBuffer );
    }

   //---------------------------------------------
   // Fin paramètre spécifique
   //---------------------------------------------

   // Test s'il y a des points de calibration
   if( tWIMOParametersSensor.ttChannel[ucChannelCpt].ttPoints[0].fRaw >= ( TFLOAT )262143 )
    {
     continue;
    }

   sDoc.append( "<h4>Calibration points:</h4>" );

   sTemp =
   "<div class=\"table-calib-point-c\">"
   "<table class=\"table-calib-point\">"
    "<thead>"
     "<tr>"
      "<th></th>"
      "<th>Standard</th>"
      "<th>Measured</th>"
     "</tr>"
    "</thead>"
    "<tbody>";
   sDoc.append( sTemp );

   // On boucle sur le nombre de points
   for( ucPointCpt = ( TUCHAR )0; ucPointCpt < ( TUCHAR )14; ucPointCpt++ )
    {
     if( tWIMOParametersSensor.ttChannel[ucChannelCpt].ttPoints[ucPointCpt].fRaw >= ( TFLOAT )262143 )
      {
       continue;
      }
     // Raw étendu pour CT
     if(  ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )3 )
       || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )4 )
       || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )6 )
       || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )7 )
       || ( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )25 ) )
      {
       qDebug( "ucPointCpt : %d", ucPointCpt );
       snprintf( tcBuffer,
                 sizeof( tcBuffer ),
                 "<tr>"
                  "<td>%d:</td>"
                  "<td>%.3f</td>"
                  "<td>%.3f</td>"
                 "</tr>",
                 ucPointCpt,
                 tWIMOParametersSensor.ttChannel[ucChannelCpt].ttPoints[ucPointCpt].fPhysical,
                 tWIMOParametersSensor.ttChannel[ucChannelCpt].ttPoints[ucPointCpt].fRaw );
      }
     else
      {
       qDebug( "ucPointCpt : %d", ucPointCpt );
       snprintf( tcBuffer,
                 sizeof( tcBuffer ),
                 "<tr>"
                  "<td>%d:</td>"
                  "<td>%.3f</td>"
                  "<td>%.1f</td>"
                 "</tr>",
                 ucPointCpt,
                 tWIMOParametersSensor.ttChannel[ucChannelCpt].ttPoints[ucPointCpt].fPhysical,
                 tWIMOParametersSensor.ttChannel[ucChannelCpt].ttPoints[ucPointCpt].fRaw );
      }
     // Ajout de la ligne
     sDoc.append( tcBuffer );
    }

   sTemp =
     "</tbody>"
    "</table>"
   "</div>";
   sDoc.append( sTemp );
   sDoc.append( "<p></p>" );
  } // Fin for

 //--------------------
 // Footer
 //--------------------
 QString sVersion = ptMainEvent->sFGetSoftwareVersion();

 sDoc.append("<hr/>");
 memset( tcBuffer, ( TCHAR )'\0', sizeof( tcBuffer ) );
 snprintf( tcBuffer,
          sizeof( tcBuffer ),
          "<div class=\"\">"
          "<h5 align=\"center\">nke Instrumentation - WiMo Calibration Tool - %s</h5>"
          "</div>",
          sVersion.toLatin1().data() );
 sDoc.append( tcBuffer );


 //sDoc.append("<br/>");
 //sDoc.append("<br/>");
 //sDoc.append("<div style=\"display: block;margin-left: auto;margin-right: auto;\">");
 sDoc.append("<p align=\"center\">");
 sDoc.append("<img src=\"Models/nke-Instrumentation-logo.png\" width=\"120\" align=\"center\"/>");
 sDoc.append("</p>");

 //sDoc.append( "</body>" );

 doc.setHtml(sDoc);
 doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
 doc.print(&printer);
}

#endif
