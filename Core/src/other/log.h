#define  GLOG_NO_ABBREVIATED_SEVERITIES
#ifndef __LOG_H__
#define __LOG_H__

#include "platform.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

void __log_init()
{
	std::string config_path = get_exe_path() + "\\log.conf";
	el::Configurations conf(config_path);
	el::Loggers::reconfigureAllLoggers(conf);
}

#endif
