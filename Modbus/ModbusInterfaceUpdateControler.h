#ifndef MODBUS_INTERFACE_UPDATE_CONTROLER_H
#define MODBUS_INTERFACE_UPDATE_CONTROLER_H

#include <QObject>
#include <QString>
#include <QTimer>
#include "readserialport.h"

class ModbusInterfaceUpdateControler : public QObject
{
 Q_OBJECT

public:
 explicit ModbusInterfaceUpdateControler( QSerialPort *serialPort, QObject * parent = nullptr );

signals:
 void updateProgressSignal( quint8 );
 void updateEndSignal( void );

public:
 // Setter nom du fichier de mise à jour
 void setUpdateInterfaceFileName( QString );
 // Démarrage de la mise à jour interface
 void startUpdateInterface( void );

private:
 // Attente de fin de passage en mode bootloader
 void waitSwitchToBooloaderMode( void );
 // Transmission des octets
 void sendBytes( void );
 // Attente de fin de mise à jour
 void waitTillFinished( void );

private slots:

private:
 QSerialPort *m_serialPort = nullptr;
 QString m_sUpdateFileName;
 quint8  m_ucUpdateProgress;
 QTimer  tTimerWaitSwitchToBooloaderMode;
 QTimer  tTimerWaitTillFinished;
 QByteArray m_qByteArray;
 quint32 m_uliByteCpt;
};

#endif // MODBUSINTERFACEUPDATECONTROLER_H
