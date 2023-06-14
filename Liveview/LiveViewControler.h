#ifndef LIVEVIEWCONTROLER_H
#define LIVEVIEWCONTROLER_H

#include <QObject>
#include <QString>

class LiveViewControler : public QObject
{
 Q_OBJECT

public:
 explicit LiveViewControler(QObject *parent = nullptr);


public slots:
 void setNumGraphicalBlocEnabled( int );
 void setNumGraphicalBlocDisabled( int );


private:

};

#endif // LIVEVIEWCONTROLER_H
