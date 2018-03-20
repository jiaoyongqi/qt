#include "widget.h"
#include "ui_widget.h"
#include <qpainter.h>

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

    //painter.drawLine(0,0,150,500);



}


void Widget::drawGrid(QPainter *painter)

{



    //painter->drawLine(0,0,500,500);

    int Margin=40;//边缘

    QRect rect;

    //取得绘图区域，大小要减去旁白

    //rect=QRect(Margin+300,Margin+300,width()-Margin-700,height()-Margin-500);

    rect=QRect(Margin+25,Margin,width()-2*Margin-10,height()-2*Margin);

    //painter->drawLine(QPoint(0, 0), QPoint(100, 100));


    for(int i=0;i<=10;i++)

    {

        int x=rect.left()+(i*(rect.width()-1)/10);
        painter->drawLine(x,rect.top(),x,rect.bottom());

    }


    for(int j=0;j<=10;j++)

    {

        int y=rect.bottom()-(j*(rect.height()-1)/10);
        painter->drawLine(rect.left()-5,y,rect.right(),y);
    }

}



Widget::~Widget()
{
    delete ui;
}
