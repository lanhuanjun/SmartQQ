#ifndef QQCORE_BASE_H_
#define QQCORE_BASE_H_
#include "platform.h"
#include <memory>
#include <sstream>
namespace qq{
	inline std::string JsonNodeToString(Json::Value root)
	{
		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "None";
		builder["indentation"] = "";
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		std::stringstream ssm;
		writer->write(root, &ssm);
		return ssm.str();
	}

	inline bool StringToJsonValue(Json::Value &root, const std::string &str)
	{
		Json::CharReaderBuilder builder;
		std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

		JSONCPP_STRING errs;
		
		bool is_ok = reader->parse(str.begin()._Ptr,str.end()._Ptr,&root,&errs);
		return errs.empty() && is_ok;
	}

	inline int Hash33(std::string str)
	{
		int e = 0;
		for (char c : str)
		{
			e += (e << 5) + (int)c;
		}
		return 0x7fffffff & e;
	}
}
#endif // !QQCORE_BASE_H_

