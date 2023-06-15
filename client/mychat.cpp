#include "mychat.h"
#include "ui_mychat.h"
#include <QDebug>
#include <QPainter>
#include <QPixmap>
void MyChat::paintEvent(QPaintEvent *p)
{
    QPainter painter(this);

    QColor color(x, y, z);

    painter.setBrush(color);
    painter.drawRect(this->rect());

}

MyChat::MyChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyChat)
{
    ui->setupUi(this);
    setWindowTitle("通讯软件V1.0");
    //初始化QVBoxLayout
    m_layout=new QVBoxLayout;
    layout=new QVBoxLayout;
    m_layout->setContentsMargins(0,0,0,0);  //控件距离边框上下左右的距离
    m_layout->setSpacing(3);  //每个控件彼此的距离
    layout->setContentsMargins(0,0,0,0);  //控件距离边框上下左右的距离
    layout->setSpacing(3);  //每个控件彼此的距离
    //把层设置到控件上
    ui->wdg_list->setLayout(m_layout);
    ui->widget_3->setLayout(layout);
    //设置小齿轮图标

    QPixmap pixmap(":/images/2fb6f114c17fd70752b5b5d3233d858.png");
    QIcon ButtonIcon(pixmap);
    ui->pb_tool2->setIcon(ButtonIcon);
    ui->pb_tool2->setIconSize(pixmap.rect().size());


    QPixmap pixmap1(":/images/ic_sysmen.png");
    QIcon ButtonIcon1(pixmap1);
    ui->pb_tool1->setIcon(ButtonIcon1);
    ui->pb_tool1->setIconSize(pixmap1.rect().size());

    //初始化菜单栏
    m_menu=new QMenu(this);
    //增加菜单选项
    m_menu->addAction("添加好友");
    m_menu->addAction("系统设置");
    //绑定点击菜单选项的信号和槽函数
    connect(m_menu,SIGNAL(triggered(QAction*)),
            this,SLOT(slot_dealMenu(QAction*)));
    x = y = z = 255;

}

MyChat::~MyChat()
{
    delete ui;
}

//添加好友
void MyChat::addFriend(useritem *item)
{
    m_layout->addWidget(item);
}
//添加群组
void MyChat::addGroup(Groupitem *item)
{
    qDebug()<<"添加群组控件";
    layout->addWidget(item);
}

//添加上群组
void MyChat::DeletFriend(useritem *item)
{
    m_layout->removeWidget(item);
}
#include <QDebug>
//设置用户之间的信息到控件上
void MyChat::setInfo(QString name, QString feeling, int iconId)
{
//    ui->lb_name->setText(name);
//    ui->le_feeling->setText(feeling);
//    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(iconId)));

    qDebug()<<"设置用户之间的信息到控件上";
    m_name = name;
    m_iconid = iconId;
    m_feeling = feeling;
    ui->lb_name->setText( m_name );
    //更改feeling
    ui->le_feeling->setText(m_feeling);

    //资源引用
    ui->pb_icon->setIcon( QIcon( QString(":/tx/%1.png").arg(iconId)));
    qDebug()<<"昵称："<<m_name;
    qDebug()<<"头像ID:"<<m_iconid;
    qDebug()<<"签名："<<m_feeling;
}

//重写关闭窗口事件
void MyChat::closeEvent(QCloseEvent *event)
{
//    event->ignore();


    if(QMessageBox::question(this,"退出","是否退出") == QMessageBox::Yes){
        //发信号
        qDebug()<<"下线发送信号";
        Q_EMIT SIG_close();
        event->accept();
    }else{
        //忽略关闭事件
        event->ignore();
    }
}

//void MyChat::setUserInfo()
//{

//}
void MyChat::on_pb_tool1_clicked()
{
    //鼠标点击坐标，向上弹出菜单栏
    //1、获取鼠标点击的坐标
    QPoint p=QCursor::pos();
    //2、计算菜单栏显示的y坐标，需要先计算菜单栏的高度
    QSize size =m_menu->sizeHint();    //获取菜单栏的绝对大小
    m_menu->exec(QPoint(p.x(),p.y()-size.height()));

}

//处理点击菜单项的槽函数
void MyChat::slot_dealMenu(QAction *action)
{
    //判断点击的是哪个菜单项
    if("添加好友"==action->text())
    {
        //处理添加好友的请求
        //发信号通知kernel
        qDebug()<<"添加好友信号发送"<<endl;
        Q_EMIT SIG_AddFriend();
    }
    else if("需要设置"==action->text())
    {
        //处理系统设置的请求
    }
}

//===================================
void MyChat::sendUserInfoSignal(){

    //封包
    STRU_USER_INFO_RQ rq;
    qDebug()<<"test";
    Q_EMIT setUserInfo( (char*)&rq,sizeof(rq));
}
//进入用户信息修改界面
void MyChat::on_pb_icon_clicked()
{
    qDebug()<<__func__;
    //发射信号到ckernel处理
    Q_EMIT SIG_dealUserInfoUpdate();
}


void MyChat::on_pushButton_clicked()
{
    //发射进入五子棋专区的信号
    Q_EMIT SIG_dealEnterZone();
}


void MyChat::on_pb_tool2_clicked()
{
    if(x == 255 &&  y ==  255 && z == 255) {
        x = y = z = 125;
    }else{
        x = y = z = 255;
    }
    MyChat::update();
    //或者QWidget::repaint()
}

//创建群组
void MyChat::on_pb_tool3_clicked()
{
    Q_EMIT SIG_CreateGroup();
}



