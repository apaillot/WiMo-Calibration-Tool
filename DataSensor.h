#ifndef DATASENSOR_H
#define DATASENSOR_H

#include <QObject>
#include <QString>

class DataSensor: public QObject
{
 Q_OBJECT
 Q_PROPERTY(int uiChannelNb           READ getChannelNb       WRITE setChannelNb       NOTIFY channelNbChanged)
 Q_PROPERTY(QString sIdentifier       READ getIdentifier      WRITE setIdentifier      NOTIFY identifierChanged)
 Q_PROPERTY(QString sSN               READ getSN              WRITE setSN              NOTIFY snChanged)
 Q_PROPERTY(int uiSN_YY               READ getSN_YY           WRITE setSN_YY           NOTIFY snYyChanged)
 Q_PROPERTY(int uiSN_No               READ getSN_No           WRITE setSN_No           NOTIFY snNoChanged)
 Q_PROPERTY(QString sManufacturer     READ getManufacturer    WRITE setManufacturer    NOTIFY manufacturerChanged)
 Q_PROPERTY(QString sFirmwareVersion  READ getFirmwareVersion WRITE setFirmwareVersion NOTIFY firmwareVersionChanged)
 Q_PROPERTY(QStringList tsChannelList READ getChannelList     WRITE setChannelList     NOTIFY channelListChanged)


public:
 explicit DataSensor( QObject * parent = nullptr );

 int     getChannelNb()       { return( uiChannelNb ); }
 QString getIdentifier()      { return( sIdentifier ); }
 QString getSN()              { return( sSN ); }
 int     getSN_YY()           { return( uiSN_YY ); }
 int     getSN_No()           { return( uiSN_No ); }
 QString getManufacturer()    { return( sManufacturer ); }
 QString getFirmwareVersion() { return( sFirmwareVersion ); }
 QStringList getChannelList() { return( tsChannelList ); }

 void setChannelNb(int uiNew)           { uiChannelNb=uiNew;      emit channelNbChanged(); }
 void setIdentifier(QString sNew)       { sIdentifier=sNew;       emit identifierChanged(); }
 void setManufacturer(QString sNew)     { sManufacturer=sNew;     emit manufacturerChanged(); }
 void setSN(QString sNew)               { sSN=sNew;               emit snChanged(); }
 void setSN_YY(int uiNew)               { uiSN_YY=uiNew;          emit snYyChanged(); }
 void setSN_No(int uiNew)               { uiSN_No=uiNew;          emit snNoChanged(); }
 void setFirmwareVersion(QString sNew)  { sFirmwareVersion=sNew;  emit firmwareVersionChanged(); }
 void setChannelList(QStringList tsNew) { tsChannelList=tsNew;    emit channelListChanged(); }

signals:
 void channelNbChanged();
 void identifierChanged();
 void manufacturerChanged();
 void snChanged();
 void snYyChanged();
 void snNoChanged();
 void firmwareVersionChanged();
 void channelListChanged();

private:
 int     uiChannelNb       = 0;
 QString sIdentifier       = "";
 QString sManufacturer     = "";
 QString sSN               = "";
 int uiSN_YY               = 0;
 int uiSN_No               = 0;
 QString sFirmwareVersion  = "";
 QStringList tsChannelList = {""};
};

#endif // DATASENSOR_H
