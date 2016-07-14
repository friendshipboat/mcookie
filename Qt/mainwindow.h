#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <winsock2.h>
#pragma comment(lib,"Ws2_32.lib")

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    WORD wVersionRequested;
    WSADATA wsaData;
    SOCKET sockSrv;
    SOCKADDR_IN from;
    SOCKADDR_IN local;
    int len;
private:
    Ui::MainWindow *ui;
private slots:
    void wifiRecvAndSend();
};

#endif // MAINWINDOW_H
