//============================================================================//
// Fichier .........: "ModBusDriver_2_01.c"
// Auteur ..........: Stéphane Dutertre
// Date de création : 21/12/06
//----------------------------------------------------------------------------//
/* Description :
    Gestion du protocole ModBus.
*/
//============================================================================//
// Evolution : 2.00
// Auteur ...: SD
// Date .....: 17/04/07
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    - Permet d'envoyer des trames ModBus (mode maître)
*/
//============================================================================//
// Evolution : 2.00 Correctif
// Auteur ...: SD
// Date .....: 11/09/07
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    - Conflit entre l'émission et la réception. En cause : la valeur du define 
      "aMODBUS_TX_FUNCTION" était identique à "aMODBUS_TX_ADDRESS" ce qui 
      se traduisait par un doublement de n° d'étape dans l'émission. 
      Conséquences : la fonction "bFModBusDriverIsTransmitting" revoyait faux,
      à l'étape "aMODBUS_TX_FUNCTION" (au lieu de vrai) ce qui ré-autorisait la 
      réception avant la fin de l'émission.
*/
//============================================================================//
// Evolution : 2.01
// Auteur ...: SD
// Date .....: 13/06/08
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    - Permet une redéclaration plus simple du composant ModBus.
*/
//============================================================================//
// Evolution : 2.02
// Auteur ...: Stéphane DUTERTRE
// Date .....: 18/10/10
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Ajout du fichier de Data.h
*/
//============================================================================//
// Evolution : 2.03
// Auteur ...: Stéphane DUTERTRE
// Date .....: 04/05/11
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Changement de l'adresse du ModBus sur un type variable.
    Evenement (macro) si activité sur le bus (trame ok)
*/
//============================================================================//
// Evolution : 2.04
// Auteur ...: Stéphane DUTERTRE
// Date .....: 28/06/13
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Changement d'une directive du composant ModBus (réception de caratère) afin d'être indépendant de la FIFO.
*/
//============================================================================//
// Evolution : Amélioration
// Auteur ...: Stéphane DUTERTRE
// Date .....: 08/07/13
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Modification de la fonction "vFModBusDriverInitialization" afin de réinitialiser les temporisations.
*/
//============================================================================//

#ifndef MODBUS_DRIVER_2_04_C
#define MODBUS_DRIVER_2_04_C

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeCMMN.h>
#include <TypeC.h>
#if( !aMODBUS_REDEFINITION )
 #include <ModBusDriver_2_04.h>
#endif
#include <string.h>

//============================================================================//
// Définition des contantes
//============================================================================//
// Les étapes de réception
#define aMODBUS_RX_ADDRESS_L ( 0 )
#if( !aMODBUS_DRIVER_16_BITS_ADDRESS )
 #define aMODBUS_RX_FUNCTION ( aMODBUS_RX_ADDRESS_L  + 1 )
#else
 #define aMODBUS_RX_ADDRESS_H ( aMODBUS_RX_ADDRESS_L + 1 )
 #define aMODBUS_RX_FUNCTION  ( aMODBUS_RX_ADDRESS_H + 1 )
#endif
#define aMODBUS_RX_DATA      ( aMODBUS_RX_FUNCTION   + 1 )
#define aMODBUS_RX_CRC_L     ( aMODBUS_RX_DATA       + 1 )
#define aMODBUS_RX_CRC_H     ( aMODBUS_RX_CRC_L      + 1 )
#define aMODBUS_RX_SCHEDULER ( aMODBUS_RX_CRC_H      + 1 )

#define aMODBUS_RX_ADDRESS   aMODBUS_RX_ADDRESS_L
#define aMODBUS_TX_ADDRESS   aMODBUS_RX_ADDRESS_L

// Les étapes d'émission
#if( !aMODBUS_DRIVER_16_BITS_ADDRESS )
 #define aMODBUS_TX_FUNCTION ( aMODBUS_TX_ADDRESS    + 1 )
#else
 #define aMODBUS_TX_ADDRESS_H ( aMODBUS_TX_ADDRESS   + 1 )
 #define aMODBUS_TX_FUNCTION  ( aMODBUS_TX_ADDRESS_H + 1 )
#endif
#define aMODBUS_TX_DATA     ( aMODBUS_TX_FUNCTION    + 1 )
#define aMODBUS_TX_CRC_L    ( aMODBUS_TX_DATA        + 1 )
#define aMODBUS_TX_CRC_H    ( aMODBUS_TX_CRC_L       + 1 )
#define aMODBUS_TX_END      ( aMODBUS_TX_CRC_H       + 1 )
#if aMODBUS_MASTER_MODE
 #define aMODBUS_TX_WAIT    ( aMODBUS_TX_END         + 1 )
#endif

//============================================================================//
// Déclaration des types
//============================================================================//
typedef struct
{
 TUCHAR  ucNet;
 #if( __bFModBusDriverWakeUp__ )
  TUXTYPE_MODBUS uxTimer;
 #endif 
 TUXTYPE_MODBUS uxTimerOnRxChar;
 TUCHAR  ucIndex;
 TUINT   uiCRC;
}
TModBusDriverRxObject;

typedef struct
{
 TUCHAR  ucNet;
 #if( aMODBUS_MASTER_MODE )
  TUCHAR ucResult;
  TUXTYPE_MODBUS uxTimer;
 #endif
 TUCHAR  ucIndex;
 TUINT   uiCRC;
}
TModBusDriverTxObject;

//============================================================================//
// Déclaration des variables non exportées
//============================================================================//
// Configuration du ModBus esclave
static TUXTYPE_MODBUS_ADDRESS uxModBusAddress = ( TUXTYPE_MODBUS_ADDRESS )aMODBUS_ADDRESS_BROADCAST;
// On gère 2 objets d'émissions et de réception
static TModBusDriverRxObject tModBusDriverRx;
static TModBusDriverTxObject tModBusDriverTx;
// Variable de gestion du ModBus
static TModBusDriverDataObject tModBusDriverDataRx, tModBusDriverDataTx;

//============================================================================//
// Déclaration des macros
//============================================================================//
// Voir fichier de configuration de FIFOModBusDriver
vMFIFOModBusDriverPushVirtualFunction();

//============================================================================//
// Déclaration des prototypes de fonctions exportées
//============================================================================//
//----------------------------------------------------------------------------//
// On fixe l'adresse du récepteur
#if( __bFModBusDriverSetAddress__ )
static TBOOL bFModBusDriverSetAddress( TUXTYPE_MODBUS_ADDRESS );
#endif
//----------------------------------------------------------------------------//
// On initialise les données de la couche et de celle en dessous
#if( __vFModBusDriverInitialization__ )
static void vFModBusDriverInitialization( void );
#endif
//----------------------------------------------------------------------------//
// On regarde si la transmission est libre
#if( __bFModBusDriverIsTransmitting__ )
static TBOOL bFModBusDriverIsTransmitting( void );
#endif
//----------------------------------------------------------------------------//
// On regarde si une transmission est en cours
#if( __bFModBusDriverTransmitter__ || __vGModBusDriverSender__ ) && aMODBUS_MASTER_MODE
static TBOOL bFModBusDriverTransmitter( TUXTYPE_MODBUS_ADDRESS , TUCHAR , TUCHAR [], TUCHAR );
#endif
//----------------------------------------------------------------------------//
// On regarde si une transmission est en cours
#if( __vGModBusDriverSender__ || __bFModBusDriverIsTransmitting__ )
static TBOOL bFModBusDriverSendFrame( void );
#endif
//----------------------------------------------------------------------------//
// Emission des trames ModBus
#if( __vGModBusDriverSender__ )
static void vGModBusDriverSender( void );
#endif
//----------------------------------------------------------------------------//
// On réveille la réception (pour un certain temps)
#if( __bFModBusDriverWakeUp__ )
static TBOOL bFModBusDriverWakeUp( void );
#endif
//----------------------------------------------------------------------------//
// Gestionnaire de temps de l'application
#if( __vFModBusDriverTick__ )
static void vFModBusDriverTick( void );
#endif
//----------------------------------------------------------------------------//
// Indique si le protocole est actif
#if( __bFModBusDriverActive__ )
static TBOOL bFModBusDriverActive( void );
#endif
//----------------------------------------------------------------------------//
/// \brief On regarde si une donnée est arrivée
#if( __bFModBusDriverHasResponse__ && aMODBUS_MASTER_MODE )
static TBOOL bFModBusDriverHasResponse( void );
#endif
//----------------------------------------------------------------------------//
// \brief Lecture de la réponse
#if( __ucFModBusDriverReadResponse__ && aMODBUS_MASTER_MODE )
static TUCHAR ucFModBusDriverReadResponse( TModBusDriverDataObject ** );
#endif
//----------------------------------------------------------------------------//
// Réception des trames ModBus
#if( __bGModBusDriverReceiver__ )
static TBOOL bGModBusDriverReceiver( void );
#endif
//------------------------------------------------------------------------------
// Gestion des automates ModBus
#if( __bGModBusDriverManager__ )
static TBOOL bGModBusDriverManager( void );
#endif

//------------------------------------------------------------------------------
// Gestion des automates ModBus
static TBOOL bFModBusDriverSendingFrameIsReady( void );


//============================================================================//
// Déclaration des variables exportées
//============================================================================//
TCONST TModBusObject tModBus = 
{
  // Obligatoire en mode esclave
 #if( __bFModBusDriverSetAddress__ && !aMODBUS_MASTER_MODE )
  bFModBusDriverSetAddress,
 #endif
 // Obligatoire
 // vFTick
 #if __vFModBusDriverTick__
  vFModBusDriverTick,
 #endif
 // vFOnRxChar
 // Obligatoire :: Voir fichier de configuration de FIFOModBusDriver
 #if __vFModBusDriverReceive__
  vMFIFOModBusDriverPush,
 #endif
 // vGOnTxEmpty
 // Obligatoire
 #if __vGModBusDriverSender__
  vGModBusDriverSender,
 #endif
 // bFOnRxFront
 // Peut ne pas être utile si le système ne passe jamais en sommeil
 #if( __bFModBusDriverWakeUp__ )
  bFModBusDriverWakeUp,
 #endif
 // bFIsRunning
 // Peut ne pas être utile
 #if( __bFModBusDriverActive__ )
  bFModBusDriverActive,
 #endif
 // bFResponse
 // Indique si l'on a reçu une trame
 #if( __bFModBusDriverHasResponse__ && aMODBUS_MASTER_MODE )
  bFModBusDriverHasResponse,
 #endif
 // ucFReadResponse
 // On lit la donnée reçu
 #if( __ucFModBusDriverReadResponse__ && aMODBUS_MASTER_MODE )
  ucFModBusDriverReadResponse,
 #endif
 // bFSend
 // Peut ne pas être utile
 #if( __bFModBusDriverTransmitter__ && aMODBUS_MASTER_MODE )
  bFModBusDriverTransmitter,
 #endif
 // bGScheduler
 // Obligatoire
 #if __bGModBusDriverManager__
  bGModBusDriverManager,
 #endif
 // vFInitialization
 // Obligatoire
 #if __vFModBusDriverInitialization__
  vFModBusDriverInitialization,
 #endif,
 #endif
 // vFInitialization
 // Obligatoire
 bFModBusDriverSendingFrameIsReady
};

//============================================================================//
// Déclaration des prototypes de fonctions non exportées
//============================================================================//

//****************************************************************************//
// Le code des fonctions
//****************************************************************************//

//------------------------------------------------------------------------------
// Calcul du CRC
//------------------------------------------------------------------------------
static TUINT uiFModBusDriverCRC( TUCHAR ucData, TUINT uiCRC )
{
 register TUCHAR ucCpt;

 uiCRC ^= ( TUINT )ucData;
 for( ucCpt = ( TUCHAR )0; ucCpt < ( TUCHAR )8; ucCpt++ )
  {
   if( uiCRC & ( TUINT )0x0001 )
    {
     uiCRC >>= ( TUINT )0x0001;
     uiCRC ^= ( TUINT )0xA001;
    }
   else uiCRC >>= ( TUINT )0x0001;
  }
 return uiCRC;
}

//----------------------------------------------------------------------------//
// On fixe l'adresse du récepteur
//----------------------------------------------------------------------------//
#if( __bFModBusDriverSetAddress__ )
static TBOOL bFModBusDriverSetAddress( TUXTYPE_MODBUS_ADDRESS uxAddress )
{
 if(   ( uxAddress > ( TUXTYPE_MODBUS_ADDRESS )aMODBUS_ADDRESS_BROADCAST ) 
    && ( uxAddress < ( TUXTYPE_MODBUS_ADDRESS )aMODBUS_ADDRESS_RESERVED  ) )
  {
   // On fixe l'adresse de l'esclave
   uxModBusAddress = uxAddress;
   // Ok
   return( ( TBOOL )aTRUE );
  }
 // Ok
 return( ( TBOOL )aFALSE );
}
#endif

//----------------------------------------------------------------------------//
// On initialise les données de la couche et de celle en dessous
//----------------------------------------------------------------------------//
#if( __vFModBusDriverInitialization__ )
static void vFModBusDriverInitialization( void )
{
 // On initialise la FIFO
 tFIFOModBusDriver.vFInitialization();
 // On active l'UART
 vMModBusDriverUartInitialization();
 // On active la réception
 vMModBusDriverUartRxTx( aMODBUS_INTERFACE_RX_ENABLED );
 // on initialise les n° d'étape
 tModBusDriverRx.ucNet = ( TUCHAR )aMODBUS_RX_ADDRESS;
 tModBusDriverTx.ucNet = ( TUCHAR )aMODBUS_TX_ADDRESS;
 #if( aMODBUS_MASTER_MODE )
  tModBusDriverTx.ucResult = ( TUCHAR )aMODBUS_CLIENT_DATA_NONE;
 #endif
 #if !aMODBUS_MASTER_MODE
  // On teste si la tempo de réception est nulle (inactivité en rcv) ou si on transmet
  tModBusDriverRx.uxTimer = ( TUXTYPE_MODBUS )0;
 #else
  // On teste si la tempo de réception est nulle (inactivité en rcv) ou si on transmet
  tModBusDriverTx.uxTimer = ( TUXTYPE_MODBUS )0;
 #endif 
}
#endif

//----------------------------------------------------------------------------//
// On regarde si la transmission est libre
//----------------------------------------------------------------------------//
#if( __bFModBusDriverIsTransmitting__ )
static TBOOL bFModBusDriverIsTransmitting( void )
{
 // On teste si l'automate de transmission et de réception sont libre
 return( ( TBOOL )(   ( tModBusDriverTx.ucNet > ( TUCHAR )aMODBUS_TX_ADDRESS )
 #if( aMODBUS_MASTER_MODE )
  // On teste si l'automate de transmission et de réception sont libre
                   && ( tModBusDriverTx.ucNet < ( TUCHAR )aMODBUS_TX_WAIT )
 #endif
  ) ); 
}
#endif

//----------------------------------------------------------------------------//
// On regarde si une transmission est en cours
//----------------------------------------------------------------------------//
#if( __bFModBusDriverTransmitter__ || __vGModBusDriverSender__ ) && aMODBUS_MASTER_MODE
static TBOOL bFModBusDriverTransmitter( TUXTYPE_MODBUS_ADDRESS uxAddress, TUCHAR ucFunction, TUCHAR tucData[], TUCHAR ucSize )
{
 // On vérifie la disponibilité de la liaison
 if( tModBusDriverTx.ucNet != ( TUCHAR )aMODBUS_TX_ADDRESS ) return( ( TBOOL )aFALSE );
 // On vérifie que la taille est cohérente
 if( ucSize > sizeof( tModBusDriverDataTx.tucData ) ) return( ( TBOOL )aFALSE );
 // On configure l'interface pour l'émission
 vMModBusDriverUartRxTx( aMODBUS_INTERFACE_TX_ENABLED );
 // On note la fonction dans la config
 tModBusDriverDataTx.ucFunction = ucFunction;
 // On note la taille des données à émettre
 if( ( tModBusDriverDataTx.ucSize = ucSize ) > ( TUINT )0 )
  // On copie les données
  memcpy( tModBusDriverDataTx.tucData, tucData, ucSize );
 // On fixe le time out
 tModBusDriverTx.uxTimer  = ( TUXTYPE_MODBUS )aMODBUS_TIMER_TX_TIME_OUT;
 // On note l'adresse
 uxModBusAddress          = uxAddress;
 // On force la réinitialisation des données pour la prochaine trame
 tModBusDriverTx.ucResult = ( TUCHAR )aMODBUS_CLIENT_DATA_NONE;
 // Les étapes d'émission
 #if( !aMODBUS_DRIVER_16_BITS_ADDRESS )
  // On appèle la routine
  tModBusDriverTx.uiCRC   = uiFModBusDriverCRC( uxModBusAddress, ( TUINT )0xFFFF );



  // On émet le premier octet de la trame FANION
  vMModBusDriverUartWrite( uxModBusAddress );
  // On passe à l'étape d'émission  des octets d'adresse
  tModBusDriverTx.ucNet   = ( TUCHAR )aMODBUS_TX_FUNCTION;

//%%AP - 2021.09.30 - Modificattion pour envoi de trame d'un coup
  while( tModBusDriverTx.ucNet != ( TUCHAR )aMODBUS_TX_END )
   {
    tModBus.vGOnTxEmpty();
   }
//%%AP - Fin

 #else
  // On appèle la routine
  tModBusDriverTx.uiCRC   = uiFModBusDriverCRC( uxModBusAddress / ( TUINT )256, ( TUINT )0xFFFF );
 // On émet le premier octet de la trame FANION
 vMModBusDriverUartWrite( uxModBusAddress / ( TUINT )256 );
  // On passe à l'étape d'émission des octets d'adresse
  tModBusDriverTx.ucNet   = ( TUCHAR )aMODBUS_TX_ADDRESS_H;
 #endif
 // Ok
 return( ( TBOOL )aTRUE );
}
#endif

//----------------------------------------------------------------------------//
// On regarde si une transmission est en cours
//----------------------------------------------------------------------------//
#if( __vGModBusDriverSender__ || __bFModBusDriverIsTransmitting__ )
static TBOOL bFModBusDriverSendFrame( void )
{
 // On vérifie la disponibilité de la liaison
 if( tModBusDriverTx.ucNet != ( TUCHAR )aMODBUS_TX_ADDRESS ) return( ( TBOOL )aFALSE );
 // On vérifie que la taille est cohérente
 if( tModBusDriverDataTx.ucSize > sizeof( tModBusDriverDataTx.tucData ) ) return( ( TBOOL )aFALSE );
 // On configure l'interface pour l'émission
 vMModBusDriverUartRxTx( aMODBUS_INTERFACE_TX_ENABLED );
 // Les étapes d'émission
 #if( !aMODBUS_DRIVER_16_BITS_ADDRESS )
  // On appèle la routine
  tModBusDriverTx.uiCRC = uiFModBusDriverCRC( uxModBusAddress, ( TUINT )0xFFFF );
  // On passe à l'étape d'émission  des octets d'adresse
  tModBusDriverTx.ucNet = ( TUCHAR )aMODBUS_TX_FUNCTION;
  // On émet le premier octet de la trame FANION
  vMModBusDriverUartWrite( uxModBusAddress );

//%%AP - 2021.09.30 - Modificattion pour envoi de trame d'un coup
  while( !tModBus.bFSendingFrameIsReady() )
   {
    tModBus.vGOnTxEmpty();
   }
//%%AP - Fin


 #else
  // On appèle la routine
  tModBusDriverTx.uiCRC = uiFModBusDriverCRC( uxModBusAddress / ( TUINT )256, ( TUINT )0xFFFF );
  // On passe à l'étape d'émission des octets d'adresse
  tModBusDriverTx.ucNet = ( TUCHAR )aMODBUS_TX_ADDRESS_H;
  // On émet le premier octet de la trame FANION
  vMModBusDriverUartWrite( uxModBusAddress / ( TUINT )256 );
 #endif
 // Ok
 return( ( TBOOL )aTRUE );
}
#endif

//----------------------------------------------------------------------------//
// Emission des trames ModBus
//----------------------------------------------------------------------------//
#if( __vGModBusDriverSender__ )
static void vGModBusDriverSender( void )
{
 register TUCHAR ucCharTx;

 // Suivant les étapes de l'automate d'émission
 switch( tModBusDriverTx.ucNet )
  {
   // Les étapes d'émission
   #if( aMODBUS_DRIVER_16_BITS_ADDRESS )
   //---------------------------------------------------------------------------
   // On émet la fonction
   //---------------------------------------------------------------------------
   case ( TUCHAR )aMODBUS_TX_ADDRESS_H :
    // on mémorise l'octet à émettre
    ucCharTx = ( TUCHAR )( uxModBusAddress % ( TUINT )256 );
    // On émet l'octet de la trame
    vMModBusDriverUartWrite( ucCharTx );   
    // On passe à l'étape d'émission des données
    tModBusDriverTx.ucNet   = ( TUCHAR )aMODBUS_TX_FUNCTION;
   break;
  #endif
   //---------------------------------------------------------------------------
   // On émet la fonction
   //---------------------------------------------------------------------------
   case ( TUCHAR )aMODBUS_TX_FUNCTION :
    // on mémorise l'octet à émettre
    ucCharTx = tModBusDriverDataTx.ucFunction;
    // On émet l'octet de la trame
    vMModBusDriverUartWrite( ucCharTx );   
    // On initialise l'index
    tModBusDriverTx.ucIndex = ( TUCHAR )0;
    // On passe à l'étape d'émission des données
    tModBusDriverTx.ucNet   = ( TUCHAR )aMODBUS_TX_DATA;
    // On regarde si la taille est nulle ou pas
    if( !tModBusDriverDataTx.ucSize )
     // On passe à l'étape d'émission du CRC (LSB)
     tModBusDriverTx.ucNet  = ( TUCHAR )aMODBUS_TX_CRC_L;
   break;
   //---------------------------------------------------------------------------
   // On émet les données
   //---------------------------------------------------------------------------
   case ( TUCHAR )aMODBUS_TX_DATA :
    // on mémorise l'octet à émettre
    ucCharTx = tModBusDriverDataTx.tucData[ tModBusDriverTx.ucIndex ];
    // On émet les données
    vMModBusDriverUartWrite( ucCharTx );
    // On vérifie que l'on a envoyé tous les octets
    if( ++tModBusDriverTx.ucIndex >= tModBusDriverDataTx.ucSize )
     // On passe à l'étape d'émission du CRC (LSB)
     tModBusDriverTx.ucNet = ( TUCHAR )aMODBUS_TX_CRC_L;
   break;
   //---------------------------------------------------------------------------
   // On émet le CRC L
   //---------------------------------------------------------------------------
   case ( TUCHAR )aMODBUS_TX_CRC_L :
    #if( aMODBUS_REAL_CRC_SPECIFICATION )
     // On émet le CRC poid faible
     vMModBusDriverUartWrite( ( TUCHAR )( tModBusDriverTx.uiCRC % ( TUINT )256) );
    #else
     // On émet le CRC poid fort
     vMModBusDriverUartWrite( ( TUCHAR )( tModBusDriverTx.uiCRC / ( TUINT )256) );
    #endif    
    // On passe à l'étape d'émission du CRC (MSB)
    tModBusDriverTx.ucNet = ( TUCHAR )aMODBUS_TX_CRC_H;
   return;
   //---------------------------------------------------------------------------
   // On émet le CRC H
   //---------------------------------------------------------------------------
   case ( TUCHAR )aMODBUS_TX_CRC_H :
    #if( aMODBUS_REAL_CRC_SPECIFICATION )
     // On émet le CRC poid fort
     vMModBusDriverUartWrite( ( TUCHAR )( tModBusDriverTx.uiCRC / ( TUINT )256 ) );
    #else
     // On émet le CRC poid faible
     vMModBusDriverUartWrite( ( TUCHAR )( tModBusDriverTx.uiCRC % ( TUINT )256 ) );
    #endif    
    // On passe à l'étape d'attente
    tModBusDriverTx.ucNet = ( TUCHAR )aMODBUS_TX_END;
    //%%AP - 2020.03.12 - Pour debug
    qDebug( "=========== Fin d'envoi ============" );
   return;
   //---------------------------------------------------------------------------
   // On repasse à l'étape d'attente une fois le dernier octet transmis
   // L'appel à TX_END est effectué lorsque l'octet CRC à été transmis
   //---------------------------------------------------------------------------
   case ( TUCHAR )aMODBUS_TX_END :
    // On active la réception
    vMModBusDriverUartRxTx( aMODBUS_INTERFACE_RX_ENABLED );
    #if( !aMODBUS_MASTER_MODE )
     // On passe à l'étape de fin
     tModBusDriverTx.ucNet = ( TUCHAR )aMODBUS_TX_ADDRESS;
    #else 
     // On passe à l'étape de fin
     tModBusDriverTx.ucNet = ( TUCHAR )aMODBUS_TX_WAIT;
    #endif
   return;
   //---------------------------------------------------------------------------
   // Si erreur grave
   //---------------------------------------------------------------------------
   default :
    // On active la réception
    vMModBusDriverUartRxTx( aMODBUS_INTERFACE_RX_ENABLED );
    // On passe à l'étape de fin
    tModBusDriverTx.ucNet = ( TUCHAR )aMODBUS_TX_ADDRESS;
   return;
  }
 // On calcule le CRC
 tModBusDriverTx.uiCRC = uiFModBusDriverCRC( ucCharTx, tModBusDriverTx.uiCRC );
}
#endif

//----------------------------------------------------------------------------//
// On réveille la réception (pour un certain temps)
//----------------------------------------------------------------------------//
#if( __bFModBusDriverWakeUp__ )
static TBOOL bFModBusDriverWakeUp( void )
{
 // On vérifie que l'automate est inactif
 if( tModBusDriverRx.uxTimer > ( TUXTYPE_MODBUS )0 ) return( ( TBOOL )aFALSE );
 // On lance le timer
 tModBusDriverRx.uxTimer = ( TUXTYPE_MODBUS )aMODBUS_TIMER_RX_TIME_OUT;
 // Ok l'automate de réception est relancé
 return( ( TBOOL )aTRUE );
}
#endif

//----------------------------------------------------------------------------//
// Gestionnaire de temps de l'application
//----------------------------------------------------------------------------//
#if( __vFModBusDriverTick__ )
static void vFModBusDriverTick( void )
{
 // on décrémente la variable de temporisation du time-out de réception
 if( tModBusDriverRx.uxTimerOnRxChar ) tModBusDriverRx.uxTimerOnRxChar--;
 // Mode maître
 #if( aMODBUS_MASTER_MODE )
  // Gestion du mode maître
  if( tModBusDriverTx.uxTimer ) tModBusDriverTx.uxTimer--;
 #else
  // on décrémente la variable de temporisation
  if( tModBusDriverRx.uxTimer ) tModBusDriverRx.uxTimer--;
 #endif
}
#endif

//----------------------------------------------------------------------------//
// Indique si le protocole est actif
//----------------------------------------------------------------------------//
#if( __bFModBusDriverActive__ )
static TBOOL bFModBusDriverActive( void )
{
 #if !aMODBUS_MASTER_MODE
  // On teste si la tempo de réception est nulle (inactivité en rcv) ou si on transmet
  return( ( TBOOL )(   ( tModBusDriverRx.uxTimer > ( TUXTYPE_MODBUS )0         ) 
                    || ( tModBusDriverRx.ucNet != ( TUCHAR )aMODBUS_RX_ADDRESS ) ) );
 #else
  // On teste si la tempo de réception est nulle (inactivité en rcv) ou si on transmet
  return( ( TBOOL )(   ( tModBusDriverTx.uxTimer >  ( TUXTYPE_MODBUS )0          ) 
                    || ( tModBusDriverTx.ucNet   != ( TUCHAR )aMODBUS_TX_ADDRESS ) ) );
 #endif
}
#endif

//----------------------------------------------------------------------------//
/// \brief On regarde si une donnée est arrivée
//----------------------------------------------------------------------------//
#if( __bFModBusDriverHasResponse__ && aMODBUS_MASTER_MODE )
static TBOOL bFModBusDriverHasResponse( void )
{
 // On retourne l'info
 return( ( TBOOL )( tModBusDriverTx.ucResult != ( TUCHAR )aMODBUS_CLIENT_DATA_NONE ) );
}
#endif

//----------------------------------------------------------------------------//
// \brief Lecture de la réponse
//----------------------------------------------------------------------------//
#if( __ucFModBusDriverReadResponse__ && aMODBUS_MASTER_MODE )
static TUCHAR ucFModBusDriverReadResponse( TModBusDriverDataObject ** ptData )
{
 register TUCHAR ucResult;

 // On mémorise l'info
 ucResult = tModBusDriverTx.ucResult;
 // On resette le flag
 tModBusDriverTx.ucResult = ( TUCHAR )aMODBUS_CLIENT_DATA_NONE;
 // Si on a reçu une réponse on la retourne
 if( ucResult == ( TUCHAR )aMODBUS_CLIENT_DATA_RECEIVED )
  *ptData = &tModBusDriverDataRx;
 else
  *ptData = NULL;
 // Erreur
 return( ucResult );
}
#endif

//----------------------------------------------------------------------------//
// Réception des trames ModBus
//----------------------------------------------------------------------------//
#if( __bGModBusDriverReceiver__ )
static TBOOL bGModBusDriverReceiver( void )
{
 register TUCHAR                 ucCharRx;
 static   TUXTYPE_MODBUS_ADDRESS uxAddress;

 // On vérifie qu'il y a un scheduler pour le serveur
 #if( __ucMModBusDriverServerScheduler__ && !aMODBUS_MASTER_MODE )
 if( tModBusDriverRx.ucNet == ( TUCHAR )aMODBUS_RX_SCHEDULER )
  {
   // on regarde si le scheduler a une réponse
   switch( ucMModBusDriverServerScheduler( &tModBusDriverDataTx ) )
    {
     // On doit attendre alors on attend
     case ( TUCHAR )aMODBUS_SERVER_WAIT     : 
     return( ( TBOOL )aTRUE );
     // On doit répondre maintenant
     case ( TUCHAR )aMODBUS_SERVER_TRANSMIT :
      // Dans tous les cas on passe à l'état repos
      tModBusDriverRx.ucNet = ( TUCHAR )aMODBUS_RX_ADDRESS;
      // On appèle la fonction d'émission
      if(   ( uxAddress != ( TUCHAR )aMODBUS_ADDRESS_BROADCAST )
         // On ne peut émettre de trame que si l'on n'a pas été interrogé en broadcast
         && bFModBusDriverSendFrame()                          ) return( ( TBOOL )aTRUE );
     // trame non valide on passe à l'étape de réception
     break;
     // le scheduler nous retourne une erreur on appèle l'automate de réception
     default :
      // Dans tous les cas on passe à l'état repos
      tModBusDriverRx.ucNet = ( TUCHAR )aMODBUS_RX_ADDRESS;
     break;
    }
  }
 #endif
 #if( !aMODBUS_MASTER_MODE )
  // Si time out alors on réinitialise l'automate
  if( !tModBusDriverRx.uxTimer ) tModBusDriverRx.ucNet = ( TUCHAR )aMODBUS_RX_ADDRESS;
 #else
  // Si time out alors on réinitialise l'automate
  if( tModBusDriverTx.ucNet != ( TUCHAR )aMODBUS_TX_ADDRESS )
   {
    // On test le time out
    if( !tModBusDriverTx.uxTimer )
     {
      // On réinitialise l'automate d'émission & de réception
      tModBusDriverTx.ucNet    = ( TUCHAR )aMODBUS_TX_ADDRESS;
      // On réinitialise la réception
      tModBusDriverRx.ucNet    = ( TUCHAR )aMODBUS_RX_ADDRESS;
      // TimeOut
      tModBusDriverTx.ucResult = ( TUCHAR )aMODBUS_CLIENT_DATA_TIME_OUT;
     }
   }
 #endif
 // On vérifie qu'il y a un octet dans la FIFO
 while( tFIFOModBusDriver.bFData() )
  {
  // Si time out de réception alors on réinitialise l'automate
  if( !tModBusDriverRx.uxTimerOnRxChar ) tModBusDriverRx.ucNet = ( TUCHAR )aMODBUS_RX_ADDRESS;
   #if( !aMODBUS_MASTER_MODE )
    // On lance le timer - permet de passer en veille si le délai sans réception est dépassé
    tModBusDriverRx.uxTimer = aMODBUS_TIMER_RX_TIME_OUT;
   #endif
   // On lance le timer - permet de réinitialiser la réception si le délai de réception entre 2 octets est dépassé
   tModBusDriverRx.uxTimerOnRxChar = aMODBUS_TIMER_RX_COMMUNICATION_TIME_OUT;
   // On dépope les octets
   ucCharRx = tFIFOModBusDriver.ucFPop();
   // On gère les états
   switch( tModBusDriverRx.ucNet )
    {
     //-------------------------------------------------------------------------
     // Etape d'attente de l'adresse (1 seul octet)
     //-------------------------------------------------------------------------
     default :
      // On réinitialise le CRC
      tModBusDriverRx.uiCRC = ( TUINT )0xFFFF;
      #if( !aMODBUS_DRIVER_16_BITS_ADDRESS )
       // On enregistre l'adresse
       uxAddress = ucCharRx;
       // On va récupérer la fonction
       tModBusDriverRx.ucNet = ( TUCHAR )aMODBUS_RX_FUNCTION;
      #else 
       // On enregistre l'adresse Poids forts
       uxAddress = ( TUINT )ucCharRx * ( TUINT )256;
       // On passe à l'étape de fin
       tModBusDriverRx.ucNet = ( TUCHAR )aMODBUS_RX_ADDRESS_H;
     break;  
     //-------------------------------------------------------------------------
     // Etape de récupération de la fonction
     //-------------------------------------------------------------------------
     case ( TUCHAR )aMODBUS_RX_ADDRESS_H :
       // On enregistre l'adresse Poids forts
       uxAddress += ( TUINT )ucCharRx;
       // On va récupérer la fonction
       tModBusDriverRx.ucNet = ( TUCHAR )aMODBUS_RX_FUNCTION;
      #endif
     break;
     //-------------------------------------------------------------------------
     // Etape de récupération de la fonction
     //-------------------------------------------------------------------------
     case ( TUCHAR )aMODBUS_RX_FUNCTION :
      // On enregsitre
      tModBusDriverDataRx.ucFunction = ucCharRx;
      // On réinitialise le compteur
      tModBusDriverRx.ucIndex        = ( TUCHAR )0;
      // On récupère la position
      if( !( tModBusDriverDataRx.ucPosition = ucMModBusDriverGetSizePosition( tModBusDriverDataRx.ucFunction ) ) )
       {
        // Test d'intégrité on vérifie que la taille reçue n'est pas supérieure au buffer
        if( ( tModBusDriverDataRx.ucSize = ucMModBusDriverGetMessageSize( tModBusDriverDataRx.ucFunction ) ) > sizeof( tModBusDriverDataRx.tucData ) )
         {
          // Erreur on réinitialise
          tModBusDriverRx.ucNet      = ( TUCHAR )aMODBUS_RX_ADDRESS;
          // On reboucle
          continue;
         }
        // On vérifie si la taille du message est nulle ou pas
        if( !tModBusDriverDataRx.ucSize )
         {
          // On passe directement au CRC        
          tModBusDriverRx.ucNet      = ( TUCHAR )aMODBUS_RX_CRC_L;
          // On sort ici
          break;
         }
       }
      // On regarde si la position est nulle alors c'est lié au message
      // On change d'étape
      tModBusDriverRx.ucNet          = ( TUCHAR )aMODBUS_RX_DATA;
     break;
     //-------------------------------------------------------------------------
     // Etape des données
     //-------------------------------------------------------------------------
     case ( TUCHAR )aMODBUS_RX_DATA :
      // On regarde s'il y a une position pour lire la taille
      if( tModBusDriverDataRx.ucPosition > ( TUCHAR )0 )
       {
        // Si la taille est nulle alors on a la bonne taille
        if( --tModBusDriverDataRx.ucPosition > ( TUCHAR )0 ) 
         {
          // On mémorise les données reçues
          tModBusDriverDataRx.tucData[ tModBusDriverRx.ucIndex ] = ucCharRx;
          // On vérifie que l'on reçoit toute la trame
          if( ++tModBusDriverRx.ucIndex >= sizeof( tModBusDriverDataRx.tucData ) )
           {
            // Erreur on réinitialise
            tModBusDriverRx.ucNet = ( TUCHAR )aMODBUS_RX_ADDRESS;
            // On reboucle
            continue;
           }         
          // On calcule le CRC
          break;
         }
        // On récupère la taille
        tModBusDriverDataRx.ucSize = ucCharRx + tModBusDriverRx.ucIndex + ( TUCHAR )1;
       }
      // On mémorise les données reçues
      tModBusDriverDataRx.tucData[ tModBusDriverRx.ucIndex ] = ucCharRx;
      // On vérifie que l'on reçoit toute la trame
      if( ++tModBusDriverRx.ucIndex >= tModBusDriverDataRx.ucSize )
       // On change d'étape
       tModBusDriverRx.ucNet = ( TUCHAR )aMODBUS_RX_CRC_L;
     break;
     //-------------------------------------------------------------------------
     // Test du CRC Low
     //-------------------------------------------------------------------------
     case ( TUCHAR )aMODBUS_RX_CRC_L :
      // On est optimiste
      tModBusDriverRx.ucNet = ( TUCHAR )aMODBUS_RX_CRC_H;
      // On teste la validité du CRC
      #if( aMODBUS_REAL_CRC_SPECIFICATION )
       if( ucCharRx != ( TUCHAR )( tModBusDriverRx.uiCRC % ( TUINT )256 ) )
      #else
       if( ucCharRx != ( TUCHAR )( tModBusDriverRx.uiCRC / ( TUINT )256 ) )
      #endif
        // C'est une erreur arrête
        tModBusDriverRx.ucNet = ( TUCHAR )aMODBUS_RX_ADDRESS;
     // On reboucle
     continue;
     //-------------------------------------------------------------------------
     // Test du CRC High
     //-------------------------------------------------------------------------
     case ( TUCHAR )aMODBUS_RX_CRC_H :
      // On est optimiste
      tModBusDriverRx.ucNet = ( TUCHAR )aMODBUS_RX_ADDRESS;
      // On teste la validité du CRC
      #if( aMODBUS_REAL_CRC_SPECIFICATION )
       if( ucCharRx == ( TUCHAR )( tModBusDriverRx.uiCRC / ( TUINT )256 ) )
      #else
       if( ucCharRx == ( TUCHAR )( tModBusDriverRx.uiCRC % ( TUINT )256 ) )
      #endif
       {
        // Pour activité
        vMModBusDriverActivityOnBusEvent();
        // On vérifie que le message est bien pour nous
        if(   ( uxAddress != uxModBusAddress                      ) 
           && ( uxAddress != ( TUCHAR )aMODBUS_ADDRESS_BROADCAST  ) )
         // On boucle
         continue;
        // On libère la ressource
        tModBusDriverTx.ucNet = ( TUCHAR )aMODBUS_TX_ADDRESS;
        // Suivant le mode de fonctionnement du protocole
        #if( !aMODBUS_MASTER_MODE )
         //*** MODE ESCLAVE
         // On appele le serveur de message
         switch( ucMModBusDriverServerManager( &tModBusDriverDataRx, &tModBusDriverDataTx ) )
          {
           // On regarde si la réponse est immédiate
           case ( TUCHAR )aMODBUS_SERVER_TRANSMIT :
            // On appèle la fonction d'émission
            if(   ( uxAddress != ( TUCHAR )aMODBUS_ADDRESS_BROADCAST )
               // On émet la trame que si l'on n'a pas reçu de msg en broadcast
               && bFModBusDriverSendFrame()                          ) return( ( TBOOL )aTRUE );
           // Trame non valide on reboucle
           continue;
           // On regarde si l'on doit attendre
           #if( __ucMModBusDriverServerScheduler__ )
            // On regarde si la demande est immédiate
            case ( TUCHAR )aMODBUS_SERVER_WAIT :
             // On va tester le scheduler
             tModBusDriverRx.ucNet = ( TUCHAR )aMODBUS_RX_SCHEDULER;
            // On va attendre
            return( ( TBOOL )aTRUE );
           #endif
           // Pas de réponse
           default :
           break;
          }
        #else
         //*** MODE MAITRE
         // On vérifie que l'esclave à bien répondu à la fonction
         tModBusDriverTx.ucResult = ( TUCHAR )aMODBUS_CLIENT_DATA_RECEIVED;
        #endif
       }
     // On reboucle
     continue;
    }
   // On calcule le CRC
   tModBusDriverRx.uiCRC = uiFModBusDriverCRC( ucCharRx, tModBusDriverRx.uiCRC );
  }
 #if( !aMODBUS_MASTER_MODE )
  // on retourne l'état de l'automate
  return( ( TBOOL )( tModBusDriverRx.uxTimer != ( TUXTYPE_MODBUS )0 ) );
 #else
   // on retourne l'état de l'automate
  return( ( TBOOL )( tModBusDriverTx.uxTimer != ( TUXTYPE_MODBUS )0 ) );
 #endif
}
#endif

//------------------------------------------------------------------------------
// Gestion des automates ModBus
//------------------------------------------------------------------------------
#if( __bGModBusDriverManager__ )
static TBOOL bGModBusDriverManager( void )
{
 // on regarde si l'on est en cours de transmission
 if( bFModBusDriverIsTransmitting() ) return( ( TBOOL )aTRUE );
 // On appèle l'automate de réception et on regarde si l'on est en cours de réception
 return( bGModBusDriverReceiver() );
}
#endif

//------------------------------------------------------------------------------
// Création d'une fonction de controle de l'emmission
//------------------------------------------------------------------------------
//#if( __bGModBusDriverManager__ )
static TBOOL bFModBusDriverSendingFrameIsReady( void )
{
 //
 return( tModBusDriverTx.ucNet == aMODBUS_TX_CRC_H );
}
//#endif

//#endif

