#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QVector>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include "pipechannel.h"
#include "globalutils.h"
#include <bestscore.h>
#include <qsound.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);
    //可通过按下键盘实现小鸟上升
    //void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent * event);
    //初始化
    void init();
    //初始化游戏相关
    void initGame();
    //初始化速度
    void initSpeed();
    //开始游戏
    void startGame();
    //停止游戏
    void stopGame();
    //绘制背景
    void drawBack();
    //绘制bird
    void drawBird();
    //绘制管道
    void drawPipe();
    //碰撞判定
    bool isCrush();
public slots:
     //循环绘制（定时器）
    void loopPaint();
    //游戏的循环
    void slotStartGame();
private:
    QPixmap background; //背景图
    QPixmap  ground; //地面
    QPixmap bird1; //小鸟的三种状态
    QPixmap bird2;
    QPixmap bird3;

    int birdNo = 1;
    int birdNoMax = 15;
    int birdY = 0; // 小鸟的位置Y坐标
    int birdX = 0; // 小鸟的位置X坐标
    QMatrix matrix;
    enum GameStatus{STOPING = 1, RUNNING}; //游戏状态枚举
    enum BirdStatus{DEFAULT, UP, DOWN}; //小鸟飞行状态枚举
    GameStatus gameStatus = GameStatus::STOPING; //游戏状态 默认停止
    BirdStatus birdStatus = BirdStatus::DOWN;
    double birdDownSpeed = 0;
    double birdUpSpeed = 2.0;
    QTimer *timer = nullptr;
    QVector<int> * v = nullptr;//地面
    int groundSize = 0; //地面个数
    int moveSpeed = 2; //移动速度
    QVector<PipeChannel*> * pipeChannels = nullptr;//管道


    QPushButton* startButton = nullptr;

};

#endif // MAINWINDOW_H
