#include "newthread.h"

QString distance1;
QString Fduo=0;
QString duo=0;
QString life;
bool stoped;
QString deep;
bool getMes=0;
QPoint enddian;
QPoint points[60];
bool ifpoints[60]={0};
char sendBuf[50];
newthread::newthread()
{

    int err = WSAStartup(MAKEWORD( 1, 1 ), &wsaData );

    if ( err != 0 ) {
       qDebug()<<"start failed"<<endl;
    }else{
       qDebug()<<"start seccess"<<endl;
    }

    if ( LOBYTE( wsaData.wVersion ) != 1 ||HIBYTE( wsaData.wVersion ) != 1 ) {
           WSACleanup( );
           qDebug()<<" failed"<<endl;
    }else{
       qDebug()<<"success"<<endl;
    }

    sockSrv=socket(AF_INET,SOCK_DGRAM,0);
    len=sizeof(SOCKADDR);

    local.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    local.sin_family=AF_INET;
    local.sin_port=htons(8080);
    int retVal = bind(sockSrv,(SOCKADDR*)&local,len);
    stoped = false;
}

void newthread::run(){
    while(!stoped){
       // qDebug()<<"thread->run"<<endl;
        Mes();
    }
    stoped=false;
}

void newthread::stop(){
    stoped=false;
    //qDebug()<<"thread stop"<<endl;
}

void newthread::Mes(){
    char recvBuf[50];
    //qDebug()<<"thread->Mes"<<endl;
    int iRecv=recvfrom(sockSrv,recvBuf,50,0,(SOCKADDR*)&from,&len);//from收到客户端的IP信息
    if(iRecv!=SOCKET_ERROR){
        qDebug()<<"receive success"<<endl;
        int* recv=(int*)(recvBuf);
        distance1=QString::number(*recv/58,10);   //一个超声波
        int* recv2=(int*)(recvBuf+4);
        deep=QString::number((*recv2/14,10)); //水深
        Fduo=QString::number((int)recvBuf[9]+256 & 0xff,10);   //前面的雷达
        duo=QString::number((int)recvBuf[8] + 256 & 0xff, 10);  //后面的舵机
        life=QString(recvBuf[11]?"yes":"no");
        int jiaodu;
        jiaodu=QString::number((int)recvBuf[9]+256 & 0xff,10).toInt();
        enddian.setX(170+150*cos(jiaodu*pi/180));enddian.setY(170-sin(jiaodu*pi/180));
        //障碍物
        int x=*recv * cos((double)jiaodu/180 * pi)/116 +170;
        int y=170-*recv * sin((double)jiaodu/180 * pi)/116;
    // qDebug()<<"x="<<x<<"y="<<y;
            points[(int)jiaodu/3].setX(x);
            points[(int)jiaodu/3].setY(y);
            enddian.setX(170+150*cos(jiaodu*pi/180));
            enddian.setY(170-150*sin(jiaodu*pi/180));
    }else{
        qDebug()<<"receive error"<<endl;
    }
    if(sendBuf[0]!=NULL){
        qDebug()<<"send begin"<<endl;
        int iSend=sendto(sockSrv,sendBuf,strlen(sendBuf)+1,0,(SOCKADDR*)&from,len);
        if(iSend==SOCKET_ERROR){
            qDebug()<<"sendto failed"<<WSAGetLastError()<<endl;
            sendBuf[0]=NULL;

        }else{
            sendBuf[0]=NULL;
            qDebug()<<"send success"<<iSend<<endl;
        }
    }
}
