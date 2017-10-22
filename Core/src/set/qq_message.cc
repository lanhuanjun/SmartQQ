//
// Created by lan on 16-9-8.
//

#include "qq_message.h"

qq::SendQQMessage::SendQQMessage(uint64 uin,
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

qq::SendQQMessage::~SendQQMessage() {

}

std::string qq::SendQQMessage::PackageMessage() {
    return message.toJsonString();;
}

qq::SendQQMessage &qq::SendQQMessage::SetFontName(std::string fontName) {
    message.content.font.name = fontName;
    return *this;
}

qq::SendQQMessage &qq::SendQQMessage::SetFontSize(int size) {
    message.content.font.size = size;
    return *this;
}

qq::SendQQMessage &qq::SendQQMessage::SetFontColor(std::string fontColor) {
    message.content.font.color = fontColor;
    return *this;
}

qq::MessageType qq::SendQQMessage::GetMessageType() {
    return message.msg_type;
}

qq::SendQQMessage &qq::SendQQMessage::SetPsessionid(std::string psessionid) {
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

const qq::MessageContent  qq::ReceiveMessage::GetQQMessage() const
{
	// TODO: 在此处插入 return 语句
	return message.content;
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
