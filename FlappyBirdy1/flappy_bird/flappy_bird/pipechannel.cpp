#include "pipechannel.h"
#include <QCoreApplication>
//静态成员初始化

//winHeight 窗口高度 h1 底下管道高度 x 位置横坐标 groundHeight 地面高度 parent
PipeChannel::PipeChannel(int winHeight,int h1, int x, int groundHeight, QObject *parent) : QObject(parent)
{
    this->h1 = h1;
    this->x = x;
    this->groundHeight = groundHeight;
    this->winHeight = winHeight;
    initPipe();
}

PipeChannel::~PipeChannel()
{

}

