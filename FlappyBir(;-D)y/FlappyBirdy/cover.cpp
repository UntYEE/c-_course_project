#include "cover.h"
#include<QPainter>
#include<QPushButton>
#include<QPixmap>
#include<QMouseEvent>

cover::cover(QWidget *parent) : QWidget(parent)
{
     this->setFixedSize(720,960);
     this->setEnabled(1);
}

void cover::paintEvent(QPaintEvent *)
{
    painter=new QPainter(this);
    coverin= QPixmap(":/res/birdy.png");
    painter->begin(this);
    painter->drawPixmap(0,0,this->width(),this->height(),coverin);
    painter->end();
}

void cover::mousePressEvent(QMouseEvent*event)
{

    if(event->button() == Qt::LeftButton)
    {
        this->hide();
    }
}
