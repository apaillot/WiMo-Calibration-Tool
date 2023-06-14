//============================================================================//
// Fichier .........: "readserialport.h"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2019/09/27
//----------------------------------------------------------------------------//
/* Description :
    Gestion du port série.
*/
//============================================================================//

#ifndef READ_SERIAL_PORT_H
#define READ_SERIAL_PORT_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
//#include <QThread>
#include <QObject>
#include <QSerialPort>
#include <QTimer>
// Type nke
#include <TypeC.h>
#include <TypeCMMN.h>

//============================================================================//
// Déclaration des constantes
//============================================================================//

//============================================================================//
// Déclaration des types
//============================================================================//

//============================================================================//
// Déclaration des variables exportées
//============================================================================//
// Activation du mode en réception
extern TBOOL bSerialPortRxMode;

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des classes exportées
//============================================================================//
class ReadSerialPort : public QObject
{

public:
    ReadSerialPort(QSerialPort *serialPort, QObject *parent = nullptr);
    //void run(); // le code du thread
    //void handleBytesWritten(qint64);
    void vFReadSerialTick100ms( void );
    void write( TUCHAR ucByte );
    void setRxMode( TBOOL );
    void startBootLoaderMode( void );
    void stopBootLoaderMode( void );
    bool isWriteBufferFree( void );

private:
    QSerialPort *m_serialPort = nullptr;
    QByteArray m_writeData;
    //QTextStream m_standardOutput;
    qint64 m_bytesWritten = 0;
    QTimer * m_ptTimerModbus;
    TBOOL m_bRXMode = aFALSE;
    bool  m_bBootLoaderMode = false;
    bool  m_bWriteBufferEmpty = true;
signals:

public slots:

private slots:
    void handleBytesWritten( qint64 );
    void handleReadyRead();
    //void handleTimeout();
    //void handleError(QSerialPort::SerialPortError error);

private:
 QByteArray tqByteArray;
};

//============================================================================//
// Déclaration des fonctions exportées
//============================================================================//

//-----------------------------------------------------------
// Envoi d'un octet
//-----------------------------------------------------------
//extern void vFQTSERIALSendByte( TUCHAR );

//-----------------------------------------------------------
// Si envoi possible sur la liaison
//-----------------------------------------------------------
extern bool bFSerialPortIsTXFree( void );

#endif // COMMUNICATION_H
