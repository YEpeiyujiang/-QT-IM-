#include "groupitem.h"
#include "ui_groupitem.h"
#include <QDebug>
Groupitem::Groupitem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Groupitem)
{
    ui->setupUi(this);
}

Groupitem::~Groupitem()
{
    delete ui;
}

void Groupitem::setInfo(int id, int iconId, QString name)
{
    qDebug()<<"群组的自定义控件";
    g_id = id;
    iconid = iconId;
    m_name = name;
    ui->label->setText(name);
    QString path =QString(":/tx/%1.png").arg(iconid);
    ui->pushButton->setIcon(QIcon(path));
    this->repaint();
}
////设置灰色头像
//void useritem::setOffline()
//{
//    qDebug()<<"进行灰色头像处理";
//    QString path=QString(":/tx/%1.png").arg(m_iconId);
//    QBitmap bmp;
//    bmp.load(path);
//    ui->pb_icon->setIcon(bmp);
//    //4、重绘控件
//    this->repaint();
//}

////单击好友头像
//void useritem::on_pb_icon_clicked()
//{
//    //发送信号给kernel，弹出与好友的聊天窗口
//    Q_EMIT SIG_UserItemClicked(m_id);
//}
//#include <QMessageBox>
//void useritem::on_pb_deletefriend_clicked()
//{
//    //发射删除好友的信号
//    if(QMessageBox::question(this,"提示","是否真的删除好友") == QMessageBox::Yes){
//        Q_EMIT SIG_DeleteFriend(m_id);
//    }
//}

//删除好友


void Groupitem::on_pushButton_clicked()
{
    //发射更改群组头像的信号
    Q_EMIT SIG_ChangeGroupInof(g_id);
}

