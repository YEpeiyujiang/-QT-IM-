#include "chatroomdialog.h"
#include "ui_chatroomdialog.h"
#include <QMessageBox>
ChatroomDialog::ChatroomDialog(QWidget *parent) :
    CustomMoveDialog(parent),
    ui(new Ui::ChatroomDialog)
{
    ui->setupUi(this);

    m_userLayout = new QVBoxLayout;

    //用户列表垂直布局
    ui->wdg_userList->setLayout( m_userLayout);

    m_userLayout->setContentsMargins( 0,0,0,0 );
    m_userLayout->setSpacing( 5 );
    //可以通过addwidget添加空间
    //m_userLayout->addWidget();
    //添加测试用户
    //    UserShow *user1 = new UserShow;
    //    user1->slot_setInfo(1, "罗人家");

    //    UserShow *user2 = new UserShow;
    //    user2->slot_setInfo(2, "罗人家");

    //    UserShow *user3 = new UserShow;
    //    user3->slot_setInfo(3, "罗人家");

    //    UserShow *user4 = new UserShow;
    //    user4->slot_setInfo(4, "罗人家");

    //    UserShow *user5 = new UserShow;
    //    user5->slot_setInfo(5, "罗人家");

    //    slot_addUser(user1);
    //    slot_addUser(user2);
    //    slot_addUser(user3);
    //    slot_addUser(user4);
    //    slot_addUser(user5);
}

ChatroomDialog::~ChatroomDialog()
{
    delete ui;
}

void ChatroomDialog::slot_setRoomID(int roomid)
{
    ui->lb_tittle->setText( QString("房间号[%1]").arg(roomid) );
}
//添加用户
void ChatroomDialog::slot_addUser(QWidget *user)
{
    m_userLayout->addWidget( user );
}
//移除用户
void ChatroomDialog::slot_removeUser(QWidget *user)
{
    user->hide();
    m_userLayout->removeWidget( user );
    delete user;
}

void ChatroomDialog::slot_setRoomClear()
{
    //设置ui
    ui->cb_audio->setChecked( false );
    ui->cb_capture->setChecked( false );
    ui->cb_desk->setChecked( false );
    //发信号
    Q_EMIT SIG_audioClose();
    Q_EMIT SIG_captureClose();
    Q_EMIT SIG_deskClose();
}

void ChatroomDialog::slot_setBigImage(int userid, QImage& img)
{
    if( ui -> wdg_bigShow->m_id == userid ){
        ui -> wdg_bigShow->slot_setImage( img );
    }
}

void ChatroomDialog::slot_setBigImageInfo(int userid, QString name)
{
    qDebug()<<"这是一个点击事件";
    ui -> wdg_bigShow->slot_setInfo( userid , name);
    //如果使用不传参的方式可以使用下述方法得到参数
    UserShow * user = (UserShow *) QObject::sender();
    qDebug()<<"id :" << user->m_id << "name ："<<user->m_name;
}

void ChatroomDialog::on_pb_close_clicked()
{
    on_pb_quit_clicked();
}


void ChatroomDialog::on_pb_max_clicked()
{
    this->slot_showMax();
}


void ChatroomDialog::on_pb_mid_clicked()
{
    this->slot_showMin();
}

//列表的显示和隐藏
void ChatroomDialog::on_pb_hide_clicked()
{
    if( ui->sa_userList->isHidden()){
        ui->sa_userList->show();
    }else{
        ui->sa_userList->hide();
    }
}

void ChatroomDialog::on_pb_quit_clicked()
{
    if(QMessageBox::question( this , "tips","是否退出房间?") == QMessageBox::Yes){
        qDebug()<<"发送挂断电话的信号";
        Q_EMIT SIG_quitRoom();
        this->close();
    }
}

#include <QDebug>
//void RoomDialog::on_cb_capture_clicked()
//{

//}

////这是麦克风
//void RoomDialog::on_cb_audio_clicked()
//{

//}

////这是摄像头
void ChatroomDialog::on_cb_desk_clicked()
{
    if( ui->cb_desk->isChecked() ){
        qDebug()<<"点击桌面采集开关";
//        ui->cb_desk->setChecked( false );
        ui->cb_capture->setChecked( false );
        //发信号
        //桌面采集开启 摄像头关闭
        Q_EMIT SIG_captureClose();
        Q_EMIT SIG_deskOpen();


    }else{
//        ui->cb_desk->setChecked( true );
        Q_EMIT SIG_deskClose();
    }
}


void ChatroomDialog::on_cb_capture_clicked()
{
    qDebug()<<"摄像头------------";
    if( ui -> cb_capture->isChecked()){
        ui->cb_desk->setChecked( false );
        Q_EMIT SIG_captureOpen();
        Q_EMIT SIG_deskClose();
    }else{
        Q_EMIT SIG_captureClose();
    }
}


void ChatroomDialog::on_cb_audio_clicked()
{
    qDebug()<<"麦克风*********************";
    if( ui->cb_audio->isChecked()){
        Q_EMIT SIG_audioOpen();
    }else{
        Q_EMIT SIG_audioClose();
    }
}




