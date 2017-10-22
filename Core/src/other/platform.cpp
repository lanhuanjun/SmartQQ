#include "platform.h"
#include <memory>
std::string get_exe_path()
{
	std::string path;
#if defined(WIN32) || defined(WIN64) || (_WIN32) || (_WIN64)
	char szFilePath[MAX_PATH + 1] = { 0 };
	long ret =  GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	if (0 != ret)
	{
		(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串
		path = szFilePath;
	}
#else
    char buf[PATH_MAX];
	int rslt = readlink("/proc/self/exe", buf, PATH_MAX);// buf返回"/a/b/c"
	if (rslt < 0 || rslt >= PATH_MAX)
	{
		path.assign(buf, rslt);
	}
#endif
	return path;
}