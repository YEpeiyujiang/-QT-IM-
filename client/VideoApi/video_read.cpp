#include "video_read.h"

Video_Read::Video_Read(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer;
    connect( m_timer , SIGNAL( timeout()) ,
             this , SLOT(slot_readMore()) );
}

Video_Read::~Video_Read()
{
    m_timer->stop();
    delete m_timer;
}

void Video_Read::start()
{
    //打开摄像头
    cap.open(0);//打开默认摄像头
    if(!cap.isOpened ()){
        QMessageBox::information(NULL,tr("提示"),tr("视频没有打开"));
        return;
    }
    //打开定时器
    m_timer->start( 1000/FRAME_RATE );
}

void Video_Read::pause()
{
    //关闭摄像头
    if( cap.isOpened())
        cap.release();
    //关闭定时器
    m_timer->stop();
}

void Video_Read::slot_readMore()
{
    //获取摄像头
    Mat frame;
    if( !cap. read (frame) )
    {
        return;
    }
    //将 opencv 采集的 BGR 的图片类型转化为 RGB24 的类型
    cvtColor(frame,frame,CV_BGR2RGB);
    //定义 QImage 对象, 用于发送数据以及图片显示
    QImage image ((unsigned const
    char*)frame.data,frame.cols,frame.rows,QImage::Format_RGB888);
    //转化为大小更小的图片  等比例缩放
    image = image.scaled( IMAGE_WIDTH,IMAGE_HEIGHT, Qt::KeepAspectRatio );
    //发送图片  //信号形式发送
    Q_EMIT SIG_videoFrame( image );


    //图片转换为 QByteArray 进行发送
    //压缩图片从 RGB24 格式压缩到 JPEG 格式, 发送出去
    QByteArray ba;
    QBuffer qbuf(&ba); // QBuffer 与 QByteArray 字节数组联立联系
    image.save( &qbuf , "JPEG" , IMAGE_QUALITY ); //将图片的数据写入 ba

    Q_EMIT SIG_videoFrameData( ba );
}
