#include "serials.h"
#include<iostream>
#include <stdio.h>
using namespace std;

Serials::Serials()
{
    isOpened=false;
}

/**
 * @brief Serials::SerialOpen-开启串口
 * @param portName-串口号（COM0-COM255）
 * @param baudrate-波特率（1200-115200）
 * @param openmode-开启模式（0-2）
 * @param databaits-数据位（0-3）
 * @param parity-校验模式（0-2）
 * @param stopbits-停止位（0-1）
 * @param flowcontrol-控制流（0-2）
 */
void Serials::SerialOpen(QString portName,QString baudrate,int openmode,int databaits,
                              int parity,int stopbits,int flowcontrol)
{

    //判断是否已经开启
    if(isOpened==true)
    {
        //关闭串口
        serial->close();
        serial->deleteLater();
        isOpened=false;
    }

    //创建串口
    serial = new QSerialPort();
    #if 1
    //设置串口号
    serial->setPortName(portName);
    //设置波特率
    printf("baud = %d\n",baudrate.toInt());
    serial->setBaudRate(baudrate.toInt());
    //OpenMode模式
    switch(openmode){
        case 0: serial->open(QIODevice::ReadWrite);break;
        case 1: serial->open(QIODevice::ReadOnly);break;
        case 2: serial->open(QIODevice::WriteOnly);break;
        default: break;
    }

    //DataBits模式
    switch(databaits){
        case 0: serial->setDataBits(QSerialPort::Data8);break;
        case 1: serial->setDataBits(QSerialPort::Data7);break;
        case 2: serial->setDataBits(QSerialPort::Data6);break;
        case 3: serial->setDataBits(QSerialPort::Data5);break;
        default: break;
    }

    //Parity模式
    switch(parity){
        case 0: serial->setParity(QSerialPort::NoParity);break;
        case 1: serial->setParity(QSerialPort::OddParity);break;
        case 2: serial->setParity(QSerialPort::EvenParity);break;
        default: break;
    }
    //StopBits模式
    switch(stopbits){
        case 0: serial->setStopBits(QSerialPort::OneStop);break;
        case 1: serial->setStopBits(QSerialPort::TwoStop);break;
        default: break;
    }
    //FlowControl模式
    switch(flowcontrol){
        case 0: serial->setFlowControl(QSerialPort::NoFlowControl);break;
        case 1: serial->setFlowControl(QSerialPort::HardwareControl);break;
        case 2: serial->setFlowControl(QSerialPort::SoftwareControl);break;
        default: break;
    }

    connect(serial,SIGNAL(readyRead()),this,SLOT(SerialRead()));

    if(serial->isOpen())
       isOpened=true;
    #endif

}

/**
 * @brief Serials::SerialClose-关闭串口
 */
void Serials::SerialClose()
{
    //判断是否已经开启
    if(isOpened==true)
    {
        //关闭串口
        serial->close();
        serial->deleteLater();
        isOpened=false;
    }
}

/**
 * @brief Serials::SerialWrite-往串口写数据
 * @param TxData-发送的数据
 */
void Serials::SerialWrite(QByteArray TxData)
{
    if(isOpened==true)
    {
        printf("write %s\n",TxData.data());
        //往串口写数据
        serial->write(TxData);
    }
}


void Serials::SerialRead()
{
    if(isOpened==true)
    {
        //serial->waitForReadyRead(1);
        QByteArray rd=serial->readAll();
        if(!rd.isEmpty())
        {
            //printf("receive rd = %s\n",rd.data());
            emit(SerialRecive(rd));
        }
    }
}




