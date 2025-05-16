# 定义编译器
CC = gcc

# 定义编译选项
CFLAGS = -Wall -Wextra -O2

# 定义目标文件
TARGETS = server client multiprocess_server

# 默认目标
all: $(TARGETS)

# 编译 server
server: server.o logger.o
	$(CC) $(CFLAGS) -o server server.o logger.o

# 编译 multiprocess_server
multiprocess_server: multiprocess_server.o logger.o
	$(CC) $(CFLAGS) -o multiprocess_server multiprocess_server.o logger.o

# 编译 client
client: client.o logger.o
	$(CC) $(CFLAGS) -o client client.o logger.o

# 生成 server.o
server.o: server.c
	$(CC) $(CFLAGS) -c server.c

# 生成 multiprocess_server.o
multiprocess_server.o: multiprocess_server.c
	$(CC) $(CFLAGS) -c multiprocess_server.c

# 生成 client.o
client.o: client.c
	$(CC) $(CFLAGS) -c client.c

# 生成 logger.o
logger.o: logger.c
	$(CC) $(CFLAGS) -c logger.c

# 清理生成的文件
clean:
	rm -f $(TARGETS) *.o *.log

# 伪目标
.PHONY: all clean