#include "maindialog.h"
#include "ui_maindialog.h"
#include "QMessageBox"

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainDialog)
{
    ui->setupUi(this);
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::closeEvent(QCloseEvent *e)
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

void MainDialog::on_pb_fivelnline_clicked()
{
    //发射信号
    Q_EMIT SIG_joinZone(fIVE_iN_lINE);

    this->hide();
}

