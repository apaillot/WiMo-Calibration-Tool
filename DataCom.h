#ifndef DATACOM_H
#define DATACOM_H

#include <QObject>
#include <QString>

class DataCom: public QObject
{
 Q_OBJECT
 Q_PROPERTY(QString sComPort         READ getComPort         WRITE setComPort         NOTIFY comPortChanged);
 Q_PROPERTY(QString sComPortState    READ getComPortState    WRITE setComPortState    NOTIFY comPortStateChanged);
 Q_PROPERTY(QString sFirmwareVersion READ getFirmwareVersion WRITE setFirmwareVersion NOTIFY firmwareVersionChanged);

 Q_PROPERTY(QStringList tsComComboList READ getComComboList);

public:
 explicit DataCom( QObject * parent = nullptr );

 QString getComPort()   { return( sComPort ); }
 QString getComPortState()     { return( sComPortState ); }
 QString getFirmwareVersion()     { return( sFirmwareVersion ); }
 const QStringList getComComboList(){ return(tsComComboList); }

 void setComPort(QString sNew)             { sComPort=sNew;         emit comPortChanged(); }
 void setComPortState(QString sNew)        { sComPortState=sNew;    emit comPortStateChanged(); }
 void setFirmwareVersion(QString sNew)     { sFirmwareVersion=sNew; emit firmwareVersionChanged(); }
 void setComComboList( QStringList tsNew ) { tsComComboList=tsNew; }

signals:
 void comPortChanged();
 void comPortStateChanged();
 void firmwareVersionChanged();

private:
 QString sComPort = "";
 QString sComPortState = "";
 QString sFirmwareVersion = "";
 QStringList tsComComboList = {""};
};

#endif // DATACOM_H
