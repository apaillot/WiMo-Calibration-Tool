//============================================================================//
// Fichier .........: "ModBusDriverConfiguration_2_01.h"
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

#ifndef MODBUS_DRIVER_CONFIGURATION_2_04_H
#define MODBUS_DRIVER_CONFIGURATION_2_04_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeC.h>
#include <FIFOModBusDriver_2_04.h>
//#include "Unit1.h"
#include <readserialport.h>
#include <WIMOClientModbus.h>

//============================================================================//
// Déclaration des constantes
//============================================================================//
// Indique si le protocole doit fonctionner en maître ou esclave
#define aMODBUS_MASTER_MODE                1
// On définit la taille de la trame
#define aMODBUS_DATA_SIZE                  252
// Si vrai MODBUS ou KELLER
#define aMODBUS_REAL_CRC_SPECIFICATION     1

//============================================================================//
/// Définition des fonctions utilisées (1 = utilisée, 0 = supprimée)
//============================================================================//

// On fixe l'adresse du récepteur
#define __bFModBusDriverSetAddress__        1
// A appeler sur IRQ timer
#define __vFModBusDriverTick__              1
// A appeler sur IRQ de réception de caractère
#define __vFModBusDriverReceive__           1
// A appeler sur IRQ d'éEmission du caractère
#define __vGModBusDriverSender__            1
// A appeler sur IRQ du front descendant sur RX
#define __bFModBusDriverWakeUp__            1
// Indique si l'automate de réception et d'émission sont actifs
#define __bFModBusDriverActive__            0
// Indique si l'on a reçu une trame
#define __bFModBusDriverHasResponse__       1
// On lit la donnée reçu
#define __ucFModBusDriverReadResponse__     1
// Envoie d'une trame mode maître
#define __bFModBusDriverTransmitter__       1
// A appeler en permanence dans le Main
#define __bGModBusDriverManager__           1
// A appeler en phase d'initialisation
#define __vFModBusDriverInitialization__    1
// On regarde si la transmission est libre
#define __bFModBusDriverIsTransmitting__    1
// Réception des trames ModBus
#define __bGModBusDriverReceiver__          1

// On définit le type d'adresse
#define TUXTYPE_MODBUS_ADDRESS             TUCHAR
// Doit obligatoirement aller de paire avec le type "TUXTYPE_MODBUS_ADDRESS"
#define aMODBUS_DRIVER_16_BITS_ADDRESS     0
// On définit le type
#define TUXTYPE_MODBUS                     TUINT

// Réponses du serveur ou scheduler ::
// Si réponse = "aTRANSMIT" alors envoi de la réponse.
// Si réponse = "aABORT" l'émission est avortée (message non géré ou erreur)
// Si réponse = "WAIT" alors le scheduler est interrogé jusqu'à qu'il retourne
// le message "aTRANSMIT" ou "aABORT"
// Gestion de la messagerie du protocole ModBus :: Application serveur
#define ucMModBusDriverServerManager         
// Definition de la fonction
//#define ucMModBusDriverGetMessageSize      ucFNEWSENSModbusMessageSize
#define ucMModBusDriverGetMessageSize      ucFWIMOModbusMessageSize
// Lecture directe
#define ucMModBusDriverGetSizePosition( _ucFunction ) ( TUCHAR )0

//%%AP - 2020.03.12 - Test augmentation des timeouts
// Time out de réception : dépend du timer réel x cette valeur
//#define aMODBUS_TIMER_RX_TIME_OUT               20
//#define aMODBUS_TIMER_TX_TIME_OUT               20
//#define aMODBUS_TIMER_RX_COMMUNICATION_TIME_OUT 20
//%%AP - 2021.09.14 - Augmentation timeout modbus
#define aMODBUS_TIMER_RX_TIME_OUT               50
#define aMODBUS_TIMER_TX_TIME_OUT               50
#define aMODBUS_TIMER_RX_COMMUNICATION_TIME_OUT 50
//#define aMODBUS_TIMER_RX_COMMUNICATION_TIME_OUT 20
//#define aMODBUS_TIMER_RX_TIME_OUT               50
//#define aMODBUS_TIMER_TX_TIME_OUT               50
//#define aMODBUS_TIMER_RX_COMMUNICATION_TIME_OUT 50

//============================================================================//
// Déclaration des types
//============================================================================//

//============================================================================//
// Déclaration des variables exportées
//============================================================================//
extern TBOOL bSerialPortRxMode;
extern ReadSerialPort * ptSerialPortWriter;

//============================================================================//
// Déclaration des macros
//============================================================================//
// Permet l'émission d'octet
//#define vMModBusDriverUartWrite( ucCharTx ) ucData = ucCharTx, Form1->ComPort0->Write( &ucData, sizeof( ucData ) )
#define vMModBusDriverUartWrite( ucCharTx )  ptSerialPortWriter->write(ucCharTx)
// Activation de la réception
//#define vMModBusDriverUartRxTx( ucMode )
// Activation de la réception
#define vMModBusDriverUartRxTx( ucMode ) \
{ \
 if( ucMode == aMODBUS_INTERFACE_RX_ENABLED ) { ptSerialPortWriter->setRxMode( aTRUE ); } \
 else { ptSerialPortWriter->setRxMode( aFALSE ); } \
}
// On initialise l'UART
#define vMModBusDriverUartInitialization()

//============================================================================//
// Déclaration des prototypes de fonctions exportées
//============================================================================//

#endif
