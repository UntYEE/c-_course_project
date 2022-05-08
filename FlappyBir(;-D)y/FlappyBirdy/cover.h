#ifndef COVER_H
#define COVER_H

#include <QWidget>
#include<QPixmap>
#include<QPushButton>
#include<QMouseEvent>
#include<QPainter>
class cover : public QWidget
{
    Q_OBJECT
public:
    explicit cover(QWidget *parent = nullptr);
    QPixmap coverin;
    void paintEvent(QPaintEvent*);
    QPushButton *getin;
    void testbutton(QMouseEvent*event);
    QPainter*painter=nullptr;
    void mousePressEvent(QMouseEvent *event);

signals:

public slots:

};

#endif // COVER_H
