#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QRegExp>


LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setUI();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::setUI()
{
    this->setWindowTitle("登录&注册");
    //加载图片 资源路径 .qrc文件下 ：/根目录
    QPixmap pixmap(":/images/background.jpg");
    //画板 添加背景
    QPalette pal(this -> palette());
    pal.setBrush(QPalette::Background ,pixmap );
    this->setPalette(pal);
    //默认是登录界面
    ui->tw_page->setCurrentIndex(0);

}

//注册界面的注册按钮响应
void LoginDialog::on_pb_register_clicked()
{
    //注册信息的过滤
    QString name = ui->le_name_register->text();
    QString tel = ui->le_tel_register->text();
    QString password = ui->le_password_register->text();
    QString confirm = ui->le_confirm_register->text();

    //上述每一个都不能为空
    if( name.isEmpty() || tel.isEmpty() ||
            password.isEmpty() || confirm.isEmpty())
    {
            QMessageBox::about(this , "提示" , "内容不能为空");

    }

    //手机号 要合法 -->正则表达式 1 3-8 0-9 9位 以什么开头^ 以什么结尾 $
    //创建正则对象
    QRegExp exp("^1[345678][0-9]\{9\}$");
    if( !exp.exactMatch(tel)){
        QMessageBox::about(this,"提示","手机号不合法，11位手机号");
        return ;
    }

    //密码不能过长
    if( password.length() > 20)
    {
        QMessageBox::about(this , "提示","密码过长，长度不能超过20");
        return ;
    }

    //昵称 不能全是空格 不能过长
    QString tmpName = name;
    tmpName = tmpName.remove(' ');//remove操作会改变字符串
    if( name.length() > 20 || tmpName.isEmpty() )
    {
        QMessageBox::about(this,"提示","昵称过长或者为空，长度不可超过20");
        return ;
    }
    //两次输入的密码要一致
    if( password != confirm )
    {
        QMessageBox::about(this , "提示","密码两次输入的不一致");
        return ;
    }
    //发送信号
    Q_EMIT SIG_registerCommit(tel,password,name);

}

void LoginDialog::closeEvebt(QCloseEvent *e)
{
    if(QMessageBox::question(this,"退出","是否退出") == QMessageBox::Yes){
        //发信号
        Q_EMIT SIG_close();
        //同意关闭时间
        e->accept();
    }else{
        //忽略关闭事件
        e->ignore();
    }
}

//登陆界面的登录按钮响应
void LoginDialog::on_pb_login_clicked()
{
    //登录信息的过滤
    QString tel = ui->le_tel->text();
    QString password = ui->le_password->text();

    //上述每一个都不能为空
    if(  tel.isEmpty() ||
            password.isEmpty() )
    {
            QMessageBox::about(this , "提示" , "内容不能为空");

    }

    //手机号 要合法 -->正则表达式 1 3-8 0-9 9位 以什么开头^ 以什么结尾 $
    //创建正则对象
    QRegExp exp("^1[345678][0-9]\{9\}$");
    if( !exp.exactMatch(tel)){
        QMessageBox::about(this,"提示","手机号不合法，11位手机号");
        return ;
    }

    //密码不能过长
    if( password.length() > 20)
    {
        QMessageBox::about(this , "提示","密码过长，长度不能超过20");
        return ;
    }

    //发送信号
    Q_EMIT SIG_loginCommit(tel,password);

}

//登录界面清空按钮响应
void LoginDialog::on_pb_clear_clicked()
{
    ui->le_tel->setText("");
    ui->le_password->setText("");
}

//注册界面清空按钮响应
void LoginDialog::on_pb_clear_register_clicked()
{
    ui->le_tel_register->setText("");
    ui->le_password_register->setText("");
    ui->le_name_register->setText("");
    ui->le_confirm_register->setText("");
}



