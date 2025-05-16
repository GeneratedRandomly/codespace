#ifndef LOGGER_H
#define LOGGER_H

void set_log_file(const char *file_path);
void log_message(const char *format, ...);

#endif // LOGGER_H