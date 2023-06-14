#ifndef HTTPUpdate_H
#define HTTPUpdate_H

#include <QObject>
#include <FileDownloader.h>
#include <GetHTTPFile.h>
#include <QString>
#include <QDirIterator>

struct FilePropertyObject
{
 QString sFileName;
 unsigned long int uliSize;
};

class HTTPUpdate : public QObject
{
Q_OBJECT
public:
 explicit HTTPUpdate(QObject *parent = 0);

 // Démarrage de la vérification de mise à jour
 void startUpdateCheck( void );
 //----------------------------
 // Setter
 //----------------------------
 // Dossier de stockage/réception des données
 void setDataReceivedPath( QString );
 // Repertoire de destination (programme)
 void setDestProgramPath( QString );
 // Url de recherche de la mise à jour
 void setHTTPPath( QString );
 // Définition du numéro de version du logiciel Qt
 void setCurrentSoftwareVersion( QString );
 // Démarrage de la mise à jour
 void startUpdate( void );

private slots:
 void versionLoaded( void );

private:
 // Ecriture des données en entrée dans un fichier
 bool bFWriteToFile( QString, QByteArray );

 // Membre
 FileDownloader * m_ptVersionCtrl;
 GetHTTPFile * m_tHTTPProcess;
 QByteArray  m_tVersion;
 unsigned long int uliFileSize;

 QString m_sVersionFileName = "version.txt";
 QString m_sFilePath = "";
 // Sauvegarde du nom du fichier de mise à jour
 QString m_sExeFileName;
 //----------------
 // Paramètrage
 //----------------
 // Repertoire de destination
 QString m_sDestProgramPath;
 // Dossier de requête HTTP
 QString m_sHTTPPath;
 // Numéro de version courant
 QString m_sCurrentSoftwareVersion;
};

#endif // HTTPUpdate_H
