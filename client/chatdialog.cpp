#include "chatdialog.h"
#include "ui_chatdialog.h"
#include<QTime>
#include "qaesencryption.h"
#include <QMessageBox>
#include "ScreenLabel.h"
#include <QFile>
#include<QFileDialog>
#define STRDATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"))
const QString passWord="hdwbdwangljhljh";
const QString ivStr="abcdeffhadd";
ChatDialog::ChatDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatDialog)
{
    m_model = new QStringListModel(this);
    conversation = 1;
    ui->setupUi(this);
    if(conversation){
        ui->pb_VA->setEnabled(true);
    }else{
        ui->pb_VA->setEnabled(false);
    }

    m_pScreenLabel = new ScreenLabel;

    connect(ui->ls_1,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_listView_clicked(QModelIndex)));

    connect(m_pScreenLabel,&ScreenLabel::sigCutMap,this,[=](const QPixmap image)
    {
//        ui->label->setPixmap(image);
//        image1 = image.toImage();
//        QFile file("./release/pic/H.png");
//        if(file.exists()){
//            file.remove();
//        }
//        image1.save("./release/pic/H.png" , "PNG" , 100);

        // 510,40 当前label大小
        QPixmap pixmap = image.scaled(120, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放

        ui->label->setPixmap(pixmap);
        QString p = "./release/pic/";
        path = QString("./release/pic/%1.png").arg(STRDATETIME).toStdString().c_str();
        image1 = image.toImage();
        image1.save( path, "JPEG" , 100); //将图片的数据写入 ba

    });
    QPixmap pixmap(":/images/1675544445027.png");
    QIcon ButtonIcon(pixmap);
    ui->pb_VA_2->setIcon(ButtonIcon);
    ui->pb_VA_2->setIconSize(pixmap.rect().size());

    QPixmap pixmap1(":/images/1675544423183.png");
    QIcon ButtonIcon1(pixmap1);
    ui->pd_Bq->setIcon(ButtonIcon1);
    ui->pd_Bq->setIconSize(pixmap1.rect().size());

    QPixmap pixmap2(":/images/1675546612526.png");
    QIcon ButtonIcon2(pixmap2);
    ui->pb_VA->setIcon(ButtonIcon2);
    ui->pb_VA->setIconSize(pixmap2.rect().size());

    QPixmap pixmap3(":/images/1675544436692.png");
    QIcon ButtonIcon3(pixmap3);
    ui->pb_Pc->setIcon(ButtonIcon3);
    ui->pb_Pc->setIconSize(pixmap3.rect().size());

    ui->Bq->setFlat(true);
    ui->pb_Pc->setFlat(true);
    ui->pb_VA->setFlat(true);
    ui->pb_VA_2->setFlat(true);
    ui->pd_Bq->setFlat(true);
    ui->pd_send->setFlat(true);
    ui->pd_send_picture->setFlat(true);
    ui->pb_tFile->setFlat(true);
    ui->send->setFlat(true);
    ui->send_picture->setFlat(true);
    ui->VA->setFlat(true);
    ui->VA_2->setFlat(true);

    ui->chat2->setStyleSheet("background:transparent;border-width:0;border-style:outset");
    ui->te_chat2->setStyleSheet("background:transparent;border-width:0;border-style:outset");

    bq = new BqDialog;

    connect(bq,SIGNAL(Bq_sig(QString)),this,SLOT(Bq_deal(QString)));
    path1 = "./release/pic/H.png";
    path2 = "./release/pic/R.png";
    m_list.clear();
}
//发送图片
void ChatDialog::on_send_picture_clicked()
{
    qDebug()<<"发送图片";
    ui->label->clear();
    //把用户输入发字符串显示到上面的浏览窗口:我【时间】....
    ui->chat2->append("<font color=\"#FF0000\">" + QString("【我】%1").arg(QTime::currentTime().toString("hh:mm:ss")) + "</font> ");
    ui->chat2->append(" ");
    ui->label->clear();
    QString file = QString("<img src=\"%1\" height=\"140\" width=\"158\"/>").arg(path);
    ui->chat2->insertHtml(file);
    ui->chat2->append(" ");

    Q_EMIT sendFriendPicture(path,image1, m_id);
}

//收到图片
void ChatDialog::slot_sendPic(QImage image)
{
    ui->chat2->append(QString("【%1】%2").arg(m_name).arg(QTime::currentTime().toString("hh:mm:ss")));
    ui->chat2->append(" ");
    QString file = QString("<img src=\"%1\" height=\"140\" width=\"158\"/>").arg(recvpath);
    ui->chat2->insertHtml(file);
    ui->chat2->append(" ");
}
//发表情
void ChatDialog::Bq_deal(QString iamge)
{
    //把用户输入发字符串显示到上面的浏览窗口:我【时间】....
    ui->chat2->append("<font color=\"#FF0000\">" + QString("【我】%1").arg(QTime::currentTime().toString("hh:mm:ss")) + "</font> ");
    ui->chat2->append(" ");
    QString file = QString("<img src=\"%1\" height=\"140\" width=\"158\"/>").arg(iamge);
    ui->chat2->insertHtml(file);
    ui->chat2->append(" ");
    QPixmap pixmap(iamge);
    QImage image = pixmap.toImage();
    bq->hide();

    Q_EMIT sendFriendPicture(iamge,image, m_id);

}

void ChatDialog::deal_PathName(QString path)
{
    //获得收到图片的路径
    recvpath = path;
    qDebug()<<"ChatDialog:"<<recvpath;

}
ChatDialog::~ChatDialog()
{
    delete ui;
    delete bq;
    bq = nullptr;
}

 //设置聊天窗口
void ChatDialog::setChatWindowInfo(QString name,int id)
{
    //1.保存当前聊天窗口对应用户的ip和昵称
    m_name=name;
    m_id=id;
    //2.设置聊天窗口标题
    setWindowTitle(QString("与【%1】的聊天").arg(m_name));
}
#include <QPainter>
void ChatDialog::paintEvent(QPaintEvent *p)
{
    QPainter painter(this);

    QColor color(125, 125, 125);

    painter.setBrush(color);
    painter.drawRect(this->rect());

}

//设置接收到的聊天内容到浏览窗口上
void ChatDialog::setChatMsg(QString content)
{
    //格式：【昵称】时间
    ui->chat2->append(QString("【%1】%2").arg(m_name).arg(QTime::currentTime().toString("hh:mm:ss")));
    QString decodeECBText=QAESEncryption::decodeAesOnECB(content,passWord,QAESEncryption::AES_128);
    //内容
    ui->chat2->append(content);

}

//设置聊天好友不在线
void ChatDialog::setFriendOffline()
{
    //格式：【昵称】时间 该用户不在线
    ui->chat2->append(QString("【%1】%2该用户不在线").arg(m_name)
                        .arg(QTime::currentTime().toString("hh:mm:ss")));
}

void ChatDialog::setEnabledFalse()
{
    qDebug()<<"没有失能吗";
    ui->pb_VA->setEnabled(false);
}

void ChatDialog::setEnableTrue()
{
    ui->pb_VA->setEnabled(true);
}

//显示可挑选的文件
void ChatDialog::selectFile(QString fileName)
{
    m_list<<fileName;
    m_model->setStringList(m_list);
    ui->ls_1->setModel(m_model);
}

void ChatDialog::on_send_clicked()
{
    //1.获取用户输入的字符串，判断是不是空
    QString content =ui->te_chat2->toPlainText(); //获取不带格式的纯文本
    QString encodeECBText=QAESEncryption::encodeAesOnECB(content,passWord,QAESEncryption::AES_128);
    qDebug()<<encodeECBText;
    if(content.isEmpty())
    {
        return ;
    }
    //2.获取用户输入的字符串，清空编辑窗口
    content=ui->te_chat2->toHtml();  //获取带格式的文本
    ui->te_chat2->clear();
    //3.把用户输入发字符串显示到上面的浏览窗口:我【时间】....
    ui->chat2->append("<font color=\"#FF0000\">" + QString("【我】%1").arg(QTime::currentTime().toString("hh:mm:ss")) + "</font> ");
    ui->chat2->append("<font color=\"#FF0000\">" + content + "</font> ");
    //4.通过信号发给kernel
    Q_EMIT SIG_sendChatMsg(content,m_id);

}

void ChatDialog::on_Bq_clicked(bool checked)
{
    //    Q_EMIT ShowBQ(m_id);
        bq->show();
}

void ChatDialog::on_VA_clicked()
{
    //进行音频通话的设置
    //发送打开音频聊天窗口的信号
    QString name = m_name;
    int friendid = m_id;
    Q_EMIT SIG_OpenAudio(name,friendid);
}

void ChatDialog::on_VA_2_clicked()
{
    m_pScreenLabel->showScreen();
}

void ChatDialog::on_pd_send_clicked()
{


}

void ChatDialog::on_pb_VA_clicked()
{

}

//截图
void ChatDialog::on_pb_VA_2_clicked()
{

}

//发送图片
void ChatDialog::on_pd_send_picture_clicked()
{

}

void ChatDialog::on_pushButton_3_clicked()
{

}

//发送表情
void ChatDialog::on_pd_Bq_clicked()
{

}

void ChatDialog::slot_Bq(int m_id)
{


}

void ChatDialog::Test(QImage image)
{

}

void ChatDialog::setChatPic(QString path)
{


}

//发送文件模块
void ChatDialog::on_pb_tFile_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this,tr("请选择要上传的文件"),".../","*.*");
    if(file_path.isEmpty())
        return;
    Q_EMIT SIG_sendFile(file_path,m_id);
}

void ChatDialog::slot_listView_clicked(const QModelIndex &idx)
{
    if(m_mapLVDoubleClickFlag.count(idx)>0)
    {
        Q_EMIT SIG_saveFile(m_model->data(idx,Qt::DisplayRole).toString(),m_id);
    }
    else
    {
        m_mapLVDoubleClickFlag[idx] = true;
        Q_EMIT SIG_recvFile(m_model->data(idx,Qt::DisplayRole).toString(),m_id);
    }
}

