//
// Created by lan on 16-9-2.
//

#ifndef QQCORE_QQ_LOGIN_H
#define QQCORE_QQ_LOGIN_H

#include "../net/http_client.h"
#include "../../../third/include/json/json.h"

#include <iostream>

#include <fstream>
#include <unordered_map>
namespace qq{
	enum QRC_Code { SUCCESS = 0, INVALID = 65, VALID = 66, SCAN = 67, UNKNOW_ERROR = 248 };

    class QQLogin {
    public:
        QQLogin(HttpClient *client);
        ~QQLogin();
    private:
        HttpClient *CLIENT;
		QQSession m_session;
    public:

        /**
         * 获取登陆信令
         * @return
         */
        bool GetLoginSig();
        /**
         * 获取二维码图片
         * @param size 数据大小
         * @return 二维码图片数据，为空：NULL
         */
        bool GetQRC(std::string &store);
        /**
         * 检查二维码
         * @param listener 回调函数 std::string msg:响应消息，响应码
         * @return 是否登陆成功
         */
        bool CheckQRC(void listener(QRC_Code,std::string));
        /**
         * 检查登陆信令
         * @param url 将上一步得到的msg地址传入
         * @return
         */
        bool CheckSig(std::string url);
        /**
         * 获取vfwebqq cookie
         * @return
         */
        bool GetVFWebqq();
        /**
         * 以上正确才可执行此函数
         * @return
         */
        bool Login();
        /**
         * 获取登陆后得到的有用信息
         * 包括p_skey,uin,psessionid,cip
         * @return
         */
		QQSession GetSession();
    private:
        /**
         * 解析检查二维码响应获取到的数据
         * @param msg 要解析的消息
         * @return returnMsg 返回消息
         * @return 返回二维码状态
         */
        QRC_Code ParseCheckQRC(const std::string &msg,std::string &returnMsg);
    };
};

#endif //QQCORE_QQ_LOGIN_H
