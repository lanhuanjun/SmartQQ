#include "../Core/src/net/http_client.h"
int main(int argc, char* argv[])
{
	qq::HttpClient *client = new qq::HttpClient();
	client->Init();
	client->SetUrl("https://www.baidu.com");
	client->Execute();
	client->Destroy();
	return 0;
}