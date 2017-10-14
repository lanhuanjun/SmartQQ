//
// Created by lan on 16-9-2.
//

#ifndef QQCORE_HTTP_CLIENT_H
#define QQCORE_HTTP_CLIENT_H

#include <string>
#include <list>
#include <unordered_map>
#include <cassert>
#include "../other/platform.h"
#include "../../../third/include/curl/curl.h"
#include "../other/easylogging++.h"


namespace qq{
	typedef std::pair<std::string, std::string> Header;
	typedef std::pair<std::string, std::string> Cookie;
	typedef std::pair<std::string, std::string> Field;
	typedef std::unordered_map<std::string, std::string> QQSession;
	struct Response
	{
		int m_code;
		std::string m_data;
		std::unordered_map<std::string, std::string> m_cookies;
		std::unordered_map<std::string, std::string> m_headers;
		Response():m_code(0)
		{}

		void Reset()
		{
			m_code = 0;
			m_data.clear();
			m_cookies.clear();
			m_headers.clear();
		}
	};
    class HttpClient {
    public:
        HttpClient();
        ~HttpClient();
        HttpClient *GetDefaultClient();
    private:
		//数据存储
		struct DataBuf
		{
			char * m_data;//数据存储
			size_t m_data_size;//数据大小
			size_t m_store_size;//存储大小

			DataBuf() :m_data(NULL), m_data_size(0), m_store_size(0) {}
			void Resize(const size_t size)
			{
				m_data = static_cast<char*>(realloc(m_data, size * sizeof(char)));
				assert(nullptr != m_data);
				m_store_size = size;
			}

			void Reset()
			{
				if(nullptr != m_data)
				{
					memset(m_data, 0, m_store_size);
				}
				m_data_size = 0;
			}
			virtual ~ DataBuf()
			{
				if(nullptr != m_data)
				{
					free(m_data);
				}
			}
		};
    private:
        CURL *m_handle;
        std::string m_url;
        std::list<Header> m_default_headers;
        std::list<Header> m_temp_headers;
        std::list<Cookie> m_send_cookies;
        std::list<Field> m_post_fields;
		//设置内容请求数据缓冲区
		DataBuf *m_content_buf;
		//设置响应头缓冲区
		DataBuf *m_header_buf;

		size_t kDefaultHeaderBufSize = 1024;
		size_t kDefaultContentBufSize = 1024 * 3;

		Response m_res;

    public:
        enum RequestMethod{GET,POST};
    public:
        /**
         * 初始化客户端
         * @return 成功：true;失败:false
         */
        bool Init();
        /**
         * 设置请求地址
         * @param url 请求地址
         */
        void SetUrl(const std::string url);
        /**
         * 设置默认请求头,整个生命周期不会被清空
         * @param header
         */
        void SetDefaultHeader(Header header);
        /**
         * 设置临时请求头,请求一次会失效
         * @param header
         */
        void SetTempHeader(Header header);
        /**
         * 设置请求的Cookie
         * @param cookie
         */
        void SetSendCookies(Cookie cookie);
        /**
         * 设置请求域
         * @param field
         */
        void SetPostField(Field field);
        /**
         * 执行请求
         * @param method 请求方式 Get和Posts
         * @return 成功：true;失败:false
         */
        bool Execute(RequestMethod method = GET);
        /**
         * 对指定字符实行编码
         * @param str
         * @return
         */
        std::string URLEncoded(std::string str);
        /**
         * 对数据进行反编码
         * @param data
         * @param size
         * @return
         */
        std::string URLUnEncoded(const char * data,int size);

		const Response * GetResponse()const;
        /**
         * 关闭客户端
         */
        void Destroy();

    private:
		void ParseResponseData();
		/*
		 * 设置请求数据
		 */
		void SetRequestData(curl_slist *headers, RequestMethod method = GET);
		/*
		解析请求头
		*/
		void ParserHeader();

		void ParserOneCookie(const std::string & str);
        /**
         * 访问内容返回的回调函数
         * @param rev 数据内存指针
         * @param size 数据字符大小
         * @param count 数据字符个数
         * @param store 用户的数据结构体
         * @return
         */
        static size_t ContentCallback(char *rev, size_t size, size_t count, DataBuf *store);
		/**
		* 访问内容返回的回调函数
		* @param rev 数据内存指针
		* @param size 数据字符大小
		* @param count 数据字符个数
		* @param store 用户的数据结构体
		* @return
		*/
		static size_t HeaderCallback(char *rev, size_t size, size_t count, DataBuf *store);
    };
}



#endif //QQCORE_HTTP_CLIENT_H
