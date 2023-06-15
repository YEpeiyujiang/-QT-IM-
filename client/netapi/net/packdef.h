#pragma once


#define _DEF_BUFFER  ( 4096 )
#define _DEF_CONTENT_SIZE	(4096)
#define _MAX_SIZE	(40)

//自定义协议   先写协议头 再写协议结构
//登录 注册 获取好友信息 添加好友 聊天 发文件 下线请求
#define _DEF_PROTOCOL_BASE	(10000)
#define _DEF_PROTOCOL_COUNT (100)

//注册
#define _DEF_PACK_REGISTER_RQ	(_DEF_PROTOCOL_BASE + 0 )
#define _DEF_PACK_REGISTER_RS	(_DEF_PROTOCOL_BASE + 1 )
//登录
#define _DEF_PACK_LOGIN_RQ	(_DEF_PROTOCOL_BASE + 2 )
#define _DEF_PACK_LOGIN_RS	(_DEF_PROTOCOL_BASE + 3 )
//好友信息
#define _DEF_PACK_FRIEND_INFO	(_DEF_PROTOCOL_BASE + 4 )
//添加好友
#define _DEF_PACK_ADDFRIEND_RQ	(_DEF_PROTOCOL_BASE + 5 )
#define _DEF_PACK_ADDFRIEND_RS	(_DEF_PROTOCOL_BASE + 6 )
//聊天
#define _DEF_PACK_CHAT_RQ	(_DEF_PROTOCOL_BASE + 7 )
#define _DEF_PACK_CHAT_RS	(_DEF_PROTOCOL_BASE + 8 )
//离线
#define _DEF_PACK_OFFLINE_RQ	(_DEF_PROTOCOL_BASE + 9 )

//返回的结果
//注册请求的结果   //注册的时候带着昵称
#define tel_is_exist		(0)
#define register_success	(1)
#define name_is_exist       (2)
//登录请求的结果
#define user_not_exist		(0)
#define password_error		(1)
#define login_success		(2)


typedef int PackType;

//协议结构
//注册
typedef struct STRU_REGISTER_RQ
{
	STRU_REGISTER_RQ():type(_DEF_PACK_REGISTER_RQ)
	{
		memset( tel  , 0, sizeof(tel));
		memset( name  , 0, sizeof(name));
		memset( password , 0, sizeof(password) );
	}
	//需要手机号码 , 密码, 昵称
	PackType type;
	char tel[_MAX_SIZE];
	char name[_MAX_SIZE];
	char password[_MAX_SIZE];

}STRU_REGISTER_RQ;

typedef struct STRU_REGISTER_RS
{
	//回复结果
	STRU_REGISTER_RS(): type(_DEF_PACK_REGISTER_RS) , result(register_success)
	{
	}
	PackType type;
	int result;

}STRU_REGISTER_RS;

//登录
typedef struct STRU_LOGIN_RQ
{
	//登录需要: 手机号 密码 
	STRU_LOGIN_RQ():type(_DEF_PACK_LOGIN_RQ)
	{
		memset( tel , 0, sizeof(tel) );
		memset( password , 0, sizeof(password) );
	}
	PackType type;
	char tel[_MAX_SIZE];
	char password[_MAX_SIZE];

}STRU_LOGIN_RQ;

typedef struct STRU_LOGIN_RS
{
	// 需要 结果 , 用户的id
	STRU_LOGIN_RS(): type(_DEF_PACK_LOGIN_RS) , result(login_success),userid(0)
	{
        memset(name , 0 , sizeof(name));
	}
	PackType type;
	int result;
	int userid;
    char name[_MAX_SIZE];


}STRU_LOGIN_RS;

//好友信息
typedef struct STRU_FRIEND_INFO
{
	STRU_FRIEND_INFO():type(_DEF_PACK_FRIEND_INFO) , userid(0) , iconid(0) , state(0) 
	{
		memset( name ,  0 , sizeof(name) );
		memset( feeling , 0 , sizeof(feeling));
	}
	//需要 用户id 头像id 昵称 签名 状态--是否在线
	PackType type;
	int userid;
	int iconid;
	int state;
	char name[_MAX_SIZE];
	char feeling[_MAX_SIZE];

}STRU_FRIEND_INFO;


//聊天
typedef struct STRU_CHAT_RQ
{
	STRU_CHAT_RQ():type(_DEF_PACK_CHAT_RQ) , userid(0) , friendid(0)
	{
		memset( content , 0 , _DEF_CONTENT_SIZE );
	}
	// 谁发给谁 服务器转发  用户1 id 用户2 id 发的内容
	PackType type;
	int userid;
	int friendid;
	char content[_DEF_CONTENT_SIZE];

}STRU_CHAT_RQ;

typedef struct STRU_CHAT_RS
{
	STRU_CHAT_RS():type(_DEF_PACK_CHAT_RS) , userid(0) , friendid(0),result(0)
	{
	}
	PackType type;
	int userid;
	int friendid; //方便找是哪个人不在线
	int result;

}STRU_CHAT_RS;

//发给服务器说明离线
typedef struct STRU_OFFLINE
{
    STRU_OFFLINE():type(_DEF_PACK_OFFLINE_RQ) , userid(0)
    {
    }
    PackType type;
    int userid;

}STRU_OFFLINE;

//五子棋相关
#define DEF_FIL_ROOM_READY          (_DEF_PROTOCOL_BASE + 30)
#define DEF_FIL_GAME_START          (_DEF_PROTOCOL_BASE + 31)
#define DEF_FIL_AI_BEGIN            (_DEF_PROTOCOL_BASE + 32)
#define DEF_FIL_AI_END              (_DEF_PROTOCOL_BASE + 33)
#define DEF_FIL_DISCARD_THIS        (_DEF_PROTOCOL_BASE + 34)
#define DEF_FIL_SURREND             (_DEF_PROTOCOL_BASE + 35)
#define DEF_FIL_PIECEDOWN           (_DEF_PROTOCOL_BASE + 36)
#define DEF_FIL_WIN                 (_DEF_PROTOCOL_BASE + 37)

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

//////////  游戏相关内容    /////////
#define  DEF_PACK_JOIN_ZONE   (_DEF_PROTOCOL_BASE + 20)
#define  DEF_PACK_LEAVE_ZONE  (_DEF_PROTOCOL_BASE + 21)
enum ENUM_PLAY_ZONE{ fIVE_iN_lINE = 0X10 , E_L_S , D_D_Z};

//加入专区
struct STRU_JOIN_ZONE//解决这是什么包 。 谁加入哪个专区
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

//专区内每个房间人数
#define DEF_ZONE_ROOM_INFO (_DEF_PROTOCOL_BASE + 26)
#define DEF_ZONE_INFO_RQ   (_DEF_PROTOCOL_BASE + 38)
#define DEF_ZOOM_ROOM_COUNT 121
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

#define DEF_JOIN_ROOM_RQ   (_DEF_PROTOCOL_BASE + 22)

//加入房间
struct STRU_JOIN_ROOM_RQ//解决这是什么包，谁加入哪个房间
{
    STRU_JOIN_ROOM_RQ():type(DEF_JOIN_ROOM_RQ),userid(0),roomid(0){}
    PackType type;
    int userid;
    int roomid;
};//发给服务器，服务器会同步房间成员信息

//房间 为了避免0出现歧义（房间号是0 或者未初始化） 所以0让出来， 1 - 120 121个元素
#define DEF_JOIN_ROOM_RS  (_DEF_PROTOCOL_BASE + 23)
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

#define DEF_ROOM_MEMBER (_DEF_PROTOCOL_BASE + 24)
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
    char name[_MAX_SIZE];
};

#define  DEF_LIAVE_ROOM_RQ (_DEF_PROTOCOL_BASE + 25)
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

//----------------------------------------------------------------------------------
//用户信息请求:登录之后的状态同步 设置用户信息
#define DEF_PACK_USER_INFO_RQ  (_DEF_PROTOCOL_BASE + 45)
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
    char m_szUser[_MAX_SIZE];
    char m_userFeeling[_MAX_SIZE];

}STRU_USER_INFO_RQ;
//设置用户状态信息
#define DEF_PACK_USER_INFO (_DEF_PROTOCOL_BASE + 46)
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
    char m_szUser[_MAX_SIZE];
    char m_userFeeling[_MAX_SIZE];
    int state;

}STRU_USER_INFO_QQ;

//转发信息
//聊天
#define DEF_PACK_CHAT_INFO (_DEF_PROTOCOL_BASE + 47)
typedef struct STRU_TCP_CHAT_RQ
{
    STRU_TCP_CHAT_RQ():type(DEF_PACK_CHAT_INFO),userId(0),friendId(0)
    {
        memset(content,0,_DEF_CONTENT_SIZE);
    }
    //协议头、自己的ID、好友的ID、聊天内容
    PackType type;
    int userId;
    int friendId;
    char content[_DEF_CONTENT_SIZE];
}STRU_TCP_CHAT_RQ;
//接收聊天
#define DEF_PACK_RCHAT_INFO (_DEF_PROTOCOL_BASE + 48)
typedef struct STRU_TCP_CHAT_RS
{
    STRU_TCP_CHAT_RS():type(DEF_PACK_RCHAT_INFO),userId(0),friendId(0)
    {
        memset(content,0,_DEF_CONTENT_SIZE);
    }
    //协议头、自己的ID、好友的ID、聊天内容
    PackType type;
    int userId;
    int friendId;
    char content[_DEF_CONTENT_SIZE];
}STRU_TCP_CHAT_RS;


//下线
#define DEF_PROTOCOL_TCP_OFFLINE_RQ (_DEF_PROTOCOL_BASE + 49)
typedef struct STRU_OFFLINE_RQ
{
    STRU_OFFLINE_RQ():type(DEF_PROTOCOL_TCP_OFFLINE_RQ),userId(0)
    {}
    //协议头、自己的ID
    PackType type;
    int userId;
}STRU_OFFLINE_RQ;

//添加好友
//添加好友的结果
#define no_this_user		(0)
#define user_refuse			(1)
#define user_offline		(2)
#define add_success			(3)
#define DEF_PROTOCOL_ADD_RQ (_DEF_PROTOCOL_BASE + 50)
typedef struct STRU_ADD_FRIEND_RQ
{
    STRU_ADD_FRIEND_RQ():type(DEF_PROTOCOL_ADD_RQ),userId(0)
    {
        memset(Usertel,0,_MAX_SIZE);
        memset(Friendtel,0,_MAX_SIZE);
    }
    //协议头、直接的id、自己的账号、对方的账号
    PackType type;
    int userId;
    char Usertel[_MAX_SIZE];
    char Friendtel[_MAX_SIZE];
}STRU_ADD_FRIEND_RQ;

#define DEF_PROTOCOL_ADD_RS (_DEF_PROTOCOL_BASE + 51)
typedef struct STRU_ADD_FRIEND_RS
{
    STRU_ADD_FRIEND_RS():type(DEF_PROTOCOL_ADD_RS),friendId(0),result(add_success)
    {
        memset(friendName,0,_MAX_SIZE);
    }
    //协议头、好友的id、好友的昵称、添加的结果
    PackType type;
    int userId;
    int friendId;
    int result;
    char friendName[_MAX_SIZE];
}STRU_ADD_FRIEND_RS;
//自定义删除好友协议
#define DEF_PROTOCOL_DELETE  (_DEF_PROTOCOL_BASE + 52)
typedef struct STRU_DEL_FRIEND_RQ
{
    STRU_DEL_FRIEND_RQ():type(DEF_PROTOCOL_DELETE),friendId(0),userId(0)
    {
    }
    PackType type;
    int userId;//自己的ID
    int friendId;//好友的ID
}STRU_DEL_FRIEND_RQ;
//删除好友的协议

//是否同意好友视频音频聊天的协议
#define DEF_PROTOCOL_VIDEOANDAUDIOANDDESK  (_DEF_PROTOCOL_BASE + 53)
typedef struct STRU_DEL_FRIENDVID_RQ
{
    STRU_DEL_FRIENDVID_RQ():type(DEF_PROTOCOL_VIDEOANDAUDIOANDDESK),userId(0)
    {
        memset(UserName,0,_MAX_SIZE);
        memset(Friendname,0,_MAX_SIZE);
    }
    //协议头、直接的id、自己的账号、对方的账号
    PackType type;
    int userId;
    char UserName[_MAX_SIZE];
    char Friendname[_MAX_SIZE];
}STRU_DEL_FRIENDVID_RQ;
//好友是否接受音频聊天
#define DEF_PROTOCOL_IFVIDEOANDAUDIOANDDESK  (_DEF_PROTOCOL_BASE + 54)
typedef struct STRU_DEL_IFFRIENDVID_RQ
{
    STRU_DEL_IFFRIENDVID_RQ():type(DEF_PROTOCOL_IFVIDEOANDAUDIOANDDESK),userId(0),result(0)
    {
        memset(UserName,0,_MAX_SIZE);
        memset(Friendname,0,_MAX_SIZE);
    }
    //协议头、直接的id、自己的账号、对方的账号
    PackType type;
    int userId;
    char UserName[_MAX_SIZE];
    char Friendname[_MAX_SIZE];
    int result;//0是拒绝 1是同意
}STRU_DEL_IFFRIENDVID_RQ;


//好友是否接受音频聊天的处理
#define DEF_PROTOCOL_IFVORFVIDEOANDAUDIOANDDESK  (_DEF_PROTOCOL_BASE + 55)
typedef struct STRU_DEL_IFFRIENDVIDVORF_RQ
{
    STRU_DEL_IFFRIENDVIDVORF_RQ():type(DEF_PROTOCOL_IFVORFVIDEOANDAUDIOANDDESK),userId(0),result(0)
    {
        memset(UserName,0,_MAX_SIZE);
        memset(Friendname,0,_MAX_SIZE);
    }
    //协议头、直接的id、自己的账号、对方的账号
    PackType type;
    int userId;
    char UserName[_MAX_SIZE];
    char Friendname[_MAX_SIZE];
    int result;//0是拒绝 1是同意
}STRU_DEL_IFFRIENDVIDVORF_RQ;

//音频数据
#define DEF_PACK_AUDIO_FRAME    (_DEF_PROTOCOL_BASE + 56)
//视频数据
#define DEF_PACK_VIDEO_FRAME    (_DEF_PROTOCOL_BASE + 57)
//挂断电话
#define DEF_QUIT_CONVERSATION   (_DEF_PROTOCOL_BASE + 58)
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

#define DEF_PACK_SENDPICTURE_RQ    (_DEF_PROTOCOL_BASE + 59)
////客户端和网盘均要修改内容 在packdef.h中
//#define DEF_PACK_SENDPICTURE_RQ (_DEF_PROTOCOL_BASE + 59)
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

#define DEF_PACK_SENDPICTURE_RS (_DEF_PROTOCOL_BASE + 60)
typedef struct STRU_SENDPICTURE_RS
{
    STRU_SENDPICTURE_RS():m_type(DEF_PACK_SENDPICTURE_RS),m_result(false){}
    PackType m_type;//数据包类型
    int m_fid;      //好友id
    bool m_result;  //传输结果
}STRU_SENDPICTURE_RS;

//发送图片的名字
#define DEF_PACK_SENDPICTURE_NAME (_DEF_PROTOCOL_BASE + 61)
typedef struct STRU_SENDPICTURE_NAME
{
    STRU_SENDPICTURE_NAME():m_type(DEF_PACK_SENDPICTURE_NAME){}
    PackType m_type;//数据包类型
    int m_fid;      //好友id
    int m_myid;
    char Name[_MAX_SIZE]; //图片名字

}STRU_SENDPICTURE_NAME;

//群组业务说明  一个群组最大在支持五个用户  一个群主  加群和退出群无需群主同意

//创建群组和群组信息初始化处理
#define DEF_PACK_CREAT_GROUP (_DEF_PROTOCOL_BASE + 62)
typedef struct STRU_SENDPICTURE_GROUP
{
    STRU_SENDPICTURE_GROUP():m_type(DEF_PACK_CREAT_GROUP){}
    PackType m_type;//数据包类型
    int m_Gid;      //群组ID
    int m_myid;     //创建者ID
    int iconid;
    char Name[_MAX_SIZE]; //群组名

}STRU_SENDPICTURE_GROUP;

//加入群组
#define DEF_PACK_JOIN_GROUP (_DEF_PROTOCOL_BASE + 63)
typedef struct STRU_SENDPIJOIN_GROUP
{
    STRU_SENDPIJOIN_GROUP():m_type(DEF_PACK_JOIN_GROUP){}
    PackType m_type;//数据包类型
    int m_Gid;      //群组ID
    int m_myid;     //创建者ID

}STRU_SENDPIJOIN_GROUP;

//退出群组
#define DEF_PACK_QUIT_GROUP (_DEF_PROTOCOL_BASE + 64)
typedef struct STRU_SENDPIQUIT_GROUP
{
    STRU_SENDPIQUIT_GROUP():m_type(DEF_PACK_QUIT_GROUP){}
    PackType m_type;//数据包类型
    int m_Gid;      //群组ID
    int m_myid;     //创建者ID


}STRU_SENDPIQUIT_GROUP;

//群组信息更改
#define DEF_PACK_QUIT_GROUP_CHANGE (_DEF_PROTOCOL_BASE + 65)
typedef struct STRU_SENDPIQUIT_GROUP_CHANGE
{
    STRU_SENDPIQUIT_GROUP_CHANGE():m_type(DEF_PACK_QUIT_GROUP_CHANGE){}
    PackType m_type;//数据包类型
    int iconid;      //群组ID
    int g_id;     //创建者ID
    char Name[_MAX_SIZE]; //群组名

}STRU_SENDPIQUIT_GROUP_CHANGE;

//文件传输确认
#define DEF_PACK_SEND_FILE_RQ (_DEF_PROTOCOL_BASE + 66)
#define DEF_PACK_SEND_FILE_RS (_DEF_PROTOCOL_BASE + 67)
#define DEF_PACK_RECV_FILE (_DEF_PROTOCOL_BASE + 68)
typedef struct STRU_SEND_FILE
{
    STRU_SEND_FILE():m_type(DEF_PACK_SEND_FILE_RQ),m_fid(0),m_myid(0){}
    PackType m_type;//数据包类型,默认发送文件请求
    char filename[100];
    int m_fid;      //好友id
    int m_myid;
}STRU_SEND_FILE;


// 数据库
/*
	表1 用户表 id->唯一标识 tel  password  name  iconid   feeling 也可以写成两张表 登录和用户信息
	列:  id  tel password  name  iconid  feeling

	表2 好友  idA  idB
	列 idA  idB  
	操作: 好友关系是双向关系  关注是单向关系  , 添加好友时 , 需要写入两行 即 A->B B->A 方便查询
	(添加好友 查找一般比插入多)

//	表3 离线消息
	
	用户数据表t_user
	create table t_user ( id bigint unsigned  AUTO_INCREMENT primary key, tel varchar (260) , 
	password varchar(260)  , name varchar (260) , icon int , feeling varchar(260));

	可以添加测试数据 
	insert into t_user ( tel,password , name , icon , feeling  ) 
	values ('12345','test', '12345' ,1,'比较懒,什么也没写')

	好友表 t_friend
	create table t_friend ( idA bigint unsigned  , idB bigint unsigned);
*/








