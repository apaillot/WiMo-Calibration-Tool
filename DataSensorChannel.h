#ifndef DATASENSORCHANNEL_H
#define DATASENSORCHANNEL_H

#include <QObject>
#include <QString>

class DataSensorChannel: public QObject
{
 Q_OBJECT
 Q_PROPERTY(bool    bActive               READ getActive          WRITE setActive          NOTIFY activeChanged)
 Q_PROPERTY(bool    bEnabled              READ getEnabled         WRITE setEnabled         NOTIFY enabledChanged)
 Q_PROPERTY(QString sName                 READ getName            WRITE setName            NOTIFY nameChanged)
 Q_PROPERTY(int uiID                      READ getID              WRITE setID              NOTIFY idChanged)
 Q_PROPERTY(QString sID                   READ getSID             WRITE setSID             NOTIFY sidChanged)
 Q_PROPERTY(QString sUnit                 READ getUnit            WRITE setUnit            NOTIFY unitChanged)
 Q_PROPERTY(QStringList tsUnitList        READ getUnitList        WRITE setUnitList        NOTIFY unitListChanged)
 Q_PROPERTY(QString sCalibrationType      READ getCalibrationType WRITE setCalibrationType NOTIFY calibrationTypeChanged)
 Q_PROPERTY(QStringList tsCalibrationList READ getCalibrationList WRITE setCalibrationList NOTIFY calibrationListChanged)
 Q_PROPERTY(int uiAverage                 READ getAverage         WRITE setAverage         NOTIFY averageChanged)
 Q_PROPERTY(QString sFormula              READ getFormula         WRITE setFormula         NOTIFY formulaChanged)
 Q_PROPERTY(QString sCalibrationDate      READ getCalibrationDate WRITE setCalibrationDate NOTIFY calibrationDateChanged)

 Q_PROPERTY(int uiIndex                 READ getIndex         WRITE setIndex         NOTIFY indexChanged)
 Q_PROPERTY(double dValue               READ getValue         WRITE setValue         NOTIFY valueChanged)
 Q_PROPERTY(QString sValue              READ getSValue        WRITE setSValue        NOTIFY svalueChanged)
 Q_PROPERTY(bool   bIsCorrectedPt       READ getIsCorrectedPt   WRITE setIsCorrectedPt   NOTIFY isCorrectedPtChanged)

public:
 explicit DataSensorChannel( QObject * parent = nullptr );

 bool getActive()                 { return( bActive ); }
 bool getEnabled()                { return( bEnabled ); }
 QString getName()                { return( sName ); }
 int getID()                      { return( uiID ); }
 QString getSID()                 { return( sID ); }
 QString getUnit()                { return( sUnit ); }
 QStringList getUnitList()        { return( tsUnitList ); }
 QString getCalibrationType()     { return( sCalibrationType ); }
 QStringList getCalibrationList() { return( tsCalibrationList ); }
 int getAverage()                 { return( uiAverage ); }
 QString getFormula()             { return( sFormula ); }
 QString getCalibrationDate()     { return( sCalibrationDate ); }
 int getIndex()                   { return( uiIndex ); }
 double getValue()                { return( dValue ); }
 QString getSValue()              { return( sValue ); }
 double getIsCorrectedPt()        { return( bIsCorrectedPt ); }


 void setActive(bool bNew)                  { bActive=bNew;     emit activeChanged(); }
 void setEnabled(bool bNew)                 { bEnabled=bNew;    emit enabledChanged(); }
 void setName(QString sNew)                 { sName=sNew;       emit nameChanged(); }
 void setID(int uiNew)                      { uiID=uiNew;       emit idChanged(); }
 void setSID(QString sNew)                  { sID=sNew;         emit sidChanged(); }
 void setUnit(QString sNew)                 { sUnit=sNew;       emit unitChanged(); }
 void setUnitList(QStringList tsNew)        { tsUnitList=tsNew; emit unitListChanged(); }
 void setCalibrationType(QString sNew)      { sCalibrationType=sNew;   emit calibrationTypeChanged(); }
 void setCalibrationList(QStringList tsNew) { tsCalibrationList=tsNew; emit calibrationListChanged(); }
 void setAverage(int uiNew)                 { uiAverage=uiNew;         emit averageChanged(); }
 void setFormula(QString sNew)              { sFormula=sNew;           emit formulaChanged(); }
 void setCalibrationDate(QString sNew)      { sCalibrationDate=sNew;   emit calibrationDateChanged(); }
 void setIndex(int uiNew)                   { uiIndex=uiNew;   emit indexChanged(); }
 void setValue(double dNew)                 { dValue=dNew;     emit valueChanged(); }
 void setSValue(QString sNew)               { sValue=sNew;     emit svalueChanged(); }
 void setIsCorrectedPt(bool xNew)           { bIsCorrectedPt=xNew;   emit isCorrectedPtChanged(); }

signals:
 void activeChanged();
 void enabledChanged();
 void nameChanged();
 void idChanged();
 void sidChanged();
 void unitChanged();
 void unitListChanged();
 void calibrationTypeChanged();
 void calibrationListChanged();
 void averageChanged();
 void formulaChanged();
 void calibrationDateChanged();
 void indexChanged();
 void valueChanged();
 void svalueChanged();
 void isCorrectedPtChanged();

private:
 bool    bActive    = false;
 bool    bEnabled   = false;
 QString sName      = "";
 int uiID           = 0;
 QString sID        = "";
 QString sUnit      = "";
 QStringList tsUnitList = {""};
 QString sCalibrationType = "";
 QStringList tsCalibrationList = {""};
 int uiAverage    = 0;
 QString sFormula = "";
 QString sCalibrationDate = "";
 int     uiIndex = 0;
 double  dValue  = 0.0;
 QString sValue  = "";
 bool bIsCorrectedPt  = false;
};


#endif // DATASENSORCHANNEL_H
