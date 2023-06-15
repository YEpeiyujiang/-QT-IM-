#include "desk_read.h"

//Desk_Read::Desk_Read(QObject *parent) : QObject(parent)
//{
//    m_timer = new QTimer;
//    connect(m_timer , SIGNAL(timeout()),this,SLOT(slot_getScreenFrame()));
//}

Desk_Read::Desk_Read(QObject *parent) : QObject(parent) , m_deskReadThread( new DeskReadThread )
{
    m_timer = new QTimer;

    m_deskReadThread->setDeskRead( this );
//    connect(m_timer , SIGNAL(timeout()),this,SLOT(slot_getScreenFrame()));

    connect(m_timer , SIGNAL(timeout()),m_deskReadThread.get(),SLOT(slot_threadReadMove()));

}

Desk_Read::~Desk_Read()
{
    if( m_timer ){
        delete m_timer;
        m_timer = nullptr;
    }
}

void Desk_Read::start()
{
    m_timer->start(1000/FRAME_RATE);
}

void Desk_Read::pause()
{
    m_timer->stop();
}

void Desk_Read::slot_getScreenFrame()
{
    //获取桌面截图
    //1、获取主屏
    QScreen *src = QApplication::primaryScreen();
    // QRect deskRect = QApplication::desktop()->screenGeometry();
    //2、获取主屏的图片 ， 可以选择截取大小
    QPixmap map = src->grabWindow( QApplication::desktop()->winId()/* ,0,0 , deskRect.width() ,
                                  deskRect.height()*/);
    QImage image = map.toImage();
    image = image.scaled( 1600, 900, Qt::KeepAspectRatio);

    Q_EMIT SIG_screenFrame(image);
    //压缩图片从 RGB24 格式压缩到 JPEG 格式, 发送出去
    QByteArray ba;
    QBuffer qbuf(&ba); // QBuffer 与 QByteArray 字节数组联立联系
    image.save( &qbuf , "JPEG" , IMAGE_QUALITY ); //将图片的数据写入 ba
    Q_EMIT SIG_screenFrameData( ba );
}

void DeskReadThread::slot_threadReadMove()
{
    m_deskRead->slot_getScreenFrame();
}

void DeskReadThread::setDeskRead(Desk_Read *newDeskRead)
{
    m_deskRead = newDeskRead;
}

