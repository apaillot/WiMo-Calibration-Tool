//============================================================================//
// Fichier .........: "main.cpp"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2020/01/16
//----------------------------------------------------------------------------//
/* Description :
    Application de communication/configuration de capteur.
*/
//============================================================================//

//============================================================================//
// Inclusion des librairies
//============================================================================//
// Type nke
#include "TypeC.h"
#include <TypeCMMN.h>
// Qt
#include <QGuiApplication>
#include <QtQml>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickView>
#include <QSplashScreen>
#include <QDebug>
#include <QDateTime>
#include <QObject>
#include <QTimer>
#include <QtQuickControls2>
#include <QSerialPortInfo>
#include <QVector>
#include <QString>
// Application utilisateur
#include <readserialport.h>
#include <ModbusDriver_2_04.h>
#include <WIMOClientModBus.h>
#include "main.h"
#include <liveview.h>
#include <calibration.h>
#include <calibpoint.h>
#include <wiper.h>
#include <closeevent.h>
#include <factoryevent.h>
#include <liveviewconfevent.h>
#include <o2event.h>
#include <windowcomevent.h>
#include <WindowDebugEvent.h>
#include <mainevent.h>
#include <HTTPUpdate.h>
#include <SensorUpdateControler.h>
#include <SensorUpdateData.h>
#include <InterfaceUpdateControler.h>
#include <InterfaceUpdateData.h>
#include <ModbusInterfaceUpdateControler.h>
#include <ModbusControler.h>
#include <TINIConfig.h>
// Class de gestion simple
#include <DashboardControler.h>
#include <LiveviewControler.h>
#include <qwtquick2.h>
#include <treemodel.h>
#include <customtype.h>
//#include <exportbouton.h>
// FTDI librairie
#include <stdafx.h>
#include <windows.h>
#include <stdio.h>
#include "ftd2xx.h"

//============================================================================//
// Déclaration des constantes
//============================================================================//

//============================================================================//
// Déclaration des types
//============================================================================//

//============================================================================//
// Déclaration des variables exportées
//============================================================================//
// Gestion du port serie/COM
ReadSerialPort * ptSerialPortWriter;
// Accès aux variables liveview
//QObject * ptLiveviewObj;
Liveview * ptLiveviewObj;
// Accès aux variables liveview
QObject * ptPlotviewObj;
// Accès aux variables calibration
Calibration * ptCalibration;
// Liveview
Liveview * ptLiveview;
// Accès aux variables wiper
QObject * ptWiperObj;
// Donnée COM
DataCom * ptDataCom;
// Donnée capteur
DataSensor * ptDataSensor;
DataSensorChannel * ptDataSensorCh[3];
DataSensorCT * ptDataSensorCT;
// Point de calibration
CalibPointModel * ptCalibPoint;
CalibPointModel tCalibPointEmpty;
// Point de calibration des voies d'acquisition
CalibPointModel tCalibPointCh0;
CalibPointModel tCalibPointCh1;
CalibPointModel tCalibPointCh2;
// Context de l'application qml
QQmlContext * ptContext;
// Ajout de la gestion dashboard
DashboardControler * ptDashboardControler;
// Accès aux évenemnt de mise à jour
FactoryEvent * ptFactoryEvent;
// Gestion des évenemnets liveview conf
LiveviewConfEvent * ptLiveviewConfEvent;
// Gestion des évenemnets O2 conf
O2Event * ptO2Event;
// Gestion de la fenêtre de COM
WindowCOMEvent * ptCOMEvent;
// Gestion de la fenêtre de debug
WindowDebugEvent * ptDebugEvent;
// Accès aux évenemnt du main
MainEvent * ptMainEvent;
//
TreeModel * ptParamTreeModel;
TreeModel * ptParamTreeModelEmpty;
// Mise à jour capteur
SensorUpdateControler * ptSensorUpdateControler;
// Mise à jour interface
InterfaceUpdateControler * ptInterfaceUpdateControler;
// Fonctionnement modbus
ModbusControler * ptModbusControler;
// Qt software version
TCONST QString sSoftwareVersionFull = QString("%1.%2.%3.%4").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD).arg(VERSION_REVISION);
// Etat des modes
TBOOL bNkeModeCpp     = ( TBOOL )aFALSE;
TBOOL bMetroModeCpp   = ( TBOOL )aFALSE;
TBOOL bNkeProdModeCpp = ( TBOOL )aFALSE;
// Accès aux évenemnt du main
MainData * ptMainData;
// INI File config
TINIConfig * ptINIConfig;

//============================================================================//
// Déclaration des variables non exportées
//============================================================================//
// Version du logiciel
static TCONST QString sSoftwareVersion     = QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD);
//static TCONST QString sSoftwareVersionFull = QString("%1.%2.%3.%4").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD).arg(VERSION_REVISION);

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des prototypes de fonctions exportées
//============================================================================//

//============================================================================//
// Déclaration des prototypes de fonctions non exportées
//============================================================================//

//****************************************************************************//
// Le code des fonctions
//****************************************************************************//
int main(int argc, char *argv[])
{
 //------------------------------------
 // Création de l'application
 //------------------------------------
 QApplication app(argc, argv);
 app.setApplicationName("WiMo Calibration Tool");
 app.setApplicationVersion(sSoftwareVersion);
 app.setWindowIcon(QIcon(":/Logo/fav2_Low.ico"));
 app.setOrganizationName("nke Instrumentation");
 app.setOrganizationDomain("https://nke-instrumentation.com/");
 // Gestion de la méthode d'affichage
 //app.setAttribute( Qt::AA_UseSoftwareOpenGL );
 //app.setAttribute( Qt::AA_UseOpenGLES );
 //Qt::AA_UseDesktopOpenGL
 //Qt::AA_UseSoftwareOpenGL;
 //app.setAttribute(Qt::AA_EnableHighDpiScaling);

 // Définition du style de la fenêtre
 QQuickStyle::setStyle("Fusion");

 //----------------
 // Splash screen
 //----------------
 qDebug("MAIN - Splash screen");
 // Si pas en debug
 //#ifndef QT_DEBUG
  // Fonctionnel
  QSplashScreen * splash = new QSplashScreen;
  splash->setPixmap(QPixmap(":/Logo/SplashScreen/Splash_screen_little.png"));
  //splash->setPixmap(QPixmap(":/Logo/SplashScreen/Splash_screen_little-2.png"));
  splash->show();
  // Maintient 2 secondes du splash screen
  //QTimer::singleShot(2000, splash, &QWidget::close);
 //#endif
 //QCoreApplication::processEvents();
 // Traitement event
 QApplication::processEvents();

 //----------------
 // INI Config
 //----------------
 qDebug("MAIN - INI Config");
 // Debug
 qDebug() << "WorkingPath" << QDir::currentPath();

 TINIConfig tINIConfig;
 ptINIConfig = &tINIConfig;

 qDebug() << "tINIConfig baudrate" << tINIConfig.ptFGeneral()->uiBaudrate();

 tINIConfig.init();
 qDebug() << tINIConfig.ptFUpdate()->sGetPath();
 qDebug() << tINIConfig.ptFUpdate()->sDestProgramPath();
 qDebug() << tINIConfig.ptFUpdate()->sHTTPPath();

 //----------------
 // Traduction
 //----------------
 qDebug("MAIN - Traduction");
 QString sLocale;
 if( tINIConfig.ptFGeneral()->sLocation() != "" )
  sLocale = tINIConfig.ptFGeneral()->sLocation();
 else
  sLocale = QLocale::system().name().section('_', 0, 0);

 QTranslator translator;
 translator.load(QString("wimo_calibration_tool_") + sLocale);
 app.installTranslator(&translator);

 //----------------
 // Update
 //----------------
 // Si pas en debug
 qDebug("MAIN - Update");
 #ifndef QT_DEBUG
  // Création du process pour la mise à jour
  HTTPUpdate tHTTPUpdate;
  // Dossier de réception des données
  tHTTPUpdate.setDataReceivedPath( tINIConfig.ptFUpdate()->sGetPath() );
  // Dossier de destination programme
  tHTTPUpdate.setDestProgramPath( tINIConfig.ptFUpdate()->sDestProgramPath() );
  // Dossier de requête HTTP
  tHTTPUpdate.setHTTPPath( tINIConfig.ptFUpdate()->sHTTPPath() );
  // Définition du numéro de version du logiciel Qt
  tHTTPUpdate.setCurrentSoftwareVersion( sSoftwareVersionFull );
  // Test si mise à jour activé
  if( tINIConfig.ptFUpdate()->bActivate() )
   // Démarrage de la mise à jour
   tHTTPUpdate.startUpdateCheck();
 #endif


 //-----------------------------
 // Déclaration des paramètres liés QML
 //-----------------------------
 // Message debug
 qDebug("MAIN - Déclaration des paramètres liés QML");
 //qmlRegisterType<Liveview>("LibLiveview", 1,0, "Liveview");
 qmlRegisterType<Wiper>("LibWiper", 1,0, "Wiper");
 qmlRegisterType<QwtQuick2Plot>("QwtQuick2", 1, 0, "QwtQuick2Plot");
 qmlRegisterType<CustomType>("LibTreeDelegate", 1, 0, "CustomType");


 // Création de la class point
 CalibPointModel calibPoint;
 ptCalibPoint = &calibPoint;

 //----------------------------
 // Création composant
 //----------------------------
 // Traitement event
 QApplication::processEvents();
 app.processEvents();
 // Message debug
 qDebug("MAIN - Création composant");
 QQmlEngine engine;

 QQmlContext *context = new QQmlContext(engine.rootContext());
 ptContext = context;
 context->setContextProperty("CurDirPath", QString(QDir::currentPath()));

 // Donnée général du programme
 DataCom tDataCom;
 ptDataCom = &tDataCom;
 context->setContextProperty("tDataCom", &tDataCom);

 // Ajout point de calibration
 context->setContextProperty("calibPoint", &calibPoint);
 //%%AP - 2021.10.27 - Modification de la gestion
 context->setContextProperty("tCalibPointCh0", &tCalibPointCh0);
 context->setContextProperty("tCalibPointCh1", &tCalibPointCh1);
 context->setContextProperty("tCalibPointCh2", &tCalibPointCh2);

 //---------
 // Donnée voie capteur
 //---------
 DataSensor tDataSensor;
 ptDataSensor = &tDataSensor;
 context->setContextProperty("tDataSensor", &tDataSensor);
 DataSensorChannel tDataSensorCh[3];
 ptDataSensorCh[0] = &tDataSensorCh[0];
 ptDataSensorCh[1] = &tDataSensorCh[1];
 ptDataSensorCh[2] = &tDataSensorCh[2];
 context->setContextProperty("tDataSensorCh0", &tDataSensorCh[0]);
 context->setContextProperty("tDataSensorCh1", &tDataSensorCh[1]);
 context->setContextProperty("tDataSensorCh2", &tDataSensorCh[2]);
 // Liste des points pour compensation CT
 DataSensorCT tDataSensorCT;
 ptDataSensorCT = &tDataSensorCT;
 context->setContextProperty("tDataSensorCT", &tDataSensorCT);

 // Gestion des évenemnets liveview conf
 ptO2Event = new O2Event();
 context->setContextProperty("tO2Event", ptO2Event);

 //-------------
 // Dashboard
 //-------------
 // Ajout de la gestion dashboard
 DashboardControler tDashboardControler;
 ptDashboardControler = &tDashboardControler;
 context->setContextProperty("tDashboardControler", ptDashboardControler);

 //-------------
 // Calibration
 //-------------
 // Support calibration
 Calibration tCalibration;
 ptCalibration = &tCalibration;
 context->setContextProperty("calibrationID", ptCalibration);

 //-------------
 // Liveview
 //-------------
 // Liveview
 Liveview tLiveview;
 ptLiveview = &tLiveview;
 context->setContextProperty("liveviewID", ptLiveview);
 LiveViewControler tLiveviewControler;
 context->setContextProperty("tLiveviewControler", &tLiveviewControler);
  // Gestion des évenemnets liveview conf
 ptLiveviewConfEvent = new LiveviewConfEvent();
 context->setContextProperty("tLiveviewConfEvent", ptLiveviewConfEvent);

 //-------------
 // Factory
 //-------------
  // Gestion de la mise à jour
 ptFactoryEvent = new FactoryEvent();
 context->setContextProperty("tFactoryEvent", ptFactoryEvent);

 //-------------
 // Globale
 //-------------
 // Gestion de la fermeture
 CloseEvent * ptCloseEvent = new CloseEvent();
 context->setContextProperty("tCloseEvent", ptCloseEvent);
 // Gestion des événemnt du main
 ptMainEvent = new MainEvent();
 context->setContextProperty("tMainEvent", ptMainEvent);
 // Gestion de la fenêtre COM
 ptCOMEvent = new WindowCOMEvent();
 context->setContextProperty("tCOMEvent", ptCOMEvent);
 // Gestion de la fenêtre de debug
 WindowDebugEvent tDebugEvent;
 ptDebugEvent = &tDebugEvent;
 context->setContextProperty("tDebugWindow", &tDebugEvent);
 // Gestion de lecture/affichage configuration WiMo
 QString sBufferTree = "Test";
 TreeModel tParamTreeModel(sBufferTree);
 TreeModel tParamTreeModelEmpty("Test2");
 ptParamTreeModel = &tParamTreeModel;
 ptParamTreeModelEmpty = &tParamTreeModelEmpty;
 //tParamTreeModel.updateData("Test\t\t\t\t4");
 context->setContextProperty("tParamTreeModel", &tParamTreeModel);

 // Port série
 // Récupération du pointeur du port COM à ouvrir
 QSerialPort * ptSerialPort = &ptCOMEvent->m_serialPort;

 //--------------
 // Data
 //--------------
 // Message debug
 qDebug("MAIN - Data");
 // Mise à jour capteur
 SensorUpdateData tSensorUpdateData;
 engine.rootContext()->setContextProperty("sensorUpdateData", &tSensorUpdateData);
 // Mise à jour interface
 InterfaceUpdateData tInterfaceUpdateData;
 engine.rootContext()->setContextProperty("interfaceUpdateData", &tInterfaceUpdateData);
 // Donnée général du programme
 MainData tMainData;
 ptMainData = &tMainData;
 engine.rootContext()->setContextProperty("tMainData", &tMainData);

 //--------------
 // Controleur
 //--------------
 // Message debug
 qDebug("MAIN - Controleur");
 // Mise à jour capteur
 ptSensorUpdateControler = new SensorUpdateControler( &tSensorUpdateData,
                                                      &tINIConfig );
 engine.rootContext()->setContextProperty("tSensorUpdateControler", ptSensorUpdateControler);
 // Mise à jour interface
 ptInterfaceUpdateControler = new InterfaceUpdateControler( ptSerialPort,
                                                            &tInterfaceUpdateData,
                                                            &tMainData,
                                                            &tINIConfig );
 engine.rootContext()->setContextProperty("tInterfaceUpdateControler", ptInterfaceUpdateControler);
 // Mise à jour capteur
 ptModbusControler = new ModbusControler( &tMainData );
 engine.rootContext()->setContextProperty("tModbusControler", ptModbusControler);

 // Transmission au QML de la variable de debug
 context->setContextProperty("bDebugQML", tINIConfig.ptFGeneral()->bDebugMode());
 // Si en debug
 #ifdef QT_DEBUG
  //context->setContextProperty("bDebugQML", true);
  context->setContextProperty("bNonCommercial", false);
  ptMainData->setNkeMode( true );
  ptMainData->setMetroMode( false );
  ptMainData->setProdMode( false );
 // Si en release
 #else
  if( QFile::exists("nc.dll") )
   context->setContextProperty("bNonCommercial", true);
  else
   context->setContextProperty("bNonCommercial", false);
  // Test si mode client ou mode nke
  ptMainData->setNkeMode( QFile::exists("nke.dll") );
  // Test si mode client ou mode nke
  ptMainData->setMetroMode( QFile::exists("metro.dll") );
  // Test si mode client ou mode nke
  ptMainData->setProdMode( QFile::exists("prod.dll") );
 #endif

 //---------------
 // Fermeture de l'application
 //---------------
 // Message debug
 qDebug("MAIN - Fermeture de l'application");
 QObject::connect((QObject*)&engine, SIGNAL(quit()), &app, SLOT(quit()));
 QObject::connect((QObject*)ptMainEvent, SIGNAL(quit()), &app, SLOT(quit()));
 //---------------
 // Création de la partie QML
 //---------------
 // Traitement event
 QApplication::processEvents();
 app.processEvents();
 // Message debug
 qDebug("MAIN - Création de la partie QML");
 QQmlComponent component(&engine, "qrc:/QML/main.qml");
 qDebug("MAIN - Création de la partie QML - Component.create");
 QObject *object = component.create(context);
 qDebug("MAIN - Accès aux variables liveview");
 // Accès aux variables liveview
 ptLiveviewObj = &tLiveview;
 // Accès aux variables liveview
 ptPlotviewObj = object->findChild<QObject*>("plotviewID");
 qDebug("MAIN - Accès aux variables wiper");
 // Accès aux variables wiper
 QObject * tWiperObj = object->findChild<QObject*>("wiperID");
 ptWiperObj = tWiperObj;

 // Message debug
 qDebug("MAIN - Création de la partie QML END");
 if( ptDataCom )
  {
   qDebug("ptDataCom OK");
   // Init
   ptDataCom->setComPort("--");
  }

 //====================================================================
 // Liaison série
 //====================================================================
 qDebug("MAIN - Configuration liaison série");

 // Test si slow mode
 if( tINIConfig.ptFGeneral()->uiBaudrate() == 9600 )
  {
   // Activation du slow mode
   ptMainData->setCOMSlowMode(true);
   qDebug() << "ptMainData->getCOMSlowMode(): " << ptMainData->getCOMSlowMode();
  }
 // Configuration baudrate port COM
 qDebug("tINIConfig.ptFGeneral()->uiBaudrate");
 int uiValeur = tINIConfig.ptFGeneral()->uiBaudrate();
 qDebug("ptCOMEvent->setBaudrate");
 ptCOMEvent->setBaudrate( uiValeur );
 qDebug("vFComPortFindAutomaticSelection");
 // Recherche de la méthode de sélection du port COM
 ptCOMEvent->vFComPortFindAutomaticSelection( true );

 // Création du composant logiciel pour utilisation du port COM
 ReadSerialPort serialPortWriter( ptSerialPort );
 ptSerialPortWriter = &serialPortWriter;

 //----------------
 // Ouverture de la fenêtre principale
 //----------------
 qDebug("MAIN - Ouverture de la fenêtre principale");
 // Ouverture de la fenêtre principale
 ptMainEvent->vFMainWindowOpen();
 //----------------
 // Numéro version logiciel
 //----------------
 // Définition du numéro de version du logiciel Qt
 ptMainEvent->vFSetSoftwareVersion( sSoftwareVersion );

 //----------------
 // Modbus
 //----------------
 qDebug("MAIN - Initialisation modbus");
 // Initialisation modbus
 vFWIMOModbusInitialization();
 // Lancement du daemon modbus
 QTimer timer;
 QObject::connect(&timer, &QTimer::timeout, bGWIMOModbusManager);
 timer.start(1);

 //----------------
 // Mise à jour interface
 //----------------
 qDebug("MAIN - Mise à jour interface");
 // Si port COM ouvert
 if(  ptSerialPort->isOpen()
   && tINIConfig.ptFInterfaceUpdate()->bActivate()
   && !ptMainData->getCOMSlowMode() )
  ptInterfaceUpdateControler->checkUpdateAvailability( false );

 qDebug("MAIN - app.exec()");
 splash->close();
 //---------------
 // Programme principal
 //---------------
 return app.exec();
}

//-------------------------------------------------------------
// Copie de la boucle principale pour processus spécifique
//-------------------------------------------------------------
//#include <QEventLoop>
static TBOOL bBusy;
//QEventLoop tMainProcessLoop;
void vFMainProcess( void )
{
 /// DEBUG7
 QApplication::processEvents();
// QCoreApplication::processEvents();
 //QCoreApplication::processEvents( QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers );
 //QCoreApplication::processEvents();
 //tMainProcessLoop.processEvents( QEventLoop::ExcludeUserInputEvents );

 //%%AP - 2021.02.11 - Á enlever ?
 // Communication Modbus
 //bBusy = bGWIMOModbusManager();
 //if( bQuitApplication ) break;
 // En veille le plus gros du temps
 if( !bBusy ) QThread::msleep(1);
 /*
 // Test si fermeture application
 if( tCloseEvent.bClose() ){
  qDebug( "Sorti" );
  serialPort.close();
  //object->destroyed()
  object->deleteLater();
  app.exit();
  //app.setPalette()
  app.quit();
  return 0;
  break;
 }
 */
}


//-------------------------------------------------------------
// Sleep avec process
//-------------------------------------------------------------
void vFSleepWithProcess( TUINT uiTime )
{
 QElapsedTimer timer;
 timer.start();

 while( !timer.hasExpired( uiTime ) )
  {
   /// DEBUG
   QCoreApplication::processEvents();
   //tMainProcessLoop.processEvents( QEventLoop::ExcludeUserInputEvents );

   //%%AP - 2021.02.11 - Á enlever ?
   // Communication Modbus
   //bBusy = bGWIMOModbusManager();

   //if( bQuitApplication ) break;
   // En veille le plus gros du temps
   QThread::msleep(1);
  }
}
/* Réécriture de la fonction pour gagner en précision ?
void vFSleepWithProcess( TUINT uiTime )
{
 TUINT uiTimeDec = uiTime;

 while(1)
  {
   QCoreApplication::processEvents();
   // Communication Modbus
   //bBusy = bGWIMOModbusManager();
   //if( bQuitApplication ) break;
   // En veille le plus gros du temps
   QThread::msleep(1);
   // Décrément
   uiTimeDec--;
   // Si Temps terminée
   if( uiTimeDec == ( TUINT )0 ) break;
  }
}
*/

//-------------------------------------------------------------
// Debug avec datetime
//-------------------------------------------------------------
void vFQDebug( QString sMsg )
{
 if( !ptINIConfig->ptFGeneral()->bDebugMode() ) return;

 qDebug() << QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss,zzz")
          << " - "
          << sMsg;
 // Ecriture en texte de debug
 ptMainData->setDebug(sMsg);

//%%AP - TODO - Restriction que en mode debug
 ptDebugEvent->vFAddDebugLine(sMsg);
}

