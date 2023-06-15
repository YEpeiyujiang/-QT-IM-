#ifndef _PACKDEF_H
#define _PACKDEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include "err_str.h"
#include <malloc.h>

#include<iostream>
#include<map>
#include<list>


//边界值
#define _DEF_SIZE 45
#define _DEF_BUFFERSIZE 4096
#define _DEF_PORT 8000
#define _DEF_SERVERIP "0.0.0.0"  //TODO

/*-------------数据库信息-----------------*/
#define _DEF_DB_NAME    "WECHAT"
#define _DEF_DB_IP      "localhost"
#define _DEF_DB_USER    "root"
#define _DEF_DB_PWD     "12345"
/*--------------------------------------*/

#define TRUE true
#define FALSE false


#define _DEF_LISTEN 128
#define _DEF_EPOLLSIZE 4096
#define _DEF_IPSIZE 16
#define _DEF_COUNT 10
#define _DEF_TIMEOUT 10
#define _DEF_SQLIEN 400



#define BOOL bool
#define DEF_PACK_BASE  (10000)
#define DEF_PACK_COUNT  (100)
//注册
#define  DEF_PACK_REGISTER_RQ    (DEF_PACK_BASE + 0)
#define  DEF_PACK_REGISTER_RS    (DEF_PACK_BASE + 1)
//登录
#define  DEF_PACK_LOGIN_RQ    (DEF_PACK_BASE + 2)
#define  DEF_PACK_LOGIN_RS    (DEF_PACK_BASE + 3)
//创建房间
#define DEF_PACK_CREATEROOM_RQ  (DEF_PACK_BASE + 4)
#define DEF_PACK_CREATEROOM_RS  (DEF_PACK_BASE + 5)
//加入房间
#define DEF_PACK_JOINROOM_RQ  (DEF_PACK_BASE + 6)
#define DEF_PACK_JOINROOM_RS  (DEF_PACK_BASE + 7)
//房间列表请求
#define DEF_PACK_ROOM_MEMBER    (DEF_PACK_BASE + 8)
//音频数据
#define DEF_PACK_AUDIO_FRAME    (DEF_PACK_BASE + 9)
//视频数据
#define DEF_PACK_VIDEO_FRAME    (DEF_PACK_BASE + 10)
//退出房间请求
#define DEF_PACK_LEAVEROOM_RQ   (DEF_PACK_BASE + 11)
//退出房间回复
#define DEF_PACK_LEAVEROOM_RS   (DEF_PACK_BASE + 12)
//音频注册
#define DEF_PACK_AUDIO_REGISTER (DEF_PACK_BASE + 13)
//视频注册
#define DEF_PACK_VIDEO_REGISTER (DEF_PACK_BASE + 14)

//注册请求结果
#define tel_is_exist		(0)
#define register_success	(1)
#define name_is_exist       (2)

//登录请求结果
#define user_not_exist		(0)
#define password_error		(1)
#define login_success		(2)
//#define user_online          3

//创建房间结果
#define room_is_exist        0
#define create_success       1

//加入房间结果
#define room_no_exist        0
#define join_success         1


#define MAX_PATH            (260 )
#define MAX_SIZE            (40  )
#define DEF_HOBBY_COUNT     (8  )
#define MAX_CONTENT_LEN     (4096 )


/////////////////////网络//////////////////////////////////////


#define DEF_MAX_BUF	  1024
#define DEF_BUFF_SIZE	  4096


typedef int PackType;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//协议结构
//注册
typedef struct STRU_REGISTER_RQ
{
    STRU_REGISTER_RQ():type(DEF_PACK_REGISTER_RQ)
    {
        memset( tel  , 0, sizeof(tel));
        memset( name  , 0, sizeof(name));
        memset( password , 0, sizeof(password) );
    }
    //需要手机号码 , 密码, 昵称
    PackType type;
    char tel[MAX_SIZE];
    char name[MAX_SIZE];
    char password[MAX_SIZE];

}STRU_REGISTER_RQ;

typedef struct STRU_REGISTER_RS
{
    //回复结果
    STRU_REGISTER_RS(): type(DEF_PACK_REGISTER_RS) , result(register_success)
    {
    }
    PackType type;
    int result;

}STRU_REGISTER_RS;

//登录
typedef struct STRU_LOGIN_RQ
{
    //登录需要: 手机号 密码
    STRU_LOGIN_RQ():type(DEF_PACK_LOGIN_RQ)
    {
        memset( tel , 0, sizeof(tel) );
        memset( password , 0, sizeof(password) );
    }
    PackType type;
    char tel[MAX_SIZE];
    char password[MAX_SIZE];

}STRU_LOGIN_RQ;

typedef struct STRU_LOGIN_RS
{
    // 需要 结果 , 用户的id
    STRU_LOGIN_RS(): type(DEF_PACK_LOGIN_RS) , result(login_success),userid(0)
    {
        memset(name , 0 , sizeof(name));
    }
    PackType type;
    int result;
    int userid;
    char name[MAX_SIZE];


}STRU_LOGIN_RS;


//创建房间请求
typedef struct STRU_CREATEROOM_RQ
{
    STRU_CREATEROOM_RQ()
    {
        m_nType = DEF_PACK_CREATEROOM_RQ;
        m_UserID = 0;
    }

    PackType m_nType;   //包类型
    int m_UserID;

}STRU_CREATEROOM_RQ;

//创建房间回复
typedef struct STRU_CREATEROOM_RS
{
    STRU_CREATEROOM_RS()
    {
        m_nType= DEF_PACK_CREATEROOM_RS;
        m_lResult = 0;
        m_RoomId = 0;
    }
    PackType m_nType;   //包类型
    int  m_lResult ;    //创建结果
    int  m_RoomId;

}STRU_CREATEROOM_RS;

//加入房间请求
typedef struct STRU_JOINROOM_RQ
{
    STRU_JOINROOM_RQ()
    {
        m_nType = DEF_PACK_JOINROOM_RQ;
        m_UserID = 0;
        m_RoomID = 0;
    }

    PackType m_nType;   //包类型
    int m_UserID;
    int m_RoomID;

}STRU_JOINROOM_RQ;

//加入房间回复
typedef struct STRU_JOINROOM_RS
{
    STRU_JOINROOM_RS()
    {
        m_nType= DEF_PACK_JOINROOM_RS;
        m_lResult = 0;
        m_RoomID = 0;
    }
    PackType m_nType;   //包类型
    int  m_lResult ;    //注册结果
    int m_RoomID;
}STRU_JOINROOM_RS;

//房间成员请求
typedef struct STRU_ROOM_MEMBER_RQ
{
    STRU_ROOM_MEMBER_RQ()
    {
        m_nType= DEF_PACK_ROOM_MEMBER;
        m_UserID =0;
        memset(m_szUser,0,MAX_SIZE);
    }
    PackType m_nType;   //包类型
    int m_UserID;
    char m_szUser[MAX_SIZE];

}STRU_ROOM_MEMBER_RQ;


//离开房间请求
typedef struct STRU_LEAVEROOM_RQ
{
    STRU_LEAVEROOM_RQ()
    {
        m_nType = DEF_PACK_LEAVEROOM_RQ;
        m_nUserId = 0;
        m_RoomId = 0;
        memset(szUserName,0,MAX_SIZE);
    }
    PackType   m_nType;   //包类型
    int    m_nUserId; //用户ID
    int    m_RoomId;
    char   szUserName[MAX_SIZE];
}STRU_LEAVEROOM_RQ;


// id -> userInfo
typedef struct UserInfo
{
    UserInfo()
    {
         m_sockfd = 0;
         m_id = 0;
         m_roomid = 0;
         memset(m_userName, 0 , MAX_SIZE);
    }
    int m_sockfd;
    int  m_id;
    int  m_roomid;//房间id
    int m_zoneid;//游戏区id ，比如五子棋100 斗地主101
    char m_userName[MAX_SIZE];
}UserInfo;

//注册音频
struct STRU_AUDIO_REGISTER
{
    STRU_AUDIO_REGISTER():m_nType(DEF_PACK_AUDIO_REGISTER){
        m_userid = 0;
    }

    PackType m_nType;   //包类型
    int m_userid;
};

//注册视频
struct STRU_VIDEO_REGISTER
{
    STRU_VIDEO_REGISTER():m_nType(DEF_PACK_VIDEO_REGISTER){
        m_userid = 0;
    }

    PackType m_nType;   //包类型
    int m_userid;
};


//  游戏相关内容    /////////

#define  DEF_PACK_JOIN_ZONE   (DEF_PACK_BASE + 20)
#define  DEF_PACK_LEAVE_ZONE  (DEF_PACK_BASE + 21)
enum ENUM_PLAY_ZONE{ fIVE_iN_lINE = 0X10 , E_L_S , D_D_Z};

//加入专区
struct STRU_JOIN_ZONE//解决这是什么包 。 DEF_PACK_BASE
{
    STRU_JOIN_ZONE():type(DEF_PACK_JOIN_ZONE),userid(0),zoneid(0){

    }
    PackType type;
    int userid;
    int zoneid;

};
//退出专区
struct STRU_LEAVE_ZONE//解决这是什么包，谁退出专区
{
    STRU_LEAVE_ZONE():type(DEF_PACK_LEAVE_ZONE),userid(0){

    }
    PackType type;
    int userid;
};

#define DEF_JOIN_ROOM_RQ   (DEF_PACK_BASE + 22)

//加入房间
struct STRU_JOIN_ROOM_RQ//解决这是什么包，谁加入哪个房间
{
    STRU_JOIN_ROOM_RQ():type(DEF_JOIN_ROOM_RQ),userid(0),roomid(0){}
    PackType type;
    int userid;
    int roomid;
};//发给服务器，服务器会同步房间成员信息

//房间 为了避免0出现歧义（房间号是0 或者未初始化） 所以0让出来， 1 - 120 121个元素
#define DEF_JOIN_ROOM_RS  (DEF_PACK_BASE + 23)
enum ENUM_ROOM_STATUS{_host , _player , _watcher};//房主 玩家 观战者
//加入房间
struct STRU_JOIN_ROOM_RS//解决这是什么包，谁加入哪个房间  是否成功 自己什么身份
{
    STRU_JOIN_ROOM_RS():type(DEF_JOIN_ROOM_RS),userid(0),roomid(0),status(_host),result(1){

    }
    PackType type;
    int userid;
    int roomid;
    int status;
    int result;//0 - fail 1 - success
};

#define DEF_ROOM_MEMBER (DEF_PACK_BASE + 24)
//房间成员
struct STRU_ROOM_MEMBER//解决这是什么包 ，  谁 ，哪个房间 ，叫什么名字
{
    STRU_ROOM_MEMBER():type(DEF_ROOM_MEMBER),userid(0),status(_player){
        memset( name , 0 , sizeof(name));
    }
    PackType type;
    int userid;
    //房间内的身份
    int status;
    char name[MAX_SIZE];
};

#define DEF_LIAVE_ROOM_RQ (DEF_PACK_BASE + 25)
//退出房间
struct STRU_LEAVE_ROOM_RQ//解决这是什么包 谁 退出了房间
{
    STRU_LEAVE_ROOM_RQ():type(DEF_LIAVE_ROOM_RQ),userid(0),status(_player),roomid(0){

    }
    PackType type;
    int userid;
    int roomid;
    int status;
};//会被转发，如果自己不是房主，房主退出，自己也会跟着退出

//专区内每个房间人数
#define DEF_ZONE_ROOM_INFO (DEF_PACK_BASE + 26)
#define DEF_ZOOM_ROOM_COUNT 121
#define DEF_ZONE_INFO_RQ   (DEF_PACK_BASE + 38)
struct STRU_ZONE_ROOM_INFO//解决这是什么包 谁退出专区
{
    STRU_ZONE_ROOM_INFO():type(DEF_ZONE_ROOM_INFO){
        memset(roomInfo , 0 , sizeof(roomInfo));
    }
    PackType type;
    int zoneid;
    int roomInfo[DEF_ZOOM_ROOM_COUNT];
};

struct STRU_ZONE_INFO_RQ{
    //解决这是什么包 请求专区内每个房间人数
    STRU_ZONE_INFO_RQ():type(DEF_ZONE_INFO_RQ),zoneid(0){
    }
    PackType type;
    int zoneid;

};
//五子棋相关
#define DEF_FIL_ROOM_READY          (DEF_PACK_BASE + 30)
#define DEF_FIL_GAME_START          (DEF_PACK_BASE + 31)
#define DEF_FIL_AI_BEGIN            (DEF_PACK_BASE + 32)
#define DEF_FIL_AI_END              (DEF_PACK_BASE + 33)
#define DEF_FIL_DISCARD_THIS        (DEF_PACK_BASE + 34)
#define DEF_FIL_SURREND             (DEF_PACK_BASE + 35)
#define DEF_FIL_PIECEDOWN           (DEF_PACK_BASE + 36)
#define DEF_FIL_WIN                 (DEF_PACK_BASE + 37)

//游戏的准备
//准备 开始 胜利 托管 弃权(当前一次) 投降  落子（谁在什么位置下了一个什么子）
struct STRU_FIL_RQ
{
    STRU_FIL_RQ(PackType _type):type(_type),userid(0),zoneid(0),roomid(0){

    }

    PackType type;//准备 开始 胜利 托管 弃权（当前一次） 投降 复用
    int userid;
    int zoneid;
    int roomid;

};//只有知道 什么专区 什么房间 才能找到相应的人

//采取另一种方式 利用客户端知道 准备的个数 以及开放开始

struct STRU_FIL_RS //当所有人都准备好 需要开始 、、都准备才能开始 两种方式 根据客户端判断
{
    PackType type;//allready 房主就可以开始了
    int userid;
    int zoneid;
    int roomid;

};

//落子
struct STRU_FIL_PIECEDOWN//什么专区的什么房间 谁在x,y位置放一个什么子
{
    STRU_FIL_PIECEDOWN():type( DEF_FIL_PIECEDOWN),userid(0),zoneid(0),roomid(0),color(0),x(-1),y(-1){
    }
    PackType type;//落子
    int userid;
    int zoneid;
    int roomid;
    int color;
    int x ;
    int y;
};

//----------------------------------------------------------
#define DEF_PACK_USER_INFO_RQ  (DEF_PACK_BASE + 45)
typedef struct STRU_USER_INFO_RQ
{
    STRU_USER_INFO_RQ()
    {
        m_nType= DEF_PACK_USER_INFO_RQ;
        m_UserID = 0;//设置 服务器写表  需要知道是谁
        m_iconid = 0;
        memset(m_szUser,0,sizeof(m_szUser));
        memset(m_userFeeling,0,sizeof(m_userFeeling));
    }
    PackType m_nType;   //包类型
    int m_UserID;
    int m_iconid;
    char m_szUser[MAX_SIZE];
    char m_userFeeling[MAX_SIZE];
}STRU_USER_INFO_RQ;

#define DEF_PACK_USER_INFO (DEF_PACK_BASE + 46)
typedef struct STRU_USER_INFO_QQ
{
    STRU_USER_INFO_QQ()
    {
        m_nType= DEF_PACK_USER_INFO;
        m_UserID = 0;//设置 服务器写表  需要知道是谁
        m_iconid = 0;
        state = 1;
        memset(m_szUser,0,sizeof(m_szUser));
        memset(m_userFeeling,0,sizeof(m_userFeeling));
    }
    PackType m_nType;   //包类型
    int m_UserID;
    int m_iconid;
    char m_szUser[MAX_SIZE];
    char m_userFeeling[MAX_SIZE];
    int state;
}STRU_USER_INFO_QQ;

//转发信息
//聊天
#define DEF_PACK_CHAT_INFO (DEF_PACK_BASE + 47)
typedef struct STRU_TCP_CHAT_RQ
{
    STRU_TCP_CHAT_RQ():type(DEF_PACK_CHAT_INFO),userId(0),friendId(0)
    {
        memset(content,0,_DEF_BUFFERSIZE);
    }
    //协议头、自己的ID、好友的ID、聊天内容
    PackType type;
    int userId;
    int friendId;
    char content[_DEF_BUFFERSIZE];
}STRU_TCP_CHAT_RQ;
//接收聊天
#define DEF_PACK_RCHAT_INFO (DEF_PACK_BASE + 48)
typedef struct STRU_TCP_CHAT_RS
{
    STRU_TCP_CHAT_RS():type(DEF_PACK_RCHAT_INFO),userId(0),friendId(0)
    {
        memset(content,0,_DEF_BUFFERSIZE);
    }
    //协议头、自己的ID、好友的ID、聊天内容
    PackType type;
    int userId;
    int friendId;
    char content[_DEF_BUFFERSIZE];
}STRU_TCP_CHAT_RS;

//下线
#define DEF_PROTOCOL_TCP_OFFLINE_RQ (DEF_PACK_BASE + 49)
typedef struct STRU_OFFLINE_RQ
{
    STRU_OFFLINE_RQ():type(DEF_PROTOCOL_TCP_OFFLINE_RQ),userId(0)
    {}
    //协议头、自己的ID
    PackType type;
    int userId;
}STRU_OFFLINE_RQ;

//添加好友请求
//添加好友结果
#define no_this_user		(0)
#define friend_refuse	    (1)
#define friend_offline		(2)
#define add_success			(3)
#define DEF_PROTOCOL_ADD_RQ (DEF_PACK_BASE + 50)
typedef struct STRU_ADD_FRIEND_RQ
{
    STRU_ADD_FRIEND_RQ():type(DEF_PROTOCOL_ADD_RQ),userId(0)
    {
        memset(Usertel,0,MAX_SIZE);
        memset(Friendtel,0,MAX_SIZE);
    }
    //协议头、直接的id、自己的昵称、对方的昵称
    PackType type;
    int userId;
    char Usertel[MAX_SIZE];
    char Friendtel[MAX_SIZE];
}STRU_ADD_FRIEND_RQ;
//添加好友请求回复
#define DEF_PROTOCOL_ADD_RS (DEF_PACK_BASE + 51)
typedef struct STRU_ADD_FRIEND_RS
{
    STRU_ADD_FRIEND_RS():type(DEF_PROTOCOL_ADD_RS),friendId(0),result(add_success)
    {
        memset(Friendtel,0,MAX_SIZE);
    }
    //协议头、好友的id、好友的昵称、添加的结果
    PackType type;
    int userId;
    int friendId;
    int result;
    char Friendtel[MAX_SIZE];
}STRU_ADD_FRIEND_RS;

//自定义删除好友协议
#define DEF_PROTOCOL_DELETE  (DEF_PACK_BASE + 52)
typedef struct STRU_DEL_FRIEND_RQ
{
    STRU_DEL_FRIEND_RQ():type(DEF_PROTOCOL_DELETE),friendId(0),userId(0)
    {
    }
    PackType type;
    int userId;//自己的ID
    int friendId;//好友的ID
}STRU_DEL_FRIEND_RQ;

//是否同意好友视频音频聊天的协议
#define DEF_PROTOCOL_VIDEOANDAUDIOANDDESK  (DEF_PACK_BASE + 53)
typedef struct STRU_DEL_FRIENDVID_RQ
{
    STRU_DEL_FRIENDVID_RQ():type(DEF_PROTOCOL_VIDEOANDAUDIOANDDESK),userId(0)
    {
        memset(UserName,0,MAX_SIZE);
        memset(Friendname,0,MAX_SIZE);
    }
    //协议头、直接的id、自己的账号、对方的账号
    PackType type;
    int userId;
    char UserName[MAX_SIZE];
    char Friendname[MAX_SIZE];
}STRU_DEL_FRIENDVID_RQ;
//好友是否接受音频聊天
#define DEF_PROTOCOL_IFVIDEOANDAUDIOANDDESK  (DEF_PACK_BASE + 54)
typedef struct STRU_DEL_IFFRIENDVID_RQ
{
    STRU_DEL_IFFRIENDVID_RQ():type(DEF_PROTOCOL_IFVIDEOANDAUDIOANDDESK),userId(0),result(0)
    {
        memset(UserName,0,MAX_SIZE);
        memset(Friendname,0,MAX_SIZE);
    }
    //协议头、直接的id、自己的账号、对方的账号
    PackType type;
    int userId;
    char UserName[MAX_SIZE];
    char Friendname[MAX_SIZE];
    int result;//0是拒绝 1是同意 2是对方不在线
}STRU_DEL_IFFRIENDVID_RQ;

//好友是否接受音频聊天
#define DEF_PROTOCOL_IFVORFVIDEOANDAUDIOANDDESK  (DEF_PACK_BASE + 55)
typedef struct STRU_DEL_IFFRIENDVIDVORF_RQ
{
    STRU_DEL_IFFRIENDVIDVORF_RQ():type(DEF_PROTOCOL_IFVORFVIDEOANDAUDIOANDDESK),userId(0),result(0)
    {
        memset(UserName,0,MAX_SIZE);
        memset(Friendname,0,MAX_SIZE);
    }
    //协议头、直接的id、自己的账号、对方的账号
    PackType type;
    int userId;
    char UserName[MAX_SIZE];
    char Friendname[MAX_SIZE];
    int result;//0是拒绝 1是同意
}STRU_DEL_IFFRIENDVIDVORF_RQ;
//挂断电话
#define DEF_QUIT_CONVERSATION   (DEF_PACK_BASE + 58)
typedef struct STRU_DEL_QUIT_CONVERSATION
{
    STRU_DEL_QUIT_CONVERSATION():type(DEF_QUIT_CONVERSATION),userId(0),friendID(0)
    {
    }
    //协议头、直接的id、自己的账号、对方的账号
    PackType type;
    int userId;
    int friendID;
}STRU_DEL_QUIT_CONVERSATION;
//音频数据
#define DEF_PACK_AUDIO_FRAME    (DEF_PACK_BASE + 56)
//视频数据
#define DEF_PACK_VIDEO_FRAME    (DEF_PACK_BASE + 57)

#define DEF_PACK_SENDPICTURE_RQ (DEF_PACK_BASE + 59)
//typedef struct STRU_SENDPICTURE_RQ
//{
//    STRU_SENDPICTURE_RQ():m_type(DEF_PACK_SENDPICTURE_RQ),m_filesize(0),m_sendsize(0)
//    {
//        memset(m_filename,0,sizeof(m_filename));
//        memset(m_data,0,sizeof(m_data));
//    }
//    PackType m_type;   //数据包类型
//    int m_uid;         //用户id
//    int m_fid;         //好友id
//    int m_filesize;    //文件大小
//    int m_sendsize;    //已发送文件大小
//    char m_filename[100];//文件名
//    char m_data[1000];   //分片数据
//}STRU_SENDPICTURE_RQ;

#define DEF_PACK_SENDPICTURE_RS (DEF_PACK_BASE + 60)
typedef struct STRU_SENDPICTURE_RS
{
    STRU_SENDPICTURE_RS():m_type(DEF_PACK_SENDPICTURE_RS),m_result(false){}
    PackType m_type;//数据包类型
    int m_fid;      //好友id
    bool m_result;  //传输结果
}STRU_SENDPICTURE_RS;


#define DEF_PACK_SENDPICTURE_NAME (DEF_PACK_BASE + 61)
typedef struct STRU_SENDPICTURE_NAME
{
    STRU_SENDPICTURE_NAME():m_type(DEF_PACK_SENDPICTURE_NAME){}
    PackType m_type;//数据包类型
    int m_fid;      //好友id
    int m_myid;
    char Name[MAX_SIZE]; //图片名字

}STRU_SENDPICTURE_NAME;

//创建群组
#define DEF_PACK_CREAT_GROUP (DEF_PACK_BASE + 62)
typedef struct STRU_SENDPICTURE_GROUP
{
    STRU_SENDPICTURE_GROUP():m_type(DEF_PACK_CREAT_GROUP){}
    PackType m_type;//数据包类型
    int m_Gid;      //群组ID
    int m_myid;     //创建者ID
    int iconid;
    char Name[MAX_SIZE]; //群组名

}STRU_SENDPICTURE_GROUP;

//加入群组
#define DEF_PACK_JOIN_GROUP (DEF_PACK_BASE + 63)
typedef struct STRU_SENDPIJOIN_GROUP
{
    STRU_SENDPIJOIN_GROUP():m_type(DEF_PACK_JOIN_GROUP){}
    PackType m_type;//数据包类型
    int m_Gid;      //群组ID
    int m_myid;

}STRU_SENDPIJOIN_GROUP;

//退出群组
#define DEF_PACK_QUIT_GROUP (DEF_PACK_BASE + 64)
typedef struct STRU_SENDPIQUIT_GROUP
{
    STRU_SENDPIQUIT_GROUP():m_type(DEF_PACK_QUIT_GROUP){}
    PackType m_type;//数据包类型
    int m_Gid;      //群组ID
    int m_myid;

}STRU_SENDPIQUIT_GROUP;

//群组信息更改
#define DEF_PACK_QUIT_GROUP_CHANGE (DEF_PACK_BASE + 65)
typedef struct STRU_SENDPIQUIT_GROUP_CHANGE
{
    STRU_SENDPIQUIT_GROUP_CHANGE():m_type(DEF_PACK_QUIT_GROUP_CHANGE){}
    PackType m_type;//数据包类型
    int iconid;      //群组ID
    int g_id;     //创建者ID
    char Name[MAX_SIZE]; //群组名

}STRU_SENDPIQUIT_GROUP_CHANGE;

//文件传输确认
#define DEF_PACK_SEND_FILE_RQ (DEF_PACK_BASE + 66)
#define DEF_PACK_SEND_FILE_RS (DEF_PACK_BASE + 67)
#define DEF_PACK_RECV_FILE (DEF_PACK_BASE + 68)
typedef struct STRU_SEND_FILE
{
    STRU_SEND_FILE():m_type(DEF_PACK_SEND_FILE_RQ),m_fid(0),m_myid(0){}
    PackType m_type;//数据包类型,默认发送文件请求
    char filename[100];
    int m_fid;      //好友id
    int m_myid;
}STRU_SEND_FILE;


#endif


