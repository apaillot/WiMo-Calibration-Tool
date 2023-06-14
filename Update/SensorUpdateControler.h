#ifndef SENSORUPDATECONTROLER_H
#define SENSORUPDATECONTROLER_H

#include <QObject>
#include <QString>
#include <GetHTTPFile.h>
#include <SensorUpdateData.h>
#include <TINIConfig.h>

class SensorUpdateControler : public QObject
{
 Q_OBJECT

public:
 explicit SensorUpdateControler( SensorUpdateData *,
                                 TINIConfig *,
                                 QObject * parent = nullptr );

signals:
 // Demande d'ouverture de la fenêtre
 void openSensorUpdateWindowSignal( void );
 // Avancement de la progression de l'update capteur
 void updateProgressSignal( quint8 );
 // Signal pas de mise à jour capteur
 void noSensorUpdateSignal( void );
 // Fin de l'update capteur
 void updateEndSignal( void );
 // Signal pas de mise à jour capteur
 void sensorUpdateSuccessSignal( void );

//-----------------------------------------------
// Fonction
//-----------------------------------------------
public:
 // Vérification si une mise à jour est dispo
 Q_INVOKABLE void checkUpdateAvailability( void );
 // Click sur le bouton start
 Q_INVOKABLE void startButtonClicked( void );

private:
 // Requête du fichier liste
 void startFilelistRequest( void );
 // Le fichier liste a été reçue
 void fileListLoaded( void );

private slots:
 //
 void onDownloadFinished( void );
 //
 void onUpdateFinished( void );
 // Avancement du téléchargement
 void onDownloadProgress( qint8 );
 // Progression de l'update capteur
 void onUpdateProgress( qint8 );

private:
 TINIConfig * m_ptINIConfig;
 // Data
 SensorUpdateData * m_ptSensorUpdateData;
 //
 GetHTTPFile m_tHTTPProcess;
 GetHTTPFile * m_ptHTTPProcess;
 QString m_sHTTPPath = "http://nke-instrumentation.support/Update/WiMo_Sensor/Mainline/";
 QString m_sSensorID = "S10";
 QString m_sFileName = "sensor.hex";
 const QString m_sUpdateListFilename = "sensorupdatelist.txt";
};

#endif // SENSORUPDATECONTROLER_H
