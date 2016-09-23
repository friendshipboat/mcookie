#ifndef NEWTHREAD_H
#define NEWTHREAD_H


#include <QThread>
#include <QWidget>
#include <QtCore>
#include <QObject>
#include <math.h>
#include <winsock2.h>
#include <QPointer>
extern QString distance1;
extern QString Fduo;
extern QString duo;
extern QString life;
extern QString deep;
extern bool stoped;
extern bool getMes;
const double pi=acos(-1.0);
extern QPoint enddian;
extern QPoint points[60];
extern bool ifPoints[60];
extern char sendBuf[50];
class newthread:public QThread
{

public:
    WORD wVersionRequested;
    WSADATA wsaData;
    SOCKET sockSrv;
    SOCKADDR_IN from;
    SOCKADDR_IN local;
    int len;

    newthread();
    void stop();
protected:
    void run();
    void Mes();
    void wait();
private:
    volatile bool stoped;
};

#endif // NEWTHREAD_H
