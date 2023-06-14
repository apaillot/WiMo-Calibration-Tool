#ifndef INTERFACEUPDATECONTROLER_H
#define INTERFACEUPDATECONTROLER_H

#include <QObject>
#include <QString>
#include <GetHTTPFile.h>
#include "readserialport.h"
#include <InterfaceUpdateData.h>
#include "ModbusInterfaceUpdateControler.h"
#include <MainData.h>
#include <TINIConfig.h>

class InterfaceUpdateControler : public QObject
{
 Q_OBJECT

public:
 explicit InterfaceUpdateControler( QSerialPort *,
                                    InterfaceUpdateData *,
                                    MainData *,
                                    TINIConfig *,
                                    QObject * parent = nullptr );

signals:
 void interfaceVersionCheckEndSignal( void );
 // Demande d'ouverture de la fenêtre
 void openInterfaceUpdateWindowSignal( void );
 // Avancement de la progression de l'update capteur
 void updateProgressSignal( quint8 );
 // Signal pas de mise à jour capteur
 void noInterfaceUpdateSignal( void );
 // Fin de l'update capteur
 void updateEndSignal( void );
 // Signal pas de mise à jour capteur
 void updateSuccessSignal( void );
 // Signal pas de mise à jour capteur
 void manualUpdateSuccessSignal( void );

//-----------------------------------------------
// Fonction public
//-----------------------------------------------
public:
 // Vérification si une mise à jour est dispo
 Q_INVOKABLE void checkUpdateAvailability( bool );
 // Click sur le bouton start
 Q_INVOKABLE void startButtonClicked( void );
 // Start mise à jour avec fichier
 Q_INVOKABLE void startManualUpdate( QString );

private:
 // Requête de la version de l'interface USB
 void startInterfaceVersionCheck( void );
 // Le fichier liste a été reçue
 void fileListLoaded( void );
 // Requête du fichier liste
 void startFilelistRequest( void );

private slots:
 //
 void onRequestVersionNumberEnd( void );
 //
 void onDownloadFinished( void );
 //
 void onUpdateFinished( void );
 // Avancement du téléchargement
 void onDownloadProgress( qint8 );
 // Progression de l'update capteur
 void onUpdateProgress( qint8 );
 // Demande du numéro de version après mise à jour
 void onUpdateVersionRequestFinished( void );

private:
 MainData   * m_ptMainData;
 TINIConfig * m_ptINIConfig;
 // Data
 InterfaceUpdateData * m_ptInterfaceUpdateData;
 //
 GetHTTPFile m_tHTTPProcess;
 GetHTTPFile * m_ptHTTPProcess;
 QString m_sHTTPPath = "http://nke-instrumentation.support/Update/WiMo_Interface_Usb/Mainline/";
 QString m_sInterfaceID = "S10";
 QString m_sFileName = "sensor.hex";
 // Couche modbus pour mise à jour interface
 ModbusInterfaceUpdateControler * m_ptModbusInterfaceUpdateControler;
 // Mise à jour manuel
 bool bManualUpdate = false;
 bool m_bUpdateCheckUser = false;
 const QString m_sUpdateListFilename = "interfaceupdatelist.txt";
};

#endif // SENSORUPDATECONTROLER_H
