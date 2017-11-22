#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serials.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//    Serials  sCom1;
    QString qComData;
    QList<QString> cComItems;
private:
    Ui::MainWindow *ui;
    QList<QString> cComData;//数据集合
    QString gRTK;
    Serials  sCom;//Robot板子
    bool cComRev;//接收标志
    QString outputcheck(QString s);

private slots:
    void SerialComRevSlot(QByteArray RxData);
    void SerialComProcessSlot();
signals:
    void SerialComProcessSignal();
};

#endif // MAINWINDOW_H
