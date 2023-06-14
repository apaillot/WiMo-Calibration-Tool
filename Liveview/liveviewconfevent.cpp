//============================================================================//
// Fichier .........: "calibrationevent.cpp"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2020/04/14
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef LIVEVIEW_CONF_EVENT_CPP
#define LIVEVIEW_CONF_EVENT_CPP

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include "liveviewconfevent.h"
#include "main.h"
#include "WIMOParameters.h"

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
LiveviewConfEvent::LiveviewConfEvent( QObject *parent ): QObject(parent)
{
}

//----------------------------------------------------------
// Constructeur de la classe
//----------------------------------------------------------
void LiveviewConfEvent::displayLiveViewConf( void )
{
 qDebug("***displayLiveViewConf***");

 unsigned int uiAcquisitionTime = tWIMOParametersSensor.tHeader.tSensor.uiAcquisitionTime;
 // Arrondie à la seconde de la valeur d'acquisition time
 uiAcquisitionTime = ( 1000 - ( uiAcquisitionTime % 1000 ) ) + uiAcquisitionTime;

 emit displayLiveViewConfSignal( tWIMOParametersSensor.ttChannel[0].tcName,
                                 tWIMOParametersSensor.ttChannel[1].tcName,
                                 tWIMOParametersSensor.ttChannel[2].tcName,
                                 uiAcquisitionTime,
                                 ( unsigned int )tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber );
}

//----------------------------------------------------------
// Constructeur de la classe
//----------------------------------------------------------
void LiveviewConfEvent::transmitLiveViewData( float fCh0Data,
                                              float fCh1Data,
                                              float fCh2Data )
{
 //unsigned int uiAcquisitionTime = tWIMOParametersSensor.tHeader.tSensor.uiAcquisitionTime;
 // Arrondie à la seconde de la valeur d'acquisition time
 //uiAcquisitionTime = ( 1000 - ( uiAcquisitionTime % 1000 ) ) + uiAcquisitionTime;

 emit transmitLiveViewDataSignal( fCh0Data, fCh1Data, fCh2Data );
}

//----------------------------------------------------------
// FOnction CPP - Déclenchement de la mesure
//----------------------------------------------------------
void LiveviewConfEvent::startMeasure( void )
{
 emit startMeasureSignal();
}

//----------------------------------------------------------
// FOnction CPP - Stop de la mesure
//----------------------------------------------------------
void LiveviewConfEvent::stopMeasure( void )
{
 emit stopMeasureSignal();
}

//----------------------------------------------------------
// Fonction QML - Test si un calibration type est activée
//----------------------------------------------------------
bool LiveviewConfEvent::bFIsCalibrationTypeActivated( unsigned int uiChannel )
{
 qDebug("uiChannel = %d",uiChannel);
 // Test d'intégrité
 if( uiChannel > ( unsigned int )3 ) return( ( bool )false );

 qDebug( "tWIMOParametersSensor.ttChannel[uiChannel].cCalibration %c", tWIMOParametersSensor.ttChannel[uiChannel].cCalibration );

 // Test si le calibration type est activé
 return( ( bool )( tWIMOParametersSensor.ttChannel[uiChannel].cCalibration != ( char )'N' ) );
}


#endif
