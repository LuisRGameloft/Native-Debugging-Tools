#ifndef _UTILS_DEBUG_
#define _UTILS_DEBUG_

#ifdef OS_ANDROID
# include <android/log.h>
# define debug(...) __android_log_print(ANDROID_LOG_INFO, "3DBasicFramework", __VA_ARGS__)
#else
# include <stdio.h>
# define debug(...) printf(__VA_ARGS__)
#endif

#endif // _UTILS_DEBUG_