#ifndef O2EVENT_H
#define O2EVENT_H

#include <QObject>
#include <QString>

class O2Event: public QObject
{
 Q_OBJECT
public:
 explicit O2Event(QObject *parent = nullptr);

 // Fonction CPP d'affichage de la config
 void configO2Display( void );

signals:
 // Fonction CPP d'affichage de la config
 void configO2DisplaySignal( bool bExternalTemperature,
                             int liTemperature,
                             unsigned int uliPressure,
                             // SC7-540-200
                             QChar ucSensorType,
                             QChar ucIntensity,
                             unsigned char ucAmp,
                             // Phase shift with sensor code
                             unsigned int uliCodeDphi0,
                             unsigned int uliCodeDphi100,
                             // Temperature offset
                             int liTemperatureOffset,
                             unsigned int uliDummy,
                             // Etat de la calibration
                             bool bCalibrated,
                             // Calibration
                             bool bExternalTemperature0,
                             int  liTemperature0,
                             bool bExternalTemperature100,
                             int  liTemperature100,
                             unsigned int uliPressure100 );

public slots:
 // Envoi de la commande passage en mode calibration
 void sendSwitchToCalibrationModeCMD( void );
 // Envoi de la commande de lecture du capteur O2
 void sendReadCMD( void );
 void sendReadTemperatureOffsetCMD( void );
 // Envoi de la commande pour écriture sensor offset dans le capteur
 void sendWriteTemperatureOffsetCMD( void );
 void readConfFromI2CSensor( void );

 // Enregistrement des settings - Envoi de la configuration vers l'interface I2C
 void sendConfToI2CSensor( void );
 // Enregistrement des settings - Envoi de la commande modbus
 void sendConfigWriteCMD( void );

 // Enregistrement des settings - Envoi de la commande modbus
 //void setSensorSettingsCMD( void );
 // Enregistrement des settings - condition in the sample
 void setFactoryModeSensorCode(
  // SC7-540-200
  QChar ucSensorType,
  QChar ucIntensity,
  unsigned char ucAmp,
  // Phase shift with sensor code
  unsigned int uliCodeDphi0,
  unsigned int uliCodeDphi100 );
 // Enregistrement du sensor code en mode 2 points
 void set2PointsModeSensorCode( QChar ucSensorType, QChar ucIntensity, unsigned char ucAmp );
 // Enregistrement condition in the sample
 void setConditionInTheSample(
   // Calibration
   bool bExternalTemperature,
   int  liTemperature,
   unsigned int uliPressure );
 // Enregistrement condition in the sample
 void setTemperatureOffset( int liTemperatureOffset );
 // Enregistrement de la température - set air calibration
 void setAirCalibrationTemperature( int liTemperature100 );
 // Enregistrement de la température - set air calibration
 void setAirCalibrationPressure( int uliPressure100 );
 // Enregistrement de la température - set 0 calibration
 void set0CalibrationTemperature( int liTemperature0 );

 // Définition des points manuelements
 void setCalibPointAirManually( unsigned int );
 void setCalibPoint0PManually( unsigned int );
 void setCalibPointManually( unsigned int uliDphi0, unsigned int uliDphi100 );
 // Save register permanently
 void saveRegisterPermanently( void );

/*
 // Enregistrement des settings - condition in the sample
 void setSensorSettings(
   // SC7-540-200
   QChar ucSensorType,
   QChar ucIntensity,
   unsigned char ucAmp,
   // Phase shift with sensor code
   unsigned int uliCodeDphi0,
   unsigned int uliCodeDphi100,
   // Calibration
   bool bExternalTemperature,
   int  liTemperature,
   unsigned int uliPressure );
*/
 //---------------------
 // Air calibration
 //---------------------
 // Paramètrage
 void setAirCalibrationSettings(
   bool bExternalTemperature100,
   int  liTemperature100,
   unsigned int uliPressure100 );
 // Set air calibration CMD
 void setAirCalibrationCMD( void );
 //---------------------
 // 0% calibration
 //---------------------
 // Paramètrage 0% calibration
 void set0PCalibrationSettings(
   bool bExternalTemperature100,
   int  liTemperature100 );
 // Set 0% calibration CMD
 void set0PCalibrationCMD( void );

/*
 // Start calibration
 void startCalibration( void );
*/
private:

};

#endif // O2EVENT_H
