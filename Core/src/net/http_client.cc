#include "http_client.h"

qq::HttpClient::HttpClient() {

}

qq::HttpClient::~HttpClient() {
	Destroy();
}
qq::HttpClient *qq::HttpClient::GetDefaultClient() {
    if(!Init()){
        return nullptr;
    }
    curl_easy_setopt(m_handle,CURLOPT_TCP_KEEPALIVE,1L);
    curl_easy_setopt(m_handle,CURLOPT_ACCEPT_ENCODING,"gzip");
    curl_easy_setopt(m_handle,CURLOPT_USERAGENT,"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36");
    return this;
}
bool qq::HttpClient::Init() {
    m_handle = curl_easy_init();
    if(!m_handle){
        return false;
    }
	m_content_buf = new DataBuf();
	m_content_buf->Resize(kDefaultContentBufSize);
	m_header_buf = new DataBuf();
	m_header_buf->Resize(kDefaultHeaderBufSize);
    curl_easy_setopt(m_handle,CURLOPT_SSL_VERIFYPEER, false);//绕过证书
    curl_easy_setopt(m_handle,CURLOPT_SSL_VERIFYHOST,true);
    curl_easy_setopt(m_handle,CURLOPT_TIMEOUT,100L);//设置请求超时
    curl_easy_setopt(m_handle,CURLOPT_WRITEFUNCTION,ContentCallback);
    curl_easy_setopt(m_handle,CURLOPT_WRITEDATA,m_content_buf);
	curl_easy_setopt(m_handle,CURLOPT_HEADERFUNCTION, HeaderCallback);
	curl_easy_setopt(m_handle,CURLOPT_HEADERDATA, m_header_buf);
    curl_easy_setopt(m_handle,CURLOPT_FOLLOWLOCATION,0L);//禁止重定向
	curl_easy_setopt(m_handle, CURLOPT_ACCEPT_ENCODING, "gzip");

    return true;
}

void qq::HttpClient::SetUrl(const std::string url) {
	this->m_url.assign(url);
}

void qq::HttpClient::SetDefaultHeader(Header header) {
    this->m_default_headers.push_back(header);
}

void qq::HttpClient::SetTempHeader(Header header) {
    this->m_temp_headers.push_back(header);
}

void qq::HttpClient::SetSendCookies(Cookie cookie) {
    this->m_send_cookies.push_back(cookie);
}

void qq::HttpClient::SetPostField(Field field) {
    m_post_fields.push_back(field);
}
bool qq::HttpClient::Execute(RequestMethod method) {
	m_res.Reset();
    if(!m_handle ){
		LOG(ERROR) <<"qq::HttpClient::Execute m_handle is null";
        return false;
    }
	curl_slist *headers = nullptr;
	SetRequestData(headers, method);
    //执行请求
    CURLcode ret = curl_easy_perform(m_handle);
	//复位响应数据
	m_res.Reset();
	ParseResponseData();
    if(CURLE_OK == ret){
		m_content_buf->m_data_size = 0;
		m_header_buf->m_data_size = 0;
    }
	else
	{
		LOG(ERROR) << "qq::HttpClient::Execute response err. code:"<< ret << " "<< curl_easy_strerror(ret);
	}
	if(nullptr != headers)
	{
		curl_slist_free_all(headers);
	}
    return CURLE_OK == ret;
}


void qq::HttpClient::Destroy() {
	if (m_handle) {
		curl_easy_cleanup(m_handle);
		curl_global_cleanup();
	}
    m_handle = nullptr;
	if(nullptr != m_content_buf)
	{
		delete m_content_buf;
	}
	if(nullptr != m_header_buf)
	{
		delete m_header_buf;
	}
	
}

void qq::HttpClient::ParseResponseData()
{
	if (0 != m_content_buf->m_data_size)
	{
		m_res.m_data.assign(m_content_buf->m_data, m_content_buf->m_data_size);
	}
	ParserHeader();
}

void qq::HttpClient::SetRequestData(curl_slist*headers,RequestMethod method)
{
	
	curl_easy_setopt(m_handle, CURLOPT_URL, m_url.c_str());//设置请求地址
	//设置请求方式
	if (RequestMethod::GET == method) {
		curl_easy_setopt(m_handle, CURLOPT_HTTPGET,true);
	}
	else {
		if (!m_post_fields.empty()){
			std::string buf;
			for (auto &item : m_post_fields) {
				buf.append(item.first).append(1,'=').append(item.second).append(1,'&');
			}
			buf[buf.length() - 1] = '\0';
			curl_easy_setopt(m_handle, CURLOPT_POSTFIELDSIZE, buf.length() - 1);
			curl_easy_setopt(m_handle, CURLOPT_COPYPOSTFIELDS, buf.c_str());
		}
		curl_easy_setopt(m_handle, CURLOPT_POST, true);
	}
	//设置请求头
	for (auto &item : m_default_headers) {
		std::string buf;
		buf.append(item.first).append(1,':').append(item.second);
		headers = curl_slist_append(headers, buf.c_str());
	}
	for (auto &item : m_temp_headers) {
		std::string buf;
		buf.append(item.first).append(1,':').append(item.second);
		headers = curl_slist_append(headers, buf.c_str());
	}
	if (nullptr != headers)
	{
		curl_easy_setopt(m_handle, CURLOPT_HTTPHEADER, headers);
	}
	//设置请求Cookies
	std::string cookies;
	for (auto &item : m_send_cookies) {
		cookies.append(item.first).append(1,'=').append(item.second).append(1,';');
	}
	if (!cookies.empty()) {
		cookies[cookies.length() - 1] = '\0';
		curl_easy_setopt(m_handle, CURLOPT_COOKIE, cookies.c_str());
		LOG(DEBUG) << "send cookies:" << cookies << std::endl;
	}
	//清空所有数据
	m_url.clear();
	m_post_fields.clear();
	m_temp_headers.clear();
	m_send_cookies.clear();
}

void qq::HttpClient::ParserHeader()
{
	if (0 == m_header_buf->m_data_size)
	{
		LOG(ERROR) << "HttpClient::ParserHeader data size is 0";
		return;
	}
	std::string str_header(m_header_buf->m_data,m_header_buf->m_data_size);
	//响应码
	size_t offset = 0;

	std::string http_version;
	for (; ' ' != str_header.at(offset); offset++)
	{
		http_version.push_back(str_header.at(offset));
	}
	m_res.m_headers.insert(std::make_pair(std::string("Http-Version"), http_version));
	std::string response_code;
	for (offset++;' ' != str_header.at(offset);offset++)
	{
		response_code.push_back(str_header.at(offset));
	}
	//m_res.m_headers.insert(std::make_pair(std::string("Response-Code"), response_code));
	m_res.m_code = std::stoi(response_code);
	for (; '\n' != str_header.at(offset) && '\r' != str_header.at(offset); offset++);
	for (offset++; offset < str_header.size();offset++)
	{
		if ('\n' == str_header.at(offset))
		{
			continue;
		}
		if ('\r' == str_header.at(offset))
		{
			continue;
		}
		std::string key;
		for (; ':' != str_header.at(offset);offset++)
		{
			key.push_back(str_header.at(offset));
		}
		std::string value;
		offset++;
		if (' ' == str_header.at(offset))
		{
			offset++;
		}
		for (;'\n' != str_header.at(offset) && '\r' != str_header.at(offset); offset++)
		{
			value.push_back(str_header.at(offset));
		}
		if ("Set-Cookie" != key)
		{
			m_res.m_headers.insert(std::make_pair(key, value));
		}
		else
		{
			ParserOneCookie(value);
		}
	}
}

void qq::HttpClient::ParserOneCookie(const std::string & str)
{
	std::string key;
	size_t offset = 0;
	for (;'=' != str.at(offset) && offset < str.size();offset++)
	{
		key.push_back(str.at(offset));
	}
	std::string value;
	for (offset++; ';' != str.at(offset) && offset < str.size(); offset++)
	{
		value.push_back(str.at(offset));
	}
	m_res.m_cookies.insert(std::make_pair(key, value));
}

size_t qq::HttpClient::ContentCallback(char *rev, size_t size, size_t count, DataBuf *store) {
	assert(nullptr != store);
    const size_t real_size = size * count;
    //如果默认内存不够用
    if((store->m_store_size - store->m_data_size) < real_size){
		store->Resize(store->m_store_size + real_size + 1);
    }
    memcpy(&(store->m_data[store->m_data_size]), rev, real_size);
	store->m_data_size += real_size;
	store->m_data[store->m_data_size] = '\0';
    return real_size;
}

size_t qq::HttpClient::HeaderCallback(char* rev, size_t size, size_t count, DataBuf* store)
{
	assert(nullptr != store);
	const size_t real_size = size * count;
	//如果默认内存不够用
	if ((store->m_store_size - store->m_data_size) < real_size) {
		store->Resize(store->m_store_size + real_size + 1);
	}
	memcpy(&(store->m_data[store->m_data_size]), rev, real_size);
	store->m_data_size += real_size;
	store->m_data[store->m_data_size] = '\0';
	return real_size;
}


std::string qq::HttpClient::URLEncoded(std::string str) {
    const char * ec = curl_easy_escape(m_handle,str.c_str(),str.length());
    return std::string(ec);
}

std::string qq::HttpClient::URLUnEncoded(const char *data, int size) {
    int outlen = 0;
    const char* un = curl_easy_unescape(m_handle,data,size,&outlen);
    std::string re(un,0,outlen);
    return re;
}

const qq::Response * qq::HttpClient::GetResponse() const
{
	return &m_res;
}








