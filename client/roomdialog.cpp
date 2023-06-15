#include "roomdialog.h"
#include "ui_roomdialog.h"
#include <QMessageBox>
#include "packdef.h"
#include <QDebug>
RoomDialog::RoomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomDialog),m_roomid(0),m_status(_player)
{
    ui->setupUi(this);
    ui->pb_start->setEnabled( false );
    //关于开局的按键 可以是kernel判断，然后给room发送 可以点击开局

    //点击开局，返回开局的信号

    //五子棋类发送的信号 转发给kernel
    connect(ui->wdg_play,SIGNAL(SIG_pieceDown(int,int,int)),this,SIGNAL(SIG_pieceDown(int,int,int)));
    connect(ui->wdg_play,SIGNAL(SIG_playerWin(int)),this,SIGNAL(SIG_playerWin(int)));
}

RoomDialog::~RoomDialog()
{
    delete ui;
}

void RoomDialog::setInfo(int roomid)
{
    m_roomid = roomid;

    QString txt = QString("五子棋-%1房").arg(roomid);

    this->setWindowTitle( txt );
}


//设置玩家身份
void RoomDialog::setUserStatus(int status)
{
    m_status = status;
    ui->wdg_play->setSelfStatus( m_status == _host?FiveInline::Black:FiveInline::White);

}

//设置房主信息
void RoomDialog::setHostInfo(int id, QString name)
{
   ui -> lb_player1_name->setText( name );
   //添加人物形象
   ui->lb_icon_player1->setPixmap(QPixmap(":/icon/avatar_6.png").copy(0,0,350,550));
   m_roomUserList.push_back( id );
}
//设置玩家信息
void RoomDialog::setPlayerInfo(int id, QString name)
{
    ui -> lb_player2_name->setText( name );
    ui->lb_icon_player2->setPixmap(QPixmap(":/icon/avatar_6.png").copy(0,0,350,550));
    m_roomUserList.push_back( id );
}
//离开房间
void RoomDialog::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::question(this , "退出房间提示","是否退出房间?") == QMessageBox::Yes){
        Q_EMIT SIG_close();
        event->accept();
    }else{
        event->ignore();
    }
}

void RoomDialog::clearRoom()
{
    //ui
    ui->lb_player1_name->setText("name");
    ui->lb_player2_name->setText("name");
    ui->lb_icon_player1->setPixmap(QPixmap(":/icon/slotwait.png"));
    ui->lb_icon_player2->setPixmap(QPixmap(":/icon/slotwait.png"));

    //游戏界面清空
    ui->pb_start->setEnabled( false );
    ui->pb_player1_ready->setEnabled( true );
    ui->pb_player2_ready->setEnabled( true );
    ui->pb_player1_ready->setCheckable(false);
    ui->pb_player2_ready->setCheckable(false);
    ui->pb_player1_ready->setText("待准备");
    ui->pb_player2_ready->setText("待准备");


    ui->pb_player1_cpu->setChecked(false);
    ui->pb_player2_cpu->setChecked(false);
    ui->pb_player1_cpu->setText("待托管");
    ui->pb_player2_cpu->setText("待托管");
    //聊天窗口清空

    //后台数据
    m_roomid = 0;
    m_roomUserList.clear();
    m_status = _player;//身份

}
//玩家退出
void RoomDialog::playerLeave( int id)
{
    //ui
    ui->lb_player2_name->setText("name");
    ui->lb_icon_player2->setPixmap(QPixmap(":/icon/slotwait.png"));
    //后台数据的改变
    for(auto ite = m_roomUserList.begin();ite != m_roomUserList.end();ite++){
        if(id == *ite){
            ite = m_roomUserList.erase(ite);
            break;
        }
    }

}

//设置开局
void RoomDialog::setGameStart()
{
    //开始  准备 全部失效
    ui->pb_player1_ready->setEnabled(false);
    ui->pb_player2_ready->setEnabled(false);
    ui->pb_start->setEnabled(false);
    //五子棋开始操作
    ui->wdg_play->slot_startGame();
}

void RoomDialog::resetAllPushButton()
{
    ui->pb_start->setEnabled( false );
    ui->pb_player1_ready->setEnabled( true );
    ui->pb_player1_ready->setChecked(false);
    ui->pb_player2_ready->setEnabled( true );
    ui->pb_player2_ready->setChecked(false);
    ui->pb_player1_ready->setText("待准备");
    ui->pb_player2_ready->setText("待准备");

    ui->pb_player1_cpu->setChecked(false);
    ui->pb_player2_cpu->setChecked(false);
    ui->pb_player1_cpu->setText("待托管");
    ui->pb_player2_cpu->setText("待托管");
}

void RoomDialog::setHostPlayByCpu(bool yes)
{
    if( yes ){
        ui->pb_player1_cpu->setText("托管中");
    }else{
        ui->pb_player1_cpu->setText("待托管");
    }

}

void RoomDialog::setPlayerplayByCpu(bool yes)
{
    if( yes ){
        ui->pb_player2_cpu->setText("托管中");
    }else{
        ui->pb_player2_cpu->setText("待托管");
    }
}

void RoomDialog::setPlayerReady(int id)
{
    if( m_roomUserList.front() == id){
        ui->pb_player1_ready->setChecked(true);
        ui->pb_player1_ready->setText("已准备");
    }
    if( m_roomUserList.back() == id && m_roomUserList.size() == 2){
        ui->pb_player2_ready->setChecked(true);
        ui->pb_player2_ready->setText("已准备");
    }
    if(ui->pb_player1_ready->isChecked() && ui->pb_player2_ready->isChecked()){
        //都准备
        qDebug()<<"开局按钮启用";
        if( m_status == _host)
            ui->pb_start->setEnabled( true );
    }
}

void RoomDialog::on_pb_start_clicked()
{
    //验证是不是自己点击
    if( m_status != _host ) return;
    Q_EMIT SIG_gameStart(0x01,m_roomid);
}

void RoomDialog::slot_pieceDown(int blackorwhite, int x, int y)
{
    ui->wdg_play->slot_pieceDown( blackorwhite , x , y);
}

void RoomDialog::on_pb_player1_ready_clicked(bool checked)
{

    //验证是不是自己点击
    if( m_status != _host ) return;
    //是 切换状态
    if( ui->pb_player1_ready->isChecked()){
        qDebug()<<"1**********";
        //发送信号
        Q_EMIT SIG_gameReady( 0x10 , m_roomid , m_roomUserList.front());
    }else{
        ui->pb_player1_ready->setText("待准备");
    }
}

void RoomDialog::on_pb_player2_ready_clicked(bool checked)
{

    //验证是不是自己点击
    if( m_status != _player ) return;
    //是 切换状态
    if( ui->pb_player2_ready->isChecked()){
        qDebug()<<"2**********";
        //发送信号
        Q_EMIT SIG_gameReady( 0x10 , m_roomid , m_roomUserList.back());
    }else{
        ui->pb_player2_ready->setText("待准备");
    }
}

//第一次点击 是托管 第二次是取消托管
void RoomDialog::on_pb_player1_cpu_clicked(bool checked)
{
    //判断自己 然后再执行
    if( m_status == _host){
        if(ui->pb_player1_cpu->isChecked()){
            //首先 托管通过网络发送 需要一个信号
            Q_EMIT SIG_playByCpuBegin( 0x10 , m_roomid , m_roomUserList.front());
            //托管信息发到服务器 再转回来 再改变文字 待托管->托管中-->不在这里完成

            //电脑自动下棋
            ui->wdg_play->setCpuColor( FiveInline::Black);
            ui->wdg_play->pieceDownByCpu();
        }else{
            //首先 托管通过网络发送 需要一个信号
            Q_EMIT SIG_playByCpuEnd( 0x10 , m_roomid , m_roomUserList.front());
            //托管信息发到服务器 再转回来 再改变文字 待托管->托管中
            //电脑自动下棋
            ui->wdg_play->setCpuColor( FiveInline::None);
        }
    }
}


void RoomDialog::on_pb_player2_cpu_clicked(bool checked)
{
    if(m_roomUserList.size() != 2) return;
    //判断自己 然后再执行
    if( m_status == _player){
        if(ui->pb_player2_cpu->isChecked()){
            //首先 托管通过网络发送 需要一个信号
            Q_EMIT SIG_playByCpuBegin( 0x10 , m_roomid , m_roomUserList.back());
            //托管信息发到服务器 再转回来 再改变文字 待托管->托管中-->不在这里完成

            //电脑自动下棋
            ui->wdg_play->setCpuColor( FiveInline::White);
            ui->wdg_play->pieceDownByCpu();
        }else{
            //首先 托管通过网络发送 需要一个信号
            Q_EMIT SIG_playByCpuEnd( 0x10 , m_roomid , m_roomUserList.back());
            //托管信息发到服务器 再转回来 再改变文字 待托管->托管中
            //电脑自动下棋
            ui->wdg_play->setCpuColor( FiveInline::None);
        }
    }
}

