//============================================================================//
// Fichier .........: "dashboard.cpp"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2020/01/28
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef DASHBOARD_CONTROLER_CPP
#define DASHBOARD_CONTROLER_CPP

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <TypeC.h>
#include <TypeCMMN.h>
#include <QDesktopServices>
#include <WIMOParameters.h>
#include "main.h"
#include "DashboardControler.h"
#include "WIMOClientModbus.h"

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
DashboardControler::DashboardControler(QObject *parent) : QObject(parent)
{
 //%%AP - 2021.04.13 - Liaison avec un controleur modbus
 // -> dataModbusReceived

 // Appel d'un slot spécifique Dashboard
 // -> Mise à jour à partir de la structure
 //  De la data dashboard

 // ou alors déclenchement d'un évenement sur la vue
}

//---------------------------------------------------------------
// Fonction avec plusieurs emit différent pour rediriger vers
// la version capteur génrérique
// la version O2
// la version balai
//---------------------------------------------------------------
void DashboardControler::vFDashboardHandleEndOfRefresh( void )
{
 // tWIMOParametersSensor.tHeader.tProduct.cType == ( TCHAR )'S'
 // On emet la fonction générale dans tous les cas
 qDebug("CPP endOfRefreshSignal BEGIN");
 emit endOfRefreshSignal();
 // Modification état variable busy
 this->setBusyRefresh( false );
 qDebug("CPP endOfRefreshSignal END");

 // Selon le type de capteur
 switch( tWIMOParametersSensor.tHeader.tProduct.cType )
  {
   // Sensor
   case( ( TCHAR )'S' ) :
    // Si le capteur est un O2
    if( tWIMOParametersSensor.ttChannel[0].ucParameter == ( TUCHAR )aWIMO_PARAMETERS_CHANNEL_OXYGEN_CONCENTRATION )
     {
      // Emit pour capteur O2
      emit displayO2();
     }
    // Sinon
    else
     {
      // Emit pour capteur classique
      emit displayGenericSensor();
     }
   break;
   // Wiper
   case( ( TCHAR )'W' ) :
    // Emit pour wiper
    emit displayWiper();
   break;
   // Erreur
   default:
   break;
  }
}

//----------------------------------------------------------
// Q_INVOKABLE:Bouton submit dashboard
//----------------------------------------------------------
Q_INVOKABLE void DashboardControler::refreshSensor( void )
{
 qDebug("Refresh sensor");
 // Modification état variable busy
 this->setBusyRefresh( true );
 // Lecture numéro de verson au démarrage
 vFWIMOModbusGetVersionNumber();
}

//----------------------------------------------------------
// Q_INVOKABLE: Ouverture page internet de support
//----------------------------------------------------------
Q_INVOKABLE void DashboardControler::openHelpSupportUrl( void )
{
 QDesktopServices::openUrl(QUrl("https://nke-instrumentation.support/index.php/wimo-calibration-tool/"));
}

#endif // DASHBOARD_CONTROLER_CPP
