#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QObject>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    t.start();
    middle.setX(170);middle.setY(170);
    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(changeMes()));
    timer->start(1000);
}

void MainWindow::changeMes(){
    MainWindow::repaint();
    ui->deeptext->setText(deep);
    ui->DJjiaodu->setText(duo);
    ui->distance1text->setText(distance1);
    ui->LDjiaodu->setText(Fduo);
    ui->lifeText->setText(life);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *){
    paint=new QPainter;
    paint->begin(this);
    paint->setBrush(QBrush(Qt::black,Qt::SolidPattern));
    paint->drawEllipse(middle,150,150);

    paint->setPen(QPen(Qt::blue,2,Qt::DashLine));
    paint->drawEllipse(middle,120,120);
    paint->drawEllipse(middle,90,90);
    paint->drawEllipse(middle,60,60);
    paint->drawEllipse(middle,30,30);
    for(int i=0;i<60;i++){
        if(!points[i].isNull()){
            paint->setBrush(QBrush(Qt::white,Qt::SolidPattern));
            paint->drawEllipse(points[i],2,2);
            paint->setPen(QPen(Qt::yellow,2,Qt::DashLine));
            paint->drawLine(middle,enddian);
        }

    }
}

void MainWindow::on_upButton_clicked()
{
    sendBuf[0]='w';
}

void MainWindow::on_leftButton_clicked()
{
    sendBuf[0]='a';
}

void MainWindow::on_downButton_clicked()
{
    sendBuf[0]='s';
}

void MainWindow::on_rightButton_clicked()
{
    sendBuf[0]='d';
}

void MainWindow::on_startbutton_clicked()
{
    stoped=0;
}

void MainWindow::on_stopbutton_clicked()
{
    stoped=1;
}
