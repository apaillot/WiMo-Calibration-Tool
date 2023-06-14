#ifndef MAINDATA_H
#define MAINDATA_H

#include <QObject>
#include <QString>

class MainData : public QObject
{
 Q_OBJECT
 Q_PROPERTY(int     uiInterfaceVersionMajor READ getInterfaceVersionMajor WRITE setInterfaceVersionMajor NOTIFY interfaceVersionMajorChanged);
 Q_PROPERTY(int     uiInterfaceVersionMinor READ getInterfaceVersionMinor WRITE setInterfaceVersionMinor NOTIFY interfaceVersionMinorChanged);
 Q_PROPERTY(int     uiInterfaceVersionBuild READ getInterfaceVersionBuild WRITE setInterfaceVersionBuild NOTIFY interfaceVersionBuildChanged);
 Q_PROPERTY(QString sDebug                  READ getDebug                 WRITE setDebug                 NOTIFY debugChanged);
 Q_PROPERTY(bool    bErrorSensorNeedUpdate  READ getErrorSensorNeedUpdate WRITE setErrorSensorNeedUpdate NOTIFY errorSensorNeedUpdateSignal);
 Q_PROPERTY(bool    bError                  READ getError                 WRITE setError                 NOTIFY errorSignal);
 Q_PROPERTY(bool    bCOMSlowMode            READ getCOMSlowMode           WRITE setCOMSlowMode           NOTIFY comSlowModeSignal);
 Q_PROPERTY(bool    bNkeMode                READ getNkeMode               WRITE setNkeMode               NOTIFY nkeModeSignal);
 Q_PROPERTY(bool    bMetroMode              READ getMetroMode             WRITE setMetroMode             NOTIFY metroModeSignal);
 Q_PROPERTY(bool    bProdMode               READ getProdMode              WRITE setProdMode              NOTIFY prodModeSignal);
 Q_PROPERTY(QString sInterfaceSN            READ getInterfaceSN           WRITE setInterfaceSN           NOTIFY interfaceSNChanged);

public:
 explicit MainData( QObject * parent = nullptr );

 int getInterfaceVersionMajor() { return( uiInterfaceVersionMajor ); }
 int getInterfaceVersionMinor() { return( uiInterfaceVersionMinor ); }
 int getInterfaceVersionBuild() { return( uiInterfaceVersionBuild ); }
 QString getDebug()             { return( m_sDebug ); }
 bool getErrorSensorNeedUpdate(){ return( m_bErrorSensorNeedUpdate ); }
 bool getError()                { return( m_bError ); }
 bool getCOMSlowMode()          { return( m_bCOMSlowMode ); }
 bool getNkeMode()              { return( m_bNkeMode ); }
 bool getMetroMode()            { return( m_bMetroMode ); }
 bool getProdMode()             { return( m_bProdMode ); }
 QString getInterfaceSN()       { return( m_sInterfaceSN ); }

 void setInterfaceVersionMajor(int uiNew)  { uiInterfaceVersionMajor=uiNew; emit interfaceVersionMajorChanged(); }
 void setInterfaceVersionMinor(int uiNew)  { uiInterfaceVersionMinor=uiNew; emit interfaceVersionMinorChanged(); }
 void setInterfaceVersionBuild(int uiNew)  { uiInterfaceVersionBuild=uiNew; emit interfaceVersionBuildChanged(); }
 void setDebug(QString sNew)               { m_sDebug=sNew; emit debugChanged(); }
 void setErrorSensorNeedUpdate( bool bNew ){ m_bErrorSensorNeedUpdate = bNew; emit errorSensorNeedUpdateSignal(); }
 void setError( bool bNew )                { m_bError = bNew; emit errorSignal(); }
 void setCOMSlowMode( bool bNew )          { m_bCOMSlowMode = bNew; emit comSlowModeSignal(); }
 void setNkeMode( bool bNew )              { m_bNkeMode = bNew; emit nkeModeSignal(); }
 void setMetroMode( bool bNew )            { m_bMetroMode = bNew; emit metroModeSignal(); }
 void setProdMode( bool bNew )             { m_bProdMode = bNew; emit prodModeSignal(); }
 void setInterfaceSN(QString sNew)         { m_sInterfaceSN=sNew; emit interfaceSNChanged(); }

signals :
 void interfaceVersionMajorChanged();
 void interfaceVersionMinorChanged();
 void interfaceVersionBuildChanged();
 void debugChanged();
 void errorSensorNeedUpdateSignal();
 void errorSignal();
 void comSlowModeSignal();
 void nkeModeSignal();
 void metroModeSignal();
 void prodModeSignal();
 void interfaceSNChanged();

private:
 int uiInterfaceVersionMajor = 0;
 int uiInterfaceVersionMinor = 0;
 int uiInterfaceVersionBuild = 0;
 QString m_sDebug = "Init";
 //
 bool m_bErrorSensorNeedUpdate = false;
 bool m_bError       = false;
 bool m_bCOMSlowMode = false;
 bool m_bNkeMode     = false;
 bool m_bMetroMode   = false;
 bool m_bProdMode    = false;
 QString m_sInterfaceSN = "";
};

#endif // MAINDATA_H
