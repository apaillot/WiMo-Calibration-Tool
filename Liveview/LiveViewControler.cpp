//============================================================================//
// Fichier .........: "LiveviewControler.cpp"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2023/02/23
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef LIVEVIEW_CONTROLER_CPP
#define LIVEVIEW_CONTROLER_CPP
//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeC.h>
#include <TypeCMMN.h>
#include <WIMOParameters.h>
#include "main.h"
#include "WIMOClientModbus.h"
#include "LiveViewControler.h"

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
LiveViewControler::LiveViewControler(QObject *parent) : QObject(parent)
{

}

//----------------------------------------------------------
// Changement de l'état de la voie
//----------------------------------------------------------
void LiveViewControler::setNumGraphicalBlocEnabled( int iChannel )
{
 // Activation
 ptDataSensorCh[iChannel]->setEnabled(true);
}
//----------------------------------------------------------
// Changement de l'état de la voie
//----------------------------------------------------------
void LiveViewControler::setNumGraphicalBlocDisabled( int iChannel )
{
 // Désactivation
 ptDataSensorCh[iChannel]->setEnabled(false);
}

#endif // LIVEVIEW_CONTROLER_CPP
