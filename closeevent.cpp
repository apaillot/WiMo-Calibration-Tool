#include "closeevent.h"
#include <QDebug>

CloseEvent::CloseEvent( QObject *parent ): QObject(parent)
{
 // Pas d'action de fermeture
 m_bClose = false;
}

void CloseEvent::exitHandler(void)
{
 /*
    qDebug() << mFileName << " closed!" << mTcpPort;
    if (mDataLog)
        mTextStream << mFileName << " closed!" << mTcpPort;
    mFile.close();
*/
 qDebug("!!!!!CLOSING EVENT!!!!!!!");
 // Fermeture
 m_bClose = true;
}

bool CloseEvent::bClose( void )
{
 // Retourne l'état de fermeture
 return( m_bClose );
}
