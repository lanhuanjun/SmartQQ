#ifndef __CORE_MAIN__
#define __CORE_MAIN__
#include "../../third/include/curl/curl.h"
#include "other/log.h"

void core_global_init(int argc, char* argv[])
{
	curl_global_init(CURL_GLOBAL_ALL);
	__log_init();
}

void core_global_clean_up()
{
	curl_global_cleanup();
}

#endif
