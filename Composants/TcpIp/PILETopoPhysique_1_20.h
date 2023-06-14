//============================================================================//
// Fichier .........: "PILETopoPhysique.h"
// Auteur ..........: Christophe SCHAEFFER
// Date de création : 2005
//----------------------------------------------------------------------------//
/* Description :
    Configuration de la topologie physique de la pile TCP/IP.
*/
//============================================================================//
// Evolution : 1.10
// Auteur ...: Stéphane DUTERTRE
// Date .....: 22/02/10
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Evolution des couches applicatives FTP (Client & Serveur) ainsi que de la couche transport TCP
*/
//============================================================================//
// Evolution : 1.20
// Auteur ...: Stéphane DUTERTRE
// Date .....: 08/02/12
//----------------------------------------------------------------------------//
/* Description de l'évolution :
    Utilisation du FILSystem 2.00 avec changement de la taille des handles.
    Modifications du client HTTP pour gérer le GET, POST, PUT, HEAD et DELETE
*/
//============================================================================//

#ifndef PILE_TOPO_PHYSIQUE_1_20_H
#define PILE_TOPO_PHYSIQUE_1_20_H

//============================================================================//
// Inclusion des librairies
//============================================================================//

//============================================================================//
// Déclaration des constantes
//============================================================================//

// Taille d'une adresse MAC
#define   aMAC_ADDRESS_SIZE    6  

// Maximum transfert unit
#define   MTU_TX    1500
#define   MTU_RX    1500

// 2 adresses MAC (12 octets) + 2 octets pour le type de protocole
// (on ne compte pas le CRC qui est créé par le composant ethernet)
#define aTAILLE_OCTETS_HEADER_ETHERNET   14 
// Taille maximale d'une trame ethernet (MTU + en-tête en octets on ne compte pas le CRC qui est en enqueue)
#define aTX_ETHERNET_FRAME_SIZE ( MTU_TX + aTAILLE_OCTETS_HEADER_ETHERNET )
#define aRX_ETHERNET_FRAME_SIZE ( MTU_RX + aTAILLE_OCTETS_HEADER_ETHERNET )  
// Taille minimale d'une trame ethernet (données + en-tête, on ne compte pas le CRC qui est en enqueue)
#define aETHERNET_FRAME_SIZE_MIN    ( 46 + aTAILLE_OCTETS_HEADER_ETHERNET )

// Taille d'en-tête PPP
#define TAILLE_OCTETS_HEADER_PPP  4
// Taille maximale d'une trame PPP
#define aTX_PPP_FRAME_SIZE   ( MTU_TX + TAILLE_OCTETS_HEADER_PPP )
// Il ne faut pas oublier en réception les 2 octets du CRC
#define aRX_PPP_FRAME_SIZE   ( MTU_RX + TAILLE_OCTETS_HEADER_PPP + sizeof(TUINT) )

//============================================================================//
// Déclaration des types
//============================================================================//

//============================================================================//
// Déclaration des variables exportées
//============================================================================//

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des fonctions exportées
//============================================================================//

#endif

