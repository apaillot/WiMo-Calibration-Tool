//============================================================================//
// Fichier .........: "factoryevent.cpp"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2020/01/28
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef WINDOW_COM_EVENT_CPP
#define WINDOW_COM_EVENT_CPP

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <QString>
#include "windowcomevent.h"
#include <WIMOParameters.h>
#include <MainData.h>

// FTDI librairie
#include <stdafx.h>
#include <windows.h>
#include <stdio.h>
#include "ftd2xx.h"

//============================================================================//
// Déclaration des constantes
//============================================================================//

//============================================================================//
// Déclaration des prototypes de fonctions non exportées
//============================================================================//

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

//****************************************************************************//
// Le code des fonctions
//****************************************************************************//
//----------------------------------------------------------
// Constructeur de la classe
//----------------------------------------------------------
ComPort::ComPort( unsigned char ucPortCom, unsigned char ucSN_Y, unsigned int uiSN_N, QString sSN )
    : m_ucPortCom( ucPortCom ),
      m_ucSN_Y( ucSN_Y ),
      m_uiSN_N( uiSN_N ),
      m_sSN( sSN )
{
}
unsigned char ComPort::ucPortCom()
{
    return m_ucPortCom;
}
unsigned char ComPort::ucSN_Y()
{
    return m_ucSN_Y;
}
unsigned int  ComPort::uiSN_N()
{
    return m_uiSN_N;
}
QString  ComPort::sSN()
{
    return m_sSN;
}

//----------------------------------------------------------
// Constructeur de la classe
//----------------------------------------------------------
WindowCOMEvent::WindowCOMEvent( QObject *parent ): QObject(parent)
{
 // Init attribut de ma classe
 m_bComPortSelected = false;
 m_sPortComName     = "";
}

//----------------------------------------------------------
// Get du nom du port COM
//----------------------------------------------------------
Q_INVOKABLE QString WindowCOMEvent::sFGetPortComName( void )
{
 // Retourne le nom du port COM
 return( this->m_sPortComName );
}

//----------------------------------------------------------
// Détection des FTDIs nke présent
//----------------------------------------------------------
Q_INVOKABLE unsigned int WindowCOMEvent::uiFDetectionCOMFtdiInterfaceNke( void )
{
 //********************************************************
 // Détection du port COM NKE
 //********************************************************
 FT_STATUS status;
 long uliComPortNumber;
 FT_DEVICE_LIST_INFO_NODE *devInfo;
 DWORD numDevs;
 DWORD i;
 // Get SN
 char tcSerialNumber[16] = {0};

 // Nombre d'interface nke détectées
 TUCHAR ucSensorInterfaceNb  = ( TUCHAR )0;
 TUCHAR ucSensorInterfaceCpt = ( TUCHAR )0;

 unsigned long uliSN_Y = ( unsigned long  )0;
 unsigned long uliSN_N = ( unsigned long  )0;

 // Création d'un object dynamique pour stockage port COM
 //QVector<ComPort> ttCOMPort;

 // Lancement d'un nouvelle détection : réinit des ports trouvés
 m_ttCOMPort.clear();
 ptMainData->setInterfaceSN("");

 // create the device information list
 status = FT_CreateDeviceInfoList(&numDevs);
 // Test status
 if( status != FT_OK )
  {
   qDebug("FT_CreateDeviceInfoList status not ok %d\n", status);
   return 0;
  }
 else
  {
   qDebug("Number of devices is %d\n", numDevs);
   // Si détection de périphérique FTDI
   if( numDevs > 0 )
    {
     // Allocate storage for list based on numDevs
     devInfo = ( FT_DEVICE_LIST_INFO_NODE * )malloc( sizeof( FT_DEVICE_LIST_INFO_NODE ) * numDevs );
     // Get the device information list
     status = FT_GetDeviceInfoList(devInfo, &numDevs);
     if( status == FT_OK )
      {
       qDebug("numDevs: %d", numDevs);
       // Boucle sur les ports détectés
       for( i = 0; i < numDevs; i++ )
        {
         qDebug( "COMPORT LOOP i: %d", i );
         // Détection
         if( memcmp( devInfo[i].Description, "WIMO_NKE", sizeof( "WIMO_NKE" ) ) == ( TINT )0 )
          {
           // DEBUG
           qDebug( "memcmp WIMO_NKE OK" );
           // Test
           if( sscanf( devInfo[i].SerialNumber, "%d-%d", &uliSN_Y, &uliSN_N ) == ( TINT )2 )
            {

             // Incrément du nombre d'interface nke détecté
             ucSensorInterfaceNb++;
             // Ouverture du port nke
             status = FT_OpenEx( devInfo[i].SerialNumber, FT_OPEN_BY_SERIAL_NUMBER, &devInfo[i].ftHandle );
             // Test du status
             if( status == FT_OK )
              {
               // Lecture du port COM
               FT_GetComPortNumber( devInfo[i].ftHandle, &uliComPortNumber );

               //FT_GetDeviceInfo( devInfo[i].ftHandle, NULL, NULL, tcSerialNumber, NULL, NULL );
               //qDebug() << "Interface serial Number: " << tcSerialNumber;
               //ptMainData->setInterfaceSN(tcSerialNumber);

               // Ajout du points
               m_ttCOMPort.append( { ( TUCHAR )uliComPortNumber, ( unsigned char )uliSN_Y, uliSN_N, devInfo[i].SerialNumber } );
               // Fermeture du port
               FT_Close(devInfo[i].ftHandle);
               // Sortie
               //break;
               qDebug( "COM port: %d", uliComPortNumber );
              }
            }
          }
         // Interface pas nke
         else
          {
           qDebug( "Interface pas nke" );
          }
         qDebug( "Dev %d:", i );
         qDebug( "Flags=0x%x", devInfo[i].Flags );
         qDebug( "Type=0x%x", devInfo[i].Type );
         qDebug( "ID=0x%x", devInfo[i].ID );
         qDebug( "LocId=0x%x", devInfo[i].LocId );
         qDebug( "SerialNumber=%s", devInfo[i].SerialNumber );
         qDebug( "Description=%s", devInfo[i].Description );
        }
      }
    }
   // Ajout
   else
    {
     //return 0;
    }
  }
 // Retour
 return( ( unsigned int )ucSensorInterfaceNb );
}

//----------------------------------------------------------
// Remplissage des ports COM de la fenêtre de détection automatique
//----------------------------------------------------------
Q_INVOKABLE void WindowCOMEvent::vFFillComPortAutomaticWindow( void )
{
 TUCHAR ucSensorInterfaceCpt;
 // Remplissage de la fenêtre
 // Clear de la fenêtre de sélection du port COM
 //tCOMEvent.vFClearCOMPortList();
 this->vFClearCOMPortList();

 // Parcourts des points pour ajout
 for( ucSensorInterfaceCpt = ( TUCHAR )0; ucSensorInterfaceCpt < ( TUCHAR )m_ttCOMPort.length(); ucSensorInterfaceCpt++ )
  {
   // liste des interfaces
   //qDebug( "Interface: %d", ttCOMPort.at( ucSensorInterfaceCpt ).ucPortCom() );
   //ttCOMPort.value( ucSensorInterfaceCpt ).ucPortCom();
   qDebug( "Interface: %d", m_ttCOMPort.at( ucSensorInterfaceCpt ).m_ucPortCom );
   //qDebug( "Interface: %d", ttCOMPort.at( ucSensorInterfaceCpt ).ucPortCom() );
   qDebug( "SN: %02d-%05d",
           m_ttCOMPort.at( ucSensorInterfaceCpt ).m_ucSN_Y,
           m_ttCOMPort.at( ucSensorInterfaceCpt ).m_uiSN_N );
   // Ajout port COM
   this->vFAddCOMPort( m_ttCOMPort.at( ucSensorInterfaceCpt ).m_ucPortCom,
                       m_ttCOMPort.at( ucSensorInterfaceCpt ).m_ucSN_Y,
                       m_ttCOMPort.at( ucSensorInterfaceCpt ).m_uiSN_N );
  }
}

//----------------------------------------------------------
// Choix de la methode de sélection du port COM
//----------------------------------------------------------
Q_INVOKABLE void WindowCOMEvent::vFComPortFindAutomaticSelection( bool bInit )
{
 qDebug( "==vFComPortFindAutomaticSelection==" );
 //TUCHAR ucSensorInterfaceNb = ( TUCHAR )this->uiFDetectionCOMFtdiInterfaceNke();
 TUCHAR ucSensorInterfaceNb;
 TUCHAR ucSensorInterfaceCpt;
 QString sPortCom;

 qDebug( "ucSensorInterfaceNb : %d", ucSensorInterfaceNb );

 // Test d'intégrité
 if( m_serialPort.isOpen() )
  {
   qDebug("Fermeture de l'ancien port COM");
   // Fermeture du port
   this->bFCloseCOMPort();
  }

 // Détection des interfaces connectées
 ucSensorInterfaceNb = ( TUCHAR )this->uiFDetectionCOMFtdiInterfaceNke();

 //-------------
 // Pas de FTDI nke
 //-------------
 // Test fichier portCom.ini
 //----------------
 // Lecture du port COM dans le fichier de configuration *.ini
 //----------------
 if( QFile::exists("portCom.ini") && bInit )
  {
   // Création de l'handler de fichier
   QFile tFileHandler( "portCom.ini" );
   // Ouverture du fichier *.ini
   if( tFileHandler.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
     sPortCom = tFileHandler.readAll();
     tFileHandler.close();
     qDebug( "Lecture fichier" );
     qDebug( sPortCom.toStdString().c_str() );
     // Nom du port COM à ouvrir
     //tCOMEvent.vFOpenCOMPort( sPortCom );
     this->bFOpenCOMPort( sPortCom );
    }
  }
 // Port COM manuel
 else if( ucSensorInterfaceNb == ( TUCHAR )0 )
  {
   qDebug("Pas d'interface nke de détectée");
   // Ouverture de la fenêtre de choix manuel du port com
   this->vFOpenCOMPortManualWindow();
  }
 //-------------
 // 1 FTDI nke durant INIT
 //-------------
 // Ouverture automatique
 else if( ( ucSensorInterfaceNb == ( TUCHAR )1 ) && bInit )
  {
   // Interface nke détecté
   qDebug("Une interface nke de détectée à l'init");

   this->m_uiPortComIndex = ( TUINT )this->iFComPortNumberByIndex(0);
   this->m_sPortComName = this->sFComPortNameByIndex(0);
   // Remplissage numéro de série
   ptMainData->setInterfaceSN( this->sFGetPortComSNByIndex(0) );
   // Nom du port COM à ouvrir
   this->bFOpenCOMPort( m_sPortComName );
  }
 //-------------
 // 1 FTDI nke mais PAS INIT
 //-------------
 // Ouverture automatique
 else if( ( ucSensorInterfaceNb == ( TUCHAR )1 ) && !bInit && 0 )
  {
   // Ouverture de la fenêtre de choix manuel du port com
   this->vFOpenCOMPortManualWindow();
   //sPortCom = this->sFComPortNameByIndex(0);
   // Nom du port COM à ouvrir
   //this->bFOpenCOMPort( sPortCom );
  }
 //-------------
 // 2-255 FTDI nke
 //-------------
 // Ouverture de la fenêtre choix port COM
 //else if( ucSensorInterfaceNb > ( TUCHAR )1 )
 else
  {
   // Remplissage de la fenêtre de détection avec les valeurs trouvées
   this->vFFillComPortAutomaticWindow();
   // Ouverture de la fenêtre choix port COM
   this->vFOpenCOMPortList();
  }
}

//----------------------------------------------------------
// Fonction CPP Clear de la liste des ports COM
//----------------------------------------------------------
void WindowCOMEvent::vFClearCOMPortList( void )
{
 emit vFClearCOMPortListSignal();
}

//----------------------------------------------------------
// Fonction CPP Ajout d'un port COM dans la liste
//----------------------------------------------------------
void WindowCOMEvent::vFAddCOMPort( TUCHAR ucPortCom, TUCHAR ucSN_Y, TUINT uiSN_N )
{
 // Génération de la ligne
 char tcBuff[64];
 QString sCOMPort, sSN;

 sprintf( tcBuff,
          "COM%u - %02u_%05u",
          ( unsigned int )ucPortCom,
          ( unsigned int )ucSN_Y,
          ( unsigned int )uiSN_N );
 sCOMPort = tcBuff;

 sprintf( tcBuff,
          "%02u_%05u",
          ( unsigned int )ucSN_Y,
          ( unsigned int )uiSN_N );
 sSN = tcBuff;

 emit vFAddCOMPortSignal( sCOMPort, ( int )ucPortCom, ( int )ucSN_Y, ( int )uiSN_N, sSN );
}

//----------------------------------------------------------
// Fonction CPP Ouverture de la fenêtre de sélection du port COM
//----------------------------------------------------------
void WindowCOMEvent::vFOpenCOMPortList( void )
{
 emit vFOpenCOMPortListSignal();
}

//----------------------------------------------------------
// Fonction CPP Vérification si port ouvert : réouverture sinon
//----------------------------------------------------------
Q_INVOKABLE bool WindowCOMEvent::bFReconnectPortCom( void )
{
 qDebug("== WindowCOMEvent::bFReconnectPortCom ==");
 QString sPortCOMName;
 TUINT uiPortCOM;

 qDebug( "sPortCOMName = this->sFGetPortComName()" );
 qDebug() << this->sFGetPortComName();
 // Get port COM
 sPortCOMName = this->sFGetPortComName();
 qDebug() << sPortCOMName;

 //qDebug();

 // On capture le numéro de capteur
 if( sscanf( sPortCOMName.toLatin1(), "COM%u", &uiPortCOM ) != 1 )
  {
   return( ( bool )false );
  }
 // Vérification du nombre
 if(  ( uiPortCOM != m_uiPortComIndex )
   || ( uiPortCOM > 255 ) )
  {
   // Invalide
   return( ( bool )false );
  }

 // Test d'intégrité
 if( m_serialPort.isOpen() )
  {
   // Fermeture du port au cas où
   m_serialPort.close();
  }
 //ptDashboardObj->setProperty("comPortState", "Close");
 // Tentative ouverture du port com
 if( m_serialPort.open(QIODevice::ReadWrite) )
  {
   //ptDashboardObj->setProperty("comPort", "COM1");
   //ptDashboardObj->setProperty("comPortState", "Open");
   //emit vFComPortSuccefullyOpenSignal();
   return( ( bool )true );
  }
 else
  {
   ptDataCom->setComPortState("Error");
   return( ( bool )false );
  }
}

//----------------------------------------------------------
// Fonction CPP Ouverture du port COM
//----------------------------------------------------------
Q_INVOKABLE void WindowCOMEvent::setBaudrate( int iBaudrate )
{
 switch( iBaudrate )
  {
   // 1200
   case( 1200 ):
    m_eBaudRate = QSerialPort::Baud1200;
   break;
   // 2400
   case( 2400 ):
    m_eBaudRate = QSerialPort::Baud2400;
   break;
   // 9600
   case( 4800 ):
    m_eBaudRate = QSerialPort::Baud4800;
   break;
   // 9600
   case( 9600 ):
    m_eBaudRate = QSerialPort::Baud9600;
   break;
   // 19200
   case( 19200 ):
    m_eBaudRate = QSerialPort::Baud19200;
   break;
   // 38400
   case( 38400 ):
    m_eBaudRate = QSerialPort::Baud38400;
   break;
   // 57600
   case( 57600 ):
    m_eBaudRate = QSerialPort::Baud57600;
   break;
   // 115200
   case( 115200 ):
    m_eBaudRate = QSerialPort::Baud115200;
   break;
   default:
    m_eBaudRate = QSerialPort::Baud57600;
  }
}

//----------------------------------------------------------
// Fonction CPP Ouverture du port COM
//----------------------------------------------------------
Q_INVOKABLE bool WindowCOMEvent::bFOpenCOMPort( QString sCOMPortName )
{
 // Test d'intégrité
 if( m_serialPort.isOpen() )
  {
   qDebug("Fermeture de l'ancien port COM");
   // Fermeture du port
   this->bFCloseCOMPort();
  }

 // Nom du port COM à ouvrir
 m_serialPort.setPortName( sCOMPortName );
 // Affiche le port com utilisé dans la fenêtre
 ptDataCom->setComPort(sCOMPortName);
 // Set baudrate
 m_serialPort.setBaudRate(m_eBaudRate);
 //m_serialPort.setBaudRate(QSerialPort::Baud9600);
 // Debug
 qDebug( "Tentative d'ouverture du port com" );
 // Tentative ouverture du port com
 if( m_serialPort.open(QIODevice::ReadWrite) )
  {
   //ptDashboardObj->setProperty("comPort", "COM1");
   ptDataCom->setComPortState("Open");
   emit vFComPortSuccefullyOpenSignal();
   return( ( bool )true );
  }
 else
  {
   ptDataCom->setComPortState("Error");
   return( ( bool )false );
  }
}

//----------------------------------------------------------
// Fonction CPP Fermeture du port COM
//----------------------------------------------------------
Q_INVOKABLE bool WindowCOMEvent::bFCloseCOMPort( void )
{
 // Fermeture du port
 m_serialPort.close();
 // On indique dans l'interface le changement d'état du port
 ptDataCom->setComPortState("Close");
 // On émet le signal de bonne fermeture
 emit vFComPortSuccefullyCloseSignal();
 // OK
 return( ( bool )true );
}

//----------------------------------------------------------
// Fonction CPP Ouverture de la fenêtre de sélection du port COM manuel
//----------------------------------------------------------
void WindowCOMEvent::vFOpenCOMPortManualWindow( void )
{
 qDebug("==WindowCOMEvent::vFOpenCOMPortManualWindow==");
 emit vFOpenCOMPortManualWindowSignal();
}

//----------------------------------------------------------
// Validation du port COM
//----------------------------------------------------------
Q_INVOKABLE void WindowCOMEvent::vFPortComSubmit( QString sPortCOM, unsigned int uiPortComIndex )
{
 qDebug("==vFPortComSubmit==");
 m_sPortComName = sPortCOM;
 m_uiPortComIndex = uiPortComIndex;
 // Validation de la selection du port COM
 m_bComPortSelected = true;

 // Ouverture (tentative) du port COM
 if(  this->bFOpenCOMPort( sPortCOM )
   && !ptMainData->getCOMSlowMode() )
  {
   // Test si dernière mise à jour
   ptInterfaceUpdateControler->checkUpdateAvailability(false);
  }
}

//----------------------------------------------------------
// Si le port COM a été sélectionné
//----------------------------------------------------------
bool WindowCOMEvent::bFComPortSelected( void )
{
 // Retourne l'état de la variable
 return( m_bComPortSelected );
}

//----------------------------------------------------------
// Nom du port COM sélectionné par index
//----------------------------------------------------------
QString WindowCOMEvent::sFComPortNameByIndex( TUCHAR ucIndex )
{

 if( m_ttCOMPort.length() == 0 )
  {
   return( "COM255" );
  }

 //m_ttCOMPort.
 QString sPortCom;
 unsigned char ucPortComIndex = m_ttCOMPort.at(ucIndex).m_ucPortCom;

 sPortCom = sPortCom.sprintf( "COM%d", ( unsigned int )ucPortComIndex );
 //sPortCom = "COM"+QString(ucPortComIndex);
 // Retourne l'état de la variable
 return( ( QString )sPortCom );
}

//----------------------------------------------------------
// Numéro du port COM sélectionné par index
//----------------------------------------------------------
TINT WindowCOMEvent::iFComPortNumberByIndex( TUCHAR ucIndex )
{

 if( m_ttCOMPort.length() == 0 )
  {
   return( ( TINT )-1 );
  }

 QString sPortCom;
 TUINT uiPortComIndex = m_ttCOMPort.at(ucIndex).m_ucPortCom;

 // Retourne l'état de la variable
 return( ( TINT )uiPortComIndex );
}

#endif

