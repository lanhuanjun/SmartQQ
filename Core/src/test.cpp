#include "net/http_client.h"
#include "core_main.h"
int main(int argc, char* argv[])
{
	core_global_init(argc, argv);
	qq::HttpClient *client = new qq::HttpClient();
	client->Init();
	client->SetUrl("https://www.baidu.com");
	client->Execute();
	client->Destroy();
	core_global_clean_up();
	return 0;
}