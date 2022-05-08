
#include<bestscore.h>
#include<qfile.h>
#include<qstring.h>
#include<qcoreapplication.h>
#include<qdebug.h>

//读入最高分
bestscore::bestscore()
{
    QFile file(QCoreApplication::applicationDirPath().append("/score.txt"));
    file.open(QFile::ReadOnly);
    QByteArray ba = file.readAll();
    file.close();
    score = ba.toInt();
}
//存储最高分
void bestscore::save()
{
    QFile file(QCoreApplication::applicationDirPath().append("/score.txt"));
    file.open(QFile::WriteOnly|QFile::Text);
    file.write(QString::number(score).toLocal8Bit());
    file.close();
}
//初始化
void bestscore::init()
{
    QFile file(QCoreApplication::applicationDirPath().append("/score.txt"));
    file.open(QFile::ReadOnly);
    QByteArray ba = file.readAll();
    file.close();
    score = ba.toInt();
}
