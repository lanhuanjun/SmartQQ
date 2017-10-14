//
// Created by lan on 16-9-6.
//

#ifndef QQCORE_QQ_FRIEND_H
#define QQCORE_QQ_FRIEND_H

#include <string>
#include <map>
#include "../other/platform.h"

namespace qq {

    /**
     * 朋友分组信息
     */
    struct FriendGroup {
        int index = 0;//显示序号
        int sort = 0;//内部序号
        std::string name = "";//分组名称
    };
    /**
     * 单个朋友信息
     */
    struct FriendInfo {
        uint64 id = 0;//uin,非qq号码
        int group_index = 0;//所在分组序号
        int face = 0;//头像id
        std::string nick_name = "";//qq昵称
        std::string mark_name = "";//备注名称
        bool is_vip = false;//是否为qq会员
        int vip_level = 0;//vip等级
    };
    struct FriendBaseInfo{
        std::unordered_map<int,FriendGroup> friendGroups;
        std::unordered_map<uint64,FriendInfo> friendInfos;
    };
    /**
     * 群的基本信息
     */
    struct GroupInfo{
       std::string name = "";
        uint64 id = 0;
        uint64 code = 0;
    };
    /**
     * 群的基本信息
     */
    struct DiscusInfo{
        std::string name = "";
        uint64 id = 0;
    };
    /**
     * 一个QQ的详细信息
     */
    struct QQInfo{
        uint64 id = 0;
        int allow = 0;
        int year = 0;
        int month = 0;
        int day = 0;
        int blood = 0;
        std::string city = "";
        std::string collage = "";
        int constel = 0;
        std::string country = "";
        std::string email = "";
        int face = 0;
        std::string gender = "";
        std::string homepage = "";
        std::string mobile = "";
        std::string nick = "";
        std::string occupation = "";
        std::string personal = "";
        std::string phone = "";
        std::string province = "";
        int shengxiao = 0;
        int stat = 0;
        int vip_info = 0;
    };

    /**
     * 最近聊天的
     */
    struct RecentItem{
        int type = 0;
        uint64 id = 0;
        std::string status = "";
    };
    /**
     * QQ所有状态
     */
    enum QQStatus{ONLINE,AWAY,CALLME,BUSY,SLIENT,HIDDEN,OFFLINE};
    /**
     * 在线好友列表
     */
    struct FriendOnLine{
        int client_type = 0;
        std::string status = "";
        uint64 id = 0;
    };
    /**
     * 群成员详细信息
     */
    struct GroupMem{
        uint64 uin = 0;
        std::string card = "";
        int flag = 0;
        std::string nick = "";
        std::string province = "";
        std::string gender = "";
        std::string contry = "";
        std::string city = "";
        int client_type = 0;
        int stat = 0;
        int is_vip = 0;
        int vip_level = 0;
    };
    /**
     * 群的详细信息
     */
    struct GroupDetailInfo{
        std::map<uint64 ,GroupMem> members;
        int class_ = 0;
        uint64 code = 0;
        uint64 create_time = 0;
        int face = 0;
        std::string fingermemo = "";
        uint64  flag = 0;
        uint64 id = 0;
        int level = 0;
        std::string memo = "";
        std::string name = "";
        int option = 0;
        uint64 owner = 0;
    };
    /**
     * 讨论组成员信息
     */
    struct DiscusMem{
        uint64 uin = 0;
        uint64 ruin = 0;
        std::string nick = "";
    };
    /**
     * 讨论组的详细信息
     */
    struct DiscusDetailInfo{
        uint64 did = 0;
        std::string name = "";
        std::map<uint64 , DiscusMem> mems;
    };
    /**
     * 消息类型
     */
    enum MessageType{USER,GROUP,DISCUS,UNKNOW_TYPE};
};

#endif //QQCORE_QQ_FRIEND_H
