#include "widget.h"
#include "ui_widget.h"
#include <qpainter.h>
#include <QBrush>
#include <QFile>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}


void Widget::paintEvent(QPaintEvent *event)

{

    QPainter painter(this);

    drawGrid(&painter);

    painter.setPen(Qt::black);

}


void Widget::drawGrid(QPainter *painter)

{

    int width = 500;
    int sum_width = 500;
    int sum_height = 500;
    int w = 50;
    int h = 50;
    int i=0,j=0,k=0;
    int x=0,y=0;

    QPen pen; //画笔
    QBrush brush(QColor(0, 255, 0, 255)); //画刷

    pen.setColor(QColor(0, 255, 0));

    for(i=0;i<=sum_width;i=i+w)
    {
        painter->drawLine(i,0,i,width);
    }


    for(j=0;j<=sum_height;j=j+h)
    {
        painter->drawLine(0,j,sum_height,j);

    }


    QFile file("/home/feng/Documents/test/Grid/text.txt");
    if(file.open(QIODevice::ReadOnly))
    {

        while(!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str(line);//"0,50\r\n"
            //qDebug("%s",str);
            j=0,k=0;
            for(i=0;i<str.length();i++)
            {
                if(str.at(i)==',')
                    j=i;
                if(str.at(i)=='\r')
                    k=i;
            }
            qDebug("j=%d,k=%d",j,k);

            x = str.mid(0,j).toInt();
            y = str.mid(j+1,k).toInt();
            qDebug("x=%d,y=%d",x,y);

        }


    }
    else
    {
        qDebug("Can't open the file!");
    }





    painter->setPen(pen); //添加画笔
    painter->setBrush(brush);
    painter->drawRect(50, 0, 50, 50);//x,y,length,length

}



Widget::~Widget()
{
    delete ui;
}
