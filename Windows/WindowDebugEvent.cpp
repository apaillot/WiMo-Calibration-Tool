#include "WindowDebugEvent.h"
#include "main.h"
#include <QFile>
#include <QDateTime>

//****************************************************************************//
// Le code des fonctions
//****************************************************************************//
//----------------------------------------------------------
// Constructeur de la classe
//----------------------------------------------------------
WindowDebugEvent::WindowDebugEvent( QObject *parent ): QObject(parent)
{

}

//----------------------------------------------------------
// Fonction CPP Ajout d'une ligne à la fenêtre de debug
//----------------------------------------------------------
void WindowDebugEvent::vFAddDebugLine( QString sDebug )
{
 QFile m_tFileHandler("debug_log.txt");

 //QDateTime tDatetime = QDateTime::currentDateTime();
 QString sDateTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss,zzz");
 sDateTime.append(" - ");

 if( m_tFileHandler.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append ) )
  {
   sDebug.prepend(sDateTime.toLatin1().data());
   sDebug.append("\n");
   //m_tFileHandler.write( sDebug->toPlainText().toLatin1() );
   m_tFileHandler.write( sDebug.toLatin1().data() );
   m_tFileHandler.close();
  }

 emit vFAddDebugLineSignal( sDebug );
}
