#ifndef CLOGIC_H
#define CLOGIC_H

#include"TCPKernel.h"

class CLogic
{
public:
    CLogic( TcpKernel* pkernel ):m_roomUserList(121 )
    {
        m_pKernel = pkernel;
        m_sql = pkernel->m_sql;
        m_tcp = pkernel->m_tcp;
        pthread_mutex_init(&m_roomListMutex ,NULL);
    }

public:
    //设置协议映射
    void setNetPackMap();
    /************** 发送数据*********************/
    void SendData( sock_fd clientfd, char*szbuf, int nlen )
    {
        m_pKernel->SendData( clientfd ,szbuf , nlen );
    }
    /************** 网络处理 *********************/
    //注册
    void RegisterRq(sock_fd clientfd, char*szbuf, int nlen);
    //登录
    void LoginRq(sock_fd clientfd, char*szbuf, int nlen);
    //加入分区
    void JoinZoneRq(sock_fd clientfd, char*szbuf, int nlen);
    //离开分区
    void LeaveZoneRq(sock_fd clientfd, char*szbuf, int nlen);
    //加入房间
    void JoinRoomRq(sock_fd clientfd, char*szbuf, int nlen);
    //离开房间
    void LeaveRoomRq(sock_fd clientfd, char*szbuf, int nlen);
    //五子棋命令转发
    void FIL_MsgSendRq(sock_fd clientfd, char*szbuf, int nlen);
    //五子棋落子转发
    void FIL_PieceDownRq(sock_fd clientfd, char*szbuf, int nlen);
    //
    void ZoneInfoRq(sock_fd clientfd, char*szbuf, int nlen);
    //设置用户信息
    void UpdateUserInfoRq(sock_fd clientfd, char*szbuf, int nlen);
    //查询好友列表
    void getUserList(int userid);
    //添加好友请求
    void dealAddFriendRq(sock_fd clientfd, char*szbuf, int nlen);
    //dealAddFriendRs添加好友请求的回复
    void dealAddFriendRs(sock_fd clientfd, char*szbuf, int nlen);

    /*******************************************/
    //更新头像
    void GetUserInfoAndSend(int id);
    //更新好友列表
    void GetFriendInfoAndSend(int id,int c_id);
    //处理聊天
    void DealChat(sock_fd clientfd, char*szbuf, int nlen);
    //处理下线请求
    void dealOfflineRq(sock_fd clientfd, char*szbuf, int nlen);
    //删除好友
    void dealDeleteFriendRs(sock_fd clientfd, char*szbuf, int nlen);
    //处理好友音频聊天请求
    void dealFriendVideoandAudioRs(sock_fd clientfd, char*szbuf, int nlen);
//    //处理好友音频聊天请求
    void dealFriendVideoandAudio(sock_fd clientfd, char*szbuf, int nlen);

    //音频
    void AudioFrame(sock_fd clientfd, char*szbuf, int nlen);
    //视频
    void VideoFrame(sock_fd clientfd, char*szbuf, int nlen);

    void quitconversationrame(sock_fd clientfd, char*szbuf, int nlen);

    void slot_dealSendPictureRq(sock_fd clientfd, char*szbuf,int nlen);//处理转发文件请求
    void slot_dealSendPictureRs(sock_fd clientfd, char*szbuf,int nlen);//处理转发文件回复
    //转发图片路径
    void slot_dealSendPicturepathname(sock_fd clientfd, char*szbuf,int nlen);
    //slot_dealCreateGroup  创建群组
    void slot_dealCreateGroup(sock_fd clientfd, char*szbuf,int nlen);
    //获取群组信息
    void getGroupInfor(int id);
    //获取小组内成员信息
    void GetGroupInfoAndSend(int id);
    //更改群组信息
    void slot_ChangeGroupInfo(sock_fd clientfd, char*szbuf,int nlen);
    //转发文件路径
    void slot_dealSendFile(sock_fd clientfd, char*szbuf,int nlen);

    void slot_dealrecvFile(sock_fd clientfd, char*szbuf,int nlen);
private:
    TcpKernel* m_pKernel;
    CMysql * m_sql;
    Block_Epoll_Net * m_tcp;
    //状态位判断 userid  state  socket
    //std::map<int , sock_fd > m_mapIdStateInfo;
    //MyMap <int ,UserInfo *> m_mapIdToUserInfo;
    MyMap<int , UserInfo*> m_mapFdToUserInfo;
    //包含好友ID
    MyMap<int , UserInfo*> m_mapOnlineFriendInfo;
    //MyMap++;
    vector<list<int>> m_roomUserList;
    pthread_mutex_t m_roomListMutex;
};

#endif // CLOGIC_H
