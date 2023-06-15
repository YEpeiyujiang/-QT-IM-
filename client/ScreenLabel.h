#ifndef SCREENLABEL_H
#define SCREENLABEL_H

#include <QObject>
#include <QLabel>

#include <QMouseEvent>
#include <QImage>
/*
 * 屏幕截图类
 * 实现原理：在QLabel（用其他的也行）上画图，QLabel全屏且在顶部显示，一层一层画图，共三层（整个屏幕图->蒙版图（设置透明）->截取的矩形图）
 * 记录鼠标左键按下位置，记录鼠标移动位置，鼠标移动刷新画图（paintEvent），鼠标左键释放截图完毕，弹出工具条
 * 接口：showScreen()
 * 获取截图：sigCutMap(const QImage)，关联测信号即可
 */

class ScreenTool;

class ScreenLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ScreenLabel(QWidget *parent = 0);

public:
    void showScreen();  //接口

private:
    void initConnectTool();

    void showTool();

protected:
    void paintEvent(QPaintEvent *)override;

    void mousePressEvent(QMouseEvent *)override;
    void mouseMoveEvent(QMouseEvent *)override;
    void mouseReleaseEvent(QMouseEvent *)override;

    void keyPressEvent(QKeyEvent *)override;

signals:
    void sigCutMap(const QPixmap);  //获取截图

private:
    QPixmap mScreenMap;  //整个桌面图
    QPixmap mCoverMap;   //蒙版
    QPixmap mRectMap;    //所截取的矩形图

    QRect cutRect;  //截图矩形部分
    QRect screenRect; //屏幕

    volatile bool mouseMove; //鼠标移动

    QPoint mPressPoint;    //左键按下点
    QPoint mReleasePoint;  //左键释放点
    QPoint mMovePoint;     //鼠标移动点

    ScreenTool *m_pScreenTool; //工具条
};


#endif // SCREENLABEL_H
