#ifndef DASHBOARDCONTROLER_H
#define DASHBOARDCONTROLER_H

#include <QObject>
#include <QString>

class DashboardControler : public QObject
{
 Q_OBJECT
 Q_PROPERTY(bool m_bBusyRefresh READ getBusyRefresh  WRITE setBusyRefresh  NOTIFY busyRefreshChanged)
public:
 explicit DashboardControler(QObject *parent = nullptr);
  // Ascesseur
 bool getBusyRefresh(){ return m_bBusyRefresh; }
 void setBusyRefresh(bool n){ m_bBusyRefresh = n; emit busyRefreshChanged(m_bBusyRefresh);}

 // Fonction CPP de fin de refresh avec succès
 void vFDashboardHandleEndOfRefresh( void );
 // Appui sur le bouton refresh
 Q_INVOKABLE void refreshSensor();
 // Ouverture page internet de support
 Q_INVOKABLE void openHelpSupportUrl( void );

signals:
 void busyRefreshChanged(bool);
 // Signal qui permet de débloquer l'affichage des onglets après refresh dans QML
 void displayGenericSensor( void );
 // Signal qui permet de débloquer l'affichage des onglets après refresh dans QML
 void displayWiper( void );
 // Signal qui permet de débloquer l'affichage des onglets après refresh dans QML
 void displayO2( void );
 // Signal qui se déclenche à la fin du refresh
 void endOfRefreshSignal( void );

public slots:
private:
 bool m_bBusyRefresh     = false;
};

#endif // DASHBOARDCONTROLER_H
