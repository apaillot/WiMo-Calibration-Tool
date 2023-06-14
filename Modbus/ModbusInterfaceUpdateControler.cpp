//============================================================================//
// Fichier .........: "WiMoO2.c"
// Auteur ..........: Alexandre Paillot
// Date de création : 2019.09.02
//----------------------------------------------------------------------------//
/* Description :
    Modbus interface update controler.
*/
//============================================================================//

#ifndef MODBUS_INTERFACE_UPDATE_CONTROLER_C
#define MODBUS_INTERFACE_UPDATE_CONTROLER_C

//============================================================================//
// Inclusion des librairies
//============================================================================//
// Utilisateur
#include "ModbusInterfaceUpdateControler.h"
#include "WIMOClientModbus.h"
#include "ModBusDriverConfiguration_2_04.h"
#include "readserialport.h"
#include "main.h"
// Qt
#include <QFile>

//============================================================================//
// Déclaration des constantes
//============================================================================//

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des fonctions exportées
//============================================================================//

//============================================================================//
// Déclaration des prototypes de fonctions non exportées
//============================================================================//

//****************************************************************************//
// Le code des fonctions
//****************************************************************************//

//----------------------------------------------------------------------------//
// Constructeur
//----------------------------------------------------------------------------//
ModbusInterfaceUpdateControler::ModbusInterfaceUpdateControler( QSerialPort *serialPort, QObject* parent ):
 QObject(parent),
 m_serialPort(serialPort)
{
 // Connection
 tTimerWaitSwitchToBooloaderMode.setSingleShot(false);
 connect( &tTimerWaitSwitchToBooloaderMode, &QTimer::timeout, this, &ModbusInterfaceUpdateControler::waitSwitchToBooloaderMode);
 // Connection
 tTimerWaitSwitchToBooloaderMode.setSingleShot(false);
 connect( &tTimerWaitTillFinished, &QTimer::timeout, this, &ModbusInterfaceUpdateControler::waitTillFinished);
}

//----------------------------------------------------------------------------//
// Setter nom du fichier de mise à jour
//----------------------------------------------------------------------------//
void ModbusInterfaceUpdateControler::setUpdateInterfaceFileName( QString sFileName )
{
 qDebug("== setUpdateInterfaceFileName ==");
 this->m_sUpdateFileName = sFileName;
 qDebug("== END setUpdateInterfaceFileName ==");
}

//----------------------------------------------------------------------------//
// Démarrage de la mise à jour interface
//----------------------------------------------------------------------------//
void ModbusInterfaceUpdateControler::startUpdateInterface( void )
{
 qDebug("== startUpdateInterface ==");
 ///%%TODO - Fonction à remplacer par Modbus controler
 // Si grafcet modbus déjà ocuppé
 if( bIsWIMOModbusGrafcetBusy() )
  {
   qDebug("bIsWIMOModbusGrafcetBusy YES");
   return;
  }
 // Création de l'handler de fichier de mise à jour
 QFile tFileHandler( this->m_sUpdateFileName );
 // Ouverture du fichier de mise à jour
 if( tFileHandler.open( QIODevice::ReadOnly ) )
  {
   // Récupération des données de mise à jour
   m_qByteArray = tFileHandler.readAll();
   // Fermeture du fichier
   tFileHandler.close();
   // Vérification qu'on a le bon nombre d'octet
   if( m_qByteArray.length() == 65536 )
    {
     // Appel de la CMD de mise à jour
     if( bFWIMOModbusStartUpdateInterfaceCMD( m_qByteArray ) )
      {
       /// Replacer dans la data view
       this->m_ucUpdateProgress = 0;
       // CMD lancée
       // Lancement d'une timer d'attente
       tTimerWaitSwitchToBooloaderMode.start(500);
      }
     else
      {
       qDebug("bFWIMOModbusStartUpdateInterfaceCMD fail");
      }
    }
   else
    {
     qDebug("Wrong bytes number");
    }
  }
  else
   {
    qDebug("Can't open update file");
   }
}

//----------------------------------------------------------------------------//
// Attente de fin de passage en mode bootloader
//----------------------------------------------------------------------------//
void ModbusInterfaceUpdateControler::waitSwitchToBooloaderMode( void )
{
 qDebug("== waitSwitchToBooloaderMode ==");
 emit updateProgressSignal( this->m_ucUpdateProgress );
 this->m_ucUpdateProgress++;
 // Pour attente 3 secondes
 if( this->m_ucUpdateProgress >= 6 )
  {
   // Stop du timer
   tTimerWaitSwitchToBooloaderMode.stop();
   // Passage en mode bootloader de la liaison série
   ptSerialPortWriter->startBootLoaderMode();
   /// Sortie de la fonction
   // A chaque envoi d'octet
   connect( m_serialPort, &QSerialPort::bytesWritten, this, &ModbusInterfaceUpdateControler::sendBytes);
   // Init du compteur d'octet envoyé
   m_uliByteCpt = 0;
   // Envoi d'un octet
   char cByte = m_qByteArray.at( ( int )m_uliByteCpt );
   m_uliByteCpt++;
   // Envoi d'un octet
   m_serialPort->write( &cByte, 1 );
  }
}

//----------------------------------------------------------------------------//
// Transmission des octets
//----------------------------------------------------------------------------//
void ModbusInterfaceUpdateControler::sendBytes( void )
{
 char cByte;

 // Envoi d'un octet
 cByte = m_qByteArray.at( ( int )m_uliByteCpt );
 // Envoi d'un octet
 m_serialPort->write( &cByte, 1 );
 m_uliByteCpt++;
 //
 if( ( m_uliByteCpt * 74 / 65535 ) > ( this->m_ucUpdateProgress - 6 )  )
  {
   this->m_ucUpdateProgress++;
   // Mise à jour de la valeur du loader
   emit updateProgressSignal( this->m_ucUpdateProgress );
   qDebug( "Update %u", this->m_ucUpdateProgress );
  }

 // Test de sortie de boucle
 if( m_uliByteCpt > ( TULONGINT )65535 )
  {
   // Debug
   qDebug("End of Interface Update");
   // Déconnexion du signal
   // Transfert terminée : Sortie de boucle
   disconnect( m_serialPort, &QSerialPort::bytesWritten, this, &ModbusInterfaceUpdateControler::sendBytes);
   //

   // Lancement d'une timer d'attente
   tTimerWaitTillFinished.start(500);
  }
}

//----------------------------------------------------------------------------//
// Attente de fin de mise à jour
//----------------------------------------------------------------------------//
void ModbusInterfaceUpdateControler::waitTillFinished( void )
{
 // Signal progression
 emit updateProgressSignal( this->m_ucUpdateProgress );
 this->m_ucUpdateProgress++;
 if( this->m_ucUpdateProgress >= 92 )
  {
   // Stop du timer
   tTimerWaitTillFinished.stop();
   ///%TODO - Déconnexion des signaux
   // Stop du mode bootloader de la liaison série
   ptSerialPortWriter->stopBootLoaderMode();
   //
   vFWIMOModbusInitialization();
   // Signal de fin de mise à jour
   emit updateEndSignal();
  }
}

#endif
