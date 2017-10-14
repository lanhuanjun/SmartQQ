//
// Created by lan on 16-9-8.
//

#include "qq_message.h"

qq::SendMessage::SendMessage(uint64 uin,
                                  uint64 msg_id,
                                  int face,
                                  qq::MessageType type,
                                  std::vector<std::pair<ContentMessageType,std::string>> msg) {
    message.id = uin;
    message.msg_id = msg_id;
    message.face = face;
    message.msg_type = type;
    message.content.msg = msg;

}

qq::SendMessage::~SendMessage() {

}

std::string qq::SendMessage::PackageMessage() {
    return message.toJsonString();;
}

qq::SendMessage &qq::SendMessage::SetFontName(std::string fontName) {
    message.content.font.name = fontName;
    return *this;
}

qq::SendMessage &qq::SendMessage::SetFontSize(int size) {
    message.content.font.size = size;
    return *this;
}

qq::SendMessage &qq::SendMessage::SetFontColor(std::string fontColor) {
    message.content.font.color = fontColor;
    return *this;
}

qq::MessageType qq::SendMessage::GetMessageType() {
    return message.msg_type;
}

qq::SendMessage &qq::SendMessage::SetPsessionid(std::string psessionid) {
    message.psessionid = psessionid;
    return *this;
}

qq::ReceiveMessage::ReceiveMessage() {
}

qq::ReceiveMessage::~ReceiveMessage() {

}

void qq::ReceiveMessage::ParseMessage(Json::Value result) {
    result = result[0];
    std::string poll_type = result["poll_type"].asString();
    Json::Value value = result["value"];
    if("message" == poll_type){
        message.messageType = USER;
        message.from_uin = value["from_uin"].asUInt64();
    } else if("group_message" == poll_type){
        message.messageType = GROUP;
        message.from_uin = value["send_uin"].asUInt64();
        message.id = value["from_uin"].asUInt64();
    } else if("discu_message" == poll_type){
        message.messageType = DISCUS;
        message.from_uin = value["send_uin"].asUInt64();
        message.id = value["from_uin"].asUInt64();
    } else{
        message.messageType = UNKNOW_TYPE;
    }
    message.msg_id = value["msg_id"].asUInt64();
    message.time = value["time"].asUInt64();
    message.to_uin = value["to_uin"].asUInt64();
    Json::Value msgContent = value["content"];
    //解析字体类型
    Json::Value fontValue = msgContent[0];
    fontValue = fontValue[1];
    message.content.font.name = fontValue["name"].asString();
    message.content.font.color = fontValue["color"].asString();
    message.content.font.size = fontValue["size"].asInt();
    Json::Value fonstyle = fontValue["style"];
    message.content.font.fontStyle.x = fonstyle[0].asInt();
    message.content.font.fontStyle.y = fonstyle[1].asInt();
    message.content.font.fontStyle.z = fonstyle[2].asInt();
    //解析消息
    int msgContentCount = msgContent.size();
    for(int i = 1 ; i < msgContentCount;++i){
        Json::Value msgType = msgContent[i];
        if(msgType.isString()){
            message.content.msg.push_back(std::make_pair(STRING,msgType.asString()));
        }else{
            std::stringstream ssm;
            ssm << msgType[1].asInt();
            message.content.msg.push_back(std::make_pair(FACE,ssm.str()));
        }
    }
    return ;
}
qq::MessageType qq::ReceiveMessage::GetMessageType() {
    return message.messageType;
}
std::vector<std::pair<qq::ContentMessageType,std::string>> qq::ReceiveMessage::GetMessage() {
    return message.content.msg;
}

uint64 qq::ReceiveMessage::GetId() {
    return message.id;
}

uint64 qq::ReceiveMessage::GetFromUin() {
    return message.from_uin;
}

uint64 qq::ReceiveMessage::GetMessageId() {
    return message.msg_id;
}

uint64 qq::ReceiveMessage::GetReceiveTime() {
    return message.time;
}

uint64 qq::ReceiveMessage::GetToUin() {
    return message.to_uin;
}


qq::QQMessage::QQMessage() {

}

qq::QQMessage::~QQMessage() {

}
