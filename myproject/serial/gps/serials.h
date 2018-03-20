#ifndef SERIALS_H
#define SERIALS_H

#include <QtGlobal>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>

class Serials : public QObject
{
    Q_OBJECT
public:
    Serials();
    void SerialOpen(QString portName,QString baudrate,int openmode,int databaits,
                                  int parity,int stopbits,int flowcontrol);
    void SerialClose();
    void SerialWrite(QByteArray TxData);

    volatile bool isOpened;

signals:
    void SerialRecive(const QByteArray);

private slots:
    void SerialRead();

private:
    QSerialPort *serial;

};

#endif // SERIALS_H
