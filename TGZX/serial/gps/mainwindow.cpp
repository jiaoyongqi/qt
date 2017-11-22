#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>

QString MainWindow::outputcheck(QString s)
{
    QString ss;
    QByteArray pp = s.toLatin1();
    char *c_str2 = pp.data();
    int result=c_str2[1];char d='*';
    for(int i=2;c_str2[i]!=d;i++)
    {
        result^=c_str2[i];
    }
    if(result<16)
    {
        ss="0"+QString::number(result,16).toUpper();
    }
    else
    {
        ss=QString::number(result,16).toUpper();
    }
    return ss;
}

void MainWindow::SerialComRevSlot(QByteArray RxData)
{
    QString rData(RxData);
    if(!rData.isEmpty())
    {
        qComData.append(rData);
        if(qComData.contains("\n"))
        {
            printf("contains nnnnnnnnnnnn \n");
            QString str=qComData.split('\n').at(0);
            int ind=str.indexOf("$");
//            printf("before del ind =%d str = %s\n",ind,str.data());
            int i;
            int result=0;
            bool ok;
            if(ind!=-1)
            {

                QByteArray pp = str.toLatin1();
                char *c_str2 = pp.data();
                char d='*';
                for(i=ind+1;c_str2[i]!=d&&i<str.length();i++)
                {
                    result^=c_str2[i];
                }
                if(i+2<str.length())
                {
                    QString xx = str.mid(i+1,2);
                    int sum=xx.toInt(&ok,16);
                    printf("SerialComRevSlot before sum == result\n");
                    if(sum==result)
                    {
                        cComItems.append(str);
                        printf("str1111 = %s\n",str.toStdString().data());
                        emit SerialComProcessSignal();
                    }
                }
            }
            printf("serial com rev slot out\n");
            qComData = qComData.right(qComData.length()-qComData.indexOf('\n')-1);
          }

    }
}

void MainWindow::SerialComProcessSlot()
{
    printf("SerialComProcessSlot in\n");
    foreach(QString ba, cComItems)
    {
        if(!ba.isEmpty())
        {
            printf("SerialComProcessSlot ba = %s\n",ba.data());
            if(ba.indexOf("$GTIMU")!=-1)
            {
                printf("deal GTIMU\n");
                QStringList qsList=ba.split(",");
                printf("qsListsize= %d\n",qsList.size());
                if(qsList.size()>8)//jyqi modify
                {
                    printf("GTIMU list \n");
                    //差分状态
                    //QStringList ee=qsList.at(15).split('*');
                    //gRTK=qsList.at(15).left(2);
                    //是否开启收集标志
                    //第位纬度值，第七位经度值，第八位海拔值
                     //QString   head=qsList.at(3);
                     QString   AccX=qsList.at(6);
                     QString   GytoZ=qsList.at(5);
                     QString   AccY=qsList.at(7);

                    printf("GytoZ=%s,AccX=%s,AccY=%s\n",GytoZ.toStdString().data(),AccX.toStdString().data(),AccY.toStdString().data());
                    if(cComRev)
                    {
                          //QString tt="$,"+head+","+lat+","+lon+","+alt+","+gRTK+","+"*";
                          QString tt="$,"+GytoZ+","+AccX+","+AccY+","+"*";
                          printf("reve1111 = %s\n",tt.toStdString().data());
                          QString ff=outputcheck(tt);
                          QString xx=tt+ff+"\r\n";
                          //存入数据
                          cComData.append(xx);

                    }
                    //刷新Com1数据显示
                    //ComDataDisplay();
                }

            }else if(ba.indexOf("$GPFPD")!=-1)
            {
                printf("deal GPFPD\n");
                QStringList gpList=ba.split(",");
                printf("GPFPD qsListsize= %d\n",gpList.size());
                if(gpList.size()>=16)//jyqi modify
                {
                    printf("GPFPD list \n");
                    QStringList ee=gpList.at(15).split('*');
                    QString Status=gpList.at(15).left(2);
                    QString   Heading=gpList.at(3);
                    QString   Lattitude=gpList.at(6);
                    QString   Longitude=gpList.at(7);
                    QString   Altitude=gpList.at(8);
                    QString   Ve=gpList.at(9);
                    QString   Vn=gpList.at(10);
                    QString   NSV1=gpList.at(13);
                    QString   NSV2=gpList.at(14);
//                    QString   Status=gpList.at(15);

                   printf("Heading=%s,Lattitude=%s,Longitude=%s,Altitude=%s,Ve=%s,Vn=%s,NSV1=%s,NSV2=%s,Status=%s\n",Heading.toStdString().data(),Lattitude.toStdString().data(),Longitude.toStdString().data(),
                          Altitude.toStdString().data(),Ve.toStdString().data(),Vn.toStdString().data()
                          ,NSV1.toStdString().data(),NSV2.toStdString().data(),Status.toStdString().data());

                }
            }
        }
        //剔除数据
        cComItems.removeFirst();
    }
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //COM1="COM1,115200,0,0,0,0,0"
    //for(int i = 0;i < 5;i++){
        this->sCom.SerialOpen("/dev/ttyUSB1","115200",0,0,0,0,0);
        if(this->sCom.isOpened==true)
        {
            printf("open successed\n");
        }
        else
        {
            printf("open failed\n");
        }

        //QString sData="$GTIMU,0,13798.300,-0.1267,0.2201,-0.0631,0.3407,0.1559,0.9255,49.8*68\n";
        //this->sCom.SerialWrite(sData.toLatin1());

    //}


    //COM1接收数据的槽
    QObject::connect(&this->sCom, SIGNAL(SerialRecive(QByteArray)), this, SLOT(SerialComRevSlot(QByteArray)));
    //COM1处理数据的槽
    QObject::connect(this, SIGNAL(SerialComProcessSignal()), this, SLOT(SerialComProcessSlot()));



   //sCom1.SerialRead();
    //QObject::connect(&this->sCom1, SIGNAL(SerialRecive(QByteArray)), this, SLOT(SerialCom1RevSlot(QByteArray)));

}

MainWindow::~MainWindow()
{
    delete ui;
}
