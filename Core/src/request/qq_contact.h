//
// Created by lan on 16-9-5.
//

#ifndef QQCORE_QQ_CONTACT_H
#define QQCORE_QQ_CONTACT_H

#include "qq_login.h"
#include "../set/qq_info.h"

namespace qq{
    class QQContact {
    public:
        QQContact(HttpClient *client);
        ~QQContact();
    private:
		HttpClient *CLIENT;
    public:
        /**
         * 获取好友列表，分组信息
         * @return
         */
        bool GetUserFriends(QQSession &session,FriendBaseInfo &friendBaseInfo);
        /**
         * 获取群列表
         * @return
         */
        bool GetGroupNameList(QQSession &session,std::unordered_map<uint64 ,GroupInfo> &groupList);
        /**
         * 获取讨论组列表
         * @return
         */
        bool GetDicusList(QQSession &session,std::unordered_map<uint64 ,DiscusDetailInfo> &discusList);


        /**
         * 获取历史聊天记录列表
         * @return
         */
        bool GetRecentList(QQSession &session,std::unordered_map<uint64 ,RecentItem> &recentList);

    private:
        /**
         * 获取hash
         * @param uin 传入qq号码
         * @param ptwebqq 传入ptwebqq
         * @return
         */
        const std::string GetHash(const long & uin,const std::string &ptwebqq);
        /**
         * 解析获取好友列表时得到的Json数据
         * @param json 响应的Json数据
         * @return 是否解析成功
         */
        bool PaserUserFriendsJson(const std::string &json,std::unordered_map<int,FriendGroup> &friendGroups,std::unordered_map<uint64,FriendInfo> &friendList);

        /**
         * 解析获取群列表时得到的Json数据
         * @param json 响应的Json数据
         * @return 是否解析成功
         */
        bool PaserGroupNameListJson(const std::string &json,std::unordered_map<uint64 ,GroupInfo> &groupInfos);
        /**
         * 解析获取讨论组列表时得到的Json数据
         * @param json 响应的Json数据
         * @return 是否解析成功
         */
        bool PaserDiscusListJson(const std::string &json,std::unordered_map<uint64 ,DiscusDetailInfo> &discusInfos);

        /**
         * 解析获取历史聊天列表时得到的Json数据
         * @param json 响应的Json数据
         * @return 是否解析成功
         */
        bool PaserRecentListJson(const std::string &json,std::unordered_map<uint64 ,RecentItem> &recentList);
    };
};



#endif //QQCORE_QQ_CONTACT_H
