#ifndef UPDATEEVENT_H
#define UPDATEEVENT_H

#include <QObject>

class FactoryEvent: public QObject
{
 Q_OBJECT
public:
 explicit FactoryEvent(QObject *parent = nullptr);

 // Fonction CPP de mise à jour valeur loader
 void updateLoaderValue( int );
 void resetLoaderValue( int );
 void saveConfigLoaderValue( int );
 void writeConfigLoaderValue( int );
 void configSettingsDisplay( void );
 void configSettingsDisplayEvosens( void );
 void endOfSNSubmit( void );
 void endOfRefreshData( void );

signals:
 // Signal qui permet d'afficher le bon nombre dans le loader
 void updateLoaderValueSignal( int uiLoaderValue );
 void resetLoaderValueSignal( int uiLoaderValue );
 void saveConfigLoaderValueSignal( int uiLoaderValue );
 void writeConfigLoaderValueSignal( int uiLoaderValue );
 void configSettingsSignal( int uiSN_Y, int uiSN_N );
 void configSettingsDisplayEvosensSignal( int uiSN_Y,
                                          int uiSN_N,
                                          int uiIntegrationTime,
                                          int uiIntegrationSum,
                                          int ucRange,
                                          int uiAverageSamples,
                                          int uiTestMode,
                                          int uiPowerOn );
 void endOfSNSubmitSignal( void );
 void endOfRefreshDataSignal( void );

public slots:
 void startUpdate( QString );
 void startReset();
 bool bFStartSaveConfig( QString, QString );
 void startWriteConfig( QString );
 void saveConfigSettings( int, int );
 void initModbus( void );
 QString sFSaveConfigSettingsEvosens( int, int, int, int, int, int, int, int );
 void saveConfigSettingsCT( int, int, float, float, float, float );

private:
 int m_uiUpdateLoaderValue;
 int m_uiResetLoaderValue;
 int m_uiSaveConfigLoaderValue;
 int m_uiWriteConfigLoaderValue;
};

#endif // UPDATEEVENT_H
