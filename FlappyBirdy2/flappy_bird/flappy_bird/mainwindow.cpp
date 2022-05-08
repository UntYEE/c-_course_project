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
        //qDebug() << "paintEvent";
      drawBack();
      drawPipe();

}

//初始化
void MainWindow::init()
{
    QSound::play(BGMPATH);
    //this->move(400, 300);
    this->setFixedSize(480, 640);
     qDebug() << "init ";
   ground = QPixmap( QCoreApplication::applicationDirPath().append("/res/ground.png")); // 使用绝对路径
    if(!ground){
        qDebug() << "ground is null" << endl;
    }
    ground = ground.scaled(20,80); // 缩放
    QPixmap bird( QCoreApplication::applicationDirPath().append("/res/bird.png"));

    startButton = new QPushButton(this);
    font = QFont( "Microsoft YaHei", 25, 75); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
    startButton->setFont(font);
    startButton->setPalette(pa);
    startButton->setText(QString("start"));
    startButton->setGeometry(width()/2 -50, height()/2 -30, 100, 60);
   // startButton->hide();

    timer = new QTimer(this);
    //事件
    connect(timer,SIGNAL(timeout()),this,SLOT(loopPaint()));
    connect(startButton,SIGNAL(clicked()),this,SLOT(slotStartGame()));
}
//初始化游戏相关
void MainWindow::initGame()
{

    int l = GlobalUtils::getRandomNum(seed_x,200); //获取随机数
    if(pipeChannels == nullptr){
        pipeChannels = new QVector<PipeChannel*>(2); //创建两个通道
    }else {
        delete (*pipeChannels)[0];
        delete (*pipeChannels)[1];
    }
    (*pipeChannels)[0] = new PipeChannel(height(), 100+l, width()+100, ground.height(), this);//窗口高度 底下管道高度 x位置横坐标 地面高度 objet parent
    (*pipeChannels)[1] = new PipeChannel(height(), 150 +l, 2 * width() + l, ground.height(), this);
}

//开始游戏
void MainWindow::startGame()
{
    QSound::play(POINTPATH);
    startButton->hide();
    initGame();
    initSpeed();
    gameStatus = GameStatus::RUNNING;
    timer->start(16);
}
//停止游戏
void MainWindow::stopGame()
{
    timer->stop();
    gameStatus = GameStatus::STOPING;

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


//循环绘制
void MainWindow::loopPaint(){
    update();
}
//循环游戏
void MainWindow::slotStartGame(){
    startGame();
}
