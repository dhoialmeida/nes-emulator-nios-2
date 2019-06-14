/* log.h - função de logging */

#ifndef LOG_HEADER
#define LOG_HEADER

#ifndef DISABLE_LOG
    #include <stdio.h>
    #define log(...) printf(__VA_ARGS__)
#else
    #define log(...)
#endif

#endif
