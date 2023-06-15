#include "fiveinline.h"
#include "ui_fiveinline.h"
#include <QPainter>
#include <QDebug>
FiveInline::FiveInline( QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FiveInline),m_board(FIL_COLS,std::vector<int>(FIL_ROWS,0)),
      m_movePoint(0,0),m_moveFlag(false),m_blackOrwhite(Black),isover(false),m_status(Black),m_colorCounter(DEFAULT_COUNT)
{
    ui->setupUi(this);
    InitAiVector();
    m_pieceColor[None] = QBrush(QColor(0,0,0,0));//最后一个0 是全透明
    m_pieceColor[Black] = QBrush(QColor(0,0,0));
    m_pieceColor[White] = QBrush(QColor(255,255,255));

    connect( &timer , SIGNAL(timeout()) , this,SLOT(repaint()));
    //repaint()会触发重绘
    timer.start(1000/90);
    //落子和落子处理
    //connect( this ,SIGNAL(SIG_pieceDown(int,int,int)), this , SLOT(slot_pieceDown(int,int,int)) );
    clear();
    //connect( &m_countTimer , SIGNAL(timeout()),this,SLOT(slot_countTimer()));
    //slot_startGame();

}


void FiveInline::setCpuColor(int newCpuColor)
{
    m_cpuColor = newCpuColor;
}

FiveInline::~FiveInline()
{
    timer.stop();
    delete ui;
}

void FiveInline::paintEvent(QPaintEvent *event)
{
    //绘制棋盘
    QPainter painer(this);//QPainter 用于绘图 ，使用有参构造，传入当前对象，得到当前控件的画布
    painer.setBrush(QBrush(QColor(255,160,0)));//通过画刷设置颜色
    painer.drawRect( FIL_MARGIN_WIDTH - FIL_DISTANCE ,
                     FIL_MARGIN_HEIGHT - FIL_DISTANCE ,
                     (FIL_COLS + 1) * FIL_SPACE + 2 * FIL_DISTANCE,
                     (FIL_ROWS + 1) * FIL_SPACE + 2 * FIL_DISTANCE
                 );
    //绘制网格线
    //先画竖线
    for( int i = 1 ; i<= FIL_COLS;i++){
        painer.setBrush(QBrush(QColor(0,0,0)));//通过画刷设置颜色
        QPoint p1(FIL_MARGIN_WIDTH + FIL_SPACE * i,FIL_MARGIN_HEIGHT + FIL_SPACE);
        QPoint p2(FIL_MARGIN_WIDTH + FIL_SPACE * i,FIL_MARGIN_HEIGHT + FIL_SPACE * FIL_ROWS);
        painer.drawLine(p1,p2);
    }
    //后画横线
    for( int i = 1 ; i<= FIL_COLS;i++){
        painer.setBrush(QBrush(QColor(0,0,0)));//通过画刷设置颜色
        QPoint p1(FIL_MARGIN_WIDTH + FIL_SPACE,FIL_MARGIN_HEIGHT + FIL_SPACE * i);
        QPoint p2(FIL_MARGIN_WIDTH + FIL_SPACE * FIL_COLS,FIL_MARGIN_HEIGHT + FIL_SPACE * i);
        painer.drawLine(p1,p2);
    }
    //画中心点
    painer.setBrush(QBrush(QColor(0,0,0)));//通过画刷设置颜色
    painer.drawEllipse(
    QPoint(290 , 290),
             6/2,6/2
                );//画圆  中心点坐标 直径x , y
    //画棋子
    //x,y点对应的颜色   m_pieceColor
    for(int x = 0; x < FIL_COLS ; ++x){
        for( int y = 0 ; y < FIL_ROWS ; ++y){
            if( m_board[x][y] != None){
                painer.setBrush(m_pieceColor[ m_board[x][y]]);//通过画刷设置颜色
                painer.drawEllipse(
                QPoint(FIL_MARGIN_WIDTH + FIL_SPACE + FIL_SPACE * x , FIL_MARGIN_HEIGHT + FIL_SPACE + FIL_SPACE * y),
                         FIL_PIECE_SIZE/2,FIL_PIECE_SIZE/2
                            );//画圆  中心点坐标 直径x , y
            }
        }
    }
    //显示移动的棋子
    if( m_moveFlag ){
        painer.setBrush( m_pieceColor[getblackOrwhite()]);//通过画刷设置颜色
        painer.drawEllipse(
        QPoint(m_movePoint.x(),m_movePoint.y()),
                 FIL_PIECE_SIZE/2,FIL_PIECE_SIZE/2
                    );//画圆  中心点坐标 直径x , y


    }
    event->accept();
}

void FiveInline::mousePressEvent(QMouseEvent *event)
{

    //捕捉点->相对坐标
    m_movePoint = event->pos();
    //位置坐标
    //相对坐标  相对窗口
    //QMoouseEvent::pos()
    //绝对坐标  相对win10桌面
    //QCursor::pos()//鼠标位置
    //QMouseEvent::globalPos();
    //点击状态

    //加入结束判断
    if(isover) goto quit;
    if( m_status != getblackOrwhite()) goto quit;
    m_moveFlag = true;
quit:
    event->accept();



}

void FiveInline::mouseMoveEvent(QMouseEvent *event)
{
    //捕捉点->相对坐标
    if(m_moveFlag){
       m_movePoint = event->pos();
    }
    //qDebug() << m_movePoint.x() << "  "<<m_movePoint.y();
    event->accept();
}

void FiveInline::mouseReleaseEvent(QMouseEvent *event)
{
    m_moveFlag = false;
    //将相对坐标 换算为棋盘坐标
    //会涉及到取整和四舍五入的问题

    //落子是四舍五入 不应该取整
    int x = 0;
    int y = 0;
    float fx = (float)event->pos().x();
    float fy = (float)event->pos().y();

    if(isover) goto quit;
    if( m_status != getblackOrwhite()) goto quit;

    fx = (fx - FIL_MARGIN_WIDTH - FIL_SPACE)/FIL_SPACE;
    fy = (fy - FIL_MARGIN_HEIGHT - FIL_SPACE)/FIL_SPACE;

    x = fx - (int)fx > 0.5 ? ((int)fx + 1) : (int)fx;
    y = fy - (int)fy > 0.5 ? ((int)fy + 1) : (int)fy;

    //是否越界
    if( isCrossLine( x , y)) return;
    Q_EMIT SIG_pieceDown(getblackOrwhite() , x , y);
    //落子的槽函数，对更新数组 切换回合
    //需要涉及到鼠标点击 移动 释放 事件

quit:
    event->accept();
}

int FiveInline::getblackOrwhite() const
{
    return m_blackOrwhite;
}

void FiveInline::changeBlackAndWhite()
{
    //0 1 2
    m_blackOrwhite = (m_blackOrwhite+1);
    if(m_blackOrwhite == 3) m_blackOrwhite = Black;
    if(m_blackOrwhite == Black)
    ui->lb_color->setText("黑子回合");
    else{
        ui->lb_color->setText("白子回合");
    }
}
//判断游戏是否结束，一方玩家是否胜利
bool FiveInline::isWin(int x, int y)
{
    //看四条线上的 八个方向的同色棋子个数，只要有一个到了5，就结束了
    int count = 1;
    //循环看四条线
    //先看一条线
    // <- ->
    for(int dr = d_z ; dr < d_count ; dr += 2){


        for(int i = 1 ; i <= 4 ; i++){
            //获取偏移后的棋子坐标
            int ix = dx[dr] * i + x;
            int iy = dy[dr] * i + y;
            //判断是否出界
            if(isCrossLine( ix , iy)) break;
            //看是否同色
            if( m_board[ix][iy] == m_board[x][y]){
                count++;
            }else break;
        }
        //->
        for(int i = 1; i <=  4 ; i++){
            //获取偏移后的棋子坐标
            int ix = dx[dr+1] * i + x;
            int iy = dy[dr+1] * i + y;
            //判断是否出界
            if(isCrossLine(ix , iy)) break;
            //看是否同色
            if(m_board[ix][iy] == m_board[x][y]){
                count++;
            }else break;

        }
        if(count >= 5)  break;//不看其他的直线
        else count = 1;//不够要看其他直线

    }
    if(count >= 5){
        isover = true;
        return true;
    }
    return false;
}

void FiveInline::clear()
{
    //默认不开AI
    setCpuColor( None );
    //清空赢法的棋子个数统计
    for( int i = 0; i < m_vecWin.size();++i){
        m_vecWin[i].clear();
    }
    //状态位
    isover = true;
    m_blackOrwhite = Black;
    m_moveFlag = false;
    //界面
    for( int x = 0 ; x < FIL_COLS ; x++){
        for ( int y = 0 ; y < FIL_ROWS ; ++y){
            m_board[x][y] = None;
        }
    }
    ui->lb_winner->setText("");
    ui->lb_color->setText("黑子回合");
//    m_colorCounter = DEFAULT_COUNT;
//    ui->lb_time->show();
//    m_countTimer.start( 1000 );
}

void FiveInline::setSelfStatus(int _status)
{
    m_status = _status;
}
//判断是否出界
bool FiveInline::isCrossLine(int x, int y)
{
    if( x < 0 || x >= 15 || y < 0 || y>= 15){
        return true;
    }
    return false;
}

void FiveInline::slot_pieceDown(int blackorwhite, int x, int y)
{
    //更新数组切换回合
    if(blackorwhite != getblackOrwhite())
        return;
    if( m_board[x][y] == None){
            m_board[x][y] = blackorwhite;
            //changeBlackAndWhite();

        if(isWin(x, y)){
//            m_countTimer.stop();
//            ui->lb_time->hide();
            QString str = getblackOrwhite() == Black ? "黑方胜":"白方胜";
            ui->lb_winner->setText(str);
            //QMessageBox::about(this , "提示" ,str);

            Q_EMIT SIG_playerWin(getblackOrwhite());

        }else{
            if(m_cpuColor != getblackOrwhite()){
                //更新玩家的情况
                //更新每种赢法 玩家的棋子个数
                for( int i = 0;i < m_vecWin.size();i++){
                    if( m_vecWin[i].board[x][y] == 1){
                        m_vecWin[i].playerCount += 1;
                        m_vecWin[i].cpuCount = 100;//无效的值
                    }

                }
            }else{
                //统计个数要更新
                for( int k = 0; k < m_vecWin.size() ; ++k){
                    if(m_vecWin[k].board[x][y] == 1){
                        m_vecWin[k].cpuCount += 1;
                        m_vecWin[k].playerCount = 100;
                    }
                }
            }
            m_colorCounter = DEFAULT_COUNT;
            //更换回合
            changeBlackAndWhite();
            //电脑下棋
            if( m_cpuColor == getblackOrwhite())
                pieceDownByCpu();
        }
    }

}

void FiveInline::slot_startGame()
{
    clear();
    isover = false;

}

//初始化所有赢法棋子布局
void FiveInline::InitAiVector()
{
    for( int y = 0 ; y < FIL_ROWS ; y++)
    {
        for( int x = 0; x < FIL_COLS - 4 ; x++)
        {
            //起点位置
            stru_win w;
            for( int k = 0; k < 5 ; k++)
            {
                w.board[ x + k][y] = 1;
            }
            m_vecWin.push_back( w );
        }
    }
    for( int x = 0; x < FIL_COLS ; x++)
    {
        for( int y = 0 ; y < FIL_ROWS - 4 ; y++)
        {
            //起点位置
            stru_win w;
            for( int k = 0; k < 5 ; k++){
                w.board[x][ y + k] = 1;
            }
            m_vecWin.push_back( w );
        }
    }
    for( int x = FIL_COLS - 1; x >= 4 ; x--)
    {
        for( int y = 0 ; y < FIL_COLS - 4 ; y++)
        {
            stru_win w;
            for( int k = 0; k < 5;k++)
            {
                w.board[ x - k][ y + k] = 1;
            }
            m_vecWin.push_back( w );
        }
    }
    for( int x = 0 ; x < FIL_COLS - 4;x++)
    {
        for( int y = 0 ; y < FIL_ROWS - 4 ; y++)
        {
            stru_win w;
            for( int k = 0; k < 5;k++)
            {
                w.board[x + k][ y + k] = 1;
            }
            m_vecWin.push_back( w );
        }
    }

}

//电脑落子：根据每种赢法 棋子的个数 给每一个无子的位置估分，得到所有的最优值，然后得到一个坐标
void FiveInline::pieceDownByCpu()
{
    if(isover) return;
    if( m_cpuColor != getblackOrwhite() ) return;
    int x = 0, y = 0, k = 0;
    int u = 0 , v = 0;//最优坐标
    int max = 0;//最高分数
    int MyScore[FIL_COLS][FIL_ROWS] = {0};
    int CpuScore[FIL_COLS][FIL_ROWS] = {0};
    //记录所有位置的分数
    //记录电脑所有位置的分数

    for( x = 0; x < FIL_COLS  ; ++x)
    {
        for( y = 0; y < FIL_ROWS ; ++y)
        {
            if( m_board[x][y] == None)
            {
                for( k = 0; k < m_vecWin.size() ; ++k)
                {
                    //评估玩家分
                    if( m_vecWin[k].board[x][y] == 1)
                    {
                        //根据该应发 棋子个数
                        switch( m_vecWin[k].playerCount)
                        {
                        case 1:
                            MyScore[x][y] += 200;
                            break;
                        case 2:
                            MyScore[x][y] += 400;
                            break;
                        case 3:
                            MyScore[x][y] += 2000;
                            break;
                        case 4:
                            MyScore[x][y] += 10000;
                            break;

                        }

                    }
                    //评估电脑在x , y分数
                    if( m_vecWin[k].board[x][y] == 1)
                    {
                        //根据该赢法 棋子个数
                        switch( m_vecWin[k].cpuCount){
                        case 1:
                            CpuScore[x][y] += 220;
                            break;
                        case 2:
                            CpuScore[x][y] += 420;
                            break;
                        case 3:
                            CpuScore[x][y] += 4100;
                            break;
                        case 4:
                            CpuScore[x][y] += 200000;
                            break;

                        }

                    }
                }
            }

        }
    }
    //估分 得到最优的目标坐标
    for( x = 0; x < FIL_COLS  ; ++x){
        for( y = 0; y < FIL_ROWS ; ++y){
            if( m_board[x][y] == None){
                //先看玩家
                if( MyScore[x][y] > max){
                    max = MyScore[x][y];
                    u = x ;
                    v = y;
                }else if( MyScore[x][y] == max){
                    if (CpuScore[x][y] > CpuScore[u][v]){
                        u = x;
                        v = y;
                    }
                }
                //再看电脑
                if( CpuScore[x][y] > max){
                    max = CpuScore[x][y];
                    u = x ;
                    v = y;
                }else if( CpuScore[x][y] == max){
                    if (MyScore[x][y] > MyScore[u][v]){
                        u = x;
                        v = y;
                    }
                }


            }
        }
    }


    Q_EMIT SIG_pieceDown( getblackOrwhite() , u , v);
}

void FiveInline::slot_countTimer()
{
//    //if()
//    //每秒更新一次
//    m_colorCounter--;
//    if( m_colorCounter <= 0)
//    {
//        //切换回合
//        changeBlackAndWhite();
//        m_colorCounter = DEFAULT_COUNT;
//        return;
//    }
//    //界面文本更新
//    ui->lb_time->setText(QString("%1秒").arg(m_colorCounter , 2 ,10 ,QChar('0')));
}
