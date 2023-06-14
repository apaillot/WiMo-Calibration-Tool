#ifndef INTERFACEUPDATEDATA_H
#define INTERFACEUPDATEDATA_H

#include <QObject>
#include <QString>

class InterfaceUpdateData : public QObject
{
 Q_OBJECT
 Q_PROPERTY(QString sInterfaceInfo           READ getInterfaceInfo          WRITE setInterfaceInfo          NOTIFY sensorInfoChanged);
 Q_PROPERTY(QString sInterfaceVersion        READ getInterfaceVersion       WRITE setInterfaceVersion       NOTIFY sensorVersionChanged);
 Q_PROPERTY(QString sUpdateVersion        READ getUpdateVersion       WRITE setUpdateVersion       NOTIFY updateVersionChanged);
 Q_PROPERTY(int     uiDownloadProgression READ getDownloadProgression WRITE setDownloadProgression NOTIFY downloadProgressionChanged);
 Q_PROPERTY(int     uiUploadProgression   READ getUploadProgression   WRITE setUploadProgression   NOTIFY uploadProgressionChanged);
 Q_PROPERTY(bool    bUpdateBusy           READ getUpdateBusy          WRITE setUpdateBusy          NOTIFY updateBusyChanged);
 //Q_PROPERTY(bool    bWindowShow           READ getWindowShow          WRITE setWindowShow          NOTIFY windowShowChanged);


public:
 explicit InterfaceUpdateData( QObject * parent = nullptr );

 QString getInterfaceInfo()      { return( sInterfaceInfo ); }
 QString getInterfaceVersion()   { return( sInterfaceVersion ); }
 QString getUpdateVersion()   { return( sUpdateVersion ); }
 int getDownloadProgression() { return( uiDownloadProgression ); }
 int getUploadProgression()   { return( uiUploadProgression ); }
 int getUpdateBusy()          { return( bUpdateBusy ); }
 //int getWindowShow()          { return( bWindowShow ); }

 void setInterfaceInfo(QString sNew)      { sInterfaceInfo=sNew; emit sensorInfoChanged(); }
 void setInterfaceVersion(QString sNew)   { sInterfaceVersion=sNew; emit sensorVersionChanged(); }
 void setUpdateVersion(QString sNew)   { sUpdateVersion=sNew; emit updateVersionChanged(); }
 void setDownloadProgression(int uiNew){ uiDownloadProgression=uiNew; emit downloadProgressionChanged(); }
 void setUploadProgression(int uiNew)  { uiUploadProgression=uiNew; emit uploadProgressionChanged(); }
 void setUpdateBusy(bool bNew)         { bUpdateBusy=bNew; emit updateBusyChanged(); }
 //void setWindowShow(bool bNew)         { bWindowShow=bNew; emit windowShowChanged(); }

signals :
 void sensorInfoChanged();
 void sensorVersionChanged();
 void updateVersionChanged();
 void downloadProgressionChanged();
 void uploadProgressionChanged();
 void updateBusyChanged();
 //void windowShowChanged();

private:
 // Propriété de la vue
 QString sInterfaceInfo       = "";
 QString sInterfaceVersion    = "";
 QString sUpdateVersion    = "";
 int     uiDownloadProgression = 0;
 int     uiUploadProgression   = 0;
 bool    bUpdateBusy    = false;
 //bool    bWindowShow    = false;
};

#endif // SENSORUPDATEDATA_H
