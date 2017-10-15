//
// Created by lan on 16-9-7.
//

#ifndef QQCORE_QQ_CONTROL_H
#define QQCORE_QQ_CONTROL_H

#include <thread>

#include "request/qq_login.h"
#include "request/qq_contact.h"
#include "request/qq_temp.h"
#include "request/qq_poll.h"
#include "net/http_client.h"

namespace qq{
    class QQControl{
    public:
        QQControl();
        ~QQControl();

    private:
        HttpClient *CLIENT;
        QQLogin * m_login;
        QQContact *m_contact;
        QQTemp *m_temp;
		QQPoll *m_poll;
		QQSession m_session;
		
    public:
        /**
         * 获取登陆的二维码图片
         * @param size
         * @return
         */
        bool GetQRCImg(std::string & data);
        /**
         * 登陆QQ
         * @param Listener消息回调函数
         * @return
         */
        bool LoginQQ(void Listener(QRC_Code,std::string));
        /**
         * 获取登陆需要的信息
         * @param url
         * @return
         */
        bool GetControlNeed(std::string url);
        /////////////////////////////////////////
        /**
         * 获取好友列表，分组信息
         * @return
         */
        bool GetUserFriends(FriendBaseInfo &friendBaseInfo);
        /**
         * 获取群列表
         * @return
         */
        bool GetGroupNameList(GroupMap &groups);
        /**
         * 获取讨论组列表
         * @return
         */
        bool GetDicusList(DiscusMap &discuss);

        /**
         * 获取历史聊天记录列表
         * @return
         */
        bool GetRecentList(RecentMap &recents);

        //////////////////////////////

        /**
         * 获取所有在线好友列表
         * @param onlines
         * @return
         */
        bool GetOnLineBuddies(std::list<FriendOnLine> &onlines);
        /**
         * 获取QQ号码
         * @param id 传入uin
         * @return 失败返回-1
         */
        uint64 GetFriendQQNum(uint64 uin);
        /**
         * 获取好友签名
         * @param uin
         * @return
         */
        std::string GetSingleLongNick(uint64 uin);
        /**
         * 获取自己的信息
         * @return
         */
        bool GetSelfInfo(QQInfo &self_info);
        /**
         * 获取qq号码信息
         * @param qi
         * @return
         */
        bool GetQQInfo(uint64 uin,QQInfo &info);
        /**
         * 获取一个群的详细信息
         * @param gcode 非uin
         * @param groupDetailInfo
         * @return
         */
        bool GetGroupDetailInfo(uint64 gcode, GroupDetailInfo &groupDetailInfo);
        /**
         * 获取讨论组的详细信息
         * @param did
         * @param dm
         * @return
         */
        bool GetDiscusDetailInfo(uint64 did,DiscusDetailInfo & ddi);
        /**
         * 获取用户的头像
         * @param uin 传入uin
         * @param size 返回数据大小
         * @return 返回数据指针，为空则不成功
         */
        bool GetUserFace(uint64 uin,std::string & data);
        /**
         * 更换qq状态
         * @param status
         * @return 是否更换成功
         */
        bool ChangeStatus(QQStatus status);
        /**
         * 轮询消息
         * @return
         */
        bool StartPoll(bool receiveMessageListener(bool hasMessage,ReceiveMessage &receiveMessage));
        /**
         * 发送消息
         * @param sendMessage
         * @return
         */
        bool SendOneMessage(SendMessage &sendMessage);
    };
};
#endif //QQCORE_QQ_CONTROL_H
