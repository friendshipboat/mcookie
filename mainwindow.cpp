#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QObject>
#include <QString>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer=new QTimer(this);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(wifiRecvAndSend()));
    timer->start(1000);


    int err = WSAStartup(MAKEWORD( 1, 1 ), &wsaData );
/*
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
*/
    sockSrv=socket(AF_INET,SOCK_DGRAM,0);
    len=sizeof(SOCKADDR);

    local.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    local.sin_family=AF_INET;
    local.sin_port=htons(8080);
    int retVal = bind(sockSrv,(SOCKADDR*)&local,len);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::wifiRecvAndSend(){

        char recvBuf[50];
        int iRecv=recvfrom(sockSrv,recvBuf,50,0,(SOCKADDR*)&from,&len);//from收到客户端的IP信息
        if(iRecv==SOCKET_ERROR){
            qDebug()<<"Recive error"<<endl;
        }
        else{
            qDebug()<<*(int*)(recvBuf+1) / 58<<endl;
            int* recv=(int*)(recvBuf+1);
            ui->recvText->setText(QString::number(*recv/58,10));
            qDebug()<<inet_ntoa(local.sin_addr)<<endl;
        }

        char sendBuf[50];
        sprintf(sendBuf,"d",inet_ntoa(from.sin_addr));

        int iSend=sendto(sockSrv,sendBuf,strlen(sendBuf)+1,0,(SOCKADDR*)&from,len);
        if(iSend==SOCKET_ERROR)
        {
            qDebug()<<"sendto failed "<<WSAGetLastError()<<endl;
        }
         else{
                   qDebug()<<"send success "<<iSend<<endl;
        }
}
