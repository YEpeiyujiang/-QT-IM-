#ifndef USERITEM_H
#define USERITEM_H

#include <QWidget>
#include<QString>

namespace Ui {
class useritem;
}

class useritem : public QWidget
{
    Q_OBJECT
signals:
    //通知kernel弹出聊天窗口的信号
    void SIG_UserItemClicked(int userId);
    //删除好友的信号
    void SIG_DeleteFriend(int userId);
public:
    explicit useritem(QWidget *parent = 0);
    ~useritem();
    //设置好友信息
    void setInfo(int id,int iconId,int state ,QString name,QString feeling);
    //设置灰色头像
    void setOffline();


private slots:
    void on_pb_icon_clicked();

    void on_pb_deletefriend_clicked();

public:
    QString m_name;//昵称
private:
    Ui::useritem *ui;
    int m_id; //好友的id
    int m_iconId;//头像id
    QString m_feeling; //签名
    int m_state;    //状态
};

#endif // USERITEM_H
