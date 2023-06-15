#ifndef CKERNEL_H
#define CKERNEL_H
#include <QFile>
#include "maindialog.h"
#include <QObject>
#include "INetMediator.h"
#include "packdef.h"
#include <vector>
#include <QTimer>
#include "logindialog.h"
#include "maindialog.h"
#include "roomdialog.h"
#include "roomitem.h"
#include "fiveinlinezone.h"
#include "mychat.h"
#include "setuserdialog.h"
#include "useritem.h"
#include "chatdialog.h"
#include "chatroomdialog.h"
#include <map>
#include "audio_read.h"
#include "audio_write.h"
#include "video_read.h"
#include "desk_read.h"
#include<QSettings>
#include<QCoreApplication>
#include<QFileInfo>
#include <QInputDialog>
#include <QRegExp>
#include <QMessageBox>
#include <QTime>
#include "qDebug"
#include "TcpClientMediator.h"
#include <QMessageBox>
#include "md5.h"
#include <QDebug>
#include "fiveinline.h"
#include "workthread.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "groupitem.h"
#include <map>

//为了优化发送导致的卡顿 ，  可以采用多线程 ， 发送在工作者线程里执行
class SendThread:public WorkThread{
    Q_OBJECT
public:
public slots:

    void slot_threadSendData( unsigned int  , char*  , int  );
private:

};

class CKernel;
//成员函数指针类型
typedef void (CKernel::*PFUN)( unsigned int  , char*  , int  );


//核心类采取单例模式 采用静态单例模式
class CKernel : public QObject
{
    Q_OBJECT
public:
    static CKernel * GetInstance(){
        //采用静态局部变量
        static CKernel kernel;
        return &kernel;
    }
public slots:
    void DestriyInstance();//回收单例成员属性 后续添加的 网络 ui类对象
    void SendData(char * buf ,int nLen);

    //网络处理
    void slot_ReadyData( unsigned int lSendIP , char* buf , int nlen );
    void slot_dealLoginRs( unsigned int lSendIP , char* buf , int nlen );
    void slot_resgisterRs(unsigned int lSendIP, char *buf, int nlen);
    void slot_dealJoinRoomRS(unsigned int lSendIP, char *buf, int nlen);
    void slot_dealRoomMembRq(unsigned int lSendIP, char *buf, int nlen);
    void slot_dealLeaveRoomRq(unsigned int lSendIP, char *buf, int nlen);
    void slot_dealZoneRoomInfo(unsigned int lSendIP, char *buf, int nlen);
    //游戏
    void slot_dealFilGameReadyRq(unsigned int lSendIP, char *buf, int nlen);
    void slot_dealFilGameStartRq(unsigned int lSendIP, char *buf, int nlen);
    void slot_dealFilPieceDownRq(unsigned int lSendIP, char *buf, int nlen);
    void slot_dealFilWinRq(unsigned int lSendIP, char *buf, int nlen);

    //窗口处理
    void slot_loginCommit(QString tel,QString password);
    void slot_registerCommit(QString tel,QString password,QString name);
    void slot_joinZone(int zoneid);
    void slot_joinRoom(int roomid);
    void slot_leaveZone();
    void slot_leaveRoom();

    //准备和开局
    void slot_fil_gameReady(int zoneid,int roomid,int userid);
    void slot_fil_gameStart(int zoneid,int roomid);
    //落子
    void slot_fil_pieceDown(int blackorwhite,int x,int y);

    //输赢
    void slot_fil_win(int blackorwhite);
    //电脑托管
    void SIG_fil_playByCpuBegin(int zoneid,int roomid,int userid);

    void SIG_fil_playByCpuEnd(int zoneid,int roomid,int userid);

    //托管
    void slot_dealFilPlayByCpuBegin(unsigned int lSendIP, char *buf, int nlen);
    void slot_dealFilPlayByCpuEnd(unsigned int lSendIP, char *buf, int nlen);

    //获取专区每个房间玩家数
    void slot_roomInfoInZone();

    //==========================================================================
    void slot_UserItemClicked(int m_id);
    //处理用户信息
    void slot_dealUserInfoRs(unsigned int lSendIP, char *buf, int nlen);
    void slot_setUserInfo(char * buf , int len);
    //处理个人信息的更新
    void slot_dealUserInfoUpdate();
    void slot_userSetCommit(int iconid , QString name , QString feeling);
    //处理进入五子棋专区的信号
    void slot_dealEnterZone();
    //处理好友列表
    void slot_dealFriendInfoRs(unsigned int lSendIP, char *buf, int nlen);
    //处理信息转发
    void slot_sendChatMsg(QString content,int id);
    //处理聊天
    void DealChatRq(unsigned int lSendIP, char *buf, int nlen);
    //处理接受聊天
    void RecvMsg(unsigned int lSendIP, char *buf, int nlen);
    //处理好友下线请求
    void DealOfflinRq(unsigned int lSendIP, char *buf, int nlen);
    //下线槽函数
    void slot_Offline();
    //处理添加好友的槽函数
    void slot_AddFriend();

    void slot_captureOpen();

    //开关设置
    void slot_audioOpen();
    void slot_audioClose();
   // void slot_captureOpen();
    void slot_captureClose();


    //添加好友回复
    void DealAddFriendRq(unsigned int lSendIP, char *buf, int nlen);
    void DealAddFriend(unsigned int lSendIP, char *buf, int nlen);
    //删除好友
    void slot_DeleteFriend(int friendid);
    //DealDeletFriendRq删除好友服务器的回复
    void DealDeletFriendRq(unsigned int lSendIP, char *buf, int nlen);
    //处理发送图片的槽函数
//    void DealsendFriendPicture( );
    //进行音频通话的槽函数
    void slot_OpenAudio(QString name , int id);
    //处理音频通话请求 同意or拒绝
    void DealVDARq(unsigned int lSendIP, char *buf, int nlen);
    //
    void DealVDARs(unsigned int lSendIP, char *buf, int nlen);




//    void slot_deskOpen();
//    void slot_deskClose();

    //发送音频
    void slot_sendAudioFrame(QByteArray &ba);
    void slot_sendVideoFrameData(QByteArray ba);
    // 以及视频回显视频回显
    void slot_refreshVideoFrame(QImage img);

//    void slot_dealRoomMemberRs(unsigned int lSendIP, char *buf, int nlen);

    //处理收到的音频数据
    void slot_dealAudioFrame(unsigned int lSendIP, char *buf, int nlen);
    void slot_dealVideoFrame(unsigned int lSendIP, char *buf, int nlen);
    //进行大图显示的设置
    void slot_refreshUserFrame(int id, QImage &img);
    //超级CV大法
    //挂断音视频电话
    void slot_quitRoom();
    void quitconversationrame(unsigned int lSendIP, char *buf, int nlen);
    //处理发送图片信号的槽函数
    void slot_SendPictureRq(QString pathname,QImage image,int id);

    void slot_dealSendPictureRq(unsigned int lSendIP, char *buf, int nlen);
    void slot_dealSendPictureRs(unsigned int lSendIP, char *buf, int nlen);

    //处理表情包窗口
    void DealShowBQ(int m_id);
    //收到图片路径的包
    void slot_dealSendPicturepathname(unsigned int lSendIP, char *buf, int nlen);
    //处理创建群组
    void slot_CreateGroup();
    //群组信息处理
    void slot_dealGroupInfo(unsigned int lSendIP, char *buf, int nlen);
    //更改群组信息的槽函数
    void slot_ChangeGroupInof(int g_id);

    //文件传输
    void slot_sendFile(QString file_path,int id);
    //接受文件
    void slot_dealSendFileRq(unsigned int lSendIP, char *buf, int nlen);
    void slot_dealSendFileRs(unsigned int lSendIP, char *buf, int nlen);
    void slot_recvFile(unsigned int lSendIP, char *buf, int nlen);
    void slot_selectFile(QString fileName,int id);

    //打开保存文件夹
    void slot_openSaveFileDir(QString fileName,int id);
signals:
    void SIG_sendVideoFrma(unsigned int lSendIP, char *buf, int nlen);
    void SIG_sendPic(QImage image);
    void Test(QImage iamge);
    //处理表情窗口信号
    void SIG_BQWin(int m_id);
    //发射路径信号给chatdialog
    void SIG_PathName(QString pant);
    void SIG_ClassA(bool classA);


private:
    void setNetPackMap();
    explicit CKernel(QObject *parent = nullptr);
    ~CKernel(){/*DestriyInstance();*/}//添加析构函数
    CKernel(const CKernel & kernel){}
    CKernel & operator= (const CKernel & kernel){
        return *this;
    }
    //设置配置文件
    void ConfigSet();

    //成员属性  网络 ui
    MainDialog * m_mainDialog;
    LoginDialog * m_loginDialog;
    FiveInLineZone * m_fiveInLineZone;
    RoomDialog * m_roomDialog;

    INetMediator * m_client;
    //协议映射表  协议头与处理函数的对应关系
    std::vector<PFUN> m_netPackFunMap;
    //个人信息
    int m_id;
    int m_roomid;
    int m_zoneid;//区id
    bool m_isHost;//是否是房主
    QString m_userName;
    char m_serverIP[20];

    //单位时间获取专区内房间内玩家个数
    QTimer m_rqTimer;
    QString m_name;

    //-------------------------------
    MyChat * m_mychat;
    setUserDialog* m_setUserDialog;
    setUserDialog* m_GroupUserDialog;
    //保存id和对应的useritem
    QMap<int,useritem*> m_mapIdToUseritem;
    //保存id和对应用户的聊天窗口
    QMap<int,ChatDialog*> m_mapIdToChatdlg;
    //创建音频聊天的窗口对象
    ChatroomDialog * m_ChatroomDialog;
    //*******************************************************************
    //    //每一个用户的视频显示
    std::map<int , UserShow*> m_mapIDToUserShow;
    //    //音频
    Audio_Read* m_audioRead;

    Audio_Write* m_audioWrite;
    //Audio_Write每一个用户 都要创建

    //std::map<int , Audio_Write*> m_mapIDToAudioWrite;
    //采集
    Video_Read * m_videoRead;
    //桌面采集
    Desk_Read * m_deskRead;
    //*******************************************************************
    //这也是一个CV大法
    int m_friendid;//这里是为了你要给谁转发音频 ， 可以通过信号和槽获取
    int conversation;

    QSharedPointer<SendThread> m_deskReadThread;

    //图片
    QImage m_image;


    QByteArray baa;

    //收到的图片应该设置的路径名
    QString recvpathName;

    //区分是群组信息更改还是个人信息更改
    //获取当前操作的组id
    int m_gid;

    //文件名-路径
    std::map<QString,QString>m_mapfilePath;

    //正在传输的文件名
    QString m_tfilename;

    //已经传输完成的文件名
    std::map<QString,QString> m_hashALFilePath;


};

#endif // CKERNEL_H
