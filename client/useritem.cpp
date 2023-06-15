#include "useritem.h"
#include "ui_useritem.h"
#include<QBitmap>
#include <QDebug>
useritem::useritem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::useritem)
{
    ui->setupUi(this);
}

useritem::~useritem()
{
    delete ui;
}

//设置好友信息
void useritem::setInfo(int id, int iconId, int state, QString name, QString feeling)
{
    qDebug()<<"设置自定义控件";
    //1、保存好友信息
    m_id=id;
    m_iconId=iconId;
    m_state=state;
    m_name=name;
    m_feeling=feeling;
//    qDebug()<<m_id;
//    qDebug()<<m_iconId;
//    qDebug()<<m_state;
//    qDebug()<<m_name;
//    qDebug()<<m_feeling;

    //2、把昵称和签名设置到控件
    ui->lb_name->setText(m_name);
    ui->lb_feeling->setText(m_feeling);

    //3、根据好友状态显示头像
    //拼出头像文件的路径
    QString path =QString(":/tx/%1.png").arg(m_iconId);
    if(m_state)
    {
        //在线，显示为彩色
        ui->pb_icon->setIcon(QIcon(path));
    }
    else
    {
        //不在线，显示为灰色
        QBitmap bmp;
        bmp.load(path);
        ui->pb_icon ->setIcon(bmp);
    }
    //4、重绘控件
    this->repaint();
}
//设置灰色头像
void useritem::setOffline()
{
    qDebug()<<"进行灰色头像处理";
    QString path=QString(":/tx/%1.png").arg(m_iconId);
    QBitmap bmp;
    bmp.load(path);
    ui->pb_icon->setIcon(bmp);
    //4、重绘控件
    this->repaint();
}

//单击好友头像
void useritem::on_pb_icon_clicked()
{
    //发送信号给kernel，弹出与好友的聊天窗口
    Q_EMIT SIG_UserItemClicked(m_id);
}
#include <QMessageBox>
void useritem::on_pb_deletefriend_clicked()
{
    //发射删除好友的信号
    if(QMessageBox::question(this,"提示","是否真的删除好友") == QMessageBox::Yes){
        Q_EMIT SIG_DeleteFriend(m_id);
    }
}

//删除好友

