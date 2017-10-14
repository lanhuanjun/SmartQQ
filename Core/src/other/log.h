#define  GLOG_NO_ABBREVIATED_SEVERITIES
#ifndef __LOG_H__
#define __LOG_H__

#include "platform.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

void __log_init()
{
	el::Configurations conf("E:\\VisualStudio\\SmartQQ\\out\\log.conf");
	el::Loggers::reconfigureAllLoggers(conf);
}

#endif
