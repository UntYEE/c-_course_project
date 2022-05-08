#include "mainwindow.h"
#include <QString>
#include <QDebug>
#include <QCoreApplication>
#include<QSound>
#include<QSoundEffect>
#include <qsize.h>

#define DIEPATH QCoreApplication::applicationDirPath().append("/res/hit.wav")
#define WINGPATH QCoreApplication::applicationDirPath().append("/res/wing.wav")
#define BGMPATH QCoreApplication::applicationDirPath().append("/res/bgm.wav")
#define POINTPATH QCoreApplication::applicationDirPath().append("/res/point.wav")

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent)
{
    init();
    initGame();
    birdDownSpeed = 0;
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
      drawBird();
}
void MainWindow::mousePressEvent(QMouseEvent * )
{
    // qDebug() << "click";
     if(gameStatus == GameStatus::RUNNING)QSound::play(WINGPATH);//鸟上升时扑打翅膀的声音
     birdStatus = BirdStatus::UP;
     initSpeed();
}
//初始化
void MainWindow::init()
{
    QSound::play(BGMPATH);
    //this->move(400, 300);
    this->setFixedSize(480*1.5, 640*1.5);
     qDebug() << "init ";
   ground = QPixmap(":/res/ground.png"); // 使用绝对路径
    if(!ground){
        qDebug() << "ground is null" << endl;
    }
    ground = ground.scaled(30,120); // 缩放
    int groundHeight = ground.height();
    background = QPixmap( QCoreApplication::applicationDirPath().append("/res/background.png"));
    background = background.scaled(this->width(), this->height() - groundHeight); //缩放

    groundSize = this->width()/ground.width() + 1; //防止除不尽有余数，所以加1
    v = new QVector<int>(groundSize);

    for(int i = 0; i < groundSize; i++){
         int x = i * ground.width();
         (*v)[i] = x;

    }
    QPixmap bird( ":/res/bird.png");
    bird1 = bird.copy(0, 0,380, 256);
    bird1 = bird1.scaled(75, 50);

    bird2 = bird.copy(399, 0, 290, 256);
    bird2= bird2.scaled(60, 50);

    bird3 = bird.copy(765, 0, 380, 256);
    bird3 = bird3.scaled(75, 50);

    scoreLabel = new QLabel(this);
    scoreLabel->setGeometry(width()/2, 0, 50, 50);

    QFont font ( "Microsoft YaHei", 20, 75); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
    scoreLabel->setFont(font);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    scoreLabel->setPalette(pa);
    this->score = 0;
    scoreLabel->setText(QString::number(this->score));

    //最高分数设置
    scoreLabel1 = new QLabel(this);
    scoreLabel1->setGeometry(0, this->height() - 50, 250, 50);

    scoreLabel1->setFont(font);
    scoreLabel1->setPalette(pa);
    scoreLabel1->setText("best score " + QString::number(best.score));

    startButton = new QPushButton(this);
    font = QFont( "Microsoft YaHei", 25, 75); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
    startButton->setFont(font);
    startButton->setPalette(pa);
    startButton->setIcon(QPixmap(":/res/start.png"));
    startButton->setFixedSize(200,65);
    startButton->setIconSize(QSize(200,256));
    startButton->setGeometry(width()/2 -50, height()/2 -30,200, 128);
   // startButton->hide();

    timer = new QTimer(this);
    //事件
    connect(timer,SIGNAL(timeout()),this,SLOT(loopPaint()));
    connect(startButton,SIGNAL(clicked()),this,SLOT(slotStartGame()));

}
//初始化游戏相关
void MainWindow::initGame()
{
    best.init();
    birdX = this->width() / 3;
    birdY = this->height() / 2 - bird1.height();
    this->score = 0;
    scoreLabel->setText(QString::number(this->score));
    scoreLabel1->setText("best score" + QString::number(best.score));
    uint seed_x = static_cast<uint>(clock());
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
//初始化速度
 void MainWindow::initSpeed()
 {
     birdDownSpeed = 2.0;
     birdUpSpeed = 5.0;
 }
//开始游戏
void MainWindow::startGame()
{
    QSound::play(POINTPATH);
    startButton->hide();
    initGame();
    initSpeed();
    gameStatus = GameStatus::RUNNING;
    birdStatus = BirdStatus::DOWN;
    timer->start(16);
}
//停止游戏
void MainWindow::stopGame()
{
    timer->stop();
    gameStatus = GameStatus::STOPING;
    startButton->show();
    if(score > best.score)
    {
        best.score = score;
        best.save();
    }
}
//绘制背景
void MainWindow::drawBack()
{
     QPainter painter(this);
     int height = ground.height();
     int pos;
     // 绘制背景图
     painter.drawPixmap(0,0, background);
     // 绘制地面
     for(int i = 0; i < groundSize; i++)
     {
         pos = (*v)[i];
         painter.drawPixmap(pos,this->height() - height, ground);//绘制一个地面图像
         // 改变坐标 移动起来
         pos -= moveSpeed;
         if(pos <= -ground.width()){
             pos = (groundSize-1) *  ground.width();
         }
         (*v)[i] = pos;
     }
}
//绘制bird
void MainWindow::drawBird()
{
     QPainter painter(this);
     QPixmap bird = bird1;
     if(gameStatus == GameStatus::RUNNING){
         //绘制哪一个 (动画效果)
         if(birdNo < birdNoMax/3){
             bird = bird1;
         }else if(birdNo < birdNoMax/3 * 2){
             bird = bird2;
         }else{
             bird = bird3;
         }
         birdNo++;
         if(birdNo > birdNoMax){
             birdNo = 1;
         }
     }
     // 小鸟下降
     if(birdStatus == BirdStatus::DOWN){
         birdY += birdDownSpeed;
         if(gameStatus == GameStatus::RUNNING)birdDownSpeed+=0.1;
         //matrix.rotate(1); //下降的同时旋转
         //bird = bird.transformed(matrix, Qt::SmoothTransformation);
     }
    // 判断是否碰撞
     if(isCrush()){
        if(gameStatus == GameStatus::RUNNING)QSound::play(DIEPATH);
        stopGame();
     }
    // 上升
     if(gameStatus == GameStatus::RUNNING){
         if(birdStatus == BirdStatus::UP){
             birdY -= birdUpSpeed;
             birdUpSpeed -= 0.2;
             //matrix.rotate(-1);
             //bird = bird.transformed(matrix, Qt::SmoothTransformation);
             if(birdUpSpeed <= 0.0){
                 birdStatus = BirdStatus::DOWN;
                 // qDebug() << "down";
                 initSpeed();
             }
         }
     }
     painter.drawPixmap(birdX, birdY, bird);
}
//绘制管道
void MainWindow::drawPipe()
{
    if(gameStatus == GameStatus::RUNNING){
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

            if(x + pipeChannel->getPipeWidth() < birdX && !pipeChannel->isScore)
            {
                QSound::play(POINTPATH);
                this->score++; //增加得分
                scoreLabel->setText(QString::number(this->score));
                pipeChannel->isScore = true;
            }
            x -= moveSpeed; // 改变坐标移动起来
            if(x <= -pipeChannel->getPipeWidth())// 移出窗口了
            {
                x = otherX + width() / 2 +  2 * l; //重新设置位置
                pipeChannel->setH1(50+l); //重新设置高度
                pipeChannel->initPipe();//重新初始化
            }
            pipeChannel->setX(x);
        }
    }
}
//碰撞检测
bool MainWindow::isCrush()
{
    if (birdY >= this->height() - ground.height() - bird1.height()){ //是否碰撞地面
        return true;
    }
    //if(birdY <= 0)return true;//是否碰撞天空
    //是否碰撞管道
    for(PipeChannel* c : *pipeChannels){
        if(birdX + bird1.width() > c->getX() && birdX < c->getX() + c->getPipeWidth()
                && (birdY < c->getPiPe2Height() || birdY+bird1.height() > c->getY1())){
            return true;
        }
    }
    return false;
}
//循环绘制
void MainWindow::loopPaint(){
    update();
}
//循环游戏
void MainWindow::slotStartGame(){
    startGame();
}

