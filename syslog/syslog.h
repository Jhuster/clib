/*
 *  COPYRIGHT NOTICE  
 *  Copyright (C) 2015, Jhuster, All Rights Reserved
 *  Author:  Jhuster(lujun.hust@gmail.com)
 *  
 *  https://github.com/Jhuster
 *   
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.  
 */
#ifndef _SYS_LOG_
#define _SYS_LOG_

#include <stdio.h>

#define SYS_LOG_LEVEL_CLOSE 0
#define SYS_LOG_LEVEL_ERROR 1
#define SYS_LOG_LEVEL_INFO  2
#define SYS_LOG_LEVEL_DEBUG 3

//#define SYS_LOG_LEVEL SYS_LOG_LEVEL_ERROR
//#define SYS_LOG_DETAIL

#ifndef SYS_LOG_TAG
#define SYS_LOG_TAG "SYSLOG"
#endif 

#ifndef SYS_LOG_LEVEL
#define SYS_LOG_LEVEL SYS_LOG_LEVEL_DEBUG
#endif

#ifndef SYS_LOG_DETAIL
#define PRINT(format,...) printf("%s: "format,SYS_LOG_TAG,##__VA_ARGS__);
#else
#define PRINT(format,...) printf("%s %s: [%s,%s,%d]: %s: " format,__DATE__,__TIME__,__FILE__,__FUNCTION__,__LINE__,SYS_LOG_TAG,##__VA_ARGS__); 
#endif

#if SYS_LOG_LEVEL >= SYS_LOG_LEVEL_DEBUG
#define LOGD(format,...) PRINT(format,##__VA_ARGS__)
#else
#define LOGD(format,...) 
#endif

#if SYS_LOG_LEVEL >= SYS_LOG_LEVEL_INFO
#define LOGI(format,...) PRINT(format,##__VA_ARGS__)
#else
#define LOGI(format,...) 
#endif

#if SYS_LOG_LEVEL >= SYS_LOG_LEVEL_ERROR
#define LOGE(format,...) PRINT(format,##__VA_ARGS__)
#else
#define LOGE(format,...) 
#endif

#endif //_SYS_LOG_

