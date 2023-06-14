#ifndef MODBUSCONTROLER_H
#define MODBUSCONTROLER_H

#include <QObject>
#include <MainData.h>

class ModbusControler : public QObject
{
 Q_OBJECT

public:
 explicit ModbusControler( MainData *, QObject * parent = nullptr );
 //explicit ModbusControler( QObject * parent = nullptr );

signals:
 // Signal end : numéro de version de l'interface/outil
 void requestInterfaceVersionNumberEndSignal( void );

public:
 // Initialisation modbus
 Q_INVOKABLE void init( void );
 // Demande numéro de version de l'interface/outil
 Q_INVOKABLE void requestInterfaceVersionNumber( void );

private slots:
 // Fin Demande numéro de version de l'interface/outil
 void onRequestInterfaceVersionNumberEnd( void );

private:
 MainData * m_ptMainData;
};

#endif // MODBUSCONTROLER_H
