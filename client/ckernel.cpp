#include "ckernel.h"
#include <QPropertyAnimation>
//客户端修改
#define STRDATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"))
//*****************************************************************************
/*
 * 获得 MD5函数
 * 协议映射关系
 * 资源回收
 * 协议包接收处理
 * 账号登录结果处理
 * 账号注册结果处理
 * 加入房间回复处理
 * 房间成员信息处理
 * 处理成员离开房间
 * 专区房间信息处理
 * 加入房间请求处理
 * 成员准备处理
 * 房主开局处理
 * 处理对方落子位置显示
 * 账号登录处理
 * 账号注册处理
 * 提交加入分区处理
 * 提交加入房间处理
 * 离开专区
 * 离开房间
 * 本身五子棋准备
 * 五子棋开局
 * 落子信息封装处理
 * 胜负信息处理
 * 电脑对战开始
 * 电脑落子信息处理
 * 开始调用电脑对战
 * 电脑对战结束
 * 房间内成员信息处理
 * 处理用户登录成功后其信息显示
 * 发送数据函数再次包装
 * 构造函数
 * 更改个人信息
 * 游戏大厅界面的显示
 * 好友信息及其列表显示处理
 * 设置用户信息
 * 处理用户信息的更新  UI界面做显示
 * 设置配置文件
 * UI聊天窗口显示
 * 接收好友信息转发处理
 * 发送给好友的信息转发处理
 * 处理接收到的好友信息
 * 处理好友的下线请求
 * 处理自身账号的下线
 * 处理添加好友
 * 处理添加好友后续
 * 接收到好友添加请求的处理
 * 处理删除好友的操作
 * 处理被删除好友的操作
 * 处理音视频窗口以及接口调用相关
 * 处理接收到的音频聊天请求及相关接口的调用
 * 处理对方音视频对己方音视频聊天请求的回复
 * 音视频相关函数开关
 * 刷新画面函数 主要使用在采集 和 采集别人的画面
 * 鼠标点击事件 预览图大图显示函数
 * 处理接收到的音视频数据流
 * 处理接收到的音频数据流
 * 发送视频流数据
 * 发送音频流数据
*/
//*****************************************************************************

/*BUG修复2023.01.14
 * 好友信息更改后 对方UI界面未刷新
 * 修复方法 服务器再次调用获取好友列表函数进行发包更新处理
 *
 * UI优化之删除按钮、昵称框、签名框
 * 修复方法 修改UI
 *
 * 加好友后 发起者未提示加好友情况失败与否
 * 修复方法 服务器发包错误
 *
 * 好友信息更改后聊天窗口对方昵称未能刷新更新
 * 修复方法 客户端获取服务器所发的更新好友列表包后 调m_mapIdToChatdlg取出对应的ChatDialog重新设置信息
 *
 * QQui退出后ChatDialog不与之一起关闭
 * 修复方法 销毁函数遍历对应的ChatDialog先hide再delete
 *
 * 打通话时 一旦一方退出 再次发起 接收的一方会以原先通话窗口为基础增加新的UI小窗口
 * 修复方法 设计一个新的协议在slot_quit函数发送出去给服务器，服务器进行转发到接收者客户端那边进行hide处理 并且进行QMessageBox提醒
 *
 * 和一个用户音频通话的时候 还能点开与其他用户的音频通话
 * 修复方法 加一个全局标志位 使其他chatdialog的音频通话按钮失能 以及服务器相关转发yes或no 的请求的补充
 *
 * 再次登录上一次在线的好友而这次=登录不在线的好友 依然是在线状态
 * 修复方法 说明服务器部分并未做移除 服务器下线移除erase
 *
 * m_friendid会被改变 导致其他人给正在音频聊天的其中一个人发信息就会导致m_friendid会改变 因此会导致音频聊天被中断
 * 修复方法 这是当初留下的坑 只要将m_friendid在自动弹出好友信息处的获值删掉即可  完全修复还要进行对方忙线的处理 否则一旦第三者也发起语音通话申请也会导致m_friendid改变
 *
 * 第三者的下线信息居然没有通知给其好友  经查证 发现是处理下线请求的时候  在线的map中移除的居然是好友而不是自己
 * 解决方法 直接调用getUserList刷线
 *
 * 麦克风问题 在音频聊天过程中 一旦点开第三者的chatdialog  那么麦克风将会失效
 *
*/

/*
 * 2023.01.15项目优化
 * 多线程优化 由于发送数据量过大 send函数滑动窗口变小 导致阻塞 卡顿
 * 优化：可以考虑使用send函数 放到工作者线程里面，让工作者线程卡顿 避免主线程卡顿
 */

/*
 * 2023.02.05项目更新
 * 添加截图发图功能
 */

/*
 * 2023.02.09项目更新
 * 收发图和表情功能完善
 */
/*
 *2023.02.26
 *添加群组（未完成）
 */
//1_1234
#define MD5_KEY 1234
static std::string getMD5(QString val){
    QString tmp = QString("%1_%2").arg(val).arg(MD5_KEY);
    MD5 md( tmp.toStdString() );//QString转化为string MD5参数为string类型
    return md.toString();
}
//宏定义简化 映射偏移
#define NetPackMap(a) m_netPackFunMap[(a) - _DEF_PROTOCOL_BASE]
//协议映射关系
void CKernel::setNetPackMap()
{
    qDebug()<<__func__;
    NetPackMap( _DEF_PACK_LOGIN_RS)      = &CKernel::slot_dealLoginRs;
    NetPackMap( _DEF_PACK_REGISTER_RS)   = &CKernel::slot_resgisterRs;
    NetPackMap(DEF_JOIN_ROOM_RS)         = &CKernel::slot_dealJoinRoomRS;
    NetPackMap(DEF_ROOM_MEMBER)          = &CKernel::slot_dealRoomMembRq;
    NetPackMap(DEF_LIAVE_ROOM_RQ)        = &CKernel::slot_dealLeaveRoomRq;

    NetPackMap(DEF_ZONE_ROOM_INFO)       = &CKernel::slot_dealZoneRoomInfo;

   NetPackMap(DEF_FIL_ROOM_READY      )  = &CKernel::slot_dealFilGameReadyRq;
   NetPackMap(DEF_FIL_GAME_START      )  = &CKernel::slot_dealFilGameStartRq;
   NetPackMap(DEF_FIL_AI_BEGIN        )  = &CKernel::slot_dealFilPlayByCpuBegin;
   NetPackMap(DEF_FIL_AI_END          )  = &CKernel::slot_dealFilPlayByCpuEnd;
//   NetPackMap(DEF_FIL_DISCARD_THIS    )  = &CKernel::slot_dealFil
//   NetPackMap(DEF_FIL_SURREND         )  = &CKernel::slot_dealFil
   NetPackMap(DEF_FIL_PIECEDOWN       )  = &CKernel::slot_dealFilPieceDownRq;
   NetPackMap(DEF_FIL_WIN             )  = &CKernel::slot_dealFilWinRq;
   //=====================================================================================
   NetPackMap(DEF_PACK_USER_INFO_RQ)           = &CKernel::slot_dealUserInfoRs;
   //获取好友列表
   NetPackMap(DEF_PACK_USER_INFO)           = &CKernel::slot_dealFriendInfoRs;
   //处理聊天相关
   NetPackMap(DEF_PACK_CHAT_INFO)           = &CKernel::DealChatRq;
   //接收信息的处理
   NetPackMap(DEF_PACK_RCHAT_INFO)           = &CKernel::RecvMsg;
   //处理好友下线请求
   NetPackMap(DEF_PROTOCOL_TCP_OFFLINE_RQ)   = &CKernel::DealOfflinRq;
   //请求 50号包
   NetPackMap(DEF_PROTOCOL_ADD_RQ)           = &CKernel::DealAddFriend;
   //DealAddFriend  51号包
   NetPackMap(DEF_PROTOCOL_ADD_RS)           = &CKernel::DealAddFriendRq;
   //游戏相关
   //52
   NetPackMap(DEF_PROTOCOL_DELETE)           = &CKernel::DealDeletFriendRq;
   //处理音频请求
   NetPackMap(DEF_PROTOCOL_IFVIDEOANDAUDIOANDDESK)           = &CKernel::DealVDARq;
   NetPackMap(DEF_PROTOCOL_IFVORFVIDEOANDAUDIOANDDESK)           = &CKernel::DealVDARs;

   //音频数据的传输
   NetPackMap(DEF_PACK_AUDIO_FRAME)            = &CKernel::slot_dealAudioFrame;
   NetPackMap(DEF_PACK_VIDEO_FRAME)            = &CKernel::slot_dealVideoFrame;

   NetPackMap(DEF_QUIT_CONVERSATION)            = &CKernel::quitconversationrame;


   NetPackMap(DEF_PACK_SENDPICTURE_RQ)            = &CKernel::slot_dealSendPictureRq;
   NetPackMap(DEF_PACK_SENDPICTURE_RS)            = &CKernel::slot_dealSendPictureRs;
   NetPackMap(DEF_PACK_SENDPICTURE_NAME)            = &CKernel::slot_dealSendPicturepathname;
   //初始化群组信息
   NetPackMap(DEF_PACK_CREAT_GROUP)            = &CKernel::slot_dealGroupInfo;

   //传输文件
   NetPackMap(DEF_PACK_SEND_FILE_RQ)            = &CKernel::slot_dealSendFileRq;
   NetPackMap(DEF_PACK_SEND_FILE_RS)            = &CKernel::slot_dealSendFileRs;
   NetPackMap(DEF_PACK_RECV_FILE)            = &CKernel::slot_recvFile;

}
//资源回收
void CKernel::DestriyInstance()
{
    qDebug()<<"进行资源回收";
    qDebug()<<__func__;
    if( m_client )
    {
        m_client -> CloseNet();
        delete m_client;
        m_client = nullptr;
    }
    if(m_mainDialog){
        delete m_mainDialog;//回收new出来的MainDialog
        m_mainDialog = nullptr;

    }
    QMap<int ,ChatDialog*>::Iterator ite = m_mapIdToChatdlg.begin();
    while(ite != m_mapIdToChatdlg.end()){
        m_mapIdToChatdlg[ite.key()]->hide();
        delete m_mapIdToChatdlg[ite.key()];
        m_mapIdToChatdlg.remove(ite.key());
        ite++;
    }
    if(m_ChatroomDialog){
        m_ChatroomDialog->hide();
        delete m_ChatroomDialog;
        m_ChatroomDialog =nullptr;
    }
    if(m_mychat){
        delete m_mychat;//回收new出来的MainDialog
        m_mychat = nullptr;

    }
    if(m_audioWrite){
        delete m_audioWrite;//回收new出来的MainDialog
        m_audioWrite = nullptr;
    }
    if( m_audioRead ){
        delete m_audioRead ; m_audioRead = nullptr;
    }
    if( m_videoRead ){
        delete m_videoRead ; m_videoRead = nullptr;
    }
    if( m_roomDialog ){
        m_roomDialog->hide();
        delete m_roomDialog; m_roomDialog = NULL;
    }
    if( m_setUserDialog ){
        m_setUserDialog->hide();
        delete m_setUserDialog; m_setUserDialog = NULL;
    }
}
//协议包接收处理
void CKernel::slot_ReadyData(unsigned int lSendIP, char *buf, int nlen)
{
    //映射协议表
    PackType type = *(int *)buf;//*(int *)按照整型取数据
    //type的处理，首先进行判定1
    if( type >= _DEF_PROTOCOL_BASE &&
            type <= _DEF_PROTOCOL_BASE + _DEF_PROTOCOL_COUNT)
    {
        //协议映射处理
        PFUN pf = NetPackMap(type);
        //执行函数
        (this->*pf)(lSendIP,buf,nlen);

    }
    //bud要回收
    delete[] buf;
}

//账号登录结果处理
void CKernel::slot_dealLoginRs(unsigned int lSendIP, char *buf, int nlen)
{
    qDebug()<<__func__;
    //拆包
    STRU_LOGIN_RS *rs = (STRU_LOGIN_RS *)buf;
    //根据不同结果显示
    switch(rs -> result)
    {
    case user_not_exist:
        QMessageBox::about(m_loginDialog , "提示","用户不存在，登录失败");
        break;
    case password_error:
        QMessageBox::about(m_loginDialog , "提示","密码错误，登录失败");
        break;
    case login_success:
        //ui 切换
        m_loginDialog -> hide();
        //      m_mainDialog->show();
        //
//        QPropertyAnimation *animation = new QPropertyAnimation(m_mychat,"windowOpacity");
//        animation->setDuration(500);
//        animation->setStartValue(0);
//        animation->setEndValue(1);
//        animation->start();

        m_mychat->show();
        m_id = rs -> userid;
        m_userName = QString::fromStdString( rs -> name);
        //m_mychat->sendUserInfoSignal();
        //存储
        break;
        default:
        QMessageBox::about(m_loginDialog , "提示","异常情况，登录失败");
        break;
    }
}
//账号注册结果处理
void CKernel::slot_resgisterRs(unsigned int lSendIP, char *buf, int nlen)
{
    qDebug()<<__func__;
    //解析数据包
    STRU_REGISTER_RS * rs  = (STRU_REGISTER_RS * )buf;

    //根据结果弹窗
    switch(rs->result){
    case tel_is_exist:
        QMessageBox::about(this->m_loginDialog,"注册提示","注册失败，手机号已存在");
        break;
    case name_is_exist:
        QMessageBox::about(this->m_loginDialog,"注册提示","注册失败，昵称已存在");
        break;
    case register_success:
        QMessageBox::about(this->m_loginDialog,"注册提示","注册成功");
        break;
    default:
        QMessageBox::about(this->m_loginDialog,"注册提示","注册异常");
        break;
    }

    //解析


}
//加入房间回复处理
void CKernel::slot_dealJoinRoomRS(unsigned int lSendIP, char *buf, int nlen)
{
    qDebug()<<__func__;
    //拆包
    STRU_JOIN_ROOM_RS * rs = (STRU_JOIN_ROOM_RS *)buf;
    if(rs -> result == 0){
        QMessageBox::about(m_fiveInLineZone , "提示","加入房间失败");
        return ;
    }
    if( rs -> status == _host)
    {
        m_isHost = true;
    }
    m_roomid = rs->roomid;

    //成功 跳转 成员赋值
    m_fiveInLineZone -> hide();

    m_roomDialog -> setInfo(m_roomid);
    m_roomDialog->show();
}
//房间成员信息处理
void CKernel::slot_dealRoomMembRq(unsigned int lSendIP, char *buf, int nlen)
{
    qDebug()<<__func__;
    //拆包 别人给你发 自己给自己发
    STRU_ROOM_MEMBER * rq= (STRU_ROOM_MEMBER *)buf;

    if(rq -> status == _host){
        //看自己的身份

        m_roomDialog -> setHostInfo( rq -> userid,QString::fromStdString(rq -> name));
    }
    if(rq -> status == _player){
        //看自己的身份
        m_roomDialog -> setPlayerInfo( rq -> userid,QString::fromStdString(rq -> name));
    }
    m_roomDialog->setUserStatus( m_isHost ? _host : _player );

}
//处理成员离开房间
void CKernel::slot_dealLeaveRoomRq(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_LEAVE_ROOM_RQ *rq = (STRU_LEAVE_ROOM_RQ *)buf;
    if( rq -> status == _host){
        //离开的对方是房主，自己也要退出
        //界面
        m_roomDialog->clearRoom();
        m_roomDialog->hide();
        m_fiveInLineZone->show();
        //后台数据
        m_roomid = 0;
        m_isHost = false;
    }else{
        m_roomDialog->playerLeave( rq -> userid );
    }
}
//专区房间信息处理
void CKernel::slot_dealZoneRoomInfo(unsigned int lSendIP, char *buf, int nlen)
{
    qDebug()<<"发送专区房间信息";
    //拆包
    STRU_ZONE_ROOM_INFO *rq = (STRU_ZONE_ROOM_INFO*)buf;
    //根据专区 找到对应窗口
    //根据数组 更新ui
    std::vector<RoomItem*> &vec = m_fiveInLineZone->getVecRoomItem();
    for( int i = 1;i<vec.size();++i){
        vec[i]->setRoomItem( rq->roomInfo[i]);
    }
}
//成员准备处理
void CKernel:: slot_dealFilGameReadyRq(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_FIL_RQ *rq = (STRU_FIL_RQ *)buf;
    //什么专区 什么房间 谁 做了什么事
    if( rq->roomid == m_roomid){
        m_roomDialog->setPlayerReady( rq->userid);
    }
}
//房主开局处理
void CKernel::slot_dealFilGameStartRq(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_FIL_RQ *rq = (STRU_FIL_RQ *)buf;
    //什么专区 什么房间 谁 做了什么事
    if( rq->roomid == m_roomid){
        m_roomDialog->setGameStart();
    }
}
//处理对方落子位置显示
void CKernel::slot_dealFilPieceDownRq(unsigned int lSendIP, char *buf, int nlen)
{
    //处理落子
    STRU_FIL_PIECEDOWN * rq = (STRU_FIL_PIECEDOWN *)buf;
    m_roomDialog->slot_pieceDown( rq->color , rq->x , rq -> y);
}

void CKernel::slot_dealFilWinRq(unsigned int lSendIP, char *buf, int nlen)
{

}

//账号登录处理
void CKernel::slot_loginCommit(QString tel, QString password)
{
    qDebug()<<__func__;
    //加密

    //封包
    STRU_LOGIN_RQ rq;//封装的是登录请求的包
    strcpy(rq.tel , tel.toStdString().c_str());
    //qDebug()<< password << "MD5" << getMD5(password).c_str();
    strcpy(rq.password,getMD5(password).c_str());
    //发送
    SendData((char * )&rq,sizeof(rq));

}
//账号注册处理
void CKernel::slot_registerCommit(QString tel, QString password, QString name)
{
    qDebug()<<__func__;

    //加密

    //封包
    STRU_REGISTER_RQ rq;//封装的是登录请求的包
    strcpy(rq.tel , tel.toStdString().c_str());
    //通过MD5加密
    strcpy(rq.password,getMD5(password).c_str());
    //兼容中文
    std::string strName = name.toStdString();
    strcpy(rq.name,strName.c_str());
    //发送
    SendData((char * )&rq,sizeof(rq));

}
//提交加入分区处理
void CKernel::slot_joinZone(int zoneid)
{
    m_rqTimer.start( 1000 );
    qDebug()<<__func__;
    //成员改变属性
    m_zoneid = zoneid;
    //请求包
    STRU_JOIN_ZONE rq;
    rq.userid = m_id;
    rq.zoneid = zoneid;

    SendData((char *) &rq , sizeof(rq));

    //ui跳转
    //显示专区
    switch(zoneid){
    case fIVE_iN_lINE:
        m_fiveInLineZone->show();
        break;
    }


}
//提交加入房间处理
void CKernel::slot_joinRoom(int roomid)
{
    qDebug()<<__func__;
    //在加入成功后隐藏
    if(m_roomid != 0){
        QMessageBox::about(nullptr,"提示","已经在房间无法加入");
    }
    //加入成功后隐藏
    //m_fiveInLineZone->hide();
    //提交请求
    STRU_JOIN_ROOM_RQ rq;
    rq.userid = m_id;
    rq.roomid = roomid;

    SendData((char *)&rq,sizeof(rq));
}
//离开专区
void CKernel::slot_leaveZone()
{
    m_rqTimer.stop();
    qDebug()<<__func__;
    //成员属性修改
    m_zoneid = 0;
    //请求
    STRU_LEAVE_ZONE rq;
    rq.userid = m_id;
    SendData((char *)&rq,sizeof(rq));
    //ui跳转
    m_mainDialog->show();


}
//离开房间
void CKernel::slot_leaveRoom()
{

    //这个人主动离开
    STRU_LEAVE_ROOM_RQ rq;
    rq.status = m_isHost ? _host : _player;
    rq.userid = m_id;

    rq.roomid = m_roomid;
    SendData((char *)& rq,sizeof(rq));
    //界面
    m_roomDialog->clearRoom();
    m_roomDialog->hide();
    m_fiveInLineZone->show();
    //后台数据
    m_roomid = 0;
    m_isHost = false;


}
//本身五子棋准备
void CKernel::slot_fil_gameReady(int zoneid, int roomid, int userid)
{
    //自己的准备

    STRU_FIL_RQ rq(DEF_FIL_ROOM_READY);
    rq.roomid = roomid;
    rq.zoneid = zoneid;
    rq.userid = userid;

    SendData( (char *)&rq , sizeof(rq));
}
//五子棋开局
void CKernel::slot_fil_gameStart(int zoneid, int roomid)
{
    STRU_FIL_RQ rq(DEF_FIL_GAME_START);
    rq.roomid = roomid;
    rq.zoneid = zoneid;
    rq.userid = m_id;

    SendData( (char *)&rq , sizeof(rq));
}
//落子信息封装处理
void CKernel:: slot_fil_pieceDown(int blackorwhite, int x, int y)
{
    //封包
    STRU_FIL_PIECEDOWN rq;
    rq.color = blackorwhite;
    rq.x = x;
    rq.y = y;
    rq.userid = m_id;
    rq.roomid = m_roomid;
    rq.zoneid = m_zoneid;

    SendData( (char *)&rq , sizeof(rq) );

}
//胜负信息处理
void CKernel::slot_fil_win(int blackorwhite)
{
    //弹窗
    QString res;
    if( m_isHost && blackorwhite == FiveInline::Black){
        res = QString("你赢了");
    }else{
        res = QString("你输了");
    }
    QMessageBox::about(m_roomDialog , "提示" ,res);
    //可以点准备和开局
    m_roomDialog->resetAllPushButton();

}
//电脑对战开始
void CKernel::SIG_fil_playByCpuBegin(int zoneid, int roomid, int userid)
{
    //发包
    STRU_FIL_RQ rq(DEF_FIL_AI_BEGIN);
    rq.roomid = roomid;
    rq.userid = userid;
    rq.zoneid = zoneid;
    SendData((char *)&rq , sizeof(rq));

}
//电脑落子信息处理
void CKernel::SIG_fil_playByCpuEnd(int zoneid, int roomid, int userid)
{
    //发包
    STRU_FIL_RQ rq(DEF_FIL_AI_END);
    rq.roomid = roomid;
    rq.userid = userid;
    rq.zoneid = zoneid;
    SendData((char *)&rq , sizeof(rq));
}
//开始调用电脑对战
void CKernel::slot_dealFilPlayByCpuBegin(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_FIL_RQ *rq = (STRU_FIL_RQ *)buf;
    //查看身份
    rq->zoneid;//
    rq->roomid;
    if( m_id == rq->userid ){
        if( m_isHost){
            m_roomDialog->setHostPlayByCpu( true );
        }else{
            m_roomDialog->setPlayerplayByCpu(true);
        }
    }else{
        if( m_isHost){
            m_roomDialog->setPlayerplayByCpu( true );
        }else{
            m_roomDialog->setHostPlayByCpu(true);
        }
    }
}
//电脑对战结束
void CKernel::slot_dealFilPlayByCpuEnd(unsigned int lSendIP, char *buf, int nlen)
{
    //拆包
    STRU_FIL_RQ *rq = (STRU_FIL_RQ *)buf;
    //查看身份
    rq->zoneid;//
    rq->roomid;
    if( m_id == rq->userid ){
        if( m_isHost){
            m_roomDialog->setHostPlayByCpu( false );
        }else{
            m_roomDialog->setPlayerplayByCpu(false);
        }
    }else{
        if( m_isHost){
            m_roomDialog->setPlayerplayByCpu( false );
        }else{
            m_roomDialog->setHostPlayByCpu(false);
        }
    }
}
//房间内成员信息处理
void CKernel::slot_roomInfoInZone()
{
    //发请求 获取专区每个房间玩家数
    STRU_ZONE_INFO_RQ rq;
    rq.zoneid = m_zoneid;
    qDebug()<<rq.type;
    SendData( ( char * )&rq,sizeof(rq));
}
//初始化处理
//处理用户登录成功后其信息显示
void CKernel::slot_dealUserInfoRs(unsigned int lSendIP, char *buf, int nlen)
{
    qDebug()<<"slot_dealUserInfoRs协议头:"<<*(int*)buf;
    //拆包
    STRU_USER_INFO_RQ *rq = (STRU_USER_INFO_RQ *)buf;
    //更新ui
    m_name = QString::fromStdString( rq-> m_szUser );
    qDebug()<<"信息初始化成功";
    qDebug()<<"m_iconid:"<<rq->m_iconid;
    m_mychat->setInfo(m_name,rq->m_userFeeling,rq->m_iconid);
    //m_mychat->slot_setInfo( m_name , rq->m_iconid);
}
//发送数据函数再次包装
void CKernel::SendData(char *buf, int nLen)
{
    m_client->SendData(0,buf,nLen);
}
//构造函数
CKernel::CKernel(QObject *parent) : QObject(parent),m_netPackFunMap(_DEF_PROTOCOL_COUNT,0)
  ,m_id(0),m_roomid(0),m_zoneid(0),m_isHost(false),m_friendid(0),conversation(1),m_ChatroomDialog(nullptr)
  ,m_deskReadThread( new SendThread),m_gid(0)
{
    qDebug()<<__func__;
    ConfigSet();
    setNetPackMap();

    m_mainDialog = new MainDialog;
    connect(m_mainDialog,SIGNAL(SIG_close()),this,SLOT(DestriyInstance()));
    connect(m_mainDialog,SIGNAL(SIG_joinZone(int)),this,SLOT(slot_joinZone(int)));

    //m_mainDialog->show();

    m_loginDialog = new LoginDialog;
    connect(m_loginDialog ,SIGNAL(SIG_close()),this,SLOT(DestriyInstance()));
    connect(m_loginDialog ,SIGNAL(SIG_loginCommit(QString,QString)),
            this,SLOT(slot_loginCommit(QString,QString)));
    connect(m_loginDialog ,SIGNAL(SIG_registerCommit(QString,QString,QString))
            ,this,SLOT(slot_registerCommit(QString,QString,QString)));

    m_loginDialog->show();

    m_fiveInLineZone = new FiveInLineZone;

    connect(m_fiveInLineZone , SIGNAL(SIG_joinRoom(int)),this,SLOT(slot_joinRoom(int)));
    connect(m_fiveInLineZone , SIGNAL(SIG_close()),this,SLOT(slot_leaveZone()));

    m_roomDialog = new RoomDialog;
    connect(m_roomDialog,SIGNAL(SIG_close()),this,SLOT(slot_leaveRoom()));

    m_client = new TcpClientMediator;
    m_client->OpenNet(m_serverIP,_DEF_TCP_PORT);
    connect(m_client,SIGNAL(SIG_ReadyData(uint,char*,int)),
            this,SLOT(slot_ReadyData(uint,char*,int)));

    connect(m_roomDialog , SIGNAL(SIG_gameReady(int,int,int)),this,SLOT(slot_fil_gameReady(int,int,int)));
    connect(m_roomDialog , SIGNAL(SIG_gameStart(int,int)),this,SLOT(slot_fil_gameStart(int,int)));

    connect(m_roomDialog , SIGNAL(SIG_pieceDown(int,int,int)),this,SLOT(slot_fil_pieceDown(int,int,int)));
    connect(m_roomDialog , SIGNAL(SIG_playerWin(int)),this,SLOT(slot_fil_win(int)));

    connect(m_roomDialog , SIGNAL(SIG_playByCpuBegin(int,int,int)),this,SLOT(SIG_fil_playByCpuBegin(int,int,int)));
    connect(m_roomDialog , SIGNAL(SIG_playByCpuEnd(int,int,int)),this,SLOT(SIG_fil_playByCpuEnd(int,int,int)));
    connect( &m_rqTimer,SIGNAL(timeout()),this,SLOT(slot_roomInfoInZone()));
    //模拟连接服务器 发送数据包
//    STRU_LOGIN_RQ rq;
//    m_client->SendData(0,(char*)&rq,sizeof(rq));
    m_mychat = new MyChat;
    connect(m_mychat,SIGNAL(setUserInfo(char * , int )),this,SLOT(slot_setUserInfo(char * , int)));
    connect(m_mychat,SIGNAL(SIG_dealUserInfoUpdate()),this,SLOT(slot_dealUserInfoUpdate()));
    connect(m_mychat,SIGNAL(SIG_close()),this,SLOT(slot_Offline()));
    connect(m_mychat,SIGNAL(SIG_AddFriend()),this,SLOT(slot_AddFriend()));
    connect(m_mychat,SIGNAL(SIG_CreateGroup()),this,SLOT(slot_CreateGroup()));
    m_setUserDialog = new setUserDialog;
    //todo该资源的回收
    connect(m_setUserDialog,SIGNAL(SIG_userSetCommit(int,QString,QString)),this,SLOT(slot_userSetCommit(int,QString,QString)));
    //绑定QQui五子棋按钮
    connect(m_mychat,SIGNAL(SIG_dealEnterZone()),this,SLOT(slot_dealEnterZone()));
    //m_useritem = new useritem;
    //todo资源回收
    m_ChatroomDialog = new ChatroomDialog;


    m_audioRead = new Audio_Read;
    m_videoRead = new Video_Read;
    m_deskRead = new Desk_Read;
    m_audioWrite = new Audio_Write;

    connect( m_audioRead , SIGNAL(SIG_audioFrame(QByteArray&)),this,SLOT(slot_sendAudioFrame(QByteArray&)));
    connect( m_videoRead , SIGNAL(SIG_videoFrame(QImage)),this,SLOT(slot_refreshVideoFrame(QImage)));
    connect( m_videoRead , SIGNAL(SIG_videoFrameData(QByteArray)),this,SLOT(slot_sendVideoFrameData(QByteArray)));

    connect( m_deskRead , SIGNAL(SIG_screenFrame(QImage)),this,SLOT(slot_refreshVideoFrame(QImage)));
    connect( m_deskRead , SIGNAL(SIG_screenFrameData(QByteArray)),this,SLOT(slot_sendVideoFrameData(QByteArray)));

    connect( m_ChatroomDialog , SIGNAL(SIG_audioOpen()) , this , SLOT(slot_audioOpen()));
    connect( m_ChatroomDialog , SIGNAL(SIG_audioClose()) , this , SLOT(slot_audioClose()));
    connect( m_ChatroomDialog , SIGNAL(SIG_captureOpen()) , this , SLOT(slot_captureOpen()));
    connect( m_ChatroomDialog , SIGNAL(SIG_captureClose()) , this , SLOT(slot_captureClose()));

    connect( m_ChatroomDialog , SIGNAL(SIG_deskOpen()) , m_deskRead , SLOT(start()));
    connect( m_ChatroomDialog , SIGNAL(SIG_deskClose()) , m_deskRead , SLOT(pause()));

    connect( m_ChatroomDialog , SIGNAL(SIG_quitRoom()),this , SLOT(slot_quitRoom()) );

    connect( this , SIGNAL(SIG_sendVideoFrma(unsigned int,char*,int)) ,
             m_deskReadThread.get() , SLOT(slot_threadSendData(unsigned int,char*,int)));






}
//更改个人信息
void CKernel::slot_userSetCommit( int iconm , QString name , QString feeling)
{
    qDebug()<<__func__;

    //发包
    //中文兼容 也是uft8

    std::string strName = name.toStdString();
    std::string strfeeling = feeling.toStdString();
    STRU_USER_INFO_RQ rq;
    rq.m_iconid = iconm;
    rq.m_UserID = m_id;
    strcpy(rq.m_userFeeling,strfeeling.c_str());
    strcpy(rq.m_szUser,strName.c_str());
    SendData((char *)&rq , sizeof(rq));
    QMessageBox::about( this->m_mychat , "提示","用户信息设置成功");

//        //处理的则是群组信息的更改
//        STRU_SENDPIQUIT_GROUP_CHANGE rq;
//        rq.iconid = iconm;
//        rq.g_id = m_gid;//组id如何
//        strcpy(rq.Name,name.toStdString().c_str());
//        qDebug()<<"发送更改后的组的信息";
//        SendData((char *)&rq , sizeof(rq));
//        QMessageBox::about( this->m_mychat , "提示","用户信息设置成功");
//    }
}
//游戏大厅界面的显示
void CKernel::slot_dealEnterZone()
{
    m_mainDialog->show();
}
//好友信息及其列表显示处理///////////////////////////////////////////////////////////////////////////////////////
void CKernel::slot_dealFriendInfoRs(unsigned int lSendIP, char *buf, int nlen)
{
    qDebug()<<"处理好友列表";

    //拆包
    STRU_USER_INFO_QQ* info = (STRU_USER_INFO_QQ*)buf;

    qDebug()<<info->m_szUser;

    //2、判断是不是自己的信息
    if(m_id==info->m_UserID)
    {
        qDebug()<<"step1";
        //2.1、把自己的信息设置到界面上
        m_mychat->setInfo(info->m_szUser,info->m_userFeeling,info->m_iconid);
        //保存昵称
        m_name=info->m_szUser;
        return ;
    }
    //如果是好友的信息，判断列表上是否已经有这个好友的useritem
    if(m_mapIdToUseritem.count(info->m_UserID)>0)
    {
        qDebug()<<"step2";
        //3.1、如果已经有这个好友了，就更新
        //3.1.1、先从map中取出useritem
        useritem* item=m_mapIdToUseritem[info->m_UserID];
        //3.1.2、设置控件上的内容
        item->setInfo(info->m_UserID,info->m_iconid,info->state,info->m_szUser,info->m_userFeeling);
        //3.1.3、打印日志，好友xxx已上线
        qDebug()<<QString("好友%1已上线").arg(info->m_szUser);
        ChatDialog * chatwin = new ChatDialog;
        chatwin->setEnableTrue();
        chatwin = m_mapIdToChatdlg[info->m_UserID];
        chatwin->setChatWindowInfo(info->m_szUser,info->m_UserID);

    }
    else
    {
        qDebug()<<"step3";
        //3.2、如果没有，就创建一个新的useritem
        //todo 资源回收
        useritem * item=new useritem;
        //3.2.1、设置控件的内容

        item->setInfo(info->m_UserID,info->m_iconid,info->state,info->m_szUser,info->m_userFeeling);
        //(int id, int iconId, int state, QString name, QString feeling)
        //3.2.2、绑定发送点击好友头像的信号和槽函数
        connect(item,SIGNAL(SIG_UserItemClicked(int))
                ,this,SLOT(slot_UserItemClicked(int)));
        connect(item,SIGNAL(SIG_DeleteFriend(int)),this,SLOT(slot_DeleteFriend(int)));
        //3.2.3、把useritem添加到好友列表上
        m_mychat->addFriend(item);
        //3.2.4、把新的useritem添加到map中
        m_mapIdToUseritem[info->m_UserID]=item;
        //创建一个新的聊天窗口对象
        ChatDialog * chatwin = new ChatDialog;
        chatwin->setEnableTrue();
        chatwin->setChatWindowInfo(info->m_szUser,info->m_UserID);
        //todo资源回收
        m_mapIdToChatdlg[info->m_UserID] = chatwin;
        connect(chatwin,SIGNAL(SIG_sendChatMsg(QString,int)),this,SLOT(slot_sendChatMsg(QString,int)));
        //绑定开启视频聊天的功能那个
        connect(chatwin,SIGNAL(SIG_OpenAudio(QString,int)),this,SLOT(slot_OpenAudio(QString ,int)));
        //绑定发送图片的槽函数
        connect(chatwin,SIGNAL(sendFriendPicture(QString,QImage,int)),this,SLOT(slot_SendPictureRq(QString,QImage,int)));
        //绑定显示图片的槽函数
        connect( this,SIGNAL(SIG_sendPic( QImage)) ,chatwin, SLOT(slot_sendPic(QImage)));
        //进行测试
        connect( this,SIGNAL(Test( QImage)) ,chatwin, SLOT(Test(QImage)));
//        //表情绑定信号和槽
//        connect(chatwin,SIGNAL(ShowBQ(int)),this,SLOT(DealShowBQ(int)));
//        //转发表情
//        connect( this,SIGNAL(SIG_BQWin(int)) ,chatwin, SLOT(slot_Bq(int)));

        connect( this,SIGNAL(SIG_PathName(QString)) ,chatwin, SLOT(deal_PathName(QString)));
        connect(chatwin,SIGNAL(SIG_sendFile(QString,int)),this,SLOT(slot_sendFile(QString,int)));
        connect(chatwin,SIGNAL(SIG_recvFile(QString,int)),this,SLOT(slot_selectFile(QString,int)));
        connect(chatwin,SIGNAL(SIG_saveFile(QString,int)),this,SLOT(slot_openSaveFileDir(QString,int)));
    }
}
//设置用户信息
void CKernel::slot_setUserInfo(char * buf , int len)
{
    //拆包
    STRU_USER_INFO_RQ* rq = (STRU_USER_INFO_RQ*)buf;
    //初始化用户id 数据库里的自增id

    rq->m_UserID = m_id;
    qDebug()<<m_id ;
    qDebug()<<rq->m_UserID ;
    m_client->SendData(0,(char*)rq,len);
}
//处理用户信息的更新  UI界面做显示
void CKernel::slot_dealUserInfoUpdate()
{
    m_setUserDialog->show();
}
//设置配置文件
void CKernel::ConfigSet()
{
    qDebug()<<__func__;
    //获取配置文件里的信息以及设置

    //.ini配置文件

    //[net] 组名 GroupName
    //key = value

    //1、ip默认
    strcpy( m_serverIP, _DEF_SERVER_IP);
    //2、设置和获取配置文件  有还是没有 配置文件在哪里？ 设置和exe同一级的目录
    //exe目录
    //路径
    QString path = QCoreApplication::applicationDirPath() + "/config.ini";
    //查看是否存在
    QFileInfo info(path);
    if(info.exists()){
        //存在
        QSettings setting( path , QSettings::IniFormat , nullptr);
        //[net] 组 写入值
        setting.beginGroup("net");
        QVariant var = setting.value( "ip");
        QString strip = var.toString();
        if( !strip.isEmpty())
            strcpy( m_serverIP, strip.toStdString().c_str());

        setting.endGroup();
    }else{
        //不存在
        QSettings setting( path , QSettings::IniFormat , nullptr);
        //[net] 组 写入值
        setting.beginGroup("net");
        setting.setValue( "ip",QString::fromStdString(m_serverIP));
        setting.endGroup();
    }
    qDebug()<<"ip" << m_serverIP;
}
//UI聊天窗口显示
void CKernel::slot_UserItemClicked(int m_id)
{
    qDebug()<<__func__;
    //显示与该好友的聊天窗口
    if(m_mapIdToChatdlg.count(m_id)>0)
    {
        qDebug()<<"按钮标志是否可以点击（）（）（）（）（）（）（）（）（）（）（）*****&&&&&&"<<conversation;
        ChatDialog* chat =m_mapIdToChatdlg[m_id];
        qDebug()<<conversation;
        if(conversation){
            chat->setEnableTrue();
        }else{
            chat->setEnabledFalse();
        }
        m_friendid = m_id;
        chat->showNormal();
    }
}
//发送给好友的信息转发处理
void CKernel::slot_sendChatMsg(QString content, int id)
{

    qDebug()<<"好友处理信息转发";
    //发给->m_id
    //封包
    STRU_TCP_CHAT_RQ rq;
    qDebug()<<"标志位:"<<rq.type;
    rq.userId=m_id;
    rq.friendId=id;
    qDebug()<<"好友id"<<rq.friendId;
    strcpy(rq.content,content.toStdString().c_str());

    //2、发送服务器
    m_client->SendData(0,(char*)&rq,sizeof(rq));
}

void CKernel::DealChatRq(unsigned int lSendIP, char *buf, int nlen)
{

}
//处理接收到的好友信息
void CKernel::RecvMsg(unsigned int lSendIP, char *buf, int nlen)
{
    //*************************************************************************************
    //*************************************************************************************
    //*************************************************************************************
    //*************************************************************************************
    //*************************************************************************************
    //*************************************************************************************
    qDebug()<<"收到转发的信息****************!!!!!!!!!!!!!!!!!!!!!!!!!!";
    STRU_TCP_CHAT_RS *rq = (STRU_TCP_CHAT_RS*)buf;
    if(m_mapIdToChatdlg.count(rq->userId)>0){
        ChatDialog * chat = m_mapIdToChatdlg[rq->userId];
        qDebug()<<"1-------"<<rq->userId;
        if(conversation){
            chat->setEnableTrue();
        }else{
            chat->setEnabledFalse();
        }
        chat->showNormal();
        chat->setChatMsg(rq->content);
    }else{
        ChatDialog * chat = m_mapIdToChatdlg[rq->friendId];
        qDebug()<<"2-------"<<rq->friendId;
        chat->showNormal();
        chat->setChatMsg(rq->content);
    }
    //*************************************************************************************
    //*************************************************************************************
    //*************************************************************************************
    //*************************************************************************************
    //*************************************************************************************
    //*************************************************************************************
}
//处理好友的下线请求
void CKernel::DealOfflinRq(unsigned int lSendIP, char *buf, int nlen)
{
    qDebug()<<"suoyi shshih siho iih 处理好友的下线请求";
    //拆包
    STRU_OFFLINE_RQ * info = (STRU_OFFLINE_RQ *)buf;
    qDebug()<<info->userId;
    //取出是谁要下线
    useritem* item=m_mapIdToUseritem[info->userId];
    if(m_mapIdToUseritem.count(info->userId)>0)
    {
        qDebug()<<"进行处理";
        useritem* item=m_mapIdToUseritem[info->userId];
        item->setOffline();
    }
}
//处理自身账号的下线
void CKernel::slot_Offline()
{
    qDebug()<<"*****************处理下线槽函数";
    STRU_OFFLINE_RQ r;
    r.userId = m_id;
    SendData((char*)&r,sizeof(r));
    DestriyInstance();
}
//
void CKernel::slot_AddFriend()
{
    qDebug()<<"进入的槽函数";
    //1、弹出窗口，让用户输入想要的添加的好友的昵称
    QString strName=QInputDialog::getText(m_mychat,"添加好友","请输入好友的昵称：");
    qDebug()<<strName;
    //2、校验输入昵称的合法性
    QString nameTemp=strName;
    if(strName.isEmpty()||nameTemp.remove(" ").isEmpty()||nameTemp.length()>10)
    {
        QMessageBox::about(m_mychat,"提示","输入的昵称不合法");
        return ;
    }
    //3、校验昵称是不是自己的昵称
    if(m_name==strName)
    {
        QMessageBox::about(m_mychat,"提示","不能添加自己为好友");
        return ;
    }
    //4、校验是不是已经好友了
     for(auto ite =m_mapIdToUseritem.begin();ite!=m_mapIdToUseritem.end();ite++)
     {
         useritem* item=*ite;
         if(item->m_name==strName)
         {
            QMessageBox::about(m_mychat,"提示",QString("已经和%1是好友了，不需要添加").arg(strName));
            return ;
         }
     }
    //5、发送添加好友情求给服务器
     STRU_ADD_FRIEND_RQ rq;
     strcpy(rq.Friendtel,strName.toStdString().c_str());
     //rq.Friendtel = strName;
     rq.userId=m_id;
     strcpy(rq.Usertel,m_name.toStdString().c_str());
     SendData((char*)&rq,sizeof(rq));

}
//后续
void CKernel::DealAddFriendRq(unsigned int lSendIP, char *buf, int nlen)
{
    //收到51号包
    qDebug()<<"#####################################################################";
    //1、拆包
    STRU_ADD_FRIEND_RS*rs =(STRU_ADD_FRIEND_RS*)buf;
    qDebug()<<rs->friendName;
    //2、根据添加解决现实提示信息
    qDebug()<<"对方同意与否";
    qDebug()<<rs->result;
    switch(rs->result)
    {
    case add_success:
        QMessageBox::about(m_mychat,"提示",QString("添加好友【%1】成功").arg(rs->friendName));
        break;
    case no_this_user:
        QMessageBox::about(m_mychat,"提示",QString("添加好友【%1】失败，不存在这个用户").arg(rs->friendName));
        break;
    case user_offline:
        QMessageBox::about(m_mychat,"提示",QString("添加好友【%1】失败，好友不在线").arg(rs->friendName));
        break;
    case user_refuse:
        QMessageBox::about(m_mychat,"提示",QString("【%1】拒绝了你的好友请求").arg(rs->friendName));
        break;
    default:
        break;

    }
}
//接收到好友添加请求的处理
void CKernel::DealAddFriend(unsigned int lSendIP, char *buf, int nlen)
{
    //收到50号包
    //1、拆包
    STRU_ADD_FRIEND_RQ *rq=(STRU_ADD_FRIEND_RQ *)buf;
    STRU_ADD_FRIEND_RS rs;

    //2、弹出提示窗口，询问用户同意还是拒绝添加好友
    QString str=QString("【%1】请求添加你为好友，是否同意？").arg(rq->Usertel);
    if(QMessageBox::Yes==QMessageBox::question(m_mychat,"提示",str))
    {
        rs.result=add_success;
    }
    else
    {
        rs.result=user_refuse;
    }
    //3、给服务器发送添加回复
    rs.friendId=rq->userId;
    qDebug()<<rs.result;
    rs.userId=m_id;
    strcpy(rs.friendName,m_name.toStdString().c_str());
    SendData((char*)&rs,sizeof(rs));
}
//处理删除好友的操作
void CKernel::slot_DeleteFriend(int friendid)
{
    qDebug()<<"删除好友";
    //封包
    STRU_DEL_FRIEND_RQ r;
    //要删除的好友
    qDebug()<<"协议头"<<r.type;
    r.friendId = friendid;
    r.userId = m_id;
    SendData((char*)&r,sizeof(r));

}
//处理被删除好友的操作
void CKernel::DealDeletFriendRq(unsigned int lSendIP, char *buf, int nlen)
{
    qDebug()<<"收到52号包";
    //拆包
    STRU_DEL_FRIEND_RQ* R = (STRU_DEL_FRIEND_RQ*) buf;
    if(m_mapIdToUseritem.count(R->friendId)>0)
    {
        qDebug()<<"进行处理";
        useritem* item=m_mapIdToUseritem[R->friendId];
        m_mychat->DeletFriend(item);
        m_mapIdToUseritem.remove(R->friendId);
    }
}
////处理发送图片的槽函数
//void CKernel::DealsendFriendPicture()
//{
//    ChatDialog * chatwin = new ChatDialog;
//    //todo资源回收
//    chatwin = m_mapIdToChatdlg[id];

//}
//处理音视频窗口以及接口调用相关
void CKernel::slot_OpenAudio(QString name, int id)
{
    ChatDialog * chatwin = m_mapIdToChatdlg[m_friendid];
    chatwin->setEnabledFalse();
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
    //创建音频对象
//    if( m_mapIDToAudioWrite.count( id ) > 0){
//        Audio_Write * aw = m_mapIDToAudioWrite[id];
//        m_mapIDToAudioWrite.erase( id );
//        delete aw;
//    }
//    Audio_Write * aw = new Audio_Write;
//    m_mapIDToAudioWrite[id] = aw;

//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
    //friendid = id;//给friendid进行了一波真正的初始化，仅仅用于音视频转发
    qDebug()<<"获取id"<<id;
    //发送一个请求包
    //设计新的协议
    STRU_DEL_FRIENDVID_RQ r;  //发送的53号包
    r.userId = m_id;//我的ID
    strcpy(r.UserName,m_name.toStdString().c_str());//我的名字
    qDebug()<<"我的名字："<<r.UserName;
    strcpy(r.Friendname,name.toStdString().c_str());//对Friendtel这个人发起请求
    qDebug()<<"好友的名字:"<<r.Friendname;
    SendData((char*)&r,sizeof(r));
    UserShow * user = new UserShow;
    user->slot_setInfo( m_id , m_name );
    //todo释放资源

    m_mapIDToUserShow[m_id] = user;
    qDebug()<<"存入的第一个#####################################:"<<m_id;
    m_ChatroomDialog->slot_addUser( user );

    UserShow * user1 = new UserShow;
    user1->slot_setInfo( m_friendid , r.Friendname );
    qDebug()<<"存入的第二个#########################:"<<m_friendid;
    //todo释放资源
    connect(user,SIGNAL(SIG_userisClicked(int,QString)),m_ChatroomDialog,SLOT(slot_setBigImageInfo(int,QString)));
    connect(user1,SIGNAL(SIG_userisClicked(int,QString)),m_ChatroomDialog,SLOT(slot_setBigImageInfo(int,QString)));
    m_mapIDToUserShow[m_friendid] = user1;
    m_ChatroomDialog->slot_addUser( user1 );
    m_ChatroomDialog->show();

}
//处理接收到的音频聊天请求及相关接口的调用
void CKernel::DealVDARq(unsigned int lSendIP, char *buf, int nlen)
{
    qDebug()<<"处理好友音频聊天请求**************************************************************";
    STRU_DEL_IFFRIENDVID_RQ* rq = (STRU_DEL_IFFRIENDVID_RQ*)buf;
    qDebug()<<"输出id  如果是6，6是叶大锤"<<rq->userId;
    if(m_mapIdToChatdlg.count(rq->userId)>0){
        ChatDialog * chat = m_mapIdToChatdlg[rq->userId];
        m_friendid = rq->userId;
        qDebug()<<"2-------"<<rq->userId;
        chat->showNormal();
        STRU_DEL_IFFRIENDVIDVORF_RQ rs;
        strcpy(rs.Friendname,rq->Friendname);
        strcpy(rs.UserName , rq->UserName);

        rs.userId = rq->userId;

        if(QMessageBox::Yes==QMessageBox::question(chat,"询问","是否接受好友音频聊天请求"))
        {
            qDebug()<<"点击的是yes============================================================";
                chat->setEnabledFalse();
                conversation = 0;
            rs.result = 1;
            qDebug()<<"接受音频请求";
            //同时打开视频框
            UserShow * user = new UserShow;
            //todo释放资源
            user->slot_setInfo( m_id , m_name );

            m_mapIDToUserShow[m_id] = user;
            m_ChatroomDialog->slot_addUser( user );
            UserShow * user1 = new UserShow;
            user1->slot_setInfo( m_friendid , rq->Friendname);
            //todo释放资源

            m_mapIDToUserShow[m_friendid] = user1;
            m_ChatroomDialog->slot_addUser( user1 );
            connect(user,SIGNAL(SIG_userisClicked(int,QString)),m_ChatroomDialog,SLOT(slot_setBigImageInfo(int,QString)));
            connect(user1,SIGNAL(SIG_userisClicked(int,QString)),m_ChatroomDialog,SLOT(slot_setBigImageInfo(int,QString)));
            m_ChatroomDialog->show();
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//            if( m_mapIDToAudioWrite.count( rq->userId ) > 0){
//                Audio_Write * aw = m_mapIDToAudioWrite[rq->userId];
//                m_mapIDToAudioWrite.erase( rq->userId );
//                delete aw;
//            }
//            Audio_Write * aw = new Audio_Write;
//            m_mapIDToAudioWrite[rq->userId] = aw;
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************

        }else{
            qDebug()<<"点击的是no=========================================================================";
            rs.result = 0;

        }
        qDebug()<<"rs - > result "<<rs.result;
        SendData((char*)&rs,sizeof(rs));

    }

//    qDebug()<<"收到转发的信息";
//    STRU_TCP_CHAT_RS *rq = (STRU_TCP_CHAT_RS*)buf;
//    if(m_mapIdToChatdlg.count(rq->userId)>0){
//        ChatDialog * chat = m_mapIdToChatdlg[rq->userId];
//        qDebug()<<"1-------"<<rq->userId;
//        chat->showNormal();
//        chat->setChatMsg(rq->content);
//    }
//    if(m_mapIdToChatdlg.count(rq->userId)>0){
//        qDebug()<<"没有进入到打开聊天框的函数";
//        ChatDialog * chat = m_mapIdToChatdlg[rq->userId];
//        chat->showNormal();
//    }else{
//        ChatDialog * chat = new ChatDialog;
//        chat->setChatWindowInfo(rq->Friendname,rq->userId);
//        m_mapIdToChatdlg[rq->userId] = chat;
//        chat->showNormal();
//        STRU_DEL_IFFRIENDVIDVORF_RQ rs;
//        strcpy(rs.Friendname,rq->Friendname);
//        strcpy(rs.UserName , rq->UserName);

//        rs.userId = rq->userId;

//        if(QMessageBox::Yes==QMessageBox::question(chat,"询问","是否接受好友音频聊天请求"))
//        {
//            qDebug()<<"接受音频请求";

//            rs.result = 1;

//        }else{
//            rs.result = 0;

//        }
//        SendData((char*)&rs,sizeof(rs));
//    }

}
//处理对方音视频对己方音视频聊天请求的回复
void CKernel::DealVDARs(unsigned int lSendIP, char *buf, int nlen)
{
    qDebug()<<"对方是否同意呢？？？？？？？？？？？？？？？？？？？？？？";

    STRU_DEL_IFFRIENDVIDVORF_RQ* r = (STRU_DEL_IFFRIENDVIDVORF_RQ*)buf;
    qDebug()<<"我TM可真无语啊："<<r->result;
    if(r->result == 1){
        qDebug()<<"对方接收音频请求了";
        //同意调用函数开始发送序列化数据  一个新的序列化协议  另外一个客户端出发协议映射表反序列化
        //先打开视频框
        ChatDialog * chat = new ChatDialog;
        chat = m_mapIdToChatdlg[m_friendid];
        chat->setEnabledFalse();
        conversation = 0;
        qDebug()<<"进行的提示";
        QMessageBox::about(chat,"提示","对方接受音频请求");
    }else if(r->result == 0){
        //拒绝
        ChatDialog * chat = new ChatDialog;
        chat = m_mapIdToChatdlg[m_friendid];
        chat->setEnableTrue();
        conversation = 1;
        qDebug()<<"进行的提示";
        QMessageBox::about(chat,"提示","对方拒绝音频请求");
    }else{
        ChatDialog * chat = new ChatDialog;
        chat = m_mapIdToChatdlg[r->userId];
        conversation = 1;
        QMessageBox::about(chat,"提示","对方不在线");
    }

}

//===========================================================================================================
void CKernel::slot_audioOpen()
{
    m_audioRead->start();
}

void CKernel::slot_audioClose()
{
    m_audioRead->pause();
}

void CKernel::slot_captureOpen()
{
    qDebug()<<"打开摄像头";
    m_videoRead->start();
}

void CKernel::slot_captureClose()
{
    m_videoRead->pause();
}

//刷新画面函数 主要使用在采集 和 采集别人的画面
void CKernel::slot_refreshVideoFrame(QImage img)
{

    slot_refreshUserFrame( m_id , img);
}
//鼠标点击事件 预览图大图显示函数
void CKernel::slot_refreshUserFrame(int id , QImage &img)
{

    //预览图显示  大图显示
    m_ChatroomDialog->slot_setBigImage( id , img );
    //列表图显示
    if(m_mapIDToUserShow.count( id ) > 0){
        UserShow * user = m_mapIDToUserShow[id];
        user->slot_setImage( img );
    }
}
//处理接收到的音频数据流
void CKernel::slot_dealAudioFrame(unsigned int lSendIP, char *buf, int nlen)
{

    //反序列化的过程

    ///音频数据帧
    /// 成员描述
    /// int type;
    /// int userId;
    /// int roomId;
    /// int hour;
    /// int min;
    /// int sec;
    /// int msec;
    /// QByteArray audioFrame;  --> char frame[]; 柔性数组
    ///
    int type;
    int userid ;
    int roomidandfriendid ;//单聊的情况下要仅仅是给对方发 因此要获得对方的ID
    //要是群组就获取其可以随机生成一个类似腾讯会议的ID，仿照其写一个也不是不可以
    int hour;
    int min ;
    int sec ;
    int msec;

    //反序列化数据
    char * tmp = buf;
    type = *(int *) tmp;
    tmp += sizeof(int);

    userid = *(int *) tmp;
    tmp += sizeof(int);

    roomidandfriendid = *(int *) tmp;
    tmp += sizeof(int);

    tmp += sizeof(int);

    tmp += sizeof(int);

    tmp += sizeof(int);

    tmp += sizeof(int);

    int audioLen = nlen - 7 * sizeof( int );
    //音频数据
    QByteArray ba (tmp , audioLen);

    m_audioWrite->slot_net_rx( ba );

}
//处理接收到的视频数据流
void CKernel::slot_dealVideoFrame(unsigned int lSendIP, char *buf, int nlen)
{
    //反序列化的过程


    ///音频数据帧
    /// 成员描述
    /// int type;
    /// int userId;
    /// int roomId;
    /// int hour;
    /// int min;
    /// int sec;
    /// int msec;
    /// QByteArray audioFrame;  --> char frame[]; 柔性数组
    ///
    int type;
    int userid ;
    int roomidandfriendid ;
    int hour;
    int min ;
    int sec ;
    int msec;

    //反序列化数据
    char * tmp = buf;
    type = *(int *) tmp;
    tmp += sizeof(int);

    userid = *(int *) tmp;
    tmp += sizeof(int);

    roomidandfriendid = *(int *) tmp;
    tmp += sizeof(int);

    tmp += sizeof(int);

    tmp += sizeof(int);

    tmp += sizeof(int);

    tmp += sizeof(int);

    int imageLen = nlen - 7 * sizeof( int );
    //音频数据
    QImage img;
    img.loadFromData( (uchar*) tmp , imageLen );
    slot_refreshUserFrame( userid , img);

//    m_img = img;
//    //update(); // 会触发绘图事件  更新
//    repaint();// 会触发绘图事件   立刻触发

}
//发送视频流数据
void CKernel::slot_sendAudioFrame(QByteArray &ba)
{
    qDebug()<<"发送音频";
    //协议头
    //发送者id:服务器知道谁发的  不给他转发
    //确认房间号：转发给谁
    //时间  小时 分钟  秒  毫秒send延迟 -> 考虑丢弃帧 需要记录时间
    //音频长度
    //音频数据
    ///音频数据帧
    /// 成员描述
    /// int type;
    /// int userId;
    /// int roomId;
    /// int hour;
    /// int min;
    /// int sec;
    /// int msec;
    /// QByteArray audioFrame;  --> char frame[]; 柔性数组
    ///
    int type= DEF_PACK_AUDIO_FRAME;
    int userid = m_id;
    int roomidandfriendid = m_friendid; // todo更改首先是知道给哪个好友发送了
    QTime tm = QTime::currentTime();
    int hour = tm.hour();
    int min = tm.minute();
    int sec = tm.second();
    int msec = tm.msec();
    char *audioData = ba.data();
    int len = ba.size();

    //序列化数据
    char * buf = new char[sizeof(int) * 7 + len];
    char * tmp = buf;
    *(int *) tmp = type;
    tmp += sizeof(int);

    *(int *) tmp = userid;
    tmp += sizeof(int);

    *(int *) tmp = roomidandfriendid;
    tmp += sizeof(int);

    *(int *) tmp = hour;
    tmp += sizeof(int);

    *(int *) tmp = min;
    tmp += sizeof(int);

    *(int *) tmp = sec;
    tmp += sizeof(int);

    *(int *) tmp = msec;
    tmp += sizeof(int);

    memcpy(tmp , audioData , len );
    tmp += len;
    SendData( buf , sizeof(int)*7 + len);
    delete[] buf;
}
//发送音频流数据
void CKernel::slot_sendVideoFrameData(QByteArray ba)
{
    ///视频数据帧
    /// 成员描述
    /// int type;
    /// int userId;
    /// int roomId;
    /// int hour;
    /// int min;
    /// int sec;
    /// int msec;
    /// QByteArray videoFrame;
    ///
    //10010
    int type= DEF_PACK_VIDEO_FRAME;
    int userid = m_id;
    int roomidandfriendid = m_friendid; // todo更改
    QTime tm = QTime::currentTime();
    int hour = tm.hour();
    int min = tm.minute();
    int sec = tm.second();
    int msec = tm.msec();
    char *audioData = ba.data();
    int len = ba.size();

    //序列化数据
    char * buf = new char[sizeof(int) * 7 + len];
    char * tmp = buf;
    *(int *) tmp = type;
    tmp += sizeof(int);

    *(int *) tmp = userid;
    tmp += sizeof(int);

    *(int *) tmp = roomidandfriendid;
    tmp += sizeof(int);

    *(int *) tmp = hour;
    tmp += sizeof(int);

    *(int *) tmp = min;
    tmp += sizeof(int);

    *(int *) tmp = sec;
    tmp += sizeof(int);

    *(int *) tmp = msec;
    tmp += sizeof(int);

    memcpy(tmp , audioData , len );
//    SendData  ( buf , sizeof(int)*7 + len);
//    delete[] buf;
    Q_EMIT SIG_sendVideoFrma(0 , buf , sizeof(int)*7 + len);

}
//挂断音视频电话
void CKernel::slot_quitRoom()
{
    qDebug()<<"X的信号呢";
    conversation = 1;
    ChatDialog * chatwin = m_mapIdToChatdlg[m_friendid];
    chatwin->setEnableTrue();
    STRU_DEL_QUIT_CONVERSATION r;
    r.userId = m_id;
    r.friendID = m_friendid;
    SendData((char*)&r,sizeof(r));
//    //声音关闭 ui 后台
    m_ChatroomDialog->slot_setRoomClear();
    //视频 回收掉
    //音频播放的回收 map

    for(auto ite = m_mapIDToUserShow.begin() ; ite != m_mapIDToUserShow.end();/*++ite*/){
        qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!进行的回收呢";
        UserShow * user =  ite->second;
        ite = m_mapIDToUserShow.erase(ite );
        m_ChatroomDialog->slot_removeUser(user );
    }
    m_audioRead->pause();
    m_videoRead->pause();
//    for(auto ite = m_mapIDToAudioWrite.begin() ; ite != m_mapIDToAudioWrite.end();/*++ite*/){
//        Audio_Write * user =  ite->second;;
//        ite = m_mapIDToAudioWrite.erase(ite );
//        delete user;
//    }

}
//处理好友挂断音视频通话
void CKernel::quitconversationrame(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_DEL_QUIT_CONVERSATION * r = (STRU_DEL_QUIT_CONVERSATION*) buf;
    qDebug()<<"移除其通话";
    m_ChatroomDialog->hide();
    ChatDialog * chatwin = m_mapIdToChatdlg[m_friendid];
    chatwin->setEnableTrue();
    for(auto ite = m_mapIDToUserShow.begin() ; ite != m_mapIDToUserShow.end();/*++ite*/){
        qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!进行的回收呢";
        UserShow * user =  ite->second;
        ite = m_mapIDToUserShow.erase(ite );
        m_ChatroomDialog->slot_removeUser(user );
    }
    //音频播放的回收 map
//    for(auto ite = m_mapIDToAudioWrite.begin() ; ite != m_mapIDToAudioWrite.end();/*++ite*/){
//        Audio_Write * user =  ite->second;;
//        ite = m_mapIDToAudioWrite.erase(ite );
//        delete user;
//    }
     QMessageBox::about(chatwin,"提示","对方已经挂断电话");
     conversation = 1;
}

void SendThread::slot_threadSendData(unsigned int socket, char *buf, int nlen)
{
    qDebug()<<"slot_threadSendData:"<<QThread::currentThreadId();
    CKernel::GetInstance()->SendData(  buf , nlen);
    delete[] buf;
}

void CKernel::slot_SendPictureRq(QString pathname, QImage image,int id)//上传文件内容请求
{
    qDebug()<<"Ckernel发送图片的处理";
    //发送图片数据
    QByteArray ba;
    QBuffer qbbuf(&ba); //把ba绑定到buf上，操作buf就等于操作ba
    image.save( &qbbuf , "PNG" , 100 );
    qDebug()<<"进行了转换";
    int type= DEF_PACK_SENDPICTURE_RQ;
    int userid = m_id;
    int roomidandfriendid = id; // todo更改首先是知道给哪个好友发送了

    char *audioData = ba.data();
    int len = ba.size();

//    QImage img;
//    img.loadFromData( (uchar*) audioData , sizeof(int)*3 + len );

//    Q_EMIT Test( img);

    //序列化数据
    char * buf = new char[sizeof(int) * 3 + len];
    char * tmp = buf;
    *(int *) tmp = type;
    tmp += sizeof(int);

    *(int *) tmp = userid;
    tmp += sizeof(int);

    *(int *) tmp = roomidandfriendid;
    tmp += sizeof(int);



    memcpy(tmp , audioData , len );
    STRU_SENDPICTURE_NAME r;
    r.m_myid = userid;
    r.m_fid = roomidandfriendid;
    strcpy(r.Name,pathname.toStdString().c_str());
    SendData((char*)&r,sizeof(r));
    SendData( buf , sizeof(int)*3 + len);
    delete[] buf;
}
//客户端同样要修改协议映射表并在类中添加函数

//处理接收图片请求
void CKernel::slot_dealSendPictureRq(unsigned int lSendIP, char *buf, int nlen)
{

    qDebug()<<"接收到图片";

    int type;
    int userid ;
    int roomidandfriendid ;


    //反序列化数据
    char * tmp = buf;
    type = *(int *) tmp;
    tmp += sizeof(int);

    userid = *(int *) tmp;
    tmp += sizeof(int);

    roomidandfriendid = *(int *) tmp;
    tmp += sizeof(int);


    int imageLen = nlen - 3 * sizeof( int );
    //音频数据
    QImage img;
    img.loadFromData( (uchar*) tmp , imageLen );

    ChatDialog * chatwin;
    if(m_mapIdToChatdlg.count(userid)>0){
        chatwin = m_mapIdToChatdlg[userid];
        chatwin->showNormal();
    }
    qDebug()<<"获得QImage1"<<endl;

//    QFile file("./release/pic/R.png");
//    if(file.exists()){
//        file.remove();
//    }
    qDebug()<<"*****************";
    qDebug()<<recvpathName;
    qDebug()<<"收到图片";
    img.save(recvpathName , "PNG" , 100);
    Q_EMIT SIG_sendPic(img);
}
//处理回复
void CKernel::slot_dealSendPictureRs(unsigned int lSendIP, char *buf, int nlen)
{
    ChatDialog * chatwin = m_mapIdToChatdlg[m_friendid];
    chatwin->setEnableTrue();

    for(auto ite = m_mapIDToUserShow.begin() ; ite != m_mapIDToUserShow.end();/*++ite*/){
        qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!进行的回收呢";
        UserShow * user =  ite->second;
        ite = m_mapIDToUserShow.erase(ite );
        m_ChatroomDialog->slot_removeUser(user );
    }

    STRU_SENDPICTURE_RS* rs = (STRU_SENDPICTURE_RS*)buf;
    if(rs->m_result)
    {
        //todo传输成功，显示在聊天内容框中
    }
    else
    {
        //删除聊天输入框中的显示图片

        //QMessageBox::about(/*主窗口页面*/chatwin,"提示","发送图片失败");
        return;
    }
}

void CKernel::slot_dealSendPicturepathname(unsigned int lSendIP, char *buf, int nlen)
{
    //收到图片路径包
    //拆包
    STRU_SENDPICTURE_NAME *r = (STRU_SENDPICTURE_NAME*)buf;
    qDebug()<<"收到图片路径包";
    recvpathName = r->Name;
    //strcpy(recvpathName, r->Name);
    qDebug()<<recvpathName;
    Q_EMIT SIG_PathName(recvpathName);
    //同时把这个信号发给chatdialog告知其路径

}

//处理创建群组
void CKernel::slot_CreateGroup()
{
    qDebug()<<"进入处理创建群组的槽函数";
    //1、弹出窗口，让用户输入想要的添加的好友的昵称
    QString strGroupName=QInputDialog::getText(m_mychat,"创建群组","请输入组名：");
    qDebug()<<strGroupName;
    //2、校验输入昵称的合法性
    QString nameTemp=strGroupName;
    if(strGroupName.isEmpty()||nameTemp.remove(" ").isEmpty()||nameTemp.length()>10)
    {
        QMessageBox::about(m_mychat,"提示","输入的组名过长");
        return ;
    }
    STRU_SENDPICTURE_GROUP rq;
    strcpy(rq.Name,strGroupName.toStdString().c_str());
    rq.m_myid = m_id;
    SendData((char*)&rq,sizeof(rq));
    qDebug()<<"发射创建群组包";
}

//群组信息处理
void CKernel::slot_dealGroupInfo(unsigned int lSendIP, char *buf, int nlen)
{
//    qDebug()<<"收到初始化信息";
//    STRU_SENDPICTURE_GROUP* info = (STRU_SENDPICTURE_GROUP *)buf;
//    Groupitem * item=new Groupitem;
//    m_GroupUserDialog = new setUserDialog;
//    item->setInfo(info->m_Gid,info->iconid,info->Name);
//    m_mychat->addGroup(item);
//    qDebug()<<info->Name;
//    connect(item,SIGNAL(SIG_ChangeGroupInof(int))
//            ,this,SLOT(slot_ChangeGroupInof(int)));
//    connect(m_GroupUserDialog,SIGNAL(SIG_userSetCommit(int,QString,QString)),
//            this,SLOT(slot_userSetCommit(int,QString,QString)));
    //3.2.1、设置控件的内容
//    item->setInfo();
//    item->setInfo(info->m_UserID,info->m_iconid,info->state,info->m_szUser,info->m_userFeeling);
    //(int id, int iconId, int state, QString name, QString feeling)
    //3.2.2、绑定发送点击好友头像的信号和槽函数
//    connect(item,SIGNAL(SIG_UserItemClicked(int))
//            ,this,SLOT(slot_UserItemClicked(int)));
//    connect(item,SIGNAL(SIG_DeleteFriend(int)),this,SLOT(slot_DeleteFriend(int)));
    //3.2.3、把useritem添加到好友列表上
    //    m_mychat->addFriend(item);
}
//更改群组信息的槽处理函数
void CKernel::slot_ChangeGroupInof(int g_id)
{
    qDebug()<<"弹出信息更改窗口";
    m_gid = g_id;
    qDebug()<<"查看当前要操作的组的id:"<<m_gid;
    //发送信号给信息更改
    m_GroupUserDialog->show();
    //发包
    //中文兼容 也是uft8
}

#include <QFileInfo>
void CKernel::slot_sendFile(QString file_path,int id)
{
    STRU_SEND_FILE rq;
    QFile file(file_path);
    QFileInfo fileinfo(file.fileName());
    m_mapfilePath[fileinfo.fileName()] = file_path;
    strcpy_s(rq.filename,100,fileinfo.fileName().toStdString().c_str());
    rq.m_fid = id;
    rq.m_myid = m_id;
    ChatDialog * chatwin;
    if(m_mapIdToChatdlg.count(id)>0){
        chatwin = m_mapIdToChatdlg[id];
        chatwin->selectFile(QString(rq.filename));
        chatwin->showNormal();
    }
    SendData((char*)&rq,sizeof(rq));
}

void CKernel::slot_dealSendFileRq(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_SEND_FILE *rq = (STRU_SEND_FILE *)buf;
    //显示文件名
    ChatDialog * chatwin;
    if(m_mapIdToChatdlg.count(rq->m_myid)>0){
        chatwin = m_mapIdToChatdlg[rq->m_myid];
        chatwin->selectFile(QString(rq->filename));
        chatwin->showNormal();
    }
}

void CKernel::slot_dealSendFileRs(unsigned int lSendIP, char *buf, int nlen)
{
    STRU_SEND_FILE *rs = (STRU_SEND_FILE *)buf;
    //收到要接收的文件名
    QString file_path = m_mapfilePath[rs->filename];
    //发送文件数据
    QByteArray ba;
    QFile file(file_path);
    file.open(QIODevice::ReadOnly);
    ba = file.readAll();
    file.close();
    int type= DEF_PACK_RECV_FILE;
    int userid = m_id;
    int roomidandfriendid = rs->m_myid; // todo更改首先是知道给哪个好友发送了

    char *audioData = ba.data();
    int len = ba.size();
    //序列化数据
    char * buf1 = new char[sizeof(int) * 3 + len];
    char * tmp = buf1;
    *(int *) tmp = type;
    tmp += sizeof(int);

    *(int *) tmp = userid;
    tmp += sizeof(int);

    *(int *) tmp = roomidandfriendid;
    tmp += sizeof(int);

    memcpy(tmp , audioData , len );
    SendData( buf1 , sizeof(int)*3 + len);
    delete[] buf1;
}

void CKernel::slot_recvFile(unsigned int lSendIP, char *buf, int nlen)
{
    //使用默认路径，接受文件数据
    int type;
    int userid ;
    int roomidandfriendid ;

    //反序列化数据
    char * tmp = buf;
    type = *(int *) tmp;
    tmp += sizeof(int);

    userid = *(int *) tmp;
    tmp += sizeof(int);

    roomidandfriendid = *(int *) tmp;
    tmp += sizeof(int);

    int fileLen = nlen - 3 * sizeof( int );

    //保存文件
    QString file_path = QFileDialog::getExistingDirectory(m_mychat, "请选择保存目录", "/")+"/"+m_tfilename;
    m_hashALFilePath[m_tfilename] = file_path;
    QFile file;
    file.setFileName(file_path);
    file.open(QIODevice::WriteOnly);
    file.write(tmp,fileLen);
    file.close();
}

void CKernel::slot_selectFile(QString fileName, int id)
{
    STRU_SEND_FILE rs;
    //选择要接收的文件
    strcpy_s(rs.filename, 100, fileName.toStdString().c_str());
    rs.m_fid = id;
    rs.m_myid = m_id;
    rs.m_type = DEF_PACK_SEND_FILE_RS;
    m_tfilename = fileName;
    SendData((char*)&rs,sizeof(rs));
}

#include<QProcess>
void CKernel::slot_openSaveFileDir(QString fileName, int id)
{
    QString file_path = m_hashALFilePath[fileName];
    ChatDialog * chatwin;
    if(m_mapIdToChatdlg.count(id)>0){
        chatwin = m_mapIdToChatdlg[id];
        chatwin->showNormal();
    }
    file_path.replace("/", "\\");    //win32下替换斜杠
    QProcess process;
    process.startDetached("explorer", QStringList() << QString("/select,") << QString("%1").arg(file_path));
}

void CKernel::DealShowBQ(int m_id)
{
}

void DealShowBQ(int m_id){


}

