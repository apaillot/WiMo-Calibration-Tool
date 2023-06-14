//============================================================================//
// Fichier .........: "wiper.h"
// Auteur ..........: Alexandre PAILLOT
// Date de création : 2020/03/24
//----------------------------------------------------------------------------//
/* Description :
*/
//============================================================================//

#ifndef WIPER_H
#define WIPER_H

//============================================================================//
// Inclusion des librairies
//============================================================================//
#include <QObject>

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

//============================================================================//
// Déclaration des prototypes de class
//============================================================================//

//--------------------------------------------------
// Class de gestion de la partie calibration
//--------------------------------------------------
class Wiper : public QObject
{
 Q_OBJECT
 Q_PROPERTY(int uiClockwise     READ getClockwise       WRITE setClockwise      NOTIFY clockwiseChanged)
 Q_PROPERTY(int uiAnticlockwise READ getAnticlockwise   WRITE setAnticlockwise  NOTIFY anticlockwiseChanged)
 Q_PROPERTY(bool bBusyState     READ getBusyState       WRITE setBusyState      NOTIFY busyStateChanged)
 Q_PROPERTY(bool bErrorState    READ getErrorState      WRITE setErrorState     NOTIFY errorStateChanged)

public:
 explicit Wiper(QObject *parent = nullptr);
 // Clockwise angle
 int getClockwise(void);
 void setClockwise(int);
 // Anticlockwise angle
 int getAnticlockwise(void);
 void setAnticlockwise(int);
 // Etat occupé : aucunes autres commandes ne doivent être lancées
 bool getBusyState(void);
 void setBusyState(bool);
 // Etat erreur : affichage de l'error cross
 bool getErrorState(void);
 void setErrorState(bool);

 // Validation angle de balai
 Q_INVOKABLE void submitWiperAngle( unsigned int  uiClockwise,
                                    unsigned int  uiAnticlockwise );
 // Sauvegarde de la configuration vers le balai
 Q_INVOKABLE void saveConfiguration( void );
 // Start wiper
 Q_INVOKABLE void startWiper( void );

signals:
 void clockwiseChanged( void );
 void anticlockwiseChanged( void );
 void busyStateChanged( void );
 void errorStateChanged( void );

private:
 int  m_uiClockwise;
 int  m_uiAnticlockwise;
 bool m_bBusyState;
 bool m_bErrorState;
};

#endif // WIPER_H
