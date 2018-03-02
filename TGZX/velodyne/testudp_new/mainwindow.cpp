#include "mainwindow.h"
#include "ui_mainwindow.h"

volatile int MainWindow::savefile = 0;
volatile double MainWindow::Cosdu[16]={0.965925,
                                     0.999847,
                                     0.974370,
                                     0.998629,
                                     0.981627,
                                     0.996194,
                                     0.987688,
                                     0.992546,
                                     0.992546,
                                     0.987688,
                                     0.996194,
                                     0.981627,
                                     0.998629,
                                     0.974370,
                                     0.999847,
                                     0.965925};


volatile double MainWindow::Sindu[16]={-0.258819,
                           0.017452,
                           -0.224951,
                           0.052335,
                           -0.190808,
                           0.087155,
                           -0.156434,
                           0.121869,
                           -0.121869,
                           0.156434,
                           -0.087155,
                           0.190808,
                           -0.052335,
                           0.224951,
                           -0.017452,
                           0.258819};

pcl::PointCloud<pcl::PointXYZ>::Ptr cloud1 (new pcl::PointCloud<pcl::PointXYZ>);
pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>);
pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered2 (new pcl::PointCloud<pcl::PointXYZ>);


/**
 * @brief MainWindow::NextcTimerSlot--计数定时器，10HZ频率
 */
void MainWindow::NextcTimerSlot()
{
    //qDebug("NextcTimerSlot 111111111");
    //提取激光雷达
    //ExtractLidarMsg();

}

void MainWindow::ReceiverLidarDataSlot()
{
    while (qLidarSocket->hasPendingDatagrams())
    {
         QByteArray datagram;

         datagram.resize(qLidarSocket->pendingDatagramSize());
         qLidarSocket->readDatagram(datagram.data(), datagram.size());
         //qLidarItems.append(datagram.toHex());
         qLidarItems.append(datagram);
         emit ProcessLidarData();
    }
}

void MainWindow::DealLidarArray(QByteArray LidarData)
{

        qLidarByte.append(LidarData);
        QByteArray find("\xff\xee",2);

        int idx = 0, from = find.length();

        while((idx = qLidarByte.indexOf(find, from)) != -1)
        {
            cLidarItems.append(qLidarByte.mid(0, idx+2));

            if(qLidarByte.length()-idx-2>=0)
                qLidarByte=qLidarByte.right(qLidarByte.length()-idx-2);
        }

}

void MainWindow::ProcessLidarDataSlot()
{

    int azimuths = 0;
    int j=0,k=0,qL=0,qH=0,qHL=0;
    int LidarArrayLength = 0;
    short int ChannelDis_recv[16];
    int LidarArrayIndex=0;

    LidarInfo lidarN_1;//前半截数据
    LidarInfo lidarN;//后半截数据


    //处理雷达数据
    foreach(QByteArray ba, qLidarItems)
    {
//        if(MainWindow::savefile == 5)
//        {
//             qDebug("ba.length=%d",ba.length());
//             for(int i =0;i<4;i++)
//             {
//                //qDebug("0x%x",ba.at(i));
//                qDebug("0x%x",(uint8_t)ba[i]);
//             }

//        }

        //MainWindow::savefile++;


        DealLidarArray(ba);



        foreach(QByteArray LidarArray, cLidarItems)
        {

            //qDebug()<<LidarArray.toStdString().data();
//            if(MainWindow::savefile == 5)
//            {
//                qDebug("savefile ==5");
//                qDebug()<<LidarArray.toHex();
//                //MainWindow::savefile++;
//            }

            if(!LidarArray.isEmpty())
            {

                    LidarArrayLength = LidarArray.length();

//                    if(100 != LidarArrayLength)
//                    {
//                        qDebug("LidarArrayLength !=1== is %d",LidarArrayLength);
//                        for(int i = 0;i<LidarArrayLength;i++)
//                        {
//                            qDebug("%x ",(uint8_t)LidarArray.at(i));

//                        }

//                    }

                    //if(100 == LidarArrayLength)
                    //{

                        LidarArrayIndex = 0;
                         //前半截数据
                        qL = (uint8_t)LidarArray.at(LidarArrayIndex);
                        qH = (uint8_t)LidarArray.at(LidarArrayIndex+1);
                        //qDebug("qL=0x%x,qH=0x%x",qL,qH);
                        qHL = (qH<<8)|(qL);
                        azimuths = qHL;
                        lidarN_1.Azimuth=azimuths;
                        qDebug("azimuths = %d",azimuths);
                        //lidarN_1.Azimuth=(float)azimuths/5729.5828;//把方位角默认单位转换为弧度

                        LidarArrayIndex = 2;
                        memset(ChannelDis_recv,0,sizeof(short int)*16);
                        for(int lj=0;lj<16;lj++)
                        {
                            qL=(uint8_t)LidarArray.at(LidarArrayIndex);
                            qH=(uint8_t)LidarArray.at(LidarArrayIndex+1);
                            qHL = (qH<<8)|(qL);
                            ChannelDis_recv[lj]=qHL;
                            LidarArrayIndex+=3;
                        }

                        //把雷达检测到的通道距离值，16根线从下到上依次存储
                        for(j=0,k=0;j<16;++k,++j)
                        {
                            lidarN_1.ChannelDis[k]=(float)ChannelDis_recv[j];//通道距离默认单位为毫米，转换成米
                            if(k+8 <16)
                                lidarN_1.ChannelDis[k+8]=(float)ChannelDis_recv[++j];

//                            lidarN_1.ChannelDis[k]=(float)ChannelDis_recv[j]/1000;//通道距离默认单位为毫米，转换成米
//                            if(k+8 <16)
//                                lidarN_1.ChannelDis[k+8]=(float)ChannelDis_recv[++j]/1000;

                        }

                        //雷达状态为1时，表示qLidarList数据读取完成，使用qLidarList_extract读取数据
                        //雷达状态为0时，表示qLidarList_extract数据读取完成，使用qLidarList读取数据
                        //if(LidarListStatus == 0)
                        {
                            qLidarList.replace(cAlculationTimes,lidarN_1);
                        }
//                        else if(LidarListStatus == 1)
//                        {
//                            qLidarList_extract.replace(cAlculationTimes,lidarN_1);
//                        }
                        cAlculationTimes++;

                        //后半截数据
                        //qDebug("后半截数据方位角LidarArrayIndex=%d",LidarArrayIndex);
                        lidarN.Azimuth=(azimuths+20);
                        //lidarN.Azimuth=(float)(azimuths+20)/5729.5828;//把方位角默认单位转换为弧度
                        //qDebug("后半截数据通道距离LidarArrayIndex=%d",LidarArrayIndex);
                        memset(ChannelDis_recv,0,sizeof(short int)*16);
                        for(int lk=0;lk<16;lk++)
                        {
                            //Channel data
                            qL=(uint8_t)LidarArray.at(LidarArrayIndex);
                            qH=(uint8_t)LidarArray.at(LidarArrayIndex+1);
                            qHL = (qH<<8)|(qL);
                            ChannelDis_recv[lk]=qHL;
                            LidarArrayIndex+=3;
                        }

                        //把雷达检测到的通道距离值，16根线从下到上依次存储
                        for(j=0,k=0;j<16;++k,++j)
                        {
                            lidarN_1.ChannelDis[k]=(float)ChannelDis_recv[j];//通道距离默认单位为毫米，转换成米
                            if(k+8 <16)
                                lidarN_1.ChannelDis[k+8]=(float)ChannelDis_recv[++j];

//                            lidarN_1.ChannelDis[k]=(float)ChannelDis_recv[j]/1000;//通道距离默认单位为毫米，转换成米
//                            if(k+8 <16)
//                                lidarN_1.ChannelDis[k+8]=(float)ChannelDis_recv[++j]/1000;

                        }

                        //if(LidarListStatus == 0)
                        {
                            qLidarList.replace(cAlculationTimes,lidarN);
                        }
//                        else if(LidarListStatus == 1)
//                        {
//                            qLidarList_extract.replace(cAlculationTimes,lidarN);
//                        }
                        cAlculationTimes++;

                        //qDebug("cAlculationTimes=%d",cAlculationTimes);
                        //数据归零
                        if(cAlculationTimes>=1800)
                        {
                            cAlculationTimes=0;

                            //修改雷达标志位
                            //if(LidarListStatus == 0)
                            //{

                                //LidarListStatus = 1;
                                MainWindow::savefile++;
                                if(MainWindow::savefile == 5)
                                    ExtractLidarMsg();
                                //qDebug("LidarListStatus=%d",LidarListStatus);
                               // continue;
                            //}
//                            else if(LidarListStatus == 1)
//                            {
//                                LidarListStatus = 0;
//                                continue;
//                            }


                        }

                    //}



                }


            cLidarItems.removeFirst();
        }

        qLidarItems.removeFirst();



#if 0
        QString rData(ba);


        if(!rData.isEmpty())
        {


            QStringList qStrList=rData.split("ffee");
            if((qStrList.size()-1)>0)
            {

                for(int i=1;i<(qStrList.size());i++)
                {
                    //雷达是否掉线状态切换
                    //PubApi::qLidarOffLine=true;
                    //qLidarCount=0;

                    bool ok;
                    int li=0;
                    int azimuths = 0;
                    int j = 0,k = 0;
                    QString qStr=qStrList.at(i);
                    QString qL,qH,qHL;
                    LidarInfo lidarN_1;//前半截数据
                    LidarInfo lidarN;//后半截数据
                    short int ChannelDis_recv[16];

                    li=4;
                    //前半截数据
                    qL=qStr.mid(0,2);
                    qH=qStr.mid(2,2);
                    qHL=qH+qL;
                    azimuths=qHL.toInt(&ok,16);//fangweijiao
                    lidarN_1.Azimuth=(float)azimuths/5729.5828;//把方位角默认单位转换为弧度


                    memset(ChannelDis_recv,0,sizeof(short int)*16);
                    for(int lj=0;lj<16;lj++)
                    {
                        //Channel data
                        qL=qStr.mid(li,2);
                        qH=qStr.mid(li+2,2);
                        qHL=qH+qL;
                        ChannelDis_recv[lj]=qHL.toInt(&ok,16);
                        //lidarN_1.ChannelDis[lj]=qHL.toInt(&ok,16);//tongdaojuli
                        li+=6;
                    }

                    //把雷达检测到的通道距离值，16根线从下到上依次存储
                    for(j=0,k=0;j<16;++k,++j)
                    {

                        lidarN_1.ChannelDis[k]=(float)ChannelDis_recv[j]/1000;//通道距离默认单位为毫米，转换成米
                        if(k+8 <16)
                            lidarN_1.ChannelDis[k+8]=(float)ChannelDis_recv[++j]/1000;

                    }

                    //雷达状态为1时，表示qLidarList数据读取完成，使用qLidarList_extract读取数据
                    //雷达状态为0时，表示qLidarList_extract数据读取完成，使用qLidarList读取数据
                    if(LidarListStatus == 0)
                    {
                        qLidarList.replace(Li,lidarN_1);
                    }
                    else if(LidarListStatus == 1)
                    {
                        qLidarList_extract.replace(Li,lidarN_1);
                    }
                    Li++;

                    //后半截数据

                    lidarN.Azimuth=(float)(azimuths+20)/5729.5828;//把方位角默认单位转换为弧度
                    memset(ChannelDis_recv,0,sizeof(short int)*16);
                    for(int lk=0;lk<16;lk++)
                    {
                        //Channel data
                        qL=qStr.mid(li,2);
                        qH=qStr.mid(li+2,2);
                        qHL=qH+qL;
                        ChannelDis_recv[lk]=qHL.toInt(&ok,16);
                        //lidarN.ChannelDis[lk]=qHL.toInt(&ok,16);//通道距离
                        li+=6;
                    }

                    //把雷达检测到的通道距离值，16根线从下到上依次存储
                    for(j=0,k=0;j<16;++k,++j)
                    {

                        lidarN_1.ChannelDis[k]=(float)ChannelDis_recv[j]/1000;//通道距离默认单位为毫米，转换成米
                        if(k+8 <16)
                            lidarN_1.ChannelDis[k+8]=(float)ChannelDis_recv[++j]/1000;

                    }

                    if(LidarListStatus == 0)
                    {
                        qLidarList.replace(Li,lidarN);
                    }
                    else if(LidarListStatus == 1)
                    {
                        qLidarList_extract.replace(Li,lidarN);
                    }
                    Li++;
                    //数据归零
                    if(Li>=1800)
                    {
                        Li=0;

                        //修改雷达标志位
                        if(LidarListStatus == 0)
                        {
                            #if 0
                            int i = 0,j = 0;
                            for(j=0;j<16;j++)
                            {
                                qDebug("LidarListStatus=%d",LidarListStatus);
                            }
                            for(i=0;i<1800;i++)
                            {

                                for(j=0;j<16;j++)
                                {
                                    qDebug("qL[%d]Azimuth=%lf,qL[%d].ChannelDis[%d]=%lf",i,qLidarList[i].Azimuth,i,j,qLidarList[i].ChannelDis[j]);
                                }

                            }
                            #endif

                            LidarListStatus = 1;
                            continue;
                        }
                        else if(LidarListStatus == 1)
                        {
#if 0
                            int i = 0,j = 0;
                            for(j=0;j<16;j++)
                            {
                                qDebug("LidarListStatus=%d",LidarListStatus);
                            }
                            for(i=0;i<1800;i++)
                            {

                                for(j=0;j<16;j++)
                                {
                                    qDebug("qLExtract[%d]Azimuth=%lf,qLExtract[%d].ChannelDis[j]=%lf",i,qLidarList_extract[i].Azimuth,i,qLidarList_extract[i].ChannelDis[j]);
                                }

                            }
#endif
                            LidarListStatus = 0;
                            continue;
                        }



                    }



                }
            }
        }

        //调试使用
        //QString rData(ba.toHex());
        //qDebug()<<rData;
        //移除数据
        qLidarItems.removeFirst();
#endif


    }
}


void MainWindow::ExtractLidarMsg()
{
    int i_lidar=0,j=0;

    //qDebug("111111111");

    for(i_lidar=0;i_lidar<1800;i_lidar++)
    {
        //if(LidarListStatus==1)
        {//read qLidarList

            for(j=0;j<16;j++)//通道距离
            {
            //    qDebug("Azimuth[%d]=%d,channeldis[%d]=%d",i_lidar,qLidarList.at(i_lidar).Azimuth,j,qLidarList.at(i_lidar).ChannelDis[j]);

            }
        }
    }


}

void MainWindow::InitLidar()
{
    //初始化
    cAlculationTimes=0;
    LidarListStatus = 0;
    LidarListExtractStatus = -1;
    //清除数据
    qLidarByte.clear();
    cLidarItems.clear();

    qLidarSocket = new QUdpSocket(this);
    //采用ShareAddress模式(即允许其它的服务连接到相同的地址和端口，特别是
    //用在多客户端监听同一个服务器端口等时特别有效)，和ReuseAddressHint模式(重新连接服务器)
    qLidarSocket->bind(2368,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);


    //初始化
    for(int i=0;i<1800;i++)
    {
        LidarInfo lidarN;
        lidarN.Azimuth=0;
        for(int j=0;j<16;j++)
        {
            lidarN.ChannelDis[j]=0;

        }
        qLidarList.append(lidarN);
        qLidarList_extract.append(lidarN);
    }

}


void MainWindow::InitTimer()
{
    //计数Timers
    cTimers = new QTimer(this);
    cTimers->setInterval(100);
    cTimers->start();
}

void MainWindow::InitSlot()
{
    //cTimer的槽
    QObject::connect(cTimers,SIGNAL(timeout()),this,SLOT(NextcTimerSlot()));
    //激光雷达接收数据的槽
    QObject::connect(qLidarSocket, SIGNAL(readyRead()),this, SLOT(ReceiverLidarDataSlot()));
    //激光雷达处理数据的槽
    QObject::connect(this, SIGNAL(ProcessLidarData()), this, SLOT(ProcessLidarDataSlot()));

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    InitLidar();
    InitTimer();
    InitSlot();

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    //关闭计数器
    if(cTimers->isActive())
        cTimers->stop();
    //关闭激光Socket
    if(qLidarSocket->isOpen())
        qLidarSocket->close();
    delete ui;
}
