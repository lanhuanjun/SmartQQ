#include <iostream>
#include <map>
#include <unordered_map>
#include "../third/include/curl/curl.h"
using namespace std;

int main(int argc, char* argv[])
{
	curl_global_init(CURL_GLOBAL_ALL);
	auto handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "gzip");
	curl_easy_setopt(handle, CURLOPT_URL, "http://www.baidu.com");
	curl_easy_perform(handle);
	curl_easy_cleanup(handle);
	curl_global_cleanup();
	return 0;
}