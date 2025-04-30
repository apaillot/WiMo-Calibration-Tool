//============================================================================//
// Fichier .........: "calibration.cpp"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2020/01/31
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef WIPER_CPP
#define WIPER_CPP
//============================================================================//
// Inclusion des librairies
//============================================================================//
#include "wiper.h"
#include "WIMOClientModBus.h"
#include <main.h>
#include <WIMOParameters.h>
// Qt
#include <QDebug>
#include <QQmlContext>
#include <QDateTime>


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
Wiper::Wiper(QObject *parent) : QObject(parent)
{

}

//----------------------------------------------------------
// Angle clockwise
//----------------------------------------------------------
int Wiper::getClockwise()
{
 return m_uiClockwise;
}
void Wiper::setClockwise(int n)
{
 m_uiClockwise = n;
 emit clockwiseChanged();
}

//----------------------------------------------------------
// Angle anticlockwise
//----------------------------------------------------------
int Wiper::getAnticlockwise()
{
 return m_uiAnticlockwise;
}
void Wiper::setAnticlockwise(int n)
{
 m_uiAnticlockwise = n;
 emit anticlockwiseChanged();
}

//----------------------------------------------------------
// Angle offset
//----------------------------------------------------------
int Wiper::getOffset()
{
 return m_uiOffset;
}
void Wiper::setOffset(int n)
{
 m_uiOffset = n;
 emit offsetChanged();
}

//----------------------------------------------------------
// Angle min
//----------------------------------------------------------
int Wiper::getMin()
{
 return m_uiMin;
}
void Wiper::setMin(int n)
{
 m_uiMin = n;
 emit minChanged();
}

//----------------------------------------------------------
// Angle max
//----------------------------------------------------------
int Wiper::getMax()
{
 return m_uiMax;
}
void Wiper::setMax(int n)
{
 m_uiMax = n;
 emit maxChanged();
}

//----------------------------------------------------------
// Busy state
//----------------------------------------------------------
bool Wiper::getBusyState()
{
 return m_bBusyState;
}
void Wiper::setBusyState(bool n)
{
 qDebug("setBusyState %d", n);
 m_bBusyState = n;
 //emit currentChannelChanged(m_uiCurrentChannel);
 emit busyStateChanged();
}

//----------------------------------------------------------
// Error state
//----------------------------------------------------------
bool Wiper::getErrorState()
{
 return m_bErrorState;
}
void Wiper::setErrorState(bool n)
{
 m_bErrorState = n;
 emit errorStateChanged();
}

//----------------------------------------------------------
// Mise à jour du tableau des points de calibration
//----------------------------------------------------------
Q_INVOKABLE void Wiper::submitWiperAngle( unsigned int uiClockwise,
                                          unsigned int uiAnticlockwise,
                                          unsigned int uiOffset  )
{
 qDebug("==Wiper::submitWiperAngle==");
 qDebug() << "uiClockwise = %d" << uiClockwise;
 qDebug("uiClockwise = %d",uiClockwise);
 qDebug("uiAnticlockwise = %d",uiAnticlockwise);
 qDebug("uiOffset = %d",uiOffset);
 // Test d'intégrité
 if(  (  ( uiClockwise > 270 )
      || ( uiClockwise < 30  ) )
   && ( uiClockwise != 0       ) ) return;
 if(  (  ( uiAnticlockwise > 270 )
      || ( uiAnticlockwise < 30  ) )
   && ( uiAnticlockwise != 0       ) ) return;
 if( uiOffset > 90 ) return;

 // Mise à jour des variables de la structure
 tWIMOParametersSensor.tHeader.tWiper.uiClockwiseAngle        = ( TUINT )uiClockwise;
 tWIMOParametersSensor.tHeader.tWiper.uiCounterClockwiseAngle = ( TUINT )uiAnticlockwise;
 tWIMOParametersSensor.tHeader.tWiper.uiOffset                = ( TUINT )uiOffset;
}


//----------------------------------------------------------
// Sauvegarde de la configuration d'une voie vers le capteur
//----------------------------------------------------------
Q_INVOKABLE void Wiper::saveConfiguration( void )
{
 qDebug("==saveConfiguration==");
 // Ecriture capteur
 vFWIMOModbusSaveGeneralConfOnlyToSensor();
}

//----------------------------------------------------------
// Start calibration
//----------------------------------------------------------
Q_INVOKABLE void Wiper::startCalibration( void )
{
 qDebug("Déclenchement calibration balai");

 // Appel de la fonction de lancement de la mesure
 vFWIMOModbusStartCalibration();
}

//----------------------------------------------------------
// Start wiper
//----------------------------------------------------------
Q_INVOKABLE void Wiper::startWiper( void )
{
 qDebug("Déclenchement balai");

 // Appel de la fonction de lancement de la mesure
 vFWIMOModbusStartClean();
}


#endif
