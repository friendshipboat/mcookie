#include "mainwindow.h"
#include <QApplication>

#include <QDebug>>
#pragma comment(lib,"Ws2_32.lib")
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();



       //closesocket(sockSrv);
       //WSACleanup();
    return a.exec();
}


