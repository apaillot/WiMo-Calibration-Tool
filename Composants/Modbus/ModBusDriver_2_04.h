//============================================================================//
// Fichier .........: "ModBus_2_01.h"
// Auteur ..........: Stéphane DUTERTRE
// Date de création : 21/12/06
//----------------------------------------------------------------------------//
/* Description :
     Données de traitement ModBus.
*/
//============================================================================//
// Evolution :
// Auteur ...:
// Date .....:
//----------------------------------------------------------------------------//
/* Description de l'évolution :
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
    Changement de l'adresse du ModBus sur un type variable
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

#ifndef MODBUS_DRIVER_2_04_H
#define MODBUS_DRIVER_2_04_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeC.h>
// Si on doit redéfinir l'objet
#if !aMODBUS_REDEFINITION
 #include <ModBusDriverConfiguration_2_04.h>
#endif
#include <ModBusData_2_04.h>

//============================================================================//
// Déclaration des constantes
//============================================================================//
// Adresse de broadcast
#define aMODBUS_ADDRESS_BROADCAST    ( 0 )
// Adresse limite
#define aMODBUS_ADDRESS_RESERVED     ( 65535)

// Permet le fonctionnement en RS232/RS422 et RS485
#define aMODBUS_INTERFACE_RX_ENABLED ( 0 )
#define aMODBUS_INTERFACE_TX_ENABLED ( 1 )

// Constante d'application utiliser par l'application serveur et/ou son scheduler
// Réponse immédiate
#define aMODBUS_SERVER_TRANSMIT      ( 0 )
// Réponse différée
#define aMODBUS_SERVER_WAIT          ( aMODBUS_SERVER_TRANSMIT      + 1 )
// Ne rien retourner
#define aMODBUS_SERVER_ABORT         ( aMODBUS_SERVER_WAIT          + 1 )

// Les états d'attente du maître
// On n'a pas reçu encore reçu de données du récepteur
#define aMODBUS_CLIENT_DATA_NONE     ( 0 )
// On n'a reçu une donnée du récepteur
#define aMODBUS_CLIENT_DATA_RECEIVED ( aMODBUS_CLIENT_DATA_NONE     + 1 )
// Le récepteur n'a pas répondu à temps
#define aMODBUS_CLIENT_DATA_TIME_OUT ( aMODBUS_CLIENT_DATA_RECEIVED + 1 )

#define vMModBusDriverActivityOnBusEvent()

//============================================================================//
// Déclaration des types
//============================================================================//
// Objet Modbus
typedef struct
{
  // On fixe l'adresse de l'esclave
 #if( __bFModBusDriverSetAddress__ && !aMODBUS_MASTER_MODE )
  TBOOL ( * bFSetAddress )( TUXTYPE_MODBUS_ADDRESS );
 #endif
 // A appeler sur IRQ timer
 #if __vFModBusDriverTick__
  void ( * vFTick )( void );
 #endif
 // A appeler sur IRQ de réception de caractère
 #if __vFModBusDriverReceive__
  void ( * vFOnRxChar )( TUCHAR );
 #endif
 // A appeler sur IRQ d'éEmission du caractère
 #if __vGModBusDriverSender__
  void ( * vGOnTxEmpty )( void );
 #endif
 // A appeler sur IRQ du front descendant sur RX
 #if( __bFModBusDriverWakeUp__ )
  TBOOL ( * bFOnRxFront )( void );
 #endif
 // Indique si l'automate de réception et d'émission sont actifs
 #if __bFModBusDriverActive__
  TBOOL ( * bFIsRunning )( void );
 #endif
 // Indique si l'on a reçu une trame
 #if( __bFModBusDriverHasResponse__ && aMODBUS_MASTER_MODE )
  TBOOL ( * bFResponse )( void );
 #endif
 // On lit la donnée reçu
 #if( __ucFModBusDriverReadResponse__ && aMODBUS_MASTER_MODE )
  TUCHAR ( * ucFReadResponse )( TModBusDriverDataObject ** );
 #endif
 // Envoie d'une trame mode maître
 #if( __bFModBusDriverTransmitter__ && aMODBUS_MASTER_MODE )
  TBOOL ( * bFSend )( TUXTYPE_MODBUS_ADDRESS , TUCHAR , TUCHAR [] , TUCHAR );
 #endif
 // A appeler en permanence dans le Main
 #if __bGModBusDriverManager__
 TBOOL ( * bGScheduler )( void );
 #endif
 // A appeler en phase d'initialisation
 #if __vFModBusDriverInitialization__
  void ( * vFInitialization )( void );
 #endif
 // Fin de génération de trame à envoyer
 //#if __vFModBusDriverInitialization__
  TBOOL ( * bFSendingFrameIsReady )( void );
 //#endif
} 
TModBusObject;

//============================================================================//
// Déclaration des variables exportées
//============================================================================//
extern TCONST TModBusObject tModBus;

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des prototypes de fonctions exportées
//============================================================================//

//============================================================================//
// Déclaration des types
//============================================================================//

#endif
