#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "logger.h"

static char log_file_path[256] = "default.log";

// 设置日志文件路径
void set_log_file(const char *file_path)
{
    strncpy(log_file_path, file_path, sizeof(log_file_path) - 1);
    log_file_path[sizeof(log_file_path) - 1] = '\0';
}

// 获取当前时间的字符串表示
char *current_time_str()
{
    static char buffer[20];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
    return buffer;
}

// 日志函数，带有时间戳和格式化字符串
void log_message(const char *format, ...)
{
    FILE *log_fp = fopen(log_file_path, "a");
    if (log_fp == NULL)
    {
        perror("Log file open failed");
        exit(EXIT_FAILURE);
    }

    va_list args;
    va_start(args, format);

    // 写入时间戳
    fprintf(log_fp, "[%s] ", current_time_str());

    // 写入格式化的日志信息
    vfprintf(log_fp, format, args);
    fprintf(log_fp, "\n");

    va_end(args);
    fclose(log_fp);
}