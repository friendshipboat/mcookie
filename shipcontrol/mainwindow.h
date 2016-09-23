#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "newthread.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    newthread t;
    QPoint middle;

    QPainter *paint;
public slots:
    void changeMes();

private slots:
    void on_upButton_clicked();

    void on_leftButton_clicked();

    void on_downButton_clicked();

    void on_rightButton_clicked();

    void on_startbutton_clicked();

    void on_stopbutton_clicked();

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent* );
};

#endif // MAINWINDOW_H
