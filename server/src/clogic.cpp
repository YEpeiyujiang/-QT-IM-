#include "clogic.h"
#include <list>
using namespace std;
void CLogic::setNetPackMap()
{
    NetPackMap(DEF_PACK_REGISTER_RQ)    = &CLogic::RegisterRq;
    NetPackMap(DEF_PACK_LOGIN_RQ)       = &CLogic::LoginRq;
    NetPackMap(DEF_PACK_JOIN_ZONE)      = &CLogic::JoinZoneRq;
    NetPackMap(DEF_PACK_LEAVE_ZONE)     = &CLogic::LeaveZoneRq;
    NetPackMap(DEF_JOIN_ROOM_RQ)        = &CLogic::JoinRoomRq;
    NetPackMap(DEF_LIAVE_ROOM_RQ)       = &CLogic::LeaveRoomRq;

    NetPackMap(DEF_ZONE_INFO_RQ)        = &CLogic::ZoneInfoRq;

    NetPackMap(DEF_FIL_ROOM_READY)       = &CLogic::FIL_MsgSendRq;
    NetPackMap(DEF_FIL_GAME_START)      = &CLogic::FIL_MsgSendRq;
    NetPackMap(DEF_FIL_AI_BEGIN)        = &CLogic::FIL_MsgSendRq;
    NetPackMap(DEF_FIL_AI_END)          = &CLogic::FIL_MsgSendRq;
    NetPackMap(DEF_FIL_DISCARD_THIS)    = &CLogic::FIL_MsgSendRq;
    NetPackMap(DEF_FIL_SURREND)         = &CLogic::FIL_MsgSendRq;
    NetPackMap(DEF_FIL_WIN)             = &CLogic::FIL_MsgSendRq;

    NetPackMap(DEF_FIL_PIECEDOWN)       = &CLogic::FIL_PieceDownRq;

    //---------------------------------------------------------------------------
    NetPackMap(DEF_PACK_USER_INFO_RQ)       = &CLogic::UpdateUserInfoRq;
    NetPackMap(DEF_PACK_CHAT_INFO)           = &CLogic::DealChat; 
    //处理下线请求
    NetPackMap(DEF_PROTOCOL_TCP_OFFLINE_RQ)           = &CLogic::dealOfflineRq;
    //添加好友请求   50
    NetPackMap(DEF_PROTOCOL_ADD_RQ)           = &CLogic::dealAddFriendRq;
    //dealAddFriendRs添加好友请求的回复
    NetPackMap(DEF_PROTOCOL_ADD_RS)           = &CLogic::dealAddFriendRs;
    //删除好友
    NetPackMap(DEF_PROTOCOL_DELETE)           = &CLogic::dealDeleteFriendRs;
    //处理好友音频聊天请求
    NetPackMap(DEF_PROTOCOL_VIDEOANDAUDIOANDDESK)           = &CLogic::dealFriendVideoandAudioRs;
    //处理好友音频聊天请求是否同意
    NetPackMap(DEF_PROTOCOL_IFVORFVIDEOANDAUDIOANDDESK)           = &CLogic::dealFriendVideoandAudio;

    NetPackMap(DEF_PACK_AUDIO_FRAME)        = &CLogic::AudioFrame;
    NetPackMap(DEF_PACK_VIDEO_FRAME)        = &CLogic::VideoFrame;
    //DEF_QUIT_CONVERSATION
    NetPackMap(DEF_QUIT_CONVERSATION)        = &CLogic::quitconversationrame;

    NetPackMap(DEF_PACK_SENDPICTURE_RQ) = &CLogic::slot_dealSendPictureRq;
    NetPackMap(DEF_PACK_SENDPICTURE_RS) = &CLogic::slot_dealSendPictureRs;
    //转发图片路径
    NetPackMap(DEF_PACK_SENDPICTURE_NAME) = &CLogic::slot_dealSendPicturepathname;
    //创建群组
    NetPackMap(DEF_PACK_CREAT_GROUP) = &CLogic::slot_dealCreateGroup;
    //更改群组信息
    NetPackMap(DEF_PACK_QUIT_GROUP_CHANGE) = &CLogic::slot_ChangeGroupInfo;

    //文件传输
    NetPackMap(DEF_PACK_SEND_FILE_RQ) = &CLogic::slot_dealSendFile;
    NetPackMap(DEF_PACK_SEND_FILE_RS) = &CLogic::slot_dealSendFile;
    NetPackMap(DEF_PACK_RECV_FILE) = &CLogic::slot_dealrecvFile;
}

//注册LeaveRoomRq
void CLogic::RegisterRq(sock_fd clientfd,char* szbuf,int nlen)
{
//    printf("clientfd:%d RegisterRq\n", clientfd);

//    //解析数据包 获取tel password name
//    STRU_REGISTER_RQ * rq = (STRU_REGISTER_RQ *)szbuf;
//    STRU_REGISTER_RS rs;
//    //STRU_REGISTER_RQ * rq = ()
//    //根据tel 查看数据库  看有没有
//    list<string> lstRes;//
//    char sqlstr[1000] = "";
//    sprintf(sqlstr,"select tel from t_user where tel = '%s';",rq->tel);
//    m_sql->SelectMysql(sqlstr,1,lstRes);
//    //有 返回结果
//    if(lstRes.size() > 0)
//    {
//        rs.result = tel_is_exist;
//    }else{
//        //没有 接下来看昵称 有没有
//        lstRes.clear();
//        sprintf(sqlstr , "select name from t_user where name = '%s';",rq -> name);
//        m_sql->SelectMysql(sqlstr,1,lstRes);
//        if(lstRes.size() > 0){
//            rs.result = name_is_exist;//有 返回结果
//        }else{

//            //没有 注册成功 更新数据库 写表
//            //sprintf(sqlstr,"insert into t_user (tel , password , name) value( '%s' ,'%s','%s')",rq->tel,rq->password,rq->name);
//            sprintf(sqlstr,"insert into t_user(tel,password,name,icon,feeling) values('%s','%s','%s',1,'太懒了 啥也没写');",rq->name,rq->password,rq->tel);
//            bool res = m_sql->UpdataMysql(sqlstr);
//            if(!res){
//                printf("updata fail : %s \n",sqlstr);
//            }
//        }
//    }

//     SendData(clientfd , (char *)&rs , sizeof(rs));
    printf("clientfd:%d RegisterRq\n", clientfd);

    //解析数据包 获取tel password name
    STRU_REGISTER_RQ * rq = (STRU_REGISTER_RQ *)szbuf;
    STRU_REGISTER_RS rs;
    //STRU_REGISTER_RQ * rq = ()
    //根据tel 查看数据库  看有没有
    list<string> lstRes;//
    char sqlstr[1000] = "";
    sprintf(sqlstr,"select tel from t_user where tel = '%s';",rq->tel);
    m_sql->SelectMysql(sqlstr,1,lstRes);
    //有 返回结果
    if(lstRes.size() > 0)
    {
        rs.result = tel_is_exist;
    }else{
        //没有 接下来看昵称 有没有
        lstRes.clear();
        sprintf(sqlstr , "select name from t_user where name = '%s';",rq -> name);
        m_sql->SelectMysql(sqlstr,1,lstRes);
        if(lstRes.size() > 0){
            rs.result = name_is_exist;//有 返回结果
        }else{

            //没有 注册成功 更新数据库 写表
            sprintf(sqlstr,"insert into t_user(tel,password,name,icon,feeling) values('%s','%s','%s',2,'shi');",rq->tel,rq->password,rq->name);
            bool res = m_sql->UpdataMysql(sqlstr);
            if(!res){
                printf("updata fail : %s \n",sqlstr);
            }
        }
    }
    //返回结果
    SendData(clientfd , (char *)&rs,sizeof(rs));
}

//登录
void CLogic::LoginRq(sock_fd clientfd ,char* szbuf,int nlen)
{
    printf("clientfd:%d LoginRq\n", clientfd);
    //        拆包
    STRU_LOGIN_RQ * rq = (STRU_LOGIN_RQ *)szbuf;

    char strsql[1024] = "";
    sprintf( strsql , "select id,password from t_user where tel = '%s';",rq->tel);
    std::list<string> lstRes;
    bool res = m_sql->SelectMysql(strsql,2,lstRes);
    if( !res ) printf("select err : %s\n",strsql);
    //返回结果
    STRU_LOGIN_RS rs;
    //根据tel 查id password
    if( lstRes.size() > 0){
        //有 tel
        //密码是否一致
        int id = atoi(lstRes.front().c_str());
        lstRes.pop_front();
        string strPasswd = lstRes.front();
        lstRes.pop_front();
        if( strcmp( strPasswd.c_str(),rq->password) != 0){

            rs.result = password_error;
            //不一致 密码错误

        }else{
            //一致 成功的   //处理添加好友请求
            rs.result = login_success;
            rs.userid = id;
            UserInfo * user = NULL;
            //id->socket 为了方便以后转发通信 UserInfo
            if(m_mapFdToUserInfo.find( id , user )){
                //查到了
                //要不要强制上一个登录的人下线
                //user
                m_mapFdToUserInfo.erase( id );
                delete user;
            }
            user = new UserInfo;
            user->m_id = id;
            user->m_sockfd = clientfd;
            m_mapFdToUserInfo.insert(id , user);

            SendData( clientfd , (char*)&rs , sizeof rs );
            //同步这个人的状态信息 让登陆者知道自己的info  获取好友列表
            GetUserInfoAndSend(id);
            //获取好友列表
            getUserList(id);
            //获取群组id
//            getGroupInfor(id);
            return;
        }


    }else{
        //没有tel 查无此人
        rs.result = user_not_exist;
    }
    SendData( clientfd , (char*)&rs , sizeof rs );

    //有 查看密码是否一致
    //不一致 返回结果
    //一致
    //如果之前有用户信息 强制下线 回收
    //把id 和 套接字 捆绑在一起
    //保存用户信息
    //返回结果 id name result
    //离线信息
    //推送
}

//获取群组信息
void CLogic::getGroupInfor(int id){
    cout<<"查询获取群组id的id列表"<<endl;
    cout<<id<<endl;
    list<string>listRes;
    char sqlBuf[1024]="";
    sprintf(sqlBuf,"select g_id from t_MyGroup where myid =%d;",id);
    m_sql->SelectMysql(sqlBuf,1,listRes);
    int GroupId=0;
    while(listRes.size()>0)
    {
        GroupId=atoi(listRes.front().c_str());
        cout<<"g_id:"<<GroupId<<endl;
        listRes.pop_front();
        GetGroupInfoAndSend(GroupId);
    }
}
//获取小组信息
void CLogic::GetGroupInfoAndSend(int id){
    int g_id = id;
    int getid[8];//小组内的信息 组id 创建者id id1 id2 id3 id4 myid icon
    //获取创建者的id
    int creater_id;
    list<string> listRescreateid;
    char sqlbufcreateid[1024] = "";
    sprintf(sqlbufcreateid,"select creater_id from t_MyGroup where g_id = %d;",id);
    m_sql->SelectMysql(sqlbufcreateid,1,listRescreateid);
    while(listRescreateid.size()>0)
    {
        creater_id=atoi(listRescreateid.front().c_str());
        cout<<"creater_id:"<<creater_id<<endl;
        listRescreateid.pop_front();
    }
    //获取成员1id
    int id1;
    list<string> listResid1;
    char sqlbufid1[1024] = "";
    sprintf(sqlbufid1,"select id1 from t_MyGroup where g_id = %d;",id);
    m_sql->SelectMysql(sqlbufid1,1,listResid1);
    while(listResid1.size()>0)
    {
        id1=atoi(listResid1.front().c_str());
        cout<<"id1:"<<id1<<endl;
        listResid1.pop_front();
    }
    //获取成员2id
    int id2;
    list<string> listResid2;
    char sqlbufid2[1024] = "";
    sprintf(sqlbufid2,"select id2 from t_MyGroup where g_id = %d;",id);
    m_sql->SelectMysql(sqlbufid2,1,listResid2);
    while(listResid2.size()>0)
    {
        id2=atoi(listResid2.front().c_str());
        cout<<"id2:"<<id2<<endl;
        listResid2.pop_front();
    }
    //获取成员3id
    int id3;
    list<string> listResid3;
    char sqlbufid3[1024] = "";
    sprintf(sqlbufid3,"select id3 from t_MyGroup where g_id = %d;",id);
    m_sql->SelectMysql(sqlbufid3,1,listResid3);
    while(listResid3.size()>0)
    {
        id3=atoi(listResid3.front().c_str());
        cout<<"id3:"<<id3<<endl;
        listResid3.pop_front();
    }
    //获取成员4id
    int id4;
    list<string> listResid4;
    char sqlbufid4[1024] = "";
    sprintf(sqlbufid4,"select id3 from t_MyGroup where g_id = %d;",id);
    m_sql->SelectMysql(sqlbufid4,1,listResid4);
    while(listResid4.size()>0)
    {
        id4=atoi(listResid4.front().c_str());
        cout<<"id4:"<<id4<<endl;
        listResid4.pop_front();
    }
    //获取自己id
    int myid;
    char sqlbufmyid[1024] = "";
    list<string> listResmyid;
    sprintf(sqlbufmyid,"select myid from t_MyGroup where g_id = %d;",id);
    m_sql->SelectMysql(sqlbufmyid,1,listResmyid);
    while(listResmyid.size()>0)
    {
        myid=atoi(listResmyid.front().c_str());
        cout<<"myid:"<<myid<<endl;
        listResmyid.pop_front();
    }
    //获取群组头像
    int icon;
    char sqlbuficon[1024] = "";
    list<string> listResicon;
    sprintf(sqlbuficon,"select icon from t_MyGroup where g_id = %d;",id);
    m_sql->SelectMysql(sqlbuficon,1,listResicon);
    while(listResicon.size()>0)
    {
        icon=atoi(listResicon.front().c_str());
        cout<<"icon:"<<icon<<endl;
        listResicon.pop_front();
    }
    //获取组名
    cout<<"获取组名"<<endl;
    string myname;
    char sqlbufname[1024] = "";
    list<string> listResname;
    sprintf(sqlbufname,"select name from t_MyGroup where g_id = %d;",id);
    m_sql->SelectMysql(sqlbufname,1,listResname);
    while(listResname.size()>0)
    {
        myname=listResname.front();
        cout<<"myname:"<<myname<<endl;
        listResname.pop_front();
    }
    getid[0] = g_id;
    getid[1] = creater_id;
    getid[2] = id1;
    getid[3] = id2;
    getid[4] = id3;
    getid[5] = id4;
    getid[6] = myid;
    getid[7] = icon;

    STRU_SENDPICTURE_GROUP r;
    r.m_Gid = g_id;
    r.m_myid = myid;
    r.iconid = icon;
    strcpy(r.Name,myname.c_str());
    cout<<"z"<<r.Name<<endl;

    UserInfo *ToMySelf = nullptr;
    if( m_mapFdToUserInfo.find(myid , ToMySelf ))
    {
        cout<<"send"<<endl;
        SendData(/*这里应为取得的socket，不是clientfd*/ToMySelf->m_sockfd,(char*)&r,sizeof(r));
    }else{
        cout<<"不在线"<<endl;
    }
//    //创建群组
//    #define DEF_PACK_CREAT_GROUP (DEF_PACK_BASE + 62)
//    typedef struct STRU_SENDPICTURE_GROUP
//    {
//        STRU_SENDPICTURE_GROUP():m_type(DEF_PACK_CREAT_GROUP){}
//        PackType m_type;//数据包类型
//        int m_Gid;      //群组ID
//        int m_myid;     //创建者ID
//        char Name[MAX_SIZE]; //群组名

//    }STRU_SENDPICTURE_GROUP;

}

//加入分区
void CLogic::JoinZoneRq(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("clientfd:%d JoinZoneRq\n", clientfd);
    //拆包 更新个人信息
    STRU_JOIN_ZONE *rq = (STRU_JOIN_ZONE *) szbuf;
    UserInfo * info = nullptr;
    if(!m_mapFdToUserInfo.find(rq->userid , info)){
        return;
    }
    info->m_zoneid = rq->zoneid;
}
//离开分区
void CLogic::LeaveZoneRq(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("clientfd:%d LeaveZoneRq\n", clientfd);
    //拆包 更新个人信息
    STRU_LEAVE_ZONE *rq = (STRU_LEAVE_ZONE *) szbuf;
    UserInfo * info = nullptr;
    if(!m_mapFdToUserInfo.find(rq->userid , info)){
        return;
    }
    info->m_zoneid = 0;
}

//加入房间请求 加入时可能多个线程同时有客户端请求，房间列表应该加锁处理
void CLogic::JoinRoomRq(sock_fd clientfd, char *szbuf, int nlen)
{
    //首先 0 - 120 数组 看房间里面的人数
    //0 加入房间的就是房主 加入到房间列表 返回
    //1 玩家 返回加入成功 需要同步信息 玩家给加入的人发 加入的人给玩家发 加入时间列表
    //2 加入失败
    printf("clientfd:%d JoinRoomRq\n", clientfd);
    //拆包
    STRU_JOIN_ROOM_RQ *rq = (STRU_JOIN_ROOM_RQ*)szbuf;
    STRU_JOIN_ROOM_RS rs;
    list<int> tmplist;
    pthread_mutex_lock(& m_roomListMutex);
    list<int> & userlst = m_roomUserList[ rq->roomid];
    switch(userlst.size()){
    case 0:
        rs.result = 1;
        rs.roomid = rq->roomid;
        rs.status = _host;
        rs.userid = rq->userid;
        userlst.push_back( rq -> userid );
        break;
    case 1:
        rs.result = 1;
        rs.roomid = rq->roomid;
        rs.status = _player;
        rs.userid = rq->userid;
        userlst.push_back( rq -> userid );
        break;
    case 2:
        rs.result = 0;
        break;
    default:
        rs.result = 0;
        break;
    }
    tmplist = userlst;
    pthread_mutex_unlock(& m_roomListMutex);
    SendData(clientfd , (char *)&rs , sizeof(rs));
    //玩家给加入的人发 加入的人给玩家发 加入时间列表
    //size == 1 自己给自己发
    if( tmplist.size() > 0){
        int loginid = rq -> userid;
        //根据id拿到用户信息
        UserInfo * loginInfo = nullptr;
        if( !m_mapFdToUserInfo.find( loginid , loginInfo)) return ;
        //写成员信息请求
        STRU_ROOM_MEMBER loginrq;
        loginrq.userid = loginid;
        loginrq.status = rs.status;
        strcpy(loginrq.name,loginInfo->m_userName);

        bool flag = false;
        for(auto ite = tmplist.begin();ite != tmplist.end();++ite){
            int status = _player;
            if(!flag){
                status = _host;
                flag = true;
            }
            int roomMemid = *ite ;
            if( roomMemid != loginid){
                //根据id拿到用户信息
                UserInfo * MemInfo = nullptr;
                if( ! m_mapFdToUserInfo.find(roomMemid ,MemInfo)) continue;
                //写成员信息请求
                STRU_ROOM_MEMBER Memrq;
                Memrq.userid = roomMemid;
                Memrq.status = status;
                strcpy( Memrq.name , MemInfo->m_userName);
                //互相发送
                SendData( loginInfo->m_sockfd , (char*)&Memrq , sizeof(Memrq));
                SendData( MemInfo->m_sockfd , (char*)&loginrq , sizeof(loginrq));
            }else{
                //自己给自己发
                SendData( loginInfo->m_sockfd , (char*)&loginrq , sizeof(loginrq ));
            }

        }
    }
}
//离开房间
void CLogic::LeaveRoomRq(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("clientfd:%d JoinRoomRq\n", clientfd);

    //拆包
    STRU_LEAVE_ROOM_RQ * rq = (STRU_LEAVE_ROOM_RQ *) szbuf;
    //谁 什么身份 离开哪个房间
//    rq->roomid;
//    rq->status;
    int leaveid = rq->userid;
    //当前的这个人的离开要发给房间里的所有人的
    list<int> &lst = m_roomUserList[rq->roomid];
    //给除了这个离开的人之外的所有人转发离开信息
    for( auto ite = lst.begin();ite != lst.end();ite++){
        int memid = *ite;
        if( leaveid != memid){
            UserInfo * memInfo = nullptr;
            if( !m_mapFdToUserInfo.find(memid,memInfo)) continue;
            SendData( memInfo->m_sockfd ,szbuf ,nlen);
        }
    }
    pthread_mutex_lock( &m_roomListMutex );
    //根据身份不同 房主 player 操作list 房间信息
    if( rq -> status == _host){
        lst.clear();
    }else if(rq -> status == _player){

        //找到离开的人 清除掉
        for( auto ite = lst.begin();ite != lst.end();ite++){
            if( leaveid == *ite){
                ite = lst.erase(ite);
                break;
            }
        }
    }
    pthread_mutex_unlock( &m_roomListMutex );
}
//五子棋命令转发
void CLogic::FIL_MsgSendRq(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("clientfd:%d FIL_MsgSendRq\n", clientfd);
    //拆包
    STRU_FIL_RQ *rq = (STRU_FIL_RQ *) szbuf;
    list<int> & lstRes = m_roomUserList[rq -> roomid];
    for(auto ite = lstRes.begin();ite != lstRes.end();++ite){
        int userid = *ite;
//        MyMap<int , UserInfo*> m_mapFdToUserInfo;
//        vector<list<int>> m_roomUserList;
        UserInfo * info = nullptr;
        if( !m_mapFdToUserInfo.find(userid , info)) continue;
        SendData( info->m_sockfd,szbuf,nlen);
    }

}
    //什么专区 什么房间 谁 发了什么
    //根据专区 拿到房间列表 根据房间 拿到房间成元
    //转发给房间里的所有人
    //根据专区 拿到房间列表   //处理添加好友请求

//五子棋落子转发
void CLogic::FIL_PieceDownRq(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("clientfd:%d FIL_PieceDownRq\n", clientfd);
    //拆包
    STRU_FIL_PIECEDOWN *rq = (STRU_FIL_PIECEDOWN *) szbuf;
    list<int> & lstRes = m_roomUserList[rq -> roomid];
    for(auto ite = lstRes.begin();ite != lstRes.end();++ite){
        int userid = * ite;
        UserInfo * info = nullptr;
        if( !m_mapFdToUserInfo.find(userid , info)) continue;
        SendData( info->m_sockfd,szbuf,nlen);
    }
    //什么专区 什么房间 谁 发了什么
    //根据专区 拿到房间列表 根据房间 拿到房间成元
    //转发给房间里的所有人
    //根据专区 拿到房间列表
}

void CLogic::ZoneInfoRq(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("clientfd:%d ZoneInfoRq\n", clientfd);
    //拆包
    STRU_ZONE_INFO_RQ *rq = (STRU_ZONE_INFO_RQ*)szbuf;
    STRU_ZONE_ROOM_INFO rs;
    rs.zoneid = rq->zoneid;
    for(int i = 1 ; i < m_roomUserList.size();++i){
        list<int >& lst = m_roomUserList[i];
        rs.roomInfo[i] = lst.size();

    }
    SendData( clientfd,(char *) &rs ,sizeof(rs));
}
//-----------------------------------------------


void CLogic::UpdateUserInfoRq(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("CreateRoomRq:%d UserInfoRq\n", clientfd);
    cout<<""<<endl;
    //拆包
    STRU_USER_INFO_RQ *rq = (STRU_USER_INFO_RQ*)szbuf;
    //mysql修改信息
    char sqlstr[1024];
    sprintf( sqlstr , "update t_user set icon = %d , name = '%s' ,feeling = '%s' where id = %d; ",rq->m_iconid,rq->m_szUser,rq->m_userFeeling
             ,rq->m_UserID);
    m_sql->UpdataMysql( sqlstr );
    GetUserInfoAndSend( rq->m_UserID );
}

void CLogic::GetUserInfoAndSend(int id){
    //获取信息 from sql
    char strsql[1024] = "";
    sprintf(strsql,"select name , icon , feeling from t_user where id = %d;",id);
    list<string> lst;
    m_sql->SelectMysql( strsql , 3 , lst);
    if( lst.size() != 3) return;
    string name = lst.front();
    lst.pop_front();
    string iconid = lst.front();
    lst.pop_front();
    string feeling = lst.front();
    lst.pop_front();
    //设置信息 userinfo
    UserInfo *user = nullptr;
    if( !m_mapFdToUserInfo.find(id , user )){
        return;
    }
    strcpy( user->m_userName , name.c_str() );
    //发送给客户端state = 0;
    STRU_USER_INFO_RQ rq;
    rq.m_UserID = id;
    rq.m_iconid = atoi( iconid.c_str());
    strcpy( rq.m_szUser , name.c_str());
    strcpy( rq.m_userFeeling,feeling.c_str());
    cout<<"void CLogic::GetUserInfoAndSend"<<endl;
    SendData( user->m_sockfd , (char *)&rq , sizeof(rq));
    cout<<"协议头:"<<*(int*)&rq<<endl;
    //2023.1.14好友信息更改后 其好友UI界面 也应做刷新
    getUserList(id);
}

void CLogic::getUserList(int userid)
{
    ////3、查询好友的id列表
    cout<<"查询好友的id列表"<<endl;
    list<string>listRes;
    char sqlBuf[1024]="";
    sprintf(sqlBuf,"select idB from t_friend where idA ='%d';",userid);
    m_sql->SelectMysql( sqlBuf , 1 , listRes);
    //4、遍历id列表
    int friendId=0;
    while(listRes.size()>0)
    {
        //从查询结果中取出好友的id
        friendId=atoi(listRes.front().c_str());
        cout<<"好友id:"<<friendId<<endl;
        listRes.pop_front();
        //5、分别查询每个好友的信息
        GetFriendInfoAndSend(friendId,userid);
    }
}

void CLogic::GetFriendInfoAndSend(int friend_id,int user_id){
    //根据获得的好友Id 判断其是否在线
    //获取信息 from sql
    int m_state = 0;
    char strsql[1024] = "";
    sprintf(strsql,"select name , icon , feeling from t_user where id = %d;",friend_id);
    list<string> lst;
    m_sql->SelectMysql( strsql , 3 , lst);
    if( lst.size() != 3) return;
    string name = lst.front();
    lst.pop_front();
    string iconid = lst.front();
    lst.pop_front();
    string feeling = lst.front();
    lst.pop_front();
    UserInfo *user = nullptr;
    if( !m_mapFdToUserInfo.find(user_id , user ))
    {
        return;
    }
    STRU_USER_INFO_QQ rq;
    cout<<"获取好友列表协议头:"<<rq.m_nType<<endl;
    rq.m_UserID = friend_id;
    rq.state = m_state;
    rq.m_iconid = atoi( iconid.c_str());
    strcpy( rq.m_szUser , name.c_str());
    strcpy( rq.m_userFeeling,feeling.c_str());
    SendData( user->m_sockfd , (char *)&rq , sizeof(rq));
    UserInfo *ToMyfriend = nullptr;
    if( !m_mapFdToUserInfo.find(friend_id , ToMyfriend ))
    {
        return;
    }else{
        cout<<"如果发现该好友"<<endl;
        //发送给客户端state = 0;
        char strsql[1024] = "";
        sprintf(strsql,"select name , icon , feeling from t_user where id = %d;",user_id);
        list<string> lst;
        m_sql->SelectMysql( strsql , 3 , lst);
        if( lst.size() != 3) return;
        string name = lst.front();
        lst.pop_front();
        string iconid = lst.front();
        lst.pop_front();
        string feeling = lst.front();
        lst.pop_front();
        STRU_USER_INFO_QQ rqtofriend;
        cout<<"获取好友列表协议头:"<<rqtofriend.m_nType<<endl;
        rqtofriend.m_UserID = user_id;
        rqtofriend.state = 1;
        rqtofriend.m_iconid = atoi( iconid.c_str());
        strcpy( rqtofriend.m_szUser , name.c_str());
        strcpy( rqtofriend.m_userFeeling,feeling.c_str());
        SendData( ToMyfriend->m_sockfd , (char *)&rqtofriend , sizeof(rqtofriend));
        cout<<"协议头:"<<*(int*)&rq<<endl;
        {
            int m_state = 0;
            char strsql[1024] = "";
            sprintf(strsql,"select name , icon , feeling from t_user where id = %d;",friend_id);
            list<string> lst;
            m_sql->SelectMysql( strsql , 3 , lst);
            if( lst.size() != 3) return;
            string name = lst.front();
            lst.pop_front();
            string iconid = lst.front();
            lst.pop_front();
            string feeling = lst.front();
            lst.pop_front();
            UserInfo *user = nullptr;
            if( !m_mapFdToUserInfo.find(user_id , user ))
            {
                return;
            }
            STRU_USER_INFO_QQ rq;
            cout<<"获取好友列表协议头:"<<rq.m_nType<<endl;
            rq.m_UserID = friend_id;
            rq.state = 1;
            rq.m_iconid = atoi( iconid.c_str());
            strcpy( rq.m_szUser , name.c_str());
            strcpy( rq.m_userFeeling,feeling.c_str());
            SendData( user->m_sockfd , (char *)&rq , sizeof(rq));
        }
    }
}

void CLogic::DealChat(sock_fd clientfd, char *szbuf, int nlen)
{
    cout<<"CLogic::DealChat"<<endl;
    //拆包
    STRU_TCP_CHAT_RQ * r = (STRU_TCP_CHAT_RQ*) szbuf;
    //先判断好友是否在线 不在线就不用进行转发了
    int friend_id = r->friendId;
    cout<<friend_id<<endl;
    UserInfo *ToMyfriend = nullptr;
    if( m_mapFdToUserInfo.find(friend_id , ToMyfriend ))
    {
        cout<<"好友在线"<<endl;
        cout<<"step1"<<endl;
        STRU_TCP_CHAT_RS rs;
        rs.friendId = r->friendId;
        strcpy(rs.content , r->content);
        rs.type = DEF_PACK_RCHAT_INFO;
        rs.userId = r->userId;
        //转发
        SendData( ToMyfriend->m_sockfd , (char *)&rs , sizeof(rs));
    }else{
        cout<<"**********************************************************"<<endl;
        STRU_TCP_CHAT_RS rs;
        rs.friendId = r->friendId;
        strcpy(rs.content , "对方不在线");
        rs.type = DEF_PACK_RCHAT_INFO;
        rs.userId = r->userId;
        //转发
        cout<<clientfd<<endl;
        SendData( clientfd , (char *)&rs , sizeof(rs));
    }
}

//void CLogic::getfriendList(int userid)
//{
//    ////3、查询好友的id列表
//    cout<<"查询好友的id列表"<<endl;
//    list<string>listRes;
//    char sqlBuf[1024]="";
//    sprintf(sqlBuf,"select idB from t_friend where idA ='%d';",userid);
//    m_sql->SelectMysql( sqlBuf , 1 , listRes);
//    //4、遍历id列表
//    int friendId=0;
//    while(listRes.size()>0)
//    {
//        //从查询结果中取出好友的id
//        friendId=atoi(listRes.front().c_str());
//        cout<<"好友id:"<<friendId<<endl;
//        listRes.pop_front();
//        //5、分别查询每个好友的信息
//    }
//}
//处理下线请求
void CLogic::dealOfflineRq(sock_fd clientfd, char *szbuf, int nlen)
{
    cout<<"处理请求"<<endl;
    //1、拆包
    STRU_OFFLINE_RQ *rs=(STRU_OFFLINE_RQ *)szbuf;
    cout<<rs->type<<endl;
    cout<<rs->userId<<endl;
    //getfriendList(rs->userId);
    //2、从数据库中查询下线用户的所有好友
    list<string> listRes;
    char sqlBuf[1024]="";
    sprintf(sqlBuf,"select idB from t_friend where idA='%d';",rs->userId);
    int friend_id=0;
    m_sql->SelectMysql( sqlBuf , 1 , listRes);
    m_mapFdToUserInfo.erase(rs->userId);
    while(listRes.size()>0)
    {
        friend_id=atoi(listRes.front().c_str());
        cout<<"处理请求:"<<friend_id<<endl;
        listRes.pop_front();
        UserInfo *ToMyfriend = nullptr;
        if( m_mapFdToUserInfo.find(friend_id , ToMyfriend ))
        {
            getUserList(friend_id);
//            cout<<"好友在线 通知每个好友，该用户已下线"<<endl;
//            m_mapFdToUserInfo.erase(friend_id);
//            STRU_OFFLINE_RQ r;
//            r.userId = rs->userId;
//            cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!"<<r.userId<<endl;
//            //转发
//            UserInfo * user = NULL;
//            if(m_mapFdToUserInfo.find( rs->userId , user )){
//                cout<<"MyId:"<<rs->userId;
//                //查到了
//                //要不要强制上一个登录的人下线
//                //user
//                m_mapFdToUserInfo.erase( rs->userId );
//                delete user;
//            }
//            SendData( ToMyfriend->m_sockfd , (char *)&r , sizeof(r));
//        }else{
//            cout<<"no friend online"<<endl;
//            return;
        }

    }

}

//添加好友请求
void CLogic::dealAddFriendRq(sock_fd clientfd, char *szbuf, int nlen)
{
    cout<<"添加好友请求"<<endl;
    //1、拆包
    STRU_ADD_FRIEND_RQ* rq=(STRU_ADD_FRIEND_RQ*)szbuf;
    //3、根据用户账号从数据库中查询用户id
    list<string> listRes;
    char sqlBuf[1024]="";
    sprintf(sqlBuf,"select id from t_user where name ='%s';",rq->Friendtel);
    m_sql->SelectMysql( sqlBuf , 1 , listRes);
    STRU_ADD_FRIEND_RS rs;
    strcpy(rs.Friendtel ,rq->Friendtel);
    cout<<"step1"<<rs.Friendtel<<endl;
    //4、根据好友是否存在
    if(listRes.size()<=0)
    {
        //4.1、如果好友不存在，直接回复客户端添加好友失败，用户不存在
        rs.result=no_this_user;
        cout<<"用户不存在"<<endl;

    }
    else
    {
        //4.2、如果好友存在，取出好友id
        rs.friendId=atoi(listRes.front().c_str());
        listRes.pop_front();
        cout<<"step1"<<rs.Friendtel<<endl;
        rs.userId = rq->userId;
        //5、根据用户id判断用户是否在线
        UserInfo *ToMyfriend = nullptr;
        if( m_mapFdToUserInfo.find(rs.friendId , ToMyfriend ))
        {
            cout<<"step2"<<rs.Friendtel<<endl;
            //6、如果用户在线，转发添加好友请求
            cout<<ToMyfriend->m_sockfd<<endl;
            SendData(ToMyfriend->m_sockfd,szbuf,nlen);
            cout<<"转发给用户"<<endl;
            return ;
        }
        else
        {
            cout<<"用户不在线"<<rs.type<<endl;
            //7、如果用户不在线，直接回复客户端添加好友失败，用户不在线
            rs.result=friend_offline;
            cout<<"发包"<<rs.type<<endl;
            cout<<clientfd<<"在线"<<endl;
            SendData(clientfd,(char*)&rs,sizeof(rs));
        }
    }
}

//dealAddFriendRs添加好友请求的回复
void CLogic::dealAddFriendRs(sock_fd clientfd, char *szbuf, int nlen)
{
    cout<<"dealAddFriendRs添加好友请求的回复"<<endl;
    //1、拆包

    STRU_ADD_FRIEND_RS*rs=(STRU_ADD_FRIEND_RS*)szbuf;
    //2、判断添加好友结果
    cout<<"result:"<<rs->result<<endl;
    if(add_success==rs->result)
    {
        cout<<"进行插入"<<endl;

        //3、如果同意添加好友，把好友关系写入数据库----应该使用数据库事务保证两条语句都成功
        char sqlBuf[1024]="";
        sprintf(sqlBuf,"insert into t_friend values('%d','%d')",rs->friendId,rs->userId);
        bool res = m_sql->UpdataMysql(sqlBuf);
        if(!res){
            printf("updata fail : %s \n",sqlBuf);
        }

        sprintf(sqlBuf,"insert into t_friend values('%d','%d')",rs->userId,rs->friendId);
        bool res1 = m_sql->UpdataMysql(sqlBuf);
        if(!res1){
            printf("updata fail : %s \n",sqlBuf);
        }
        cout<<"**********************************************************************"<<rs->friendId<<endl;
        cout<<"**********************************************************************"<<rs->userId<<endl;;
        //获取好友列表
        getUserList(rs->userId);
        getUserList(rs->friendId);
    }else{
        rs->result=friend_refuse;

    }
    //4、不管是否不同意添加好友，都转发添加结果给客户端
    UserInfo *ToMyfriend = nullptr;
    m_mapFdToUserInfo.find(rs->friendId , ToMyfriend );
    cout<<"发包"<<rs->type<<endl;
    cout<<ToMyfriend->m_sockfd<<"在线"<<endl;
    cout<<"name:"<<rs->Friendtel<<endl;
    cout<<rs->result<<endl;
    STRU_ADD_FRIEND_RS r;
    r.friendId = rs->friendId;
    strcpy(r.Friendtel,rs->Friendtel);
    r.result = rs->result;
    r.type = rs->type;
    r.userId = rs->userId;
    SendData(ToMyfriend->m_sockfd,(char*)&r,sizeof(r));
}

void CLogic::dealDeleteFriendRs(sock_fd clientfd, char *szbuf, int nlen)
{
    //拆包
    STRU_DEL_FRIEND_RQ* r = (STRU_DEL_FRIEND_RQ*)szbuf;
    //不需要判断是否是自身好友 一定是自己的好友
    int Frienduserid = r->friendId;
    int Myuserid = r->userId;
    cout<<"Frienduserid"<<Frienduserid<<endl;
    cout<<"Myuserid"<<Myuserid<<endl;
    //先delete好友关系
    char sqlBuf[1024]="";
    sprintf(sqlBuf,"delete from t_friend where idA = '%d' and idB = '%d'",Myuserid,Frienduserid);
    bool res = m_sql->UpdataMysql(sqlBuf);
    if(!res){
        printf("updata fail : %s \n",sqlBuf);
    }
    sprintf(sqlBuf,"delete from t_friend where idA = '%d' and idB = '%d';",Frienduserid,Myuserid);
    m_sql->UpdataMysql(sqlBuf);
    bool res1 = m_sql->UpdataMysql(sqlBuf);
    if(!res){
        printf("updata fail : %s \n",sqlBuf);
    }
    STRU_DEL_FRIEND_RQ rs;
    rs.userId = Myuserid;
    rs.friendId = Frienduserid;
    //判断好友是否在线
    UserInfo *ToMyfriend = nullptr;
    if( m_mapFdToUserInfo.find(Frienduserid , ToMyfriend ))
    {
        //在线就发送好友信息更改
        cout<<"在线就发送好友信息更改"<<endl;

        cout<<"Frienduserid"<<Frienduserid<<endl;
        cout<<"Myuserid"<<Myuserid<<endl;
        getUserList(Frienduserid);
        getUserList(Myuserid);

        STRU_DEL_FRIEND_RQ toanother;
        toanother.friendId = Myuserid ;
        SendData(ToMyfriend->m_sockfd,(char*)&toanother,sizeof(toanother));
        SendData(clientfd,(char*)&rs,sizeof(rs));

    }
    else
    {
        cout<<"不在线就不发送给好友"<<endl;
        cout<<"Frienduserid"<<Frienduserid<<endl;
        cout<<"Myuserid"<<Myuserid<<endl;
        getUserList(Myuserid);
        //不在线就不发送给好友
        SendData(clientfd,(char*)&rs,sizeof(rs));
    }

}
//处理好友音频聊天请求
void CLogic::dealFriendVideoandAudioRs(sock_fd clientfd, char *szbuf, int nlen)
{
    cout<<"处理好友音频聊天请求"<<endl;
    STRU_DEL_FRIENDVID_RQ* r = (STRU_DEL_FRIENDVID_RQ *)szbuf;
    STRU_DEL_IFFRIENDVID_RQ rs;
    cout<<"idddddd:"<<r->userId;
    strcpy(rs.Friendname,r->UserName);
    strcpy(rs.UserName,r->Friendname);

    //根据tel 查看数据库  看有没有
    list<string> lstRes;
    char sqlstr[1000] = "";
    sprintf(sqlstr,"select id from t_user where name = '%s';",rs.UserName);
    m_sql->SelectMysql(sqlstr,1,lstRes);
    int Frienduserid = atoi(lstRes.front().c_str());
    lstRes.pop_front();
    cout<<"Frienduserid:"<<Frienduserid<<endl;
    //数据库里一定能找到 该好友
    //判断好友是否在线
    UserInfo *ToMyfriend = nullptr;
    if( m_mapFdToUserInfo.find(Frienduserid , ToMyfriend ))
    {
        cout<<"在线就发送给好友音频聊天请求"<<endl;
        //在线就发送给好友音频聊天请求
        cout<<"name"<<ToMyfriend->m_userName<<endl;
        cout<<ToMyfriend->m_userName<<"的id 6 yedachui:"<<ToMyfriend->m_id<<endl;
        cout<<ToMyfriend->m_sockfd<<"7/6:"<<endl;
        cout<<ToMyfriend->m_id<<"m_id"<<endl;
        rs.userId = r->userId;
        SendData(ToMyfriend->m_sockfd,(char*)&rs,sizeof(rs));

    }else{
        rs.result = 2;
//        SendData(r->m,(char*)&rs,sizeof(rs));
        cout<<"好友不在线 无法进行音频聊天"<<endl;
        //好友不在线 返回告诉音频聊天发起人该好友不在线
        //todo不在线提醒
    }

}

void CLogic::dealFriendVideoandAudio(sock_fd clientfd, char *szbuf, int nlen)
{
    cout<<"处理对方对音频聊天的同意与否"<<endl;
    STRU_DEL_IFFRIENDVIDVORF_RQ * r = (STRU_DEL_IFFRIENDVIDVORF_RQ*)szbuf;
    STRU_DEL_IFFRIENDVIDVORF_RQ rs;
    strcpy(rs.Friendname,r->Friendname);
    strcpy(rs.UserName,r->UserName);
    rs.userId = r->userId;
    rs.result = r->result;

    if(rs.result == 1){
        //同意就开始调用函数转发序列化数据
        //一个新的序列化协议
        list<string> lstRes;
        char sqlstr[1000] = "";
        sprintf(sqlstr,"select id from t_user where name = '%s';",rs.Friendname);
        cout<<"rs.Username="<<rs.UserName<<endl;
        m_sql->SelectMysql(sqlstr,1,lstRes);
        int Frienduserid = atoi(lstRes.front().c_str());
        cout<<"Frienduerid="<<Frienduserid<<endl;
        lstRes.pop_front();
        UserInfo *ToMyfriend = nullptr;
        if( m_mapFdToUserInfo.find(Frienduserid , ToMyfriend ))
        {
            rs.userId = ToMyfriend->m_id;
            SendData(ToMyfriend->m_sockfd,(char*)&rs,sizeof(rs));
        }
    }else{
        //拒绝返回 提示告诉他对方拒绝
        list<string> lstRes;
        char sqlstr[1000] = "";
        cout<<"拒绝返回 提示告诉他对方拒绝"<<endl;
        sprintf(sqlstr,"select id from t_user where name = '%s';",rs.Friendname);
        cout<<"rs.Username="<<rs.UserName<<endl;
        m_sql->SelectMysql(sqlstr,1,lstRes);
        int Frienduserid = atoi(lstRes.front().c_str());
        cout<<"Frienduerid="<<Frienduserid<<endl;
        lstRes.pop_front();
        UserInfo *ToMyfriend = nullptr;
        if( m_mapFdToUserInfo.find(Frienduserid , ToMyfriend ))
        {
            cout<<"下个包发给谁:"<<rs.Friendname<<endl;
            cout<<"id  "<<rs.userId<<endl;
            cout<<"iddd"<<ToMyfriend->m_id<<endl;
            rs.userId = ToMyfriend->m_id;
            SendData(ToMyfriend->m_sockfd,(char*)&rs,sizeof(rs));
        }
    }
}
//===================================================================================================
void CLogic::AudioFrame(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("CreateRoomRq:%d AudioFrame\n", clientfd);
    //拆包
    char * tmp = szbuf;
    //type
    //userid
    //roomid
    tmp+=sizeof(int);
    int userid = *(int *)tmp;//按照四个字节读
    tmp+=sizeof(int);
    int friendid = *(int*)tmp;

    UserInfo *ToMyfriend = nullptr;
    if( m_mapFdToUserInfo.find(friendid , ToMyfriend ))
    {
        SendData( ToMyfriend->m_sockfd , szbuf , nlen );
    }
}

void CLogic::VideoFrame(sock_fd clientfd, char *szbuf, int nlen)
{
    printf("CreateRoomRq:%d VideoFrame\n", clientfd);
    //拆包
    char * tmp = szbuf;
    //type
    //userid
    //roomid
    tmp+=sizeof(int);
    int userid = *(int *)tmp;//按照四个字节读
    tmp+=sizeof(int); 
    int friendid = *(int*)tmp;
    UserInfo *ToMyfriend = nullptr;
    if( m_mapFdToUserInfo.find(friendid , ToMyfriend ))
    {
        cout<<"好友在线"<<endl;
        cout<<"Myfriend:"<<ToMyfriend->m_userName<<endl;
        SendData( ToMyfriend->m_sockfd , szbuf , nlen );
    }
}

void CLogic::quitconversationrame(sock_fd clientfd, char *szbuf, int nlen)
{
    cout<<"X的信号呢"<<endl;
    STRU_DEL_QUIT_CONVERSATION* r = (STRU_DEL_QUIT_CONVERSATION *)szbuf;
    UserInfo *ToMyfriend = nullptr;
    if( m_mapFdToUserInfo.find(r->friendID , ToMyfriend ))
    {
        cout<<"好友在线"<<endl;
        cout<<"SendToName:"<<ToMyfriend->m_userName<<endl;
        SendData( ToMyfriend->m_sockfd , szbuf , nlen );
    }

}
//处理转发请求
void CLogic::slot_dealSendPictureRq(sock_fd clientfd, char *szbuf, int nlen)
{

    cout<<DEF_PACK_SENDPICTURE_RQ<<endl;
    char * tmp = szbuf;
    //type
    //userid
    //roomid
    tmp+=sizeof(int);
    int userid = *(int *)tmp;//按照四个字节读
    tmp+=sizeof(int);
    int friendid = *(int*)tmp;

    UserInfo *ToMyfriend = nullptr;
    if( m_mapFdToUserInfo.find(friendid , ToMyfriend ))
    {
        cout<<"好友在线"<<endl;
        cout<<"Myfriend:"<<ToMyfriend->m_userName<<endl;
        SendData( ToMyfriend->m_sockfd , szbuf , nlen );
    }
}
//处理转发文件回复
void CLogic::slot_dealSendPictureRs(sock_fd clientfd, char *szbuf, int nlen)
{
    //拆包
    STRU_SENDPICTURE_RS *rq = (STRU_SENDPICTURE_RS *)szbuf;
    //TODO:根据用户数据包中id取得对应socket
    int friend_id = rq->m_fid;
    UserInfo *ToMyfriend = nullptr;
    if( m_mapFdToUserInfo.find(friend_id , ToMyfriend ))
    {
         cout<<"在线"<<endl;
        SendData(/*这里应为取得的socket，不是clientfd*/ToMyfriend->m_sockfd,szbuf,nlen);
    }else{
        cout<<"不在线"<<endl;
    }
}
//转发图片路径
void CLogic::slot_dealSendPicturepathname(sock_fd clientfd, char *szbuf, int nlen)
{
    cout<<"转发图片路径"<<endl;
    STRU_SENDPICTURE_NAME* r = (STRU_SENDPICTURE_NAME *)szbuf;
    int friend_id = r->m_fid;
    UserInfo *ToMyfriend = nullptr;
    if( m_mapFdToUserInfo.find(friend_id , ToMyfriend ))
    {
        cout<<"在线"<<endl;
        cout<<ToMyfriend->m_userName<<endl;
        SendData(/*这里应为取得的socket，不是clientfd*/ToMyfriend->m_sockfd,szbuf,nlen);
    }else{
        cout<<"不在线"<<endl;

    }

}
//创建群组
void CLogic::slot_dealCreateGroup(sock_fd clientfd, char *szbuf, int nlen)
{
    cout<<"创建群组"<<endl;

    STRU_SENDPICTURE_GROUP * r =  (STRU_SENDPICTURE_GROUP * )szbuf;
    cout<<r->Name<<endl;
    cout<<r->m_myid<<endl;
    int m_id = r->m_myid;
    char sqlstr[1000] = "";
    sprintf(sqlstr,"insert into t_MyGroup(name ,creater_id,id1,id2,id3,id4,myid,icon)values('%s',%d,0,0,0,0,%d,1);",r->Name,r->m_myid,r->m_myid);
    bool res = m_sql->UpdataMysql(sqlstr);
    if(!res){
        printf("updata fail : %s \n",sqlstr);
    }

}
//更改群组信息
void CLogic::slot_ChangeGroupInfo(sock_fd clientfd, char *szbuf, int nlen)
{
    cout<<"更改群组信息"<<endl;
}

void CLogic::slot_dealSendFile(sock_fd clientfd, char *szbuf, int nlen)
{
    //拆包
    cout<<"slot_dealSendFile"<<endl;
    STRU_SEND_FILE *rq = (STRU_SEND_FILE *)szbuf;
    //TODO:根据用户数据包中id取得对应socket
    int friend_id = rq->m_fid;
    UserInfo *ToMyfriend = nullptr;
    if( m_mapFdToUserInfo.find(friend_id , ToMyfriend ))
    {
         cout<<"在线"<<endl;
        SendData(/*这里应为取得的socket，不是clientfd*/ToMyfriend->m_sockfd,szbuf,nlen);
    }else{
        cout<<"不在线"<<endl;
    }
}

void CLogic::slot_dealrecvFile(sock_fd clientfd, char *szbuf, int nlen)
{
    cout<<"slot_dealrecvFile"<<endl;
    char * tmp = szbuf;
    tmp+=2*sizeof(int);
    int friendid = *(int*)tmp;

    UserInfo *ToMyfriend = nullptr;
    if( m_mapFdToUserInfo.find(friendid , ToMyfriend ))
    {
        cout<<"好友在线"<<endl;
        cout<<"Myfriend:"<<ToMyfriend->m_userName<<endl;
        SendData( ToMyfriend->m_sockfd , szbuf , nlen );
    }
}




