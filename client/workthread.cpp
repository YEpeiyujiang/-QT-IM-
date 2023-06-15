#include "workthread.h"
#include <QDebug>
WorkThread::WorkThread(QObject *parent) : QObject(parent)
{
    m_thread = new QThread;
    this->moveToThread( m_thread );
    //将该对象放入新建线程, 以后执行的信号对应的槽函数就都在这个线程里

    //开启线程
    m_thread->start();
}

WorkThread::~WorkThread()
{
    if( m_thread )
    {
        m_thread->quit();
        m_thread->wait(100);
        if( m_thread->isRunning() )
        {
            m_thread->terminate();
        }
        delete m_thread; m_thread = nullptr;
    }
    qDebug()<<"WorkThread::~WorkThread";
}
