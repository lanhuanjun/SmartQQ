//
// Created by lan on 16-9-5.
//

#ifndef QQCORE_QQ_CONTACT_H
#define QQCORE_QQ_CONTACT_H

#include "qq_login.h"
#include "../set/qq_info.h"

namespace qq{

	typedef std::unordered_map<uint64, GroupInfo> GroupMap;
	typedef std::unordered_map<uint64, DiscusDetailInfo> DiscusMap;
	typedef std::unordered_map<uint64, RecentItem> RecentMap;
	typedef std::unordered_map<int, FriendGroup> FriendGroupMap;
	typedef std::unordered_map<uint64, FriendInfo> FriendInfoMap;
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
        bool GetGroupNameList(QQSession &session, GroupMap &groupMap);
        /**
         * 获取讨论组列表
         * @return
         */
        bool GetDicusList(QQSession &session,DiscusMap &discusMap);


        /**
         * 获取历史聊天记录列表
         * @return
         */
        bool GetRecentList(QQSession &session,RecentMap &recentMap);

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
        bool PaserUserFriendsJson(const std::string &json,FriendGroupMap &friendGroups,FriendInfoMap &friendInfos);

        /**
         * 解析获取群列表时得到的Json数据
         * @param json 响应的Json数据
         * @return 是否解析成功
         */
        bool PaserGroupNameListJson(const std::string &json,GroupMap &groupInfos);
        /**
         * 解析获取讨论组列表时得到的Json数据
         * @param json 响应的Json数据
         * @return 是否解析成功
         */
        bool PaserDiscusListJson(const std::string &json,DiscusMap &discusInfos);

        /**
         * 解析获取历史聊天列表时得到的Json数据
         * @param json 响应的Json数据
         * @return 是否解析成功
         */
        bool PaserRecentListJson(const std::string &json,RecentMap &recentList);
    };
};



#endif //QQCORE_QQ_CONTACT_H
