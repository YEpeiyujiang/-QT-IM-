#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QPaintEvent>
#include <bqdialog.h>
#include <QModelIndex>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <map>

class ScreenLabel;
namespace Ui {
class ChatDialog;
}
class ChatDialog : public QWidget
{
    Q_OBJECT
signals:
    //发送聊天内容给kernel
    void SIG_sendChatMsg(QString content,int m_id);
    //发送图片
    void sendFriendPicture(QString path,QImage image,int id);
    //发送打开音频聊天的窗口信号
    void SIG_OpenAudio(QString name,int id);
    //表情包处理
    void ShowBQ(int m_id);
    //点击发送文件
    void SIG_sendFile(QString file_path,int id);
    //选择接收的文件名
    void SIG_recvFile(QString fileName,int id);
    //打开保存文件夹
    void SIG_saveFile(QString fileName,int id);

public:
    explicit ChatDialog(QWidget *parent = 0);
    ~ChatDialog();
    //设置聊天窗口
    void setChatWindowInfo(QString name,int id);
    void paintEvent(QPaintEvent * p);
    //设置接收到的聊天内容到浏览窗口上
    void setChatMsg(QString content);
    //显示接收的图片到窗口
    void setChatPic(QString path);
    //设置聊天好友不在线
    void setFriendOffline();
//    //开始发送图片
//    void startTransfer();
//    //图片转base64字符串
//    QByteArray getImageData(const QImage&);
    void setEnabledFalse();
    void setEnableTrue();

    void selectFile(QString fileName);
private slots:
    void slot_sendPic(QImage image);

    void Test(QImage image);

    void on_pd_send_clicked();

//    void on_pb_Pc_clicked();

//    void on_pd_send_picture_clicked();

    void on_pb_VA_clicked();

    void on_pb_VA_2_clicked();

    void on_pd_send_picture_clicked();

    void on_pd_Bq_clicked();

    void slot_Bq(int m_id);

    void on_send_clicked();

    void on_Bq_clicked(bool checked);

    void on_VA_clicked();

    void on_send_picture_clicked();

    void on_pushButton_3_clicked();

    void on_VA_2_clicked();

    void Bq_deal(QString iamge);

//    void on_pushButton_4_clicked();
    void deal_PathName(QString path);


    void on_pb_tFile_clicked();

    void slot_listView_clicked(const QModelIndex &idx);

private:
    Ui::ChatDialog *ui;
    QString m_name;
    int m_id;
    QStringListModel *m_model;
    QStringList m_list;
    std::map<QModelIndex,bool> m_mapLVDoubleClickFlag;
public:
    QString fileName;     // 保存文件路径
    QString currentImageName;//图片名
    QByteArray outBlock;  // 数据缓冲区，即存放每次要发送的数据块
    QImage image;//图片
    qint64 totalBytes;    // 发送数据的总大小
    int conversation;//失能标志

    ScreenLabel *m_pScreenLabel;
    QImage image1 ;
    QImage image2;
    BqDialog * bq;
    QString path1;
    QString path2;
    QString path;
    QString recvpath;//收到的图片的路径

};

#endif // CHATDIALOG_H
