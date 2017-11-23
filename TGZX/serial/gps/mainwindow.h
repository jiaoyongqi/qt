#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serials.h"

namespace Ui {
class MainWindow;
}


typedef struct _UltrasonicRadarInfo
{
  double RADAR_R_Ultrasonic ;     //超声波雷达数据
}UltrasonicRadarInfo;


typedef struct _MmwRadarInfo
{
  double RADAR_R_Distance;     //前车与本车的相对距离，单位m
  double RADAR_R_Lateral;      //前车与本车的横向距离，单位m，左侧为负
  double RADAR_R_Speed;        //前车与本车的相对速度，单位m/s
}MmwRadarInfo;

static volatile UltrasonicRadarInfo ult_radar_data[4];
static volatile MmwRadarInfo mmw_radar_data[8];


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//    Serials  sCom1;
    QString qComData;
    //串口1接收的数据
    QByteArray qCom1Byte;
    QList<QString> cComItems;
    //COM1集合
    QList<QByteArray> cCom1Items;
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
    void SerialCom1ProcessSlot();
signals:
    void SerialComProcessSignal();
    void SerialCom1ProcessSignal();
};

#endif // MAINWINDOW_H
