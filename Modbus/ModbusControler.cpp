#include <QDebug>
#include "ModbusControler.h"
#include "WIMOClientModBus.h"

//============================================================================//
// Déclaration des prototypes de fonctions non exportées
//============================================================================//

//****************************************************************************//
// Le code des fonctions
//****************************************************************************//

//----------------------------------------------------------------------------//
// Constructeur
//----------------------------------------------------------------------------//
ModbusControler::ModbusControler( MainData * ptMainData, QObject* parent ):
 QObject(parent),
 m_ptMainData( ptMainData )
 /*
ModbusControler::ModbusControler( QObject* parent ):
 QObject(parent)
 */
{
 // Fin de requête - Interface version
 connect( this, &ModbusControler::requestInterfaceVersionNumberEndSignal, this, &ModbusControler::onRequestInterfaceVersionNumberEnd );
}

//============================================================================//
// Code
//============================================================================//

//----------------------------------------------------------------------------//
// Initialisation modbus
//----------------------------------------------------------------------------//
Q_INVOKABLE void ModbusControler::init( void )
{
 qDebug( "== ModbusControler::init ==" );
 // Demande du numéro de version
 //vFWIMOModbusGetInterfaceVersionNumber();
}

//----------------------------------------------------------------------------//
// Demande numéro de version de l'interface/outil
//----------------------------------------------------------------------------//
Q_INVOKABLE void ModbusControler::requestInterfaceVersionNumber( void )
{
 qDebug( "== ModbusControler::requestInterfaceVersionNumber ==" );
 // Demande du numéro de version
 vFWIMOModbusGetInterfaceVersionNumber();
}

//----------------------------------------------------------------------------//
// Slot - Fin demande numéro de version de l'interface/outil
//----------------------------------------------------------------------------//
void ModbusControler::onRequestInterfaceVersionNumberEnd( void )
{
 qDebug( "onRequestInterfaceVersionNumberEnd start" );
 m_ptMainData->setInterfaceVersionMajor( ( int )tWIMOModbusInterfaceVersion.ucMajor );
 m_ptMainData->setInterfaceVersionMinor( ( int )tWIMOModbusInterfaceVersion.ucMinor );
 m_ptMainData->setInterfaceVersionBuild( ( int )tWIMOModbusInterfaceVersion.uiBuild );
 qDebug( "onRequestInterfaceVersionNumberEnd Stop" );
}
