#ifndef FIVEINLINE_H
#define FIVEINLINE_H

#include <QWidget>
#include <vector>
#include <QPaintEvent>
#include <QTimer>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui { class FiveInline; }
QT_END_NAMESPACE

#define DEFAULT_COUNT (45)

//界面是580 * 580固定的

//外边距
#define FIL_MARGIN_HEIGHT   (50)
#define FIL_MARGIN_WIDTH    (50)
//行列数  五子棋规范为15*15
#define FIL_COLS            (15)
#define FIL_ROWS            (15)
//边和边之间的边距
#define FIL_SPACE           (30)
//棋子的大小
#define FIL_PIECE_SIZE      (28)
//棋盘边缘缩进的距离
#define FIL_DISTANCE        (10)
using namespace std;
struct stru_win
{
    stru_win():board(FIL_COLS,vector<int>(FIL_ROWS,0)),playerCount(0),cpuCount(0){

    }
    void clear(){
        playerCount = 0;
        cpuCount = 0;
    }
    vector<vector<int>> board;//胜利的棋子布局
    int playerCount;
    int cpuCount;
};

//1、首先绘制棋盘 网格线  棋子
//2、鼠标点击，出现棋子，然后鼠标移动，棋子跟着移动，鼠标释放，棋子落子
//3、定时器刷新界面 1 秒 25帧
//4、落子 出界 要放弃 合法的 发送信号
//落子的槽函数，对更新数组 切换回合
//需要涉及到鼠标点击 移动 释放 事件

//关于输赢
//根据当前点，依次查看左右 上下 左上右下 左下右上  四条直线上的棋子个数
//初始棋子个数为1，然后根据方向换算新的坐标，查看是否出界，出界 break ， 不然看是否和当前棋子同色
//如果count 为5 结束，没有到5则继续看其他直线
//所有都看完没到5 则游戏结束

class FiveInline : public QWidget
{
    Q_OBJECT
signals:
    //落子信号：鼠标释放捕捉落点的位置
    void SIG_pieceDown(int blackorwhite , int x ,int y);//什么颜色子 落在什么位置
    void SIG_playerWin( int blackorWhite);

public:
    FiveInline(QWidget *parent = nullptr);
    ~FiveInline();
    //重绘事件：绘图背景 棋盘 棋子等
    void paintEvent(QPaintEvent * event);

    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    //获取当前黑白回合
    int getblackOrwhite() const;
    //切换回合
    void changeBlackAndWhite();
    //判断是胜利
    bool isWin(int x ,int y);
    //清空
    void clear();

    void setSelfStatus(int _status);
    enum ENUM_BLACK_OR_WHITE{None = 0 , Black ,White};
    //五子棋AI
    //初始化所有赢法
    void InitAiVector();
    //电脑落子
    void pieceDownByCpu();

    void setCpuColor(int newCpuColor);
private:
    Ui::FiveInline *ui;
    //记录当前是黑子还是白字的回合
    int m_blackOrwhite;
    //鼠标移动鼠标所在位置
    QPoint m_movePoint;
    //移动标志 标志棋子是否随着鼠标移动的，也就是鼠标是不是一个一直在点击的状态
    bool m_moveFlag;

    //存棋盘上棋子的二维数组
    std::vector<std::vector<int>> m_board;
    //棋子颜色数组
    QBrush m_pieceColor[3];
    //定时器
    QTimer timer;
    //判断是否出界
    bool isCrossLine(int x , int y);
    //方向
    enum enum_director{d_z,d_y,d_s,d_x,d_zs,d_yx,d_zx,d_ys,d_count};
    //根据方向对坐标偏移 每次是一个单位
    int dx[8] = { -1 , 1, 0 ,0 , -1 , 1, -1, 1  };
    int dy[8] = { 0 , 0 , -1 , 1 , -1 , 1, 1, -1};
    //判断游戏结束标志
    bool isover;

    //网络版本 玩家身份  不是自己回合是不能动的
    int m_status;

    //五子棋AI
    vector <stru_win> m_vecWin;
    //电脑的回合
    int m_cpuColor;

    //回合定时
    //每秒的定时
    QTimer m_countTimer;
    //剩余时间计数器
    int m_colorCounter;//初始值 45秒

public slots:
    void slot_pieceDown(int blackorwhite , int x ,int y);
    void slot_startGame();
    void slot_countTimer();
};
#endif // FIVEINLINE_H
