#ifndef MAINEVENT_H
#define MAINEVENT_H

#include <QObject>
#include <QSplashScreen>

class MainEvent: public QObject
{
 Q_OBJECT
public:
 explicit MainEvent(QObject *parent = nullptr);

 // Fonction CPP
 void vFSetSoftwareVersion( QString );
 // Capture du numéro de série
 QString sFGetSoftwareVersion( void );
 // Ouverture de la fenêtre principale
 void vFMainWindowOpen( void );
 // Ouverture de la fenêtre d'erreur
 void vFMainWindowErrorOpen( QString );
 // Ouverture de la fenêtre de mise à jour
 void vFMainWindowUpdateDetectedOpen( QString );
 // Clear data de la fenêtre principale
 void vFMain_ClearData( void );

signals:
 // Fonction CPP
 void setSoftwareVersionSignal( QString sVersion );
 // Ouverture de la fenêtre principale
 void mainWindowOpen( void );
 // Ouverture de la fenêtre d'erreur
 void mainWindowErrorOpen( QString sErrorMsg );
 // Ouverture de la fenêtre de mise à jour
 void mainWindowUpdateDetectedOpen( QString sNewVersionNumber );
 // Fermeture générale de l'application
 void quit( void );
 // Clear data de la fenêtre principale
 void vFMain_ClearDataSignal( void );

public slots:
 // Ouverture de la fenêtre de détection de mise à jour
 Q_INVOKABLE void vFMainWindowStartUpdate( void );
 // Effacement de la fenêtre parameter
 Q_INVOKABLE void clearParamWindows( void );
 // Ouverture de la fenêtre display configuration
 Q_INVOKABLE void vFMainEventDisplayParamWindow( void );
 // Test de l'état du grafcet modbus
 Q_INVOKABLE bool isModbusBusyState( void );
 // Get de la configuration global
 Q_INVOKABLE void getSensorConfiguration( void );
 // Sauvegarde de la configuration dde la configuration général vers le capteur
 Q_INVOKABLE void saveSensorConfiguration( void );
 // Get de la configuration d'une voie vers le capteur
 Q_INVOKABLE void getChannelConfiguration( unsigned int );
 // Sauvegarde de la configuration d'une voie vers le capteur
 Q_INVOKABLE void saveChannelConfiguration( unsigned int uiChannel );
 // Vérification activation voie de mesure
 Q_INVOKABLE bool isChannelActivated( int );
 Q_INVOKABLE void setChannelActivation( int, bool );
 // Get pH calibration temperature
 Q_INVOKABLE float getPHCalibrationTemperature( void );
 // Set pH calibration temperature
 Q_INVOKABLE void setPHCalibrationTemperature( float );
 // Affichage du splash screen
 Q_INVOKABLE void displayScreen( void );
 // Ouverture explorateur windows
 Q_INVOKABLE void openWindowsExplorer( void );
 // Start acquisition premier échantillon
 Q_INVOKABLE void beginAcquisition( void );

private:
 QString m_sVersion;
 QSplashScreen * m_splash;
};

#endif // MAINEVENT_H
