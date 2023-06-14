#include "TINIConfig.h"
#include <QObject>
#include <QSettings>
#include <QDebug>

//-------------------------------------------------------------
// Constructeur
//-------------------------------------------------------------
TINIConfig::TINIConfig( void )
//TINIConfig::TINIConfig( QObject* parent ):
// QObject(parent)
{
 m_ptGeneral         = new TINIGeneral();
 m_ptUpdate          = new TINIUpdate();
 m_ptInterfaceUpdate = new TINIInterfaceUpdate();
 m_ptSensorUpdate    = new TINISensorUpdate();
 // qDebug() << this->tInterfaceUpdate().sHTTPPath();
 /*
 qDebug() << this->tInterfaceUpdate_sHTTPPath();
 //
 m_tSoftwareUpdate_bActivate = true;
 m_tSoftwareUpdate_sHTTPPath = "https://nke-instrumentation.support/Update/WiMo_Calibration_Tool/Mainline/x64/";
 m_tSoftwareUpdate_sGetPath  = "Get/";
 m_tSoftwareUpdate_sDestProgramPath = "Move/";
 //
 m_tInterfaceUpdate_bActivate = true;
 m_tInterfaceUpdate_sHTTPPath = "https://nke-instrumentation.support/Update/WiMo_Interface_Usb/";
 //
 m_tSensorUpdate_bActivate = true;
 m_tSensorUpdate_sHTTPPath = "https://nke-instrumentation.support/Update/WiMo_Sensor/";
*/
}

//-------------------------------------------------------------
// Initialisation de la configuration .ini
//-------------------------------------------------------------
void TINIConfig::init( void )
{
 // Ouverture config.ini
 QSettings settings("config.ini", QSettings::IniFormat);

 //-----------
 // GLOBAL
 //-----------
 qDebug(  ) << "settings.contains( GLOBAL/BaudRate ) : " << settings.contains( "GLOBAL/BaudRate" );
 // Configuration général
 if( settings.contains( "GLOBAL/BaudRate" ) ){
  qDebug("settings.contains( GLOBAL/BaudRate )");
  qDebug() << "ini baudrate " << settings.value("GLOBAL/BaudRate").toInt();
  this->ptFGeneral()->setBaudrate( settings.value("GLOBAL/BaudRate").toInt() );
 }
 if( settings.contains( "GLOBAL/RecordMeasureLocation" ) ){
  qDebug("settings.contains( GLOBAL/RecordMeasureLocation )");
  this->ptFGeneral()->setRecordMesureLocation( settings.value("GLOBAL/RecordMeasureLocation").toString() );
 }
 qDebug() << "Baudrate: " << this->ptFGeneral()->uiBaudrate();
 // Mode debug nke
 if( settings.contains( "GLOBAL/Debug" ) ){
  qDebug("settings.contains( GLOBAL/Debug )");
  this->ptFGeneral()->setDebugMode( settings.value("GLOBAL/Debug").toBool() );
 }
 // Localisation
 if( settings.contains( "GLOBAL/Location" ) ){
  qDebug("settings.contains( GLOBAL/Location )");
  this->ptFGeneral()->setLocation( settings.value("GLOBAL/Location").toString() );
 }

 //-----------
 // UPDATE
 //-----------
 // Mise à jour logiciel
 if( settings.contains( "UPDATE/Activate" ) ){
   qDebug() << settings.value( "UPDATE/Activate" ).toString();
   qDebug() << settings.value( "UPDATE/Activate" ).toBool();
   this->ptFUpdate()->setActivate( false );
   this->ptFUpdate()->setActivate( settings.value( "UPDATE/Activate" ).toBool() );
 }
 if( settings.contains( "UPDATE/HTTPPath" ) )
  this->ptFUpdate()->setHTTPPath( settings.value( "UPDATE/HTTPPath" ).toString() );
 if( settings.contains( "UPDATE/GetPath" ) )
  this->ptFUpdate()->setGetPath( settings.value( "UPDATE/GetPath" ).toString() );
 if( settings.contains( "UPDATE/DestProgramPath" ) )
  this->ptFUpdate()->setDestProgramPath( settings.value( "UPDATE/DestProgramPath" ).toString() );
 qDebug() << this->ptFUpdate()->sGetPath();
 qDebug() << this->ptFUpdate()->sDestProgramPath();
 qDebug() << this->ptFUpdate()->sHTTPPath();

 //-----------
 // INTERFACE_UPDATE
 //-----------
 // Mise à jour interface usb
 if( settings.contains( "INTERFACE_UPDATE/Activate" ) )
  this->ptFInterfaceUpdate()->setActivate( settings.value( "INTERFACE_UPDATE/Activate" ).toBool() );
 if( settings.contains( "INTERFACE_UPDATE/HTTPPath" ) )
  this->ptFInterfaceUpdate()->setHTTPPath( settings.value( "INTERFACE_UPDATE/HTTPPath" ).toString() );

 //-----------
 // SENSOR_UPDATE
 //-----------
 // Mise à jour capteur
 if( settings.contains( "SENSOR_UPDATE/Activate" ) )
  this->ptFSensorUpdate()->setActivate( settings.value( "SENSOR_UPDATE/Activate" ).toBool() );
 if( settings.contains( "SENSOR_UPDATE/HTTPPath" ) )
  this->ptFSensorUpdate()->setHTTPPath( settings.value( "SENSOR_UPDATE/HTTPPath" ).toString() );

}
