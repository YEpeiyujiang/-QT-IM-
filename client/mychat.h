#ifndef MYCHAT_H
#define MYCHAT_H

#include <QPaintEvent>
#include <QWidget>
#include<QVBoxLayout>
#include"useritem.h"
#include<QMenu>
#include<QCloseEvent>
#include<QMessageBox>
#include "packdef.h"
#include "groupitem.h"
namespace Ui {
class MyChat;
}

class MyChat : public QWidget
{
    Q_OBJECT
signals:
    //发送用户点击"点击好友"菜单项的信号
    void SIG_AddFriend();

    void setUserInfo(char * buf,int len);
    //关闭信号 发信号到ckernel触发信号与槽机制 回收资源
    void SIG_close();
    //发射修改个人信息模块的信号
    void SIG_dealUserInfoUpdate();
    //发射进入五子棋专区的信号
    void SIG_dealEnterZone();
    //创建群组
    void SIG_CreateGroup();
public:
    void paintEvent(QPaintEvent * p);
    explicit MyChat(QWidget *parent = 0);
    ~MyChat();
    //添加好友
    void addFriend(useritem *item);
    //添加群组控件
    void addGroup(Groupitem * item);
    //删除好友
    void DeletFriend(useritem *item);
    //设置用户之间的信息到控件上
    void setInfo(QString name,QString feeling ,int iconId);
    //重写关闭窗口事件
    void closeEvent(QCloseEvent *event);
    void sendUserInfoSignal();
public slots:

private slots:
    void on_pb_tool1_clicked();
    //处理点击菜单项的槽函数
    void slot_dealMenu(QAction* action);
    void on_pb_icon_clicked();

    void on_pushButton_clicked();

    void on_pb_tool2_clicked();

    void on_pb_tool3_clicked();



private:
    Ui::MyChat *ui;
    //垂直布局
    QVBoxLayout * m_layout;
    QVBoxLayout * layout;
    //菜单栏
    QMenu* m_menu;
    //用户头像
    int m_iconid;
    //用户名字
    QString m_name;
    //用户签名
    QString m_feeling;
    int x ;
    int y ;
    int z ;
};

#endif // MYCHAT_H
