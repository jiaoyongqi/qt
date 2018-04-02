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
    painter->setPen(pen); //添加画笔
    painter->setBrush(brush);

    pen.setColor(QColor(0, 255, 0));

    for(i=0;i<=sum_width;i=i+w)
    {
        painter->drawLine(i,0,i,width);//x1,y1,x2,y2
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
            //qDebug("j=%d,k=%d",j,k);

            x = str.mid(0,j).toInt();
            y = str.mid(j+1,k).toInt();
            qDebug("x=%d,y=%d",x,y);

            //huizhi wangge

            painter->drawRect(x, y, w, h);//x,y,length,length


        }


    }
    else
    {
        qDebug("Can't open the file!");
    }


    file.close();



    QString strx;
    QString stry;

    int x_test=0,y_test=100;
    strx.setNum(x_test);
    stry.setNum(y_test);
    QString content = strx + "," + stry +"\n";


    //write file
    QFile writefile("/home/feng/Documents/test/Grid/writetext.txt");
    if(!writefile.open(QIODevice::WriteOnly|QIODevice::Append)){
        qDebug("writefile打开失败");
    }else{
        qDebug("writefile打开成功");
        //获得文件大小
        qint64 pos;
        pos = writefile.size();
        //重新定位文件输入位置，这里是定位到文件尾端
        writefile.seek(pos);
        QString content1 = "0,50\n";
        //写入文件
        qint64 length = -1;
        length = writefile.write(content.toLatin1(),content.length());
        if(length == -1){
            qDebug("写入文件失败");
        }else{
            qDebug("写入文件成功");
        }



    }
    writefile.close();




}



Widget::~Widget()
{
    delete ui;
}
