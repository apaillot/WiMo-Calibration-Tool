#ifndef WINDOWCOMEVENT_H
#define WINDOWCOMEVENT_H

#include <QObject>
#include <QString>
#include <TypeC.h>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <main.h>

class ComPort
{
public:
    ComPort( unsigned char ucPortCom, unsigned char ucSN_Y, unsigned int uiSN_N, QString sSN );

    unsigned char ucPortCom();
    unsigned char ucSN_Y();
    unsigned int  uiSN_N();
    QString  sSN();
//private:
    unsigned char m_ucPortCom;
    unsigned char m_ucSN_Y;
    unsigned int  m_uiSN_N;
    QString  m_sSN;
};
/*
// Stockage des ports COM nke détecté
class ComPortObject : public QAbstractListModel
{
 Q_OBJECT
public:
 ComPortObject(QObject *parent = nullptr);

 private:

};
*/

class WindowCOMEvent: public QObject
{
 Q_OBJECT
public:
 explicit WindowCOMEvent(QObject *parent = nullptr);

 void vFClearCOMPortList( void );
 void vFAddCOMPort( TUCHAR, TUCHAR, TUINT );
 void vFOpenCOMPortList( void );
 void vFOpenCOMPortManualWindow( void );

 bool bFComPortSelected( void );
 // Nom du port COM sélectionné par index
 QString sFComPortNameByIndex( TUCHAR ucIndex );
 // Fonction CPP Fermeture du port COM
 Q_INVOKABLE bool bFCloseCOMPort( void );
 // Ecriture du baudrate
 Q_INVOKABLE void setBaudrate( int );

 // Port série
 QSerialPort m_serialPort;

signals:
 void vFClearCOMPortListSignal( void );
 void vFAddCOMPortSignal( QString sCOMPort, int iPortCom, int iSN_Y, int uliSN_N, QString sSN );
 void vFOpenCOMPortListSignal( void );
 void vFOpenCOMPortManualWindowSignal( void );
 void vFComPortSuccefullyOpenSignal( void );
 void vFComPortSuccefullyCloseSignal( void );

public slots:
 Q_INVOKABLE void vFPortComSubmit( QString sPortCOM, unsigned int uiPortComIndex );
 // Détection des FTDIs nke présent
 Q_INVOKABLE unsigned int uiFDetectionCOMFtdiInterfaceNke( void );
 // Fonction CPP Vérification si port ouvert : réouverture sinon
 Q_INVOKABLE bool bFReconnectPortCom( void );
 // Ouverture du port COM
 Q_INVOKABLE bool bFOpenCOMPort( QString sPortCom );
 // Remplissage des ports COM de la fenêtre de détection automatique
 void vFFillComPortAutomaticWindow( void );
 // Choix de la methode de sélection du port COM
 Q_INVOKABLE void vFComPortFindAutomaticSelection( bool bInit );
 // Get du nom du port COM
 Q_INVOKABLE QString sFGetPortComName( void );
 // Numéro du port COM sélectionné par index
 Q_INVOKABLE TINT iFComPortNumberByIndex( TUCHAR );
 // Get du nom du port COM
 Q_INVOKABLE QString sFGetPortComSNByIndex( TUCHAR ucIndex ){ return(m_ttCOMPort[ucIndex].sSN()); }

private:
 QString m_sPortComName;
 unsigned int m_uiPortComIndex;
 bool m_bComPortSelected = false;

 //%%AP - 2020.09.21 - Tentative public
 // Port série
 //QSerialPort m_serialPort;

 // Création d'un object dynamique pour stockage port COM
 QVector<ComPort> m_ttCOMPort;
 // Vitesse de la liaison
 QSerialPort::BaudRate m_eBaudRate = QSerialPort::Baud57600;
};

#endif // WINDOWCOMEVENT_H
