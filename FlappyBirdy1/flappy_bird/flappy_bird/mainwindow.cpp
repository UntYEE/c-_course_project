#include "mainwindow.h"
#include <QString>
#include <QDebug>
#include <QCoreApplication>
#define DIEPATH QCoreApplication::applicationDirPath().append("/res/hit.wav")
#define WINGPATH QCoreApplication::applicationDirPath().append("/res/wing.wav")
#define BGMPATH QCoreApplication::applicationDirPath().append("/res/bgm.wav")
#define POINTPATH QCoreApplication::applicationDirPath().append("/res/point.wav")

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent)
{
    init();
    initGame();
}

MainWindow::~MainWindow()
{
    delete  pipeChannels;
}
void MainWindow::paintEvent(QPaintEvent *)
{

      drawPipe();

}

//绘制管道
void MainWindow::drawPipe()
{
    QPainter painter(this);
    uint seed_x = static_cast<uint>(clock());
    int l = GlobalUtils::getRandomNum(seed_x,200); //获取随机数
    int otherX = 0; //另一个通道的位置， 只有两个管道
    for(int i = 0; i < pipeChannels->size(); i++)
    {
        PipeChannel * pipeChannel = (*pipeChannels)[i];
        if(i ==0)
        {
            otherX = (*pipeChannels)[1]->getX();
        }else {
            otherX = (*pipeChannels)[0]->getX();
        }
        pipeChannel->draw(painter);
        int x = pipeChannel->getX();
        x -= moveSpeed; // 改变坐标移动起来

    }
}


