#ifndef DESK_READ_H
#define DESK_READ_H

#include <QObject>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <QBuffer>
#include <QScreen>
#include <QImage>
#include "video_common.h"
#include "workthread.h"

class Desk_Read;
class DeskReadThread : public WorkThread{
    Q_OBJECT
public slots:
    void slot_threadReadMove();
public:

    void setDeskRead(Desk_Read *newDeskRead);

private:
    Desk_Read * m_deskRead;

};

class Desk_Read : public QObject
{
    Q_OBJECT
public:
    explicit Desk_Read(QObject *parent = nullptr);
    ~Desk_Read();
signals:
    void SIG_screenFrame(QImage img);
    void SIG_screenFrameData(QByteArray ba);

public slots:
    void start();
    void pause();
    void slot_getScreenFrame();
private:
    QTimer *m_timer;

    QSharedPointer<DeskReadThread> m_deskReadThread;


};

#endif // DESK_READ_H
//需要 输出 截屏  (使用qt) ->信号的形式输出
//需要开启和关闭
//定时器超时  执行截屏   ->槽函数  并发送信号
