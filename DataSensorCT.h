#ifndef DATACTSENSOR_H
#define DATACTSENSOR_H

#include <QObject>
#include <QString>

class DataSensorCT: public QObject
{
 Q_OBJECT
 Q_PROPERTY(QString sTCompensationA     READ getTCompensationA      WRITE setTCompensationA      NOTIFY sTCompensationAChanged)
 Q_PROPERTY(QString sTCompensationB     READ getTCompensationB      WRITE setTCompensationB      NOTIFY sTCompensationBChanged)
 Q_PROPERTY(QString sTCompensationC     READ getTCompensationC      WRITE setTCompensationC      NOTIFY sTCompensationCChanged)
 Q_PROPERTY(QString sTCompensationRaw20 READ getTCompensationRaw20  WRITE setTCompensationRaw20  NOTIFY sTCompensationRaw20Changed)

public:
 explicit DataSensorCT( QObject * parent = nullptr );

 QString getTCompensationA()     { return( sTCompensationA ); }
 QString getTCompensationB()     { return( sTCompensationB ); }
 QString getTCompensationC()     { return( sTCompensationC ); }
 QString getTCompensationRaw20() { return( sTCompensationRaw20 ); }

 void setTCompensationA(QString sNew)     { sTCompensationA=sNew;     emit sTCompensationAChanged(); }
 void setTCompensationB(QString sNew)     { sTCompensationB=sNew;     emit sTCompensationBChanged(); }
 void setTCompensationC(QString sNew)     { sTCompensationC=sNew;     emit sTCompensationCChanged(); }
 void setTCompensationRaw20(QString sNew) { sTCompensationRaw20=sNew; emit sTCompensationRaw20Changed(); }

signals:
 void sTCompensationAChanged();
 void sTCompensationBChanged();
 void sTCompensationCChanged();
 void sTCompensationRaw20Changed();

private:
 QString sTCompensationA     = "";
 QString sTCompensationB     = "";
 QString sTCompensationC     = "";
 QString sTCompensationRaw20 = "";
};

#endif // DATACTSENSOR_H
