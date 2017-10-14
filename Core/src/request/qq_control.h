//
// Created by lan on 16-9-7.
//

#ifndef QQCORE_QQ_CONTROL_H
#define QQCORE_QQ_CONTROL_H

#include <thread>

#include "qq_login.h"
#include "qq_contact.h"
#include "http_client.h"
#include "qq_temp.h"
#include "qq_set/log_ini.h"

namespace qq{
    class QQControl{
    public:
        QQControl(Log &log);
        ~QQControl();

    private:
        HttpClient *client_;
        QQLogin * qqLogin_;
        QQContact *qqContact_;
        QQTemp *qqTemp_;
        map<string,Header> need_;
        Log *log_ = NULL;
    public:
        /**
         * 获取登陆的二维码图片
         * @param size
         * @return
         */
        const char* GetQRCImg(int &size);
        /**
         * 将数据保存到文件中
         * @param filePath
         * @param data
         * @param size
         * @return
         */
        bool SaveImgToFile(string filePath,const char * data,int size);
        /**
         * 登陆QQ
         * @param Listener消息回调函数
         * @return
         */
        bool LoginQQ(void Listener(QQLogin::QRC_Code status,string msg));
        /**
         * 获取登陆需要的信息
         * @param url
         * @return
         */
        bool GetControlNeed(string url);
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
        bool GetGroupNameList(std::map<u_int64_t ,GI> &groupList);
        /**
         * 获取讨论组列表
         * @return
         */
        bool GetDicusList(std::map<u_int64_t ,DI> &discusList);

        /**
         * 获取历史聊天记录列表
         * @return
         */
        bool GetRecentList(std::map<u_int64_t ,RI> &recentList);

        //////////////////////////////

        /**
         * 获取所有在线好友列表
         * @param onlines
         * @return
         */
        bool GetOnLineBuddies(list<FriendOnLine> &onlines);
        /**
         * 获取QQ号码
         * @param id 传入uin
         * @return 失败返回-1
         */
        u_int64_t GetFriendQQNum(u_int64_t uin);
        /**
         * 获取好友签名
         * @param uin
         * @return
         */
        string GetSingleLongNick(u_int64_t uin);
        /**
         * 获取自己的信息
         * @return
         */
        bool GetSelfInfo(QI &qi);
        /**
         * 获取qq号码信息
         * @param qi
         * @return
         */
        bool GetQQInfo(u_int64_t uin,QI &qi);
        /**
         * 获取一个群的详细信息
         * @param gcode 非uin
         * @param groupDetailInfo
         * @return
         */
        bool GetGroupDetailInfo(u_int64_t gcode, GroupDetailInfo &groupDetailInfo);
        /**
         * 获取讨论组的详细信息
         * @param did
         * @param dm
         * @return
         */
        bool GetDiscusDetailInfo(u_int64_t did,DDI & ddi);
        /**
         * 获取用户的头像
         * @param uin 传入uin
         * @param size 返回数据大小
         * @return 返回数据指针，为空则不成功
         */
        const char *GetUserFace(u_int64_t uin,int &size);
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
        bool Poll(bool receiveMessageListener(bool hasMessage,ReceiveMessage &receiveMessage));
        /**
         * 发送消息
         * @param sendMessage
         * @return
         */
        bool SendOneMessage(SendMessage &sendMessage);
    };
};
#endif //QQCORE_QQ_CONTROL_H
