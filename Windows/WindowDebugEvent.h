#ifndef WINDOWDEBUGEVENT_H
#define WINDOWDEBUGEVENT_H

#include <QObject>
#include <qDebug>

class WindowDebugEvent: public QObject
{
 Q_OBJECT
public:
 explicit WindowDebugEvent(QObject *parent = nullptr);
 // Ajout d'une ligne de debug
 void vFAddDebugLine( QString sDebug );

signals:
 void vFAddDebugLineSignal( QString sDebug );

public slots:

private:

};

#endif // WINDOWDEBUGEVENT_H
