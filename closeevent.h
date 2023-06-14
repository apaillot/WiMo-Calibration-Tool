#ifndef CLOSEEVENT_H
#define CLOSEEVENT_H

#include <QObject>

class CloseEvent: public QObject
{
 Q_OBJECT
public:
 //();
 explicit CloseEvent(QObject *parent = nullptr);
 bool bClose( void );

public slots:
 void exitHandler(void);
private:
 bool m_bClose;
};

#endif // CLOSEEVENT_H
