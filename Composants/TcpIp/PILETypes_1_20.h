//============================================================================//
// Fichier .........: "PILETypes.h"
// Auteur ..........: Christophe SCHAEFFER
// Date de création : 16/08/05
//----------------------------------------------------------------------------//
/* Description :
     Types spécifiques de la pile TCP/IP.
*/
//============================================================================//

#ifndef PILE_TYPES_H
#define PILE_TYPES_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeCMMN.h>
#include <TypeC.h>
#include <PILETopoPhysique_1_20.h>

//============================================================================//
// Déclaration des constantes
//============================================================================//
// Différents modes de la pile TCP/IP
#define aPILE_TCP_IP_ETHERNET    0
#define aPILE_TCP_IP_PPP_CLIENT  ( aPILE_TCP_IP_ETHERNET + 1 )
#define aPILE_TCP_IP_PPP_SERVER  ( aPILE_TCP_IP_PPP_CLIENT + 1 )

//============================================================================//
// Déclaration des types
//============================================================================//

// Identification du protocole encapsulé par la couche liaison
typedef TUCHAR IdProtocole;

// Définition d'un type correspondant à une adresse IP
typedef union
{
 TUCHAR    tucIP[ sizeof( TULONGINT ) ];
 TULONGINT uliIP;
}
TIPv4;

// Définition d'un type correspondant à une adresse MAC
typedef union
{
 TUCHAR tucMAC[ aMAC_ADDRESS_SIZE ];
 TUINT  tuiMAC[ aMAC_ADDRESS_SIZE / sizeof( TUINT ) ];
}
TMACv4;

// Configuration du system
typedef struct
{
 TMACv4 tMACv4PhysicalAddress;
 TIPv4  tIPv4Local;
 TIPv4  tIPv4SubnetMask;
 TIPv4  tIPv4Gateway;
}
TIPv4System;

// SD : Permet d'accélérer le traitement pour le passage en LITTLE ENDIAN
typedef union
{
 TUINT uiData;
 TUCHAR tucData[ sizeof( TUINT ) / sizeof( TUCHAR ) ];
}
TWord16;

// Structure IP des dispositifs connectés en PPP
typedef struct
{
 TIPv4 tIPv4Local;
 TIPv4 tIPv4Distant;
}
TPPPBinding;

// En-tête de trame Ethernet
typedef struct
{
 TMACv4 tMACv4Dst;
 TMACv4 tMACv4Src;
 TUINT uiProtocol;
}
TEthernetHeader;

// En-Tête de trame PPP
typedef struct
{
 TUCHAR ucAdresse;
 TUCHAR ucCommande;
 TUINT uiProtocole;
}
TPPPHeader;

// Checksum PPP
typedef TUINT T_ChecksumPPP;

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

