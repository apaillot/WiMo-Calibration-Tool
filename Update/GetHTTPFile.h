#ifndef GETHTTPFILE_H
#define GETHTTPFILE_H

#include <QObject>
#include <FileDownloader.h>
#include <QString>

class GetHTTPFile : public QObject
{
Q_OBJECT
public:
 explicit GetHTTPFile(QObject *parent = 0);

 void setFileName( QString );
 void setHTTPPath( QString );
 void startRequest( void );
 void loadData();
 QByteArray getData();

signals:
 void dataLoaded( void );
 void dataLoadProgressSignal( qint8 );

private:
 QString m_sFileName;
 QString m_sHTTPPath = "";
 FileDownloader * m_ptHTTPCtrl;
 QUrl m_tHTTPUrl;
 QByteArray m_tData;
};

#endif // GETHTTPFILE_H
