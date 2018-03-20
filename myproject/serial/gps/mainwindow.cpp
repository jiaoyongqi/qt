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

/**
 * @brief MainWindow::SerialCom1ProcessSlot--串口1处理数据的槽
 */
void MainWindow::SerialCom1ProcessSlot()
{
    foreach(QByteArray ba, cCom1Items)
    {
        if(!ba.isEmpty())
        {
            int aa=ba.length();
            // aa = 83
            //printf("aa = %d\n",aa);
            if(aa==83)
            {
#if 1
                QString rData(ba);
                printf("need del data ba = %s\n",rData.toStdString().data());

                int cc=0;
                for (int i=4;i<aa-4;i++)
                    cc+=(int)(ba.at(i)&0x000000ff);
                if((cc&0x000000ff)==(int)(ba.at(aa-4)&0x000000ff))   //校验正确，并且数据帧长度正确
                {
                    //串口状态
                    int cCom1Count=0;

                    //执行状态
                    //PubApi::uRun=(int)(ba.at(4)&0x03);
                    int uRun = (int)(ba.at(4)&0x03);
                    printf("uRun=%d \n",uRun);

                    //毫米波雷达注册
                    if((int)(ba.at(4)&0x04)==0x04)
                    {
                        printf("mmw register is true\n");
                        //PubApi::cRadarRegister=true;
                        static volatile bool MmwRadarRegister=true;
                    }
                    else
                    {
                        printf("mmw register is false\n");
                        //PubApi::cRadarRegister=false;
                         static volatile bool MmwRadarRegister=false;
                    }

                    //毫米波雷达掉线
                    if((int)(ba.at(4)&0x08)==0x08)
                    {
                         printf("mmw is status true\n");
                        //PubApi::cRadarOffLine=true;
                        static volatile bool MmwRadarOffLine=true;
                    }
                    else
                    {
                        printf("mmw is status false\n");
                        //PubApi::cRadarOffLine=false;
                        static volatile bool MmwRadarOffLine=false;
                    }

                    //电池电压状态
                    if((int)(ba.at(4)&0x10)==0x10)
                    {
                        printf("volatile is status true\n");
                        //PubApi::cGPSOffLine=true;
                        static volatile bool Bat_volt_low=true;
                    }
                    else
                    {
                         printf("volatile is status false\n");
                        //PubApi::cGPSOffLine=false;
                        static volatile bool Bat_volt_low=false;
                    }
                    //超声波雷达掉线状态
                    if((int)(ba.at(4)&0x20)==0x20)
                    {
                        printf("ult is status true\n");
                        //PubApi::Bat_volt_low=true;
                        static volatile bool UltrasonicradarOffLine = true;
                    }
                    else
                    {
                        printf("ult is status false\n");
                        //PubApi::Bat_volt_low=false;
                        static volatile bool UltrasonicradarOffLine = false;
                    }

                    //超声波雷达 Ultrasonic radar
//                    for(int i=0;i<4;i++)
//                    {
//                        ult_radar_data[j].RADAR_R_Ultrasonic=

//                    }


                    //毫米波雷达数据
                    for(int j=0;j<8;j++)
                    {
#if 0
                        //毫米波雷达数据 ok
                        PubApi::radar_data[j].RADAR_R_Distance=(double)(((ba.at(9+j*6+1)&0x000000ff)<<8)|(ba.at(9+j*6)&0x000000FF))/100;
                        PubApi::radar_data[j].RADAR_R_Lateral=(double)(((ba.at(9+j*6+3))<<8)|(ba.at(9+j*6+2)&0xFF))/100;
                        PubApi::radar_data[j].RADAR_R_Speed=(double)(((ba.at(9+j*6+5))<<8)|(ba.at(9+j*6+4)&0xFF))/100;
#endif
                        mmw_radar_data[j].RADAR_R_Distance=(double)(((ba.at(9+j*6+1)&0x000000ff)<<8)|(ba.at(9+j*6)&0x000000FF))/100;
                        mmw_radar_data[j].RADAR_R_Lateral=(double)(((ba.at(9+j*6+3))<<8)|(ba.at(9+j*6+2)&0xFF))/100;
                        mmw_radar_data[j].RADAR_R_Speed=(double)(((ba.at(9+j*6+5))<<8)|(ba.at(9+j*6+4)&0xFF))/100;

                        printf("mmw_radar_data[%d].RADAR_R_Distanc = %lf,mmw_radar_data[%d].RADAR_R_Lateral=%lf,mmw_radar_data[%d].RADAR_R_Speed=%lf\n",j,mmw_radar_data[j].RADAR_R_Distance,j,mmw_radar_data[j].RADAR_R_Lateral,j,mmw_radar_data[j].RADAR_R_Speed);

                    }

//                    PubApi::THR_MONI=(double)(ba.at(68)&0x000000FF)/100;
//                    PubApi::BRK_MONI=(double)(ba.at(69)&0x000000FF)/100;
                      static volatile double THR_MONI=(double)(ba.at(57)&0x000000FF)/100;
                      static volatile double BRK_MONI=(double)(ba.at(58)&0x000000FF)/100;
                      printf("THR = %lf,BRK = %lf\n",THR_MONI,BRK_MONI);

                    //刷新Com1数据显示
                    //Com1DataDisplay();

                }
                 #endif
            }
        }
        //移除数据
        cCom1Items.removeFirst();
    }
}


//deal
void MainWindow::SerialComRevSlot(QByteArray RxData)
{
    qCom1Byte.append(RxData);
    if(!qCom1Byte.isEmpty())
    {
        QByteArray find("\xFD\xFE\xFF",3);
        int idx = 0,from = find.length();
        while((idx = qCom1Byte.indexOf(find,from)) != -1)
        {
            //printf("idx = %d\n",idx);
            cCom1Items.append(qCom1Byte.mid(0,idx+3));
            emit SerialCom1ProcessSignal();
            if(qCom1Byte.length()-idx-3>=0)
                 qCom1Byte=qCom1Byte.right(qCom1Byte.length()-idx-3);

        }
    }
}


#if 0
//deal gps data
void MainWindow::SerialComRevSlot1(QByteArray RxData)
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
//          printf("before del ind =%d str = %s\n",ind,str.data());
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

#endif

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
    this->sCom.SerialOpen("/dev/ttyUSB0","115200",0,0,0,0,0);
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


#if 0
    // deal gps
    //COM1接收数据的槽
    QObject::connect(&this->sCom, SIGNAL(SerialRecive(QByteArray)), this, SLOT(SerialComRevSlot(QByteArray)));
    //COM1处理数据的槽
    QObject::connect(this, SIGNAL(SerialComProcessSignal()), this, SLOT(SerialComProcessSlot()));
#endif
    //COM1接收数据的槽
    QObject::connect(&this->sCom, SIGNAL(SerialRecive(QByteArray)), this, SLOT(SerialComRevSlot(QByteArray)));
    //COM1处理数据的槽
    QObject::connect(this, SIGNAL(SerialCom1ProcessSignal()), this, SLOT(SerialCom1ProcessSlot()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
