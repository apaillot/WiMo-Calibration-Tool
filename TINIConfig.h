#ifndef TINICONFIG_H
#define TINICONFIG_H

#include <QObject>
#include <QString>

// Mise à jour
class TINIUpdate
{
public:
 TINIUpdate(){}

 bool bActivate( void ){ return( m_bActivate ); }
 QString sHTTPPath( void ){ return( m_sHTTPPath ); }
 QString sGetPath( void ){ return( m_sGetPath ); }
 QString sDestProgramPath( void ){ return( m_sDestProgramPath ); }

 void setActivate( bool bActivate ){ m_bActivate = bActivate; }
 void setHTTPPath( QString sHTTPPath ){ m_sHTTPPath = sHTTPPath; }
 void setGetPath( QString sGetPath ){ m_sGetPath  = sGetPath; }
 void setDestProgramPath( QString sDestProgramPath ){ m_sDestProgramPath = sDestProgramPath; }

private:
 bool m_bActivate = true;
 QString m_sHTTPPath = "https://nke-instrumentation.support/Update/WiMo_Calibration_Tool/Mainline/x64/";
 QString m_sGetPath  = "Get/";
 QString m_sDestProgramPath = "Move/";
};

class TINIInterfaceUpdate
{
public:
 TINIInterfaceUpdate(){}
 bool     bActivate( void ){ return( m_bActivate ); }
 QString  sHTTPPath( void ){ return( m_sHTTPPath ); }
 void setActivate( bool bActivate ){ m_bActivate = bActivate; }
 void setHTTPPath( QString sHTTPPath ){ m_sHTTPPath = sHTTPPath; }

private:
 bool     m_bActivate = true;
 QString  m_sHTTPPath = "http://nke-instrumentation.support/Update/WiMo_Interface_Usb/";
};

class TINISensorUpdate
{
public:
 TINISensorUpdate(){}
 bool     bActivate( void ){ return( m_bActivate ); }
 QString  sHTTPPath( void ){ return( m_sHTTPPath ); }
 void setActivate( bool bActivate ){ m_bActivate = bActivate; }
 void setHTTPPath( QString sHTTPPath ){ m_sHTTPPath = sHTTPPath; }

private:
 bool     m_bActivate = true;
 QString  m_sHTTPPath = "http://nke-instrumentation.support/Update/WiMo_Sensor/";
};

class TINIGeneral
{
public:
 TINIGeneral(){}
 int uiBaudrate( void ){ return( m_uiBaudrate ); }
 QString sRecordMesureLocation( void ){ return( m_sRecordMesureLocation ); }
 void setBaudrate( int uiNew ){ m_uiBaudrate = uiNew; }
 void setRecordMesureLocation( QString sNew ){ m_sRecordMesureLocation = sNew; }
 bool bDebugMode( void ){ return( m_bDebugMode ); }
 void setDebugMode( bool bNew ){ m_bDebugMode = bNew; }
 QString sLocation( void ){ return( m_sLocation ); }
 void    setLocation( QString sNew ){ m_sLocation = sNew; }

private:
 int     m_uiBaudrate = 57600;
 QString m_sRecordMesureLocation = "../Measure";
 bool    m_bDebugMode = false;
 QString m_sLocation  = "";
};

class TINIConfig
{
public:
 TINIConfig();

 // Initialisation de la configuration .ini
 void init( void );

 TINIGeneral * ptFGeneral( void ){ return( m_ptGeneral ); }
 TINIUpdate * ptFUpdate( void ){ return( m_ptUpdate ); }
 TINIInterfaceUpdate * ptFInterfaceUpdate( void ){ return( m_ptInterfaceUpdate ); }
 TINISensorUpdate * ptFSensorUpdate( void ){ return( m_ptSensorUpdate ); }

private:
 TINIGeneral         * m_ptGeneral;
 TINIUpdate          * m_ptUpdate;
 TINIInterfaceUpdate * m_ptInterfaceUpdate;
 TINISensorUpdate    * m_ptSensorUpdate;
};

/*
class TINIConfig
{
public:
 TINIConfig();

 // Initialisation de la configuration .ini
 void init( void );

 TINIUpdate tUpdate( void ){ return( m_tUpdate ); }
 TINIInterfaceUpdate tInterfaceUpdate( void ){ return( m_tInterfaceUpdate ); }
 TINISensorUpdate    tSensorUpdate( void ){ return( m_tSensorUpdate ); }

private:
 bool    m_tSoftwareUpdate_bActivate = true;
 QString m_tSoftwareUpdate_sHTTPPath = "https://nke-instrumentation.support/Update/WiMo_Calibration_Tool/Mainline/x64/";
 QString m_tSoftwareUpdate_sGetPath  = "Get/";
 QString m_tSoftwareUpdate_sDestProgramPath = "Move/";

 TINIUpdate m_tUpdate;
 TINIInterfaceUpdate m_tInterfaceUpdate;
 TINISensorUpdate    m_tSensorUpdate;
};
*/

/*
class TINIConfig: public QObject
{
 Q_OBJECT

public:
 TINIConfig( QObject * parent = nullptr );

 // Initialisation de la configuration .ini
 void init( void );

 bool    tSoftwareUpdate_bActivate( void ){ return( m_tSoftwareUpdate_bActivate ); }
 QString tSoftwareUpdate_sHTTPPath( void ){ return( m_tSoftwareUpdate_sHTTPPath ); }
 QString tSoftwareUpdate_sGetPath( void ){ return( m_tSoftwareUpdate_sGetPath ); }
 QString tSoftwareUpdate_sDestProgramPath( void ){ return( m_tSoftwareUpdate_sDestProgramPath ); }

 void setSoftwareUpdate_Activate( bool bActivate )   { m_tSoftwareUpdate_bActivate = bActivate; }
 void setSoftwareUpdate_HTTPPath( QString sHTTPPath ){ m_tSoftwareUpdate_sHTTPPath = sHTTPPath; }
 void setSoftwareUpdate_GetPath( QString sGetPath )  { m_tSoftwareUpdate_sGetPath  = sGetPath; }
 void setSoftwareUpdate_DestProgramPath( QString sDestProgramPath ){ m_tSoftwareUpdate_sDestProgramPath = sDestProgramPath; }

 bool     tInterfaceUpdate_bActivate( void ){ return( m_tInterfaceUpdate_bActivate ); }
 QString  tInterfaceUpdate_sHTTPPath( void ){ return( m_tInterfaceUpdate_sHTTPPath ); }
 void setInterfaceUpdate_Activate( bool bActivate ){ m_tInterfaceUpdate_bActivate = bActivate; }
 void setInterfaceUpdate_HTTPPath( QString sHTTPPath ){ m_tInterfaceUpdate_sHTTPPath = sHTTPPath; }

 bool     tSensorUpdate_bActivate( void ){ return( m_tSensorUpdate_bActivate ); }
 QString  tSensorUpdate_sHTTPPath( void ){ return( m_tSensorUpdate_sHTTPPath ); }
 void setSensorUpdate_Activate( bool bActivate ){ m_tSensorUpdate_bActivate = bActivate; }
 void setSensorUpdate_HTTPPath( QString sHTTPPath ){ m_tSensorUpdate_sHTTPPath = sHTTPPath; }

private:

 bool    m_tSoftwareUpdate_bActivate = true;
 QString m_tSoftwareUpdate_sHTTPPath = "https://nke-instrumentation.support/Update/WiMo_Calibration_Tool/Mainline/x64/";
 QString m_tSoftwareUpdate_sGetPath  = "Get/";
 QString m_tSoftwareUpdate_sDestProgramPath = "Move/";

 bool    m_tInterfaceUpdate_bActivate = true;
 QString m_tInterfaceUpdate_sHTTPPath = "https://nke-instrumentation.support/Update/WiMo_Interface_Usb/";

 bool    m_tSensorUpdate_bActivate = true;
 QString m_tSensorUpdate_sHTTPPath = "https://nke-instrumentation.support/Update/WiMo_Sensor/";
};
*/

#endif // TINICONFIG_H
