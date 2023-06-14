#ifndef LIVEVIEW_CONF_EVENT_H
#define LIVEVIEW_CONF_EVENT_H

#include <QObject>
#include <QString>

class LiveviewConfEvent: public QObject
{
 Q_OBJECT
public:
 explicit LiveviewConfEvent(QObject *parent = nullptr);

 // Fonction CPP de mise à jour
 void displayLiveViewConf( void );
 void transmitLiveViewData( float fCh0Data,
                            float fCh1Data,
                            float fCh2Data );
 void startMeasure( void );
 void stopMeasure( void );

signals:
 // Signal
 void displayLiveViewConfSignal( QString sCh0Name,
                                 QString sCh1Name,
                                 QString sCh2Name,
                                 unsigned int uiAcquisitionTime,
                                 unsigned int uiChannelNumber );
 // Signal
 void transmitLiveViewDataSignal( float fCh0Data,
                                  float fCh1Data,
                                  float fCh2Data );
 void startMeasureSignal( void );
 void stopMeasureSignal( void );

public slots:
 // Test si la voie a un calibration type activé
 bool bFIsCalibrationTypeActivated( unsigned int uiChannel );

};

#endif // CALIBRATIONCONFEVENT_H
