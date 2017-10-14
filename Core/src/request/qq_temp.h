//
// Created by lan on 16-9-7.
//

#ifndef QQCORE_QQ_TEMP_H
#define QQCORE_QQ_TEMP_H

#include "../net/http_client.h"
#include "../set/qq_info.h"
#include "../set/qq_message.h"
#include <iostream>
#include <unordered_map>

namespace qq{
    class QQTemp {
    public:
        QQTemp(HttpClient * client);
        ~QQTemp();

	private:
		HttpClient *CLIENT;

    public:
        /**
         * 获取所有在线好友列表
         * @param onlines
         * @return
         */
        bool GetOnLineBuddies(QQSession &session,std::list<FriendOnLine> &onlines);
        /**
         * 获取QQ号码
         * @param id 传入uin
         * @return 失败返回-1
         */
        bool GetFriendQQNum(QQSession &session, uint64 uin , uint64 &qq_num);
        /**
         * 获取好友签名
         * @param uin
         * @return
         */
        bool GetSingleLongNick(QQSession &session,uint64 uin ,std::string &long_nick);
        /**
         * 获取自己的信息
         * @return
         */
        bool GetSelfInfo(QQSession &session,QQInfo &qi);
        /**
         * 获取qq号码信息
         * @param qi
         * @return
         */
        bool GetQQInfo(QQSession &session,uint64 uin, QQInfo &qi);
        /**
         * 获取一个群的详细信息
         * @param uin
         * @param groupDetailInfo
         * @return
         */
        bool GetGroupDetailInfo(QQSession &session, uint64 uin, GroupDetailInfo &groupDetailInfo);
        /**
         * 获取讨论组的详细信息
         * @param did
         * @param dm
         * @return
         */
        bool GetDiscusDetailInfo(QQSession &session,uint64 did,DiscusDetailInfo & ddi);
        /**
         * 获取用户的头像
         * @param uin 传入uin
         * @param size 返回数据大小
         * @return 返回数据指针，为空则不成功
         */
        bool GetUserFace(QQSession &session,uint64 uin,std::string &data);
        /**
         * 更换qq状态
         * @param status
         * @return 是否更换成功
         */
        bool ChangeStatus(QQSession &session,QQStatus status);
        /**
         * 发送消息
         * @param sendMessage
         * @return
         */
        bool SendOneMessage(QQSession &session,SendMessage sendMessage);
    private:
        /**
         * 解析在线好友列表
         * @param onlines
         * @return
         */
        bool ParseOnLineBuddies(std::string &json,std::list<FriendOnLine> &onlines);

        /**
          * 解析一个群的详细信息
          * @param uin
          * @param groupDetailInfo
          * @return
          */
        bool ParseGroupDetailInfo(std::string &json, GroupDetailInfo &groupDetailInfo);
        /**
         * 解析一个讨论组的详细信息
         * @param json
         * @param dm
         * @return
         */
        bool ParseDiscusDetailInfo(std::string &json,DiscusDetailInfo &ddi);
    protected:
        /**
         * 解析一个好友的详细信息
         * @param json
         * @param qi
         * @return
         */
        bool ParseQQInfo(const std::string &json,QQInfo &qi);
    };
};



#endif //QQCORE_QQ_TEMP_H
