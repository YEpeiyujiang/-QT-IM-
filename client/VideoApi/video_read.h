#ifndef VIDEO_READ_H
#define VIDEO_READ_H

#include <QObject>

#include "video_common.h"
#include"QMessageBox"
#include<QTimer>
#include<QBuffer>

//采集摄像头视频
class Video_Read : public QObject
{
    Q_OBJECT
public:
    explicit Video_Read(QObject *parent = nullptr);
    ~Video_Read();
signals:
    void SIG_videoFrame( QImage img );//传拷贝构造，二倍空间  使用引用可以节省空间 注意要在一个线程使用，多线程下会导致空间释放引用失败
    void SIG_videoFrameData( QByteArray bt );
public slots:
    void start();
    void pause();
    void slot_readMore();
private:
    //摄像头对象
    cv::VideoCapture cap;
    QTimer * m_timer;
};

#endif // VIDEO_READ_H
