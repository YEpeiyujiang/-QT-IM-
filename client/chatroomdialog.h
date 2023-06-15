#ifndef CHATROOMDIALOG_H
#define CHATROOMDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include "uiapi/customwidget.h"
#include "usershow.h"
#include <QDebug>
namespace Ui {
class ChatroomDialog;
}
//无边框设计
class ChatroomDialog : public CustomMoveDialog
{
    Q_OBJECT
signals:
    void SIG_quitRoom( );
    //摄像头 音频 桌面
    void SIG_audioOpen();
    void SIG_audioClose();
    void SIG_captureOpen();
    void SIG_captureClose();
    void SIG_deskOpen();
    void SIG_deskClose();

public:
    explicit ChatroomDialog(QWidget *parent = nullptr);
    ~ChatroomDialog();
public slots:
    void slot_setRoomID( int roomid );

    void slot_addUser(  QWidget *user);

    void slot_removeUser(  QWidget *user);

    void slot_setRoomClear();

    void slot_setBigImage( int userid , QImage &img);

    void slot_setBigImageInfo(int userid , QString name);

private slots:
    void on_pb_close_clicked();

    void on_pb_max_clicked();

    void on_pb_mid_clicked();

    void on_pb_hide_clicked();

    void on_pb_quit_clicked();

    void on_cb_capture_clicked();

    void on_cb_audio_clicked();

    void on_cb_desk_clicked();



private:
    Ui::ChatroomDialog *ui;
    //用于用户列表的处置布局
    QVBoxLayout * m_userLayout;
public:
    //失能标志
    int conversation;

};
//预览大图  点击user 发信号 id name 根据这个设置大图
//刷新图片会根据id刷新图片

#endif // ROOMDIALOG_H
