//============================================================================//
// Fichier .........: "factoryevent.cpp"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2020/01/28
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef FACTORY_EVENT_CPP
#define FACTORY_EVENT_CPP

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include "factoryevent.h"
#include <WIMOClientModBus.h>
#include <WIMOParameters.h>
#include "main.h"
#include <QFile>
#include <QDir>

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
FactoryEvent::FactoryEvent( QObject *parent ): QObject(parent)
{

}

//----------------------------------------------------------
// Fonction CPP de mise à jour valeur loader
//----------------------------------------------------------
void FactoryEvent::updateLoaderValue( int uiLoaderValue )
{
 qDebug("FactoryEvent::updateLoaderValue");
 m_uiUpdateLoaderValue = uiLoaderValue;
 emit updateLoaderValueSignal( uiLoaderValue );
 qDebug("FactoryEvent::updateLoaderValue END");
}

void FactoryEvent::startUpdate( QString qurl )
{
 qDebug("!!!!!START UPDATE!!!!!!!");
 // Appel de la fonction dédiée
 vFWIMOModbusStartUpdate( qurl );
}

//----------------------------------------------------------
// Fonction CPP de reset
//----------------------------------------------------------
void FactoryEvent::resetLoaderValue( int uiLoaderValue )
{
 m_uiResetLoaderValue = uiLoaderValue;
 emit resetLoaderValueSignal( uiLoaderValue );
}

void FactoryEvent::startReset()
{
 qDebug("!!!!!START RESET!!!!!!!");
 // Appel de la fonction dédiée
 vFWIMOModbusStartReset();
}

//----------------------------------------------------------
// Fonction CPP de sauvegarde de config
//----------------------------------------------------------
void FactoryEvent::saveConfigLoaderValue( int uiLoaderValue )
{
 m_uiSaveConfigLoaderValue = uiLoaderValue;
 emit updateLoaderValue( uiLoaderValue );
}

//----------------------------------------------------------
// Fonction CPP de sauvegarde de config
//----------------------------------------------------------
bool FactoryEvent::bFStartSaveConfig( QString sFileName, QString sPath )
{
 qDebug("!!!!!START SAVE CONFIG!!!!!!!");
 // Appel de la fonction dédiée
 //vFWIMOModbusStartSaveConfig( qurl );
 char tcFileName[ 32 ];
 QFile tFileHandler;
 QDir tDir;

 // Si le nom de fichier n'est pas renseigné
 if( sFileName == "" )
  {
   // Préparation du nom de fichier
   snprintf( tcFileName, sizeof( tcFileName ),
             "%02d%05d_%s.hex",
             tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY,
             tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN,
             tWIMOParametersSensor.tHeader.tProduct.tcIdentifier );
   // Remplacement du nom de fichier
   sFileName = QString( tcFileName );
  }
 // Test si contient un repertoire
 if( sPath != "" )
  {
   // Test si le repertoire existe
   if( !tDir.exists( sPath ) )
    {
     // Création du repertoire
     tDir.mkpath( sPath );
    }
   // Création de l'handler de fichier
   tFileHandler.setFileName( sPath+"/"+sFileName );
  }
 else
  {
   // Création de l'handler de fichier
   tFileHandler.setFileName( sFileName );
  }

 // Suppression si existant
 tFileHandler.remove();
 // Ouverture du fichier *.ini
 if( tFileHandler.open( QIODevice::WriteOnly ) )
  {
   tFileHandler.write( ( char * )&tWIMOParametersSensor, sizeof( tWIMOParametersSensor ) );
   tFileHandler.close();
  }
 else
  {
   qDebug("Error to create config file");
  }
}

//----------------------------------------------------------
// Fonction CPP d'écriture de config
//----------------------------------------------------------
void FactoryEvent::writeConfigLoaderValue( int uiLoaderValue )
{
 m_uiWriteConfigLoaderValue = uiLoaderValue;
 emit writeConfigLoaderValue( uiLoaderValue );
}

void FactoryEvent::startWriteConfig( QString qUrl )
{
 QByteArray qByteArray;

 qDebug("!!!!!START WRITE CONFIG!!!!!!!");
 // Appel de la fonction dédiée
 //vFWIMOModbusStartWriteConfig( qurl );

 // Création de l'handler de fichier
 QFile tFileHandler( qUrl );
 // Suppression si existant
 //tFileHandler.remove();
 // Ouverture du fichier *.ini
 if( tFileHandler.open( QIODevice::ReadOnly ) )
  {
   qByteArray = tFileHandler.readAll();
   //tFileHandler.write( ( char * )&tWIMOParametersSensor, sizeof( tWIMOParametersSensor ) );
   tFileHandler.close();
   // Vérification du nombre d'octet lu
   if( qByteArray.length() == 866 )
    {
     // Ecrasement de la configuration
     memcpy( ( char * )&tWIMOParametersSensor, qByteArray, sizeof( tWIMOParametersSensor ) );
     // Lancement d'une écriture complère de configuration vers le capteur
     //vFWIMOModbusSaveConfigToSensor();
     vFWIMOModbusWriteConfigToSensorFactory();
    }
   else
    {
     qDebug( "qByteArray.length() : %d", qByteArray.length() );
    }
  }
}

//----------------------------------------------------------
// Fonction CPP d'écriture de config
//----------------------------------------------------------
void FactoryEvent::configSettingsDisplay( void )
{
 // Sauvegarde de la config
 emit configSettingsSignal( tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY,
                            tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN );
}
//----------------------------------------------------------
// Fonction CPP d'écriture de config
//----------------------------------------------------------
void FactoryEvent::configSettingsDisplayEvosens( void )
{
 qDebug("configSettingsDisplayEvosens");
 // Sauvegarde de la config
 emit configSettingsDisplayEvosensSignal( tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY,
                                          tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN,
                                          tWIMOParametersSensor.tHeader.tSensor.tEvosens.uiIntegrationTime,
                                          tWIMOParametersSensor.tHeader.tSensor.tEvosens.uiIntegrationSum,
                                          tWIMOParametersSensor.tHeader.tSensor.tEvosens.ucRange,
                                          tWIMOParametersSensor.tHeader.tSensor.tEvosens.uiAverageSamples,
                                          tWIMOParametersSensor.tHeader.tSensor.tEvosens.ucTestMode,
                                          tWIMOParametersSensor.tHeader.tSensor.tEvosens.uiPowerOn );
}
//----------------------------------------------------------
// Fonction CPP : sauvegarde congiuration evosens
//----------------------------------------------------------
QString FactoryEvent::sFSaveConfigSettingsEvosens( int uiSN_Y,
                                                   int uiSN_N,
                                                   int uiIntegrationTime,
                                                   int uiIntegrationSum,
                                                   int uiRange,
                                                   int uiAverageSamples,
                                                   int ucTestMode,
                                                   int uiPowerOn )
{
 qDebug("saveConfigSettingsEvosens");
 // Test SN year
 if(  ( uiSN_Y < 0 )
   || ( uiSN_Y > 255 ) )
  {
   return("Value must be between 0 and 255");
  }
 // Test SN year
 if(  ( uiSN_Y < 0 )
   || ( uiSN_Y > 65535 ) )
  {
   return("Value must be between 0 and 65535");
  }

 // Sauvegarde de la config
 tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY = ( TUCHAR )uiSN_Y;
 tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN = ( TUINT )uiSN_N;
 tWIMOParametersSensor.tHeader.tSensor.tEvosens.uiIntegrationTime = ( TUINT )uiIntegrationTime;
 tWIMOParametersSensor.tHeader.tSensor.tEvosens.uiIntegrationSum  = ( TUINT )uiIntegrationSum;
 tWIMOParametersSensor.tHeader.tSensor.tEvosens.ucRange           = ( TUCHAR )uiRange;
 tWIMOParametersSensor.tHeader.tSensor.tEvosens.uiAverageSamples  = ( TUINT )uiAverageSamples;
 tWIMOParametersSensor.tHeader.tSensor.tEvosens.ucTestMode        = ( TUCHAR )ucTestMode;
 tWIMOParametersSensor.tHeader.tSensor.tEvosens.uiPowerOn         = ( TUINT )uiPowerOn;
 // Sauvegarde de la config dans le capteur
 vFWIMOModbusSaveGeneralConfOnlyToSensor();
 // Valide
 return("");
}
//----------------------------------------------------------
// Fonction CPP d'écriture de config
//----------------------------------------------------------
void FactoryEvent::saveConfigSettings( int uiSN_Y, int uiSN_N )
{
 qDebug("==saveConfigSettings==");
 // Sauvegarde de la config
 tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY = ( TUCHAR )uiSN_Y;
 tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN = ( TUINT )uiSN_N;
 // Sauvegarde de la config dans le capteur
 vFWIMOModbusSaveGeneralConfOnlyToSensor();
}
//----------------------------------------------------------
// Fonction CPP d'écriture de config
//----------------------------------------------------------
void FactoryEvent::saveConfigSettingsCT( int uiSN_Y, int uiSN_N,
                                         float fA1, float fA2, float fA3, float fRaw20 )
{
 qDebug("==saveConfigSettingsCT==");
 // Sauvegarde de la config
 tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY = ( TUCHAR )uiSN_Y;
 tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN = ( TUINT )uiSN_N;
 tWIMOParametersSensor.tHeader.tCT.tCompensation.fA1    = ( TFLOAT )fA1;
 tWIMOParametersSensor.tHeader.tCT.tCompensation.fA2    = ( TFLOAT )fA2;
 tWIMOParametersSensor.tHeader.tCT.tCompensation.fA3    = ( TFLOAT )fA3;
 tWIMOParametersSensor.tHeader.tCT.tCompensation.fRaw20 = ( TFLOAT )fRaw20;
 // Sauvegarde de la config dans le capteur
 vFWIMOModbusSaveGeneralConfOnlyToSensor();
}

//----------------------------------------------------------
// Fonction CPP d'écriture de config
//----------------------------------------------------------
void FactoryEvent::endOfSNSubmit( void )
{
 TCHAR tcString[32];

 qDebug("==endOfSNSubmit==");
 // Mise à jour du SN
 sprintf( tcString, "%02d-%05d", tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY, tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN );
 ptDataSensor->setSN(tcString);
 // Fermeture du busy
 emit endOfSNSubmitSignal();
}

//----------------------------------------------------------
// Fonction CPP d'écriture de config
//----------------------------------------------------------
void FactoryEvent::endOfRefreshData( void )
{
 // Fermeture du busy
 emit endOfRefreshDataSignal();
}

//----------------------------------------------------------
// Fonction CPP de mise à jour du numéro de version
//----------------------------------------------------------
// Remplacé par ModbusControler
/*
void FactoryEvent::getInterfaceVersion( void )
{
 // Mise à jour sur l'interface
 //emit updateInterfaceVersionSignal( sInterfaceVersion );
 // Appel requête Modbus
 vFWIMOModbusGetInterfaceVersionNumber();
}
*/
//----------------------------------------------------------
// Fonction CPP de mise à jour du numéro de version
//----------------------------------------------------------
void FactoryEvent::initModbus( void )
{
 // Initialisation à nouveau du modbus
 vFWIMOModbusInitialization();
}

#endif
