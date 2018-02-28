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
    //qDebug("111111111");
    //提取激光雷达
    ExtractLidarMsg();

}

void MainWindow::ReceiverLidarDataSlot()
{
    //qDebug("111111");
    while (qLidarSocket->hasPendingDatagrams())
    {
         QByteArray datagram;

         datagram.resize(qLidarSocket->pendingDatagramSize());
         qLidarSocket->readDatagram(datagram.data(), datagram.size());
         qLidarItems.append(datagram.toHex());
         emit ProcessLidarData();
    }
}


void MainWindow::ProcessLidarDataSlot()
{

    //处理雷达数据
    foreach(QByteArray ba, qLidarItems)
    {
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



                    if(LidarListStatus == 0)
                    {
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

                        qLidarList.replace(Li,lidarN_1);
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

                        qLidarList.replace(Li,lidarN);
                        Li++;
                        //数据归零
                        if(Li>=1800)
                        {
                            Li=0;
                            LidarListStatus = 1;
                            LidarListExtractStatus = 0;
                            int i = 0,j = 0;
                            for(j=0;j<16;j++)
                            {
                                qDebug("QList1 start");
                            }
                            for(i=0;i<1800;i++)
                            {

                                for(j=0;j<16;j++)
                                {
                                    qDebug("qL[%d]Azimuth=%lf,qL[%d].ChannelDis[j]=%lf",i,qLidarList[i].Azimuth,i,qLidarList[i].ChannelDis[j]);
                                }

                            }

                            for(j=0;j<16;j++)
                            {
                                qDebug("QList1 end");
                            }
                            continue;
                        }

                    }
                    else if (LidarListExtractStatus == 0)
                    {

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

                        qLidarList_extract.replace(Li,lidarN_1);
                        Li++;

                        //后半截数据li
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

                        qLidarList_extract.replace(Li,lidarN);
                        Li++;
                        //数据归零
                        if(Li>=1800)
                        {
                            Li=0;
                            LidarListExtractStatus = 1;
                            LidarListStatus = 0;
                            int i = 0,j = 0;
                            for(i=0;i<1800;i++)
                            {

                                for(j=0;j<16;j++)
                                {
                                    qDebug("qL[%d]Azimuth=%lf,qL[%d].ChannelDis[j]=%lf",i,qLidarList_extract[i].Azimuth,i,qLidarList_extract[i].ChannelDis[j]);
                                }

                            }

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
    }
}


void MainWindow::ExtractLidarMsg()
{
    int i_lidar=0,j=0;


        for(i_lidar=0;i_lidar<1800;i_lidar++)
        {
            for(j=0;j<16;j++)//通道距离
            {

            }
        }


}

void MainWindow::InitLidar()
{
    //初始化
    LidarListStatus = 0;
    LidarListExtractStatus = -1;
    qLidarSocket = new QUdpSocket(this);
    //采用ShareAddress模式(即允许其它的服务连接到相同的地址和端口，特别是
    //用在多客户端监听同一个服务器端口等时特别有效)，和ReuseAddressHint模式(重新连接服务器)
    qLidarSocket->bind(2368,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);

    Li=0;
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
