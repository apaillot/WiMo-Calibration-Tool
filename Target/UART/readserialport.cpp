//============================================================================//
// Fichier .........: "ReadSerialPort.cpp"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2019/09/26
//----------------------------------------------------------------------------//
/* Description :
    Gestion de la lecture d'octet de la liaison série.
*/
//============================================================================//

#ifndef READ_SERIAL_PORT_C
#define READ_SERIAL_PORT_C

//============================================================================//
// Inclusion des librairies
//============================================================================//
// Windows
#include <windows.h>
// Qt
#include <QThread>
#include <QSerialPort>
#include <QDebug>
#include <QQueue>
#include <QTimer>
#include <QFile>
#include <QTextStream>
// Type nke
#include "TypeC.h"
#include "TypeCMMN.h"
// Programme
#include "readserialport.h"
#include "ModBusDriver_2_04.h"
#include "main.h"

//============================================================================//
// Déclaration des constantes
//============================================================================//
// Numéro du port com utilisé
#define aPORT "COM1"

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

//============================================================================//
// Déclaration des prototypes de fonctions non exportées
//============================================================================//

//****************************************************************************//
// Le code des fonctions
//****************************************************************************//

//----------------------------------------------------------------------------//
// Test liaison série sur Qt
//----------------------------------------------------------------------------//
ReadSerialPort::ReadSerialPort( QSerialPort *serialPort, QObject *parent ) :
  QObject(parent),
  m_serialPort(serialPort)
{
 connect(m_serialPort, &QSerialPort::readyRead, this, &ReadSerialPort::handleReadyRead);
 connect(m_serialPort, &QSerialPort::bytesWritten, this, &ReadSerialPort::handleBytesWritten);
 // Init membre
 m_bytesWritten = 0;
 // Timer
 m_ptTimerModbus = new QTimer(this);
 connect(m_ptTimerModbus, &QTimer::timeout, this, &ReadSerialPort::vFReadSerialTick100ms);
 m_ptTimerModbus->start(100);

 tqByteArray.clear();
}

//----------------------------------------------------------------------------//
// Set RX Mode
//----------------------------------------------------------------------------//
void ReadSerialPort::setRxMode( TBOOL bRxMode )
{
 m_bRXMode = bRxMode;
}

//----------------------------------------------------------------------------//
// Test liaison série sur Qt
//----------------------------------------------------------------------------//
void ReadSerialPort::write( TUCHAR ucByte )
{
 TUINT uiDummy;
 //int nombresOctets;
 QByteArray tByteToSendLocal;
 char cByte = ( char )ucByte;

 //%%AP - 2021.05.05 - Modificatino névralgique
 //QByteArray tqByteArray;
 //tqByteArray.append( ( char )cByte );

 //qDebug("ByteS : %#02X, %u, %c",ucByte,ucByte,ucByte);

 //QThread::msleep(1);
 //QThread::usleep(1);
 // Test si on est en mode bootloader
 if( m_bBootLoaderMode )
  {
   //qDebug("ByteS : %#02X, %u, %c",ucByte,ucByte,ucByte);
   // Lock du buffer d'écriture
   m_bWriteBufferEmpty = false;
  }
 else
  {
   tqByteArray.append( ( char )cByte );
   //while( tModBus.bFSendingFrameIsReady() );
   if( !tModBus.bFSendingFrameIsReady() ) return;

   //qDebug("tModBus.bFSendingFrameIsReady() true");

   m_serialPort->write(tqByteArray);
   return;
  }
 //qDebug("ByteS : %#02X, %u, %c",ucByte,ucByte,ucByte);
 // Envoi de l'octet
 //m_serialPort->write(tqByteArray);
 m_serialPort->write( &cByte, 1 );
}

//----------------------------------------------------------------------------//
// Write callback
//----------------------------------------------------------------------------//
void ReadSerialPort::handleBytesWritten( qint64 bytes )
{
 m_bytesWritten += bytes;
 //qDebug("Byte write : %d", m_bytesWritten);

 // Test si on est en mode bootloader
 if( m_bBootLoaderMode )
  {
   //qDebug("Byte write : %d", m_bytesWritten);
   // Libération du buffer d'écriture
   m_bWriteBufferEmpty = true;
  }
 else
  {
   //qDebug("Byte write : %d", m_bytesWritten);


   tqByteArray.clear();
   // Gestion des octets
   tModBus.vGOnTxEmpty();
  }
}

//----------------------------------------------------------------------------//
// Read callback
//----------------------------------------------------------------------------//
void ReadSerialPort::handleReadyRead()
{
 QByteArray tReadData;
 TUINT uiReadCpt;
 TUCHAR ucByte;

 tReadData = m_serialPort->readAll();
 // Si en mode bootloader : on skip
 if( m_bBootLoaderMode ) return;

 // On boucle sur le nombre d'octet
 for( uiReadCpt = ( TUINT )0; uiReadCpt < tReadData.size(); uiReadCpt++ )
  {
   ucByte = ( TUCHAR )tReadData[uiReadCpt];
   // Prise en compte des octets reçus
   tModBus.vFOnRxChar( ucByte );
   //qDebug("ByteR : %#02X, %u, %c", ucByte, ucByte, ucByte);
  }
}

//----------------------------------------------------------------------------//
// Base de temps à 100ms
//----------------------------------------------------------------------------//
void ReadSerialPort::vFReadSerialTick100ms( void )
{
 // Communication modbus
 tModBus.vFTick();
}

//----------------------------------------------------------------------------//
// Start mode bootloader
//----------------------------------------------------------------------------//
void ReadSerialPort::startBootLoaderMode( void )
{
 // Activation du mode bootloader
 m_bBootLoaderMode = true;
 // Le buffer d'envoi est libre
 m_bWriteBufferEmpty = true;
 // Réinit variable de debug
 m_bytesWritten = 0;
}
//----------------------------------------------------------------------------//
// Stop mode bootloader
//----------------------------------------------------------------------------//
void ReadSerialPort::stopBootLoaderMode( void )
{
 // Désactivation du mode bootloader
 m_bBootLoaderMode = false;
}
//----------------------------------------------------------------------------//
// Attente que l'octet soit bien partie
//----------------------------------------------------------------------------//
bool ReadSerialPort::isWriteBufferFree( void )
{
 // Renvoi si le buffer d'écriture bootloader est libre
 return( m_bWriteBufferEmpty );
}

#endif
