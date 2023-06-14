#include "liveview.h"
#include "WIMOClientModBus.h"
#include <WIMOParameters.h>
// Qt
#include <QFile>
#include <QDateTime>

//----------------------------------------------------------
// Constructeur de la classe
//----------------------------------------------------------
Liveview::Liveview(QObject *parent)
: QObject(parent)
{
 m_ptFileHandler = nullptr;
}

//----------------------------------------------------------
// Channel number
//----------------------------------------------------------
int Liveview::getChannelNumber() const
{
 return m_uiChannelNumber;
}
void Liveview::setChannelNumber( int n )
{
 m_uiChannelNumber = n;
 emit channelNumberChanged(m_uiChannelNumber);
}

//----------------------------------------------------------
// Recording Sample Delay
//----------------------------------------------------------
int Liveview::getRecordingSampleDelay() const
{
 return m_uiRecordingSampleDelay;
}
void Liveview::setRecordingSampleDelay( int n )
{
 m_uiRecordingSampleDelay = n;
 emit recordingSampleDelayChanged(m_uiRecordingSampleDelay);
}

//----------------------------------------------------------
// Measure finished
//----------------------------------------------------------
bool Liveview::getMeasureFinished() const
{
 return m_bMeasureFinished;
}
void Liveview::setMeasureFinished( bool n )
{
 m_bMeasureFinished = n;
 emit measureFinishedChanged(m_bMeasureFinished);
}

//----------------------------------------------------------
// Ch0 active
//----------------------------------------------------------
bool Liveview::getCh0Active() const
{
 return m_bCh0Active;
}
void Liveview::setCh0Active( bool n )
{
 m_bCh0Active = n;
 emit ch0ActiveChanged(m_bCh0Active);
}
//----------------------------------------------------------
// Ch0 parameter
//----------------------------------------------------------
int Liveview::getCh0Parameter() const
{
 return m_uiCh0Parameter;
}
void Liveview::setCh0Parameter( int n )
{
 m_uiCh0Parameter = n;
 emit ch0ParameterChanged(m_uiCh0Parameter);
}
//----------------------------------------------------------
// Channel 0 : Name
//----------------------------------------------------------
QString Liveview::getCh0Name() const
{
 return m_sCh0Name;
}
void Liveview::setCh0Name(const QString &n)
{
 m_sCh0Name = n;
 emit ch0NameChanged( m_sCh0Name );
}
//----------------------------------------------------------
// Channel 0 : Unit
//----------------------------------------------------------
QString Liveview::getCh0Unit() const
{
 return m_sCh0Unit;
}
void Liveview::setCh0Unit(const QString &n)
{
 m_sCh0Unit = n;
 emit ch0UnitChanged( m_sCh0Unit );
}
//----------------------------------------------------------
// Channel 0 : SN
//----------------------------------------------------------
QString Liveview::getCh0SN() const
{
 return m_sCh0SN;
}
void Liveview::setCh0SN(const QString &n)
{
 m_sCh0SN = n;
 emit ch0SNChanged( m_sCh0SN );
}
//----------------------------------------------------------
// Channel 0 : Data
//----------------------------------------------------------
QString Liveview::getCh0Data() const
{
 return m_sCh0Data;
}
void Liveview::setCh0Data(const QString &n)
{
 m_sCh0Data = n;
 emit ch0DataChanged( m_sCh0Data );
}
float Liveview::getFCh0Data()
{
 return m_fCh0Data;
}
void Liveview::setFCh0Data(float n)
{
 m_fCh0Data = n;
 emit fCh0DataChanged( m_fCh0Data );
}

//----------------------------------------------------------
// Ch1 active
//----------------------------------------------------------
bool Liveview::getCh1Active() const{ return m_bCh1Active; }
void Liveview::setCh1Active( bool n ){ m_bCh1Active = n; emit ch1ActiveChanged(m_bCh1Active);}
//----------------------------------------------------------
// Ch1 parameter
//----------------------------------------------------------
int Liveview::getCh1Parameter() const{ return m_uiCh1Parameter; }
void Liveview::setCh1Parameter( int n ){ m_uiCh1Parameter = n; emit ch1ParameterChanged(m_uiCh1Parameter);}
//----------------------------------------------------------
// Channel 1 : Name
//----------------------------------------------------------
QString Liveview::getCh1Name() const{ return m_sCh1Name; }
void Liveview::setCh1Name(const QString &n){ m_sCh1Name = n; emit ch1NameChanged( m_sCh1Name );}
//----------------------------------------------------------
// Channel 1 : Unit
//----------------------------------------------------------
QString Liveview::getCh1Unit() const{ return m_sCh1Unit;}
void Liveview::setCh1Unit(const QString &n){ m_sCh1Unit = n; emit ch1UnitChanged( m_sCh1Unit );}
//----------------------------------------------------------
// Channel 1 : SN
//----------------------------------------------------------
QString Liveview::getCh1SN() const{ return m_sCh1SN;}
void Liveview::setCh1SN(const QString &n){ m_sCh1SN = n; emit ch1SNChanged( m_sCh1SN );}
//----------------------------------------------------------
// Channel 1 : Data
//----------------------------------------------------------
QString Liveview::getCh1Data() const{ return m_sCh1Data;}
void Liveview::setCh1Data(const QString &n){ m_sCh1Data = n; emit ch1DataChanged( m_sCh1Data );}
float Liveview::getFCh1Data(){ return m_fCh1Data;}
void Liveview::setFCh1Data( float n ){ m_fCh1Data = n; emit fCh1DataChanged( m_fCh1Data );}

//----------------------------------------------------------
// Ch2 active
//----------------------------------------------------------
bool Liveview::getCh2Active() const{ return m_bCh2Active;}
void Liveview::setCh2Active( bool n ){ m_bCh2Active = n; emit ch2ActiveChanged(m_bCh2Active);}
//----------------------------------------------------------
// Ch2 parameter
//----------------------------------------------------------
int Liveview::getCh2Parameter() const{ return m_uiCh2Parameter;}
void Liveview::setCh2Parameter( int n ){ m_uiCh2Parameter = n; emit ch2ParameterChanged(m_uiCh2Parameter);}
//----------------------------------------------------------
// Channel 2 : Name
//----------------------------------------------------------
QString Liveview::getCh2Name() const{ return m_sCh2Name;}
void Liveview::setCh2Name(const QString &n){ m_sCh2Name = n; emit ch2NameChanged( m_sCh2Name );}
//----------------------------------------------------------
// Channel 2 : Unit
//----------------------------------------------------------
QString Liveview::getCh2Unit() const{ return m_sCh2Unit;}
void Liveview::setCh2Unit(const QString &n){ m_sCh2Unit = n; emit ch2UnitChanged( m_sCh2Unit );}
//----------------------------------------------------------
// Channel 2 : SN
//----------------------------------------------------------
QString Liveview::getCh2SN() const{ return m_sCh2SN;}
void Liveview::setCh2SN(const QString &n){ m_sCh2SN = n; emit ch2SNChanged( m_sCh2SN );}
//----------------------------------------------------------
// Channel 2 : Data
//----------------------------------------------------------
QString Liveview::getCh2Data() const{ return m_sCh2Data;}
void Liveview::setCh2Data(const QString &n){ m_sCh2Data = n; emit ch2DataChanged( m_sCh2Data );}
float Liveview::getFCh2Data(){ return m_fCh2Data;}
void Liveview::setFCh2Data(float n){ m_fCh2Data = n; emit fCh2DataChanged( m_fCh2Data );}

//----------------------------------------------------------
// Start mesure liveview
//----------------------------------------------------------
Q_INVOKABLE void Liveview::startMeasure( void )
{
 // Appel de la fonction de lancement de la mesure
 vFWIMOModbusStartMeasure();
}

//----------------------------------------------------------
// Start mesure liveview init
//----------------------------------------------------------
Q_INVOKABLE void Liveview::startMeasureInit( void )
{
 qDebug("Liveview::startMeasureInit");
 QDateTime qDateTime;

 // Appel de la fonction de lancement de la mesure
 //vFWIMOModbusStartMeasure();
 char tcBuffer[ 64 ]={0};
 TUCHAR ucCpt;

 // Gestion appui
 //if( ucModbusState == aMODBUS_STATE_FREE )
  {
   // Get du datetime courant
   qDateTime = QDateTime::currentDateTime();
   // Ouverture fichier
   snprintf( tcBuffer,
             sizeof( tcBuffer ),
             "%02d%05d_data_%04d%02d%02d_%02d%02d%02d.csv",
             tWIMOParametersSensor.tHeader.tProduct.ucSerialNumberY,
             tWIMOParametersSensor.tHeader.tProduct.uiSerialNumberN,
             qDateTime.date().year(), qDateTime.date().month(),  qDateTime.date().day(),
             qDateTime.time().hour(), qDateTime.time().minute(), qDateTime.time().second() );

   if( m_ptFileHandler != nullptr )
    {
     delete m_ptFileHandler;
    }
   m_ptFileHandler = new QFile( "../Measure/"+QString(tcBuffer) );
   //QFile tFileHandler( tcBuffer );

   // Ouverture du fichier *.ini
   if( m_ptFileHandler->open( QIODevice::WriteOnly ) )
    {
     // Réinit du buffer d'écriture
     memset( m_tcBuffer, ( TCHAR )'\0', sizeof( tcBuffer ) );
     qDebug( m_tcBuffer );

     m_ptFileHandler->write("Timestamp(Standard)", strlen("Timestamp(Standard)") );

     //for( ucCpt = 0; ucCpt < tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber; ucCpt++ )
     for( ucCpt = 0; ucCpt < this->m_uiChannelNumber; ucCpt++ )
      snprintf( ( char * )&m_tcBuffer[ strlen( m_tcBuffer ) ],
                sizeof( m_tcBuffer ),
                ",CH%d:%s(%s)",
                ucCpt,
                tWIMOParametersSensor.ttChannel[ ucCpt ].tcName,
                tWIMOParametersSensor.ttChannel[ ucCpt ].tcUnit );
     memcpy( ( TCHAR * )&m_tcBuffer[ strlen( m_tcBuffer ) ], "\r\n", sizeof( "\r\n" ) );

     qDebug( m_tcBuffer );
     m_ptFileHandler->write( m_tcBuffer, strlen( m_tcBuffer ) );

     //m_ptFileHandler->write("TEST", sizeof("TEST"));
     m_ptFileHandler->close();
    }
  }
}

//----------------------------------------------------------
// Enregistrement de la mesure
//----------------------------------------------------------
void Liveview::recordMeasureSample( float fResult0, float fResult1, float fResult2 )
{
 qDebug( "Liveview::recordMeasureSample : %f %f %f", fResult0, fResult1, fResult2 );
 QDateTime qDateTime;
 char tcBuffer[1024];
 if(  ( m_ptFileHandler != nullptr )
   && ( m_ptFileHandler->open( QIODevice::WriteOnly | QIODevice::Append ) ) )
  {
   // Get du datetime courant
   qDateTime = QDateTime::currentDateTime();
   // Selon le nombre de voie
   switch( tWIMOParametersSensor.tHeader.tSensor.ucChannelNumber )
    {
     //------------
     // 1 voie
     //------------
     case( ( TUCHAR )1 ):
      // Format
      sprintf( tcBuffer,
                "%04d-%02d-%02d %02d:%02d:%02d,%.4f\r\n",
                qDateTime.date().year(), qDateTime.date().month(), qDateTime.date().day(),
                qDateTime.time().hour(), qDateTime.time().minute(), qDateTime.time().second(),
                fResult0 );
     break;
     //------------
     // 2 voies
     //------------
     case( ( TUCHAR )2 ):
      // Format
      snprintf( tcBuffer,
                sizeof( tcBuffer ),
                "%04d-%02d-%02d %02d:%02d:%02d,%.4f,%.4f\r\n",
                qDateTime.date().year(), qDateTime.date().month(), qDateTime.date().day(),
                qDateTime.time().hour(), qDateTime.time().minute(), qDateTime.time().second(),
                fResult0,
                fResult1 );
     break;
     //------------
     // 3 voies
     //------------
     case( ( TUCHAR )3 ):
      // Format
      snprintf( tcBuffer,
                sizeof( tcBuffer ),
                "%04d-%02d-%02d %02d:%02d:%02d,%.4f,%.4f,%.4f\r\n",
                qDateTime.date().year(), qDateTime.date().month(), qDateTime.date().day(),
                qDateTime.time().hour(), qDateTime.time().minute(), qDateTime.time().second(),
                fResult0,
                fResult1,
                fResult2 );
     break;
     default:
     break;
    }



   m_ptFileHandler->write( tcBuffer, strlen( tcBuffer ) );
   m_ptFileHandler->close();
  }
}

//----------------------------------------------------------
// Stop mesure liveview init
//----------------------------------------------------------
Q_INVOKABLE void Liveview::stopMeasureInit( int uiPeriodMeasure )
{
 char tcBuffer[ 1024 ];

 qDebug( "== stopMeasureInit ==" );
 // Ouverture du fichier
 if(  ( m_ptFileHandler != nullptr )
   && (  m_ptFileHandler->open( QIODevice::WriteOnly | QIODevice::Append ) ) )
  {
   // Complément métrologie
   //strcpy( tcBuffer, "\r\n# <WIMO>\r\n# 	<DATA eD0=\"1 sec\"/>\r\n# </WIMO>\r\n" );
   // Format
   snprintf( tcBuffer,
             sizeof( tcBuffer ),
             "\r\n# <WIMO>\r\n# 	<DATA eD0=\"%d sec\"/>\r\n# </WIMO>\r\n",
             uiPeriodMeasure
            );
   m_ptFileHandler->write( tcBuffer, strlen( tcBuffer ) );
   m_ptFileHandler->close();
   m_ptFileHandler = nullptr;
  }
}


