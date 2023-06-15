#include "setuserdialog.h"
#include "ui_setuserdialog.h"
#include <QMessageBox>
setUserDialog::setUserDialog(QWidget *parent) :
    QDialog(parent),m_iconid(0),
    ui(new Ui::setUserDialog)
{
    ui->setupUi(this);

    for (int i= 0 ; i < 36 ; ++i){
        ui->cbx_icon->addItem(QIcon(QString(":/tx/%1.png").arg(i)) , "");
    }

}

setUserDialog::~setUserDialog()
{
    delete ui;
}
#include <QDebug>
void setUserDialog::on_pb_commit_clicked()
{
    qDebug()<<"第三步，点击提交，并且发射信号";
    int iconid = ui->cbx_icon->currentIndex();
    QString name = ui->le_name->text();
    QString feeling = ui->le_feeling->text();
    //重复判定
    if( name.length() == 0 || name.length() > 20){
        QMessageBox::about( this , "tips","昵称非法");
        return;
    }
    if( feeling.length() == 0 || feeling.length() > 20){
        QMessageBox::about( this , "tips","昵称非法");
        return;
    }
    if( m_iconid == iconid && m_name == name && feeling == m_feeling) return;
    this->hide();
    //修改信号和槽
    qDebug()<<"昵称："<<name;
    qDebug()<<"头像ID:"<<iconid;
    qDebug()<<"签名："<<feeling;
    Q_EMIT SIG_userSetCommit(iconid , name , feeling);
    slot_setInfo(iconid , name , feeling);
    qDebug()<<"将信息设置到demodialogui上";


}


void setUserDialog::on_pb_clear_clicked()
{
    ui->le_feeling->setText("");
    ui->le_name->setText("");
}

void setUserDialog::slot_setInfo(int iconid, QString name, QString feeling)
{
    m_iconid= iconid;
    m_name = name;
    m_feeling = feeling;
    ui->cbx_icon->setCurrentIndex( m_iconid );
    ui->le_feeling->setText(m_feeling);
    ui->le_name->setText( m_name );
}



void setUserDialog::slot_setGroupInfo(int iconid, QString name,QString feeling)
{
    m_iconid= iconid;
    m_name = name;
    m_feeling = feeling;
    ui->cbx_icon->setCurrentIndex( m_iconid );
    ui->le_feeling->setText(m_feeling);
    ui->le_name->setText( m_name );
}
void setUserDialog::slot_classA(int classA)
{
    if(classA){
        classA = 1;
    }else{
       classA = 0;
    }

}

//
