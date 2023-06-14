#ifndef LIVEVIEW_H
#define LIVEVIEW_H

// Type nke
#include <TypeC.h>
#include <TypeCMMN.h>
// Qt
#include <QObject>
#include <QString>
#include <QFile>

class Liveview : public QObject
{
 Q_OBJECT
 Q_PROPERTY(int channelNumber        READ getChannelNumber        WRITE setChannelNumber        NOTIFY channelNumberChanged)
 Q_PROPERTY(int recordingSampleDelay READ getRecordingSampleDelay WRITE setRecordingSampleDelay NOTIFY recordingSampleDelayChanged)
 Q_PROPERTY(bool measureFinished     READ getMeasureFinished      WRITE setMeasureFinished      NOTIFY measureFinishedChanged)
 // Ch0
 Q_PROPERTY(bool ch0Active    READ getCh0Active     WRITE setCh0Active     NOTIFY ch0ActiveChanged)
 Q_PROPERTY(int ch0Parameter  READ getCh0Parameter  WRITE setCh0Parameter  NOTIFY ch0ParameterChanged)
 Q_PROPERTY(QString ch0Name   READ getCh0Name       WRITE setCh0Name       NOTIFY ch0NameChanged)
 Q_PROPERTY(QString ch0Unit   READ getCh0Unit       WRITE setCh0Unit       NOTIFY ch0UnitChanged)
 Q_PROPERTY(QString ch0SN     READ getCh0SN         WRITE setCh0SN         NOTIFY ch0SNChanged)
 Q_PROPERTY(QString ch0Data   READ getCh0Data       WRITE setCh0Data       NOTIFY ch0DataChanged)
 Q_PROPERTY(float   fCh0Data  READ getFCh0Data      WRITE setFCh0Data      NOTIFY fCh0DataChanged)
 // Ch1
 Q_PROPERTY(bool ch1Active    READ getCh1Active     WRITE setCh1Active     NOTIFY ch1ActiveChanged)
 Q_PROPERTY(int ch1Parameter  READ getCh1Parameter  WRITE setCh1Parameter  NOTIFY ch1ParameterChanged)
 Q_PROPERTY(QString ch1Name   READ getCh1Name       WRITE setCh1Name       NOTIFY ch1NameChanged)
 Q_PROPERTY(QString ch1Unit   READ getCh1Unit       WRITE setCh1Unit       NOTIFY ch1UnitChanged)
 Q_PROPERTY(QString ch1SN     READ getCh1SN         WRITE setCh1SN         NOTIFY ch1SNChanged)
 Q_PROPERTY(QString ch1Data   READ getCh1Data       WRITE setCh1Data       NOTIFY ch1DataChanged)
 Q_PROPERTY(float   fCh1Data  READ getFCh1Data      WRITE setFCh1Data      NOTIFY fCh1DataChanged)
 // Ch2
 Q_PROPERTY(bool ch2Active    READ getCh2Active     WRITE setCh2Active     NOTIFY ch2ActiveChanged)
 Q_PROPERTY(int ch2Parameter  READ getCh2Parameter  WRITE setCh2Parameter  NOTIFY ch2ParameterChanged)
 Q_PROPERTY(QString ch2Name   READ getCh2Name       WRITE setCh2Name       NOTIFY ch2NameChanged)
 Q_PROPERTY(QString ch2Unit   READ getCh2Unit       WRITE setCh2Unit       NOTIFY ch2UnitChanged)
 Q_PROPERTY(QString ch2SN     READ getCh2SN         WRITE setCh2SN         NOTIFY ch2SNChanged)
 Q_PROPERTY(QString ch2Data   READ getCh2Data       WRITE setCh2Data       NOTIFY ch2DataChanged)
 Q_PROPERTY(float   fCh2Data  READ getFCh2Data      WRITE setFCh2Data      NOTIFY fCh2DataChanged)

public:
 Liveview(QObject *parent = 0);
 // Com Port
 int getChannelNumber() const;
 void setChannelNumber( int );
 // Recording sample delay
 int getRecordingSampleDelay() const;
 void setRecordingSampleDelay( int );
 // La mesure est terminée
 bool getMeasureFinished() const;
 void setMeasureFinished( bool );

 // Ch0 - Active
 bool getCh0Active() const;
 void setCh0Active( bool );
 // Ch0 - Parameter index
 int getCh0Parameter() const;
 void setCh0Parameter( int );
 // Ch0 - Name
 QString getCh0Name() const;
 void setCh0Name( const QString & );
 // Ch0 - Unit
 QString getCh0Unit() const;
 void setCh0Unit( const QString & );
 // Ch0 - Serial number
 QString getCh0SN() const;
 void setCh0SN( const QString & );
 // Ch0 - Data
 QString getCh0Data() const;
 void setCh0Data( const QString & );
 // Ch0 - Data
 float getFCh0Data();
 void setFCh0Data( float );

 // Ch1 - Active
 bool getCh1Active() const;
 void setCh1Active( bool );
 // Ch1 - Parameter index
 int getCh1Parameter() const;
 void setCh1Parameter( int );
 // Ch1 - Name
 QString getCh1Name() const;
 void setCh1Name( const QString & );
 // Ch1 - Unit
 QString getCh1Unit() const;
 void setCh1Unit( const QString & );
 // Ch1 - Serial number
 QString getCh1SN() const;
 void setCh1SN( const QString & );
 // Ch1 - Data
 QString getCh1Data() const;
 void setCh1Data( const QString & );
 float getFCh1Data();
 void setFCh1Data( float );

 // Ch2 - Active
 bool getCh2Active() const;
 void setCh2Active( bool );
 // Ch2 - Parameter index
 int getCh2Parameter() const;
 void setCh2Parameter( int );
 // Ch2 - Name
 QString getCh2Name() const;
 void setCh2Name( const QString & );
 // Ch2 - Unit
 QString getCh2Unit() const;
 void setCh2Unit( const QString & );
 // Ch2 - Serial number
 QString getCh2SN() const;
 void setCh2SN( const QString & );
 // Ch2 - Data
 QString getCh2Data() const;
 void setCh2Data( const QString & );
 float getFCh2Data();
 void setFCh2Data( float );

 // Enregistrement échantillon
 void recordMeasureSample( float, float, float );

 // Start mesure liveview
 Q_INVOKABLE void startMeasure( void );
 // Start mesure liveview init
 Q_INVOKABLE void startMeasureInit( void );
 // Stop mesure liveview init
 Q_INVOKABLE void stopMeasureInit( int );

signals:
 void channelNumberChanged( int );
 void recordingSampleDelayChanged( int );

 void measureFinishedChanged( bool );
 // Ch0
 void ch0ActiveChanged( bool );
 void ch0ParameterChanged( int );
 void ch0NameChanged( QString );
 void ch0UnitChanged( QString );
 void ch0SNChanged( QString );
 void ch0DataChanged( QString );
 void fCh0DataChanged( float );

 // Ch1
 void ch1ActiveChanged( bool );
 void ch1ParameterChanged( int );
 void ch1NameChanged( QString );
 void ch1UnitChanged( QString );
 void ch1SNChanged( QString );
 void ch1DataChanged( QString );
 void fCh1DataChanged( float );
 // Ch2
 void ch2ActiveChanged( bool );
 void ch2ParameterChanged( int );
 void ch2NameChanged( QString );
 void ch2UnitChanged( QString );
 void ch2SNChanged( QString );
 void ch2DataChanged( QString );
 void fCh2DataChanged( float );

private:
 int m_uiChannelNumber;
 // Temps entre deux échantillons réèls
 int m_uiRecordingSampleDelay;
 bool m_bMeasureFinished;
 char m_tcBuffer[1024];
 // Ch0
 bool m_bCh0Active = true;
 int m_uiCh0Parameter = 0;
 QString m_sCh0Name = "";
 QString m_sCh0Unit = "";
 QString m_sCh0SN = "";
 QString m_sCh0Data = "";
 float   m_fCh0Data = 0.0;
 // Ch1
 bool m_bCh1Active = true;
 int m_uiCh1Parameter = 0;
 QString m_sCh1Name = "";
 QString m_sCh1Unit = "";
 QString m_sCh1SN = "";
 QString m_sCh1Data = "";
 float   m_fCh1Data = 0.0;
 // Ch2
 bool m_bCh2Active = true;
 int m_uiCh2Parameter = 0;
 QString m_sCh2Name = "";
 QString m_sCh2Unit = "";
 QString m_sCh2SN = "";
 QString m_sCh2Data = "";
 float   m_fCh2Data = 0.0;
 // Création du fichier
 QFile * m_ptFileHandler = nullptr;
};

#endif // LIVEVIEW_H
