//============================================================================//
// Fichier .........: "main.h"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2020/02/20
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef MAIN_H
#define MAIN_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
// Type nke
#include <TypeC.h>
#include <TypeCMMN.h>
// Lib standart
#include <string.h>
#include <stdio.h>
#include <stddef.h>
// Qt
#include <QDebug>
#include <QQmlContext>
#include <QObject>
#include <QTranslator>
// Application
// Class data
#include <DataCom.h>
#include <DataSensor.h>
#include <DataSensorCT.h>
#include <DataSensorChannel.h>
// Class utilisateur
#include <liveview.h>
#include <calibration.h>
#include <calibpoint.h>
#include <DashboardControler.h>
#include <wiper.h>
#include <factoryevent.h>
#include <liveviewconfevent.h>
#include <o2event.h>
#include <windowcomevent.h>
#include <WindowDebugEvent.h>
#include <mainevent.h>
#include <treemodel.h>
#include <SensorUpdateControler.h>
#include <SensorUpdateData.h>
#include <ModbusInterfaceUpdateControler.h>
#include <InterfaceUpdateControler.h>
#include <InterfaceUpdateData.h>
#include <ModbusControler.h>
#include <MainData.h>

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
// Accès aux variables liveview
extern Liveview * ptLiveviewObj;
// Accès aux variables liveview
extern QObject * ptPlotviewObj;
// Accès aux variables calibration
extern Calibration * ptCalibration;
// Accès aux variables wiper
extern QObject * ptWiperObj;
// Liveview
extern Liveview * ptLiveview;
// Donnée COM
extern DataCom * ptDataCom;
// Donnée capteur
extern DataSensor * ptDataSensor;
extern DataSensorChannel * ptDataSensorCh[3];
extern DataSensorCT * ptDataSensorCT;
// Point de calibration
extern CalibPointModel * ptCalibPoint;
extern CalibPointModel * ptCalibPoint2;
extern CalibPointModel tCalibPointEmpty;
// Point de calibration des voies d'acquisition
extern CalibPointModel tCalibPointCh0;
extern CalibPointModel tCalibPointCh1;
extern CalibPointModel tCalibPointCh2;
// Context de l'application qml
extern QQmlContext * ptContext;
// Ajout de la gestion dashboard
extern DashboardControler * ptDashboardControler;
// Accès aux évenemnt de mise à jour
extern FactoryEvent * ptFactoryEvent;
// Gestion des évenements liveview conf
extern LiveviewConfEvent * ptLiveviewConfEvent;
// Gestion des évenements O2 conf
extern O2Event * ptO2Event;
// Gestion de la fenêtre de COM
//extern WindowCOMEvent * ptCOMEvent;
// Gestion de la fenêtre de debug
extern WindowDebugEvent * ptDebugEvent;
// Main
extern MainEvent * ptMainEvent;
//
extern TreeModel * ptParamTreeModel;
extern TreeModel * ptParamTreeModelEmpty;
// Qt Soft version
extern TCONST QString sSoftwareVersionFull;
// Mise à jour capteur
extern SensorUpdateControler * ptSensorUpdateControler;
extern SensorUpdateData * ptSensorUpdateData;
// Mise à jour interface
extern InterfaceUpdateControler * ptInterfaceUpdateControler;
extern InterfaceUpdateData * ptInterfaceUpdateData;
// Fonctionnement modbus
extern ModbusControler * ptModbusControler;
// Accès aux évenemnt du main
extern MainData * ptMainData;
// INI File config
extern TINIConfig * ptINIConfig;
// Mode spéciaux
extern TBOOL bNkeModeCpp;
extern TBOOL bMetroModeCpp;
extern TBOOL bNkeProdModeCpp;

//============================================================================//
// Déclaration des variables non exportées
//============================================================================//

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des prototypes de fonctions exportées
//============================================================================//
//-------------------------------------------------------------
// Copie de la boucle principale pour processus spécifique
//-------------------------------------------------------------
extern void vFMainProcess( void );
//-------------------------------------------------------------
// Sleep avec process
//-------------------------------------------------------------
extern void vFSleepWithProcess( TUINT );
//-------------------------------------------------------------
// Debug avec datetime
//-------------------------------------------------------------
extern void vFQDebug( QString );

#endif // MAIN_H
