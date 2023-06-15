#if _MSC_VER >= 1600    // VC2010
#pragma  execution_character_set("UTF-8")
#endif

#include "ScreenLabel.h"
#include "screentool.h"

#include <QDesktopWidget>
#include <QScreen>
#include <QGuiApplication>
#include <QApplication>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>

#define textH 30    //文字区域高度
#define textW 200   //文字区域宽度
#define rectB 2     //矩形框厚度
#define opacity 0.5  //蒙版透明度

#define CurrentTime QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")

ScreenLabel::ScreenLabel(QWidget *parent)
    :QLabel(parent)
    , mouseMove(false)
{   
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);  //在顶层
    this->setCursor(Qt::CrossCursor); //鼠标十字架

    m_pScreenTool = new ScreenTool;  //工具条

    //mCoverMap.load("images/mask.png");  //蒙版(弄一张纯色也行)
    mCoverMap = QPixmap(1920,1080);  //自己定义一张图片（大小随意，反正是纯色拉伸）
    mCoverMap.fill(QColor(100,100,100,100)); //填充纯色并设置透明度(不设置在画图时设置也行)，提高性能

    this->initConnectTool();
}

void ScreenLabel::initConnectTool()
{
    connect(m_pScreenTool,&ScreenTool::sigSave,this,[=]{  //保存图片
        QString path = QFileDialog::getSaveFileName(this,"保存图片",CurrentTime,"PNG (*.png);;BMP (*.bmp);;JPEG (*.jpg;*.jpeg)");
        if(!path.isEmpty())
        {
            mRectMap.save(path);
            this->close();
            m_pScreenTool->close();
        }
    });

    connect(m_pScreenTool,&ScreenTool::sigCancel,this,[=]{  //取消
        mouseMove = false;
        update();
    });

    connect(m_pScreenTool,&ScreenTool::sigExit,this,[=]{  //退出
        this->close();
    });

    connect(m_pScreenTool,&ScreenTool::sigOk,this,[=]{  //确定
        emit sigCutMap(mRectMap);
        this->close();
    });
}


void ScreenLabel::showScreen()
{


    QScreen* screen = QGuiApplication::primaryScreen();
    QDesktopWidget* desktop = QApplication::desktop();
    screenRect = desktop->screenGeometry();
    mScreenMap = screen->grabWindow(desktop->winId(), 0, 0, screenRect.width(), screenRect.height());  //整个屏幕图

    this->showFullScreen(); //全屏显示
    //this->setCursor(Qt::CrossCursor); //鼠标十字架
    mouseMove = false;
    update();

}

void ScreenLabel::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    //p.setRenderHint(QPainter::SmoothPixmapTransform);
    //p.setRenderHint(QPainter::Antialiasing); //抗锯齿（不用它，反正都是横平竖直的，没锯齿产生（除了画文字））

    p.drawPixmap(this->rect(),mScreenMap);  //画全屏幕图

    //设置一层蒙版
    //p.setOpacity(opacity); //蒙版透明度(蒙版已设置了就不用再此设置) 在此设置会影响性能
    p.drawPixmap(this->rect(),mCoverMap); //还是用贴图性能好
    //p.setBrush(QColor(100,100,100));
    //p.drawRect(this->rect()); //用画刷太影响性能了

    //鼠标移动截取
    if(mouseMove)
    {             
        int w = mMovePoint.x()-mPressPoint.x();
        int h = mMovePoint.y()-mPressPoint.y();

        //QRect cutRect;  //截图部分
        QRect textRect; //文字框
        QRect borderRect; //边框

        //分4种情况，区别矩形起始点位置
        if(w>0 && h>0) //往右下角滑动截图（常规方式）
        {
            cutRect    = QRect(mPressPoint,mMovePoint);
            mRectMap   = mScreenMap.copy(cutRect); //截取矩形部分图

            textRect   = QRect(mPressPoint.x(),mPressPoint.y()-textH,textW,textH);  //文字矩形框
            borderRect = QRect(mPressPoint.x()-rectB/2,mPressPoint.y()-rectB/2,w+rectB,h+rectB); //截图部分框
        }
        else if(w<0 && h<0) //往左上角滑动截图
        {
            cutRect    = QRect(mMovePoint,mPressPoint);
            mRectMap   = mScreenMap.copy(cutRect); //截取矩形部分

            textRect   = QRect(mMovePoint.x(),mMovePoint.y()-textH,textW,textH);
            borderRect = QRect(mMovePoint.x()-rectB/2,mMovePoint.y()-rectB/2,-w+rectB,-h+rectB);
        }
        else if(h>0 && w<0 ) //往左下角滑动截图
        {
            cutRect    = QRect(mMovePoint.x(),mPressPoint.y(),-w,h);
            mRectMap   = mScreenMap.copy(cutRect); //截取矩形部分

            textRect   = QRect(mMovePoint.x(),mPressPoint.y()-textH,textW,textH);
            borderRect = QRect(mMovePoint.x()-rectB/2,mPressPoint.y()-rectB/2,-w+rectB,h+rectB);
        }
        else if(w>0 && h<0) //往右上角滑动截图
        {
            cutRect    = QRect(mPressPoint.x(),mMovePoint.y(),w,-h);
            mRectMap   = mScreenMap.copy(cutRect); //截取矩形部分

            textRect   = QRect(mPressPoint.x(),mMovePoint.y()-textH,textW,textH);
            borderRect = QRect(mPressPoint.x()-rectB/2,mMovePoint.y()-rectB/2,w+rectB,-h+rectB);
        }


        //p.setOpacity(1.0); //设置不透明
        p.setPen(QPen(Qt::white,1, Qt::DashDotLine)); //笔
        p.setFont(QFont("微软雅黑",12));

        p.drawPixmap(cutRect,mRectMap); //画截图
        p.drawText(textRect,QString("%1×%2").arg(qAbs(w)).arg(qAbs(h))); //画文字
        p.setPen(QPen(QColor(0,174,255),rectB));
        p.drawRect(borderRect); //画边框
    }


    QWidget::paintEvent(event);
}

void ScreenLabel::keyPressEvent(QKeyEvent *event)
{
    // Esc 键退出截图;
    if (event->key() == Qt::Key_Escape)
    {
        this->close();
    }
    // Eeter键完成截图;
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        this->close();
    }
}


void ScreenLabel::mousePressEvent(QMouseEvent *e)
{

   if(e->button() == Qt::LeftButton)
   {
       mPressPoint = e->pos();   //记录鼠标左键按下位置
   }

   QWidget::mousePressEvent(e);
}

//正常情况下，鼠标按下并移动才会触发此函数
void ScreenLabel::mouseMoveEvent(QMouseEvent *e)
{
    mMovePoint = e->pos(); //移动
    if(!mouseMove)
        mouseMove = true;
    update();
    QWidget::mouseMoveEvent(e);
}

void ScreenLabel::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        mReleasePoint = e->pos(); //鼠标左键释放位置
//        if(mouseMove)
//            mouseMove = false;
        //在此显示工具条或者关闭
        this->showTool();
        //this->close();
    }

    QWidget::mouseReleaseEvent(e);
}

void ScreenLabel::showTool()
{
    //判断工具条显示位置是否超出屏幕范围了,工具条高度 48
    int toolW = m_pScreenTool->getToolWidth();
    int toolH = m_pScreenTool->getToolHeight();

    int x = cutRect.x()+cutRect.width()+rectB/2;
    int y = cutRect.y()+cutRect.height()+rectB*2;

    int px, py;

    if((y+toolH)<=screenRect.height())  //下方
    {
        if(x-toolW>0)   //右下方
        {
            px = x-toolW;
            py = y;
        }
        else  //左下方
        {
            px = cutRect.x()+rectB/2;
            py = y;
        }
    }
    else if(cutRect.y()-rectB*2-toolH>=0) //上方
    {
        if(x-toolW>0)  //右上方
        {
            px = x-toolW;
            py = cutRect.y()-rectB*2-toolH;
        }
        else  //左上方
        {
            px = 0;
            py = cutRect.y()-rectB*2-toolH;
        }

    }
    else //在截图内部
    {
        if(x-toolW>0) //内部右下角
        {
            px = x-rectB/2-toolW;
            py = cutRect.y()+cutRect.height()-rectB*2-toolH;
        }
        else //内部左下角
        {
            px =  cutRect.x()+rectB/2;
            py = cutRect.y()+cutRect.height()-rectB*2-toolH;
        }
    }
    m_pScreenTool->setShowPos(px,py);
}
