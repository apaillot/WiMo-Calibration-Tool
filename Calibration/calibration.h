//============================================================================//
// Fichier .........: "calibration.h"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2020/02/21
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef CALIBRATION_H
#define CALIBRATION_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <QObject>
#include <QString>
using namespace std;
#include <TypeC.h>
#include <TypeCMMN.h>

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

//============================================================================//
// Déclaration des variables non exportées
//============================================================================//

//============================================================================//
// Déclaration des macros
//============================================================================//

//============================================================================//
// Déclaration des prototypes de fonctions exportées
//============================================================================//
//----------------------------------------------------------
// Reset des points de calibration dans objet QML
//----------------------------------------------------------
//extern void vFCalibrationResetPointToQml( void );
//----------------------------------------------------------
// Reset des points de calibration dans objet QML
//----------------------------------------------------------
extern void vFCalibrationResetPointToQmlWithChannel( TUCHAR );
//----------------------------------------------------------
// Remplissage des points de calibration dans objet QML
//----------------------------------------------------------
extern void vFCalibrationFillPointToQML( TUINT );
//----------------------------------------------------------
// Ajout d'un point de calibration
//----------------------------------------------------------
extern void vFCalibrationAddPoint( TUINT, TFLOAT, TFLOAT );

//============================================================================//
// Déclaration des prototypes de class
//============================================================================//

//--------------------------------------------------
// Class de gestion de la partie calibration
//--------------------------------------------------
class Calibration : public QObject
{
 Q_OBJECT
 Q_PROPERTY(int channelNumber  READ getChannelNumber   WRITE setChannelNumber  NOTIFY channelNumberChanged)
 Q_PROPERTY(int currentChannel READ getCurrentChannel  WRITE setCurrentChannel NOTIFY currentChannelChanged)
 Q_PROPERTY(int iCurrentChannelID READ getCurrentChannelID  WRITE setCurrentChannelID  NOTIFY currentChannelIDChanged)
 Q_PROPERTY(bool bBusyState    READ getBusyState       WRITE setBusyState      NOTIFY busyStateChanged)
 Q_PROPERTY(bool bErrorState   READ getErrorState      WRITE setErrorState     NOTIFY errorStateChanged)
 //%%AP - 2021.11.18 - Ajout
 Q_PROPERTY(QStringList tsChannelList READ getChannelList     WRITE setChannelList     NOTIFY channelListChanged)

 // Ch0
 Q_PROPERTY(QString ch0Name     READ getCh0Name      WRITE setCh0Name       NOTIFY ch0NameChanged)
 Q_PROPERTY(QString ch0Unit     READ getCh0Unit      WRITE setCh0Unit       NOTIFY ch0UnitChanged)
 Q_PROPERTY(int     ch0ID       READ getCh0ID        WRITE setCh0ID         NOTIFY ch0IDChanged)
 Q_PROPERTY(QString ch0Type     READ getCh0Type      WRITE setCh0Type       NOTIFY ch0TypeChanged)
 Q_PROPERTY(int     ch0Average  READ getCh0Average   WRITE setCh0Average    NOTIFY ch0AverageChanged)
 Q_PROPERTY(QString ch0Formula  READ getCh0Formula   WRITE setCh0Formula    NOTIFY ch0FormulaChanged)
 Q_PROPERTY(QString ch0DateTime READ getCh0DateTime  WRITE setCh0DateTime   NOTIFY ch0DateTimeChanged)
 /*
 Q_PROPERTY(float   ch0A1       READ getCh0A1        WRITE setCh0A1         NOTIFY ch0A1Changed)
 Q_PROPERTY(float   ch0A2       READ getCh0A2        WRITE setCh0A2         NOTIFY ch0A2Changed)
 Q_PROPERTY(float   ch0A3       READ getCh0A3        WRITE setCh0A3         NOTIFY ch0A3Changed)
 Q_PROPERTY(float   raw20       READ getRaw20        WRITE setRaw20         NOTIFY raw20Changed)
 */
 Q_PROPERTY(float   calibTheoRaw1 READ getCalibTheoRaw1 WRITE setCalibTheoRaw1 NOTIFY calibTheoRaw1Changed)
 Q_PROPERTY(float   calibTheoRaw2 READ getCalibTheoRaw2 WRITE setCalibTheoRaw2 NOTIFY calibTheoRaw2Changed)
 Q_PROPERTY(float   calibTheoPhy1 READ getCalibTheoPhy1 WRITE setCalibTheoPhy1 NOTIFY calibTheoPhy1Changed)
 Q_PROPERTY(float   calibTheoPhy2 READ getCalibTheoPhy2 WRITE setCalibTheoPhy2 NOTIFY calibTheoPhy2Changed)

  // Ch1
 Q_PROPERTY(QString ch1Name     READ getCh1Name      WRITE setCh1Name       NOTIFY ch1NameChanged)
 Q_PROPERTY(QString ch1Unit     READ getCh1Unit      WRITE setCh1Unit       NOTIFY ch1UnitChanged)
 Q_PROPERTY(int     ch1ID       READ getCh1ID        WRITE setCh1ID         NOTIFY ch1IDChanged)
 Q_PROPERTY(QString ch1Type     READ getCh1Type      WRITE setCh1Type       NOTIFY ch1TypeChanged)
 Q_PROPERTY(int     ch1Average  READ getCh1Average   WRITE setCh1Average    NOTIFY ch1AverageChanged)
 Q_PROPERTY(QString ch1Formula  READ getCh1Formula   WRITE setCh1Formula    NOTIFY ch1FormulaChanged)
 Q_PROPERTY(QString ch1DateTime READ getCh1DateTime  WRITE setCh1DateTime   NOTIFY ch1DateTimeChanged)
 // Ch2
 Q_PROPERTY(QString ch2Name     READ getCh2Name      WRITE setCh2Name       NOTIFY ch2NameChanged)
 Q_PROPERTY(QString ch2Unit     READ getCh2Unit      WRITE setCh2Unit       NOTIFY ch2UnitChanged)
 Q_PROPERTY(int     ch2ID       READ getCh2ID        WRITE setCh2ID         NOTIFY ch2IDChanged)
 Q_PROPERTY(QString ch2Type     READ getCh2Type      WRITE setCh2Type       NOTIFY ch2TypeChanged)
 Q_PROPERTY(int     ch2Average  READ getCh2Average   WRITE setCh2Average    NOTIFY ch2AverageChanged)
 Q_PROPERTY(QString ch2Formula  READ getCh2Formula   WRITE setCh2Formula    NOTIFY ch2FormulaChanged)
 Q_PROPERTY(QString ch2DateTime READ getCh2DateTime  WRITE setCh2DateTime   NOTIFY ch2DateTimeChanged)

public:
 Calibration( QObject *parent=nullptr );
 // Channel number
 int getChannelNumber(void);
 void setChannelNumber(int);
 // Current channel
 int getCurrentChannel(void);
 void setCurrentChannel(int);
 // ID de la voie de mesure
 int getCurrentChannelID(void);
 void setCurrentChannelID(int);
 // Etat occupé : aucunes autres commandes ne doivent être lancées
 bool getBusyState(void);
 void setBusyState(bool);
 // Etat erreur : affichage de l'error cross
 bool getErrorState(void);
 void setErrorState(bool);
 QStringList getChannelList() { return( tsChannelList ); }
 void setChannelList(QStringList tsNew) { tsChannelList=tsNew;    emit channelListChanged(); }

 // Ch0 - Name
 QString getCh0Name() const;
 void setCh0Name( const QString & );
 // Ch0 - Unit
 QString getCh0Unit() const;
 void setCh0Unit( const QString & );
 // Ch0 - ID
 int getCh0ID() const;
 void setCh0ID( int );
 // Ch0 - Type
 QString getCh0Type() const;
 void setCh0Type( const QString & );
 // Ch0 - Average
 int getCh0Average() const;
 void setCh0Average( int );
 // Ch0 - Formula
 QString getCh0Formula() const;
 void setCh0Formula( const QString & );
 // Ch0 - DateTime
 QString getCh0DateTime() const;
 void setCh0DateTime( const QString & );
 /*
 // Ch0 - A1
 float getCh0A1() const;
 void setCh0A1( const float );
 // Ch0 - A2
 float getCh0A2() const;
 void setCh0A2( const float );
 // Ch0 - A3
 float getCh0A3() const;
 void setCh0A3( const float );
 // Ch0 - A3
 float getRaw20() const;
 void setRaw20( const float );
 */

 // Ch0 - Calib Theo Raw1
 float getCalibTheoRaw1() const;
 void setCalibTheoRaw1( const float );
 // Ch0 - Calib Theo  Raw2
 float getCalibTheoRaw2() const;
 void setCalibTheoRaw2( const float );
 // Ch0 - Calib Theo Phy1
 float getCalibTheoPhy1() const;
 void setCalibTheoPhy1( const float );
 // Ch0 - Calib Theo Phy2
 float getCalibTheoPhy2() const;
 void setCalibTheoPhy2( const float );


 // Ch1 - Name
 QString getCh1Name() const;
 void setCh1Name( const QString & );
 // Ch1 - Unit
 QString getCh1Unit() const;
 void setCh1Unit( const QString & );
 // Ch1 - ID
 int getCh1ID() const;
 void setCh1ID( int );
 // Ch1 - Type
 QString getCh1Type() const;
 void setCh1Type( const QString & );
 // Ch1 - Average
 int getCh1Average() const;
 void setCh1Average( int );
 // Ch1 - Formula
 QString getCh1Formula() const;
 void setCh1Formula( const QString & );
 // Ch1 - DateTime
 QString getCh1DateTime() const;
 void setCh1DateTime( const QString & );

 // Ch2 - Name
 QString getCh2Name() const;
 void setCh2Name( const QString & );
 // Ch2 - Unit
 QString getCh2Unit() const;
 void setCh2Unit( const QString & );
 // Ch2 - ID
 int getCh2ID() const;
 void setCh2ID( int );
 // Ch2 - Type
 QString getCh2Type() const;
 void setCh2Type( const QString & );
 // Ch2 - Average
 int getCh2Average() const;
 void setCh2Average( int );
 // Ch2 - Formula
 QString getCh2Formula() const;
 void setCh2Formula( const QString & );
 // Ch2 - DateTime
 QString getCh2DateTime() const;
 void setCh2DateTime( const QString & );

 // Current channel
 Q_INVOKABLE void removeCalibPoint( int, int );
 // Current channel
 Q_INVOKABLE void removeAllCalibPoint( void );
 // Mise à jour point calibration
 Q_INVOKABLE void updateCalibPoint( void );
 // Validation calibration settings
 Q_INVOKABLE void submitCalibSettings( unsigned int  uiChannelSettings,
                                       QString       sUnit,
                                       unsigned char ucCalibTYpe,
                                       unsigned int  uiAverage );
 // Mise à jour du tableau des points de calibration
 Q_INVOKABLE void submitCTCompensationFactor( float, float, float, float );
 // Ajout d'un point de calibration
 Q_INVOKABLE void addCalibPoint( unsigned int, float, float );
 // Sauvegarde du datetime de la calibration
 Q_INVOKABLE QString sSaveCurrentCalibrationDateTime( unsigned int );
 // Sauvegarde de la calibration au format PDF
 Q_INVOKABLE void exportCalibrationToPdf( QString sFilePath );
 // Test si un fichier existe
 Q_INVOKABLE bool bFIsCalibrationReportExist( QString sFilenamePath );
 // Génération du nom du rapport de calibration
 Q_INVOKABLE QString sFGenerateCalibReportFilename( void );
 // Renvoi le dossier courant
 Q_INVOKABLE QString getCurrentPathFile( void );

signals:
 void channelNumberChanged( void );
 void currentChannelChanged( void );
 void currentChannelIDChanged( void );
 void busyStateChanged( void );
 void errorStateChanged( void );
// void currentChannelChanged(int iCurrentChannel);
 void channelListChanged();

 // Ch0
 void ch0NameChanged( QString );
 void ch0UnitChanged( QString );
 void ch0IDChanged( int );
 void ch0TypeChanged( QString );
 void ch0AverageChanged( int );
 void ch0FormulaChanged( QString );
 void ch0DateTimeChanged( QString );
 /*
 void ch0A1Changed( float );
 void ch0A2Changed( float );
 void ch0A3Changed( float );
 void raw20Changed( float );
 */
 void calibTheoRaw1Changed( float );
 void calibTheoRaw2Changed( float );
 void calibTheoPhy1Changed( float );
 void calibTheoPhy2Changed( float );

 // Ch1
 void ch1NameChanged( QString );
 void ch1UnitChanged( QString );
 void ch1IDChanged( int );
 void ch1TypeChanged( QString );
 void ch1AverageChanged( int );
 void ch1FormulaChanged( QString );
 void ch1DateTimeChanged( QString );
 // Ch2
 void ch2NameChanged( QString );
 void ch2UnitChanged( QString );
 void ch2IDChanged( int );
 void ch2TypeChanged( QString );
 void ch2AverageChanged( int );
 void ch2FormulaChanged( QString );
 void ch2DateTimeChanged( QString );

private:
 int  m_uiChannelNumber    = 0;
 int  m_uiCurrentChannel   = 0;
 int  m_iCurrentChannelID  = 0;
 bool m_bBusyState         = false;
 bool m_bErrorState        = false;
 QStringList tsChannelList = {""};
 // Ch0
 QString m_sCh0Name     = "";
 QString m_sCh0Unit     = "";
 int     m_iCh0ID       = 0;
 QString m_sCh0Type     = "";
 int     m_iCh0Average  = 1;
 QString m_sCh0Formula  = "";
 QString m_sCh0DateTime = "";
 /*
 float   m_fCh0A1 = 0;
 float   m_fCh0A2 = 0;
 float   m_fCh0A3 = 0;
 float   m_fRaw20 = 0;
 */
 float   m_fRaw1 = 0;
 float   m_fPhy1 = 0;
 float   m_fRaw2 = 0;
 float   m_fPhy2 = 0;
 // Ch1
 QString m_sCh1Name     = "";
 QString m_sCh1Unit     = "";
 int     m_iCh1ID       = 0;
 QString m_sCh1Type     = "";
 int     m_iCh1Average  = 1;
 QString m_sCh1Formula  = "";
 QString m_sCh1DateTime = "";

 // Ch2
 QString m_sCh2Name     = "";
 QString m_sCh2Unit     = "";
 int     m_iCh2ID       = 0;
 QString m_sCh2Type     = "";
 int     m_iCh2Average  = 1;
 QString m_sCh2Formula  = "";
 QString m_sCh2DateTime = "";
 // Autre
 char m_tcSaveDateTime[sizeof( "HH:MM:SS JJ/MM/AA" ) + 2 + 1];
 QString m_sSaveDateTime;
};



#endif // CALIBRATION_H
