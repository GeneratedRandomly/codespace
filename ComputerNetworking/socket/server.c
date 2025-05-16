// 服务器端代码
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "logger.h"

#define PORT 8888
#define BUFFER_SIZE 1024
#define EOF_MARKER "\xFF\xFF\xFF\xFF"
#define LOG_FILE "server.log"

// 错误处理函数
void handle_error(const char *message, int socket_fd)
{
    log_message(message);
    if (socket_fd != -1)
        close(socket_fd);
    exit(EXIT_FAILURE);
}

void write_file_from_client(int sockfd)
{
    int n;
    FILE *fp;
    char *filename = "received_file.txt";
    char buffer[BUFFER_SIZE];

    fp = fopen(filename, "w");
    if (fp == NULL)
        handle_error("File open failed", sockfd);

    log_message("prepare to receive file");

    while ((n = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0)
    {
        // 检查缓冲区最后四个有意义的字符是否为EOF_MARKER
        if (n >= 4 && memcmp(buffer + n - 4, EOF_MARKER, 4) == 0)
        {
            // 写入除EOF_MARKER以外的数据
            fwrite(buffer, sizeof(char), n - 4, fp);
            fflush(fp);
            break;
        }
        fwrite(buffer, sizeof(char), n, fp);
        fflush(fp);
        memset(buffer, 0, BUFFER_SIZE);
    }

    if (n == -1)
        handle_error("Receive file failed", sockfd);

    send(sockfd, "File received successfully.", strlen("File received successfully."), 0);

    fclose(fp);
}

void handle_client(int client_socket)
{
    char buffer[BUFFER_SIZE];

    while (1)
    {
        // 接收客户端消息
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received == -1)
        {
            log_message("Receive failed");
            break;
        }
        else if (bytes_received == 0) // 客户端关闭连接
            break;

        // 检查是否为退出命令
        if (strcmp(buffer, "exit") == 0)
        {
            send(client_socket, "Server shutting down.", strlen("Server shutting down."), 0);
            send(client_socket, "exit", strlen("exit"), 0);
            log_message("Server shutting down.");
            break;
        }
        else if (strncmp(buffer, "sendfile ", 9) == 0) // 检查是否为发送文件命令
        {
            // 提取文件名
            char filename[BUFFER_SIZE];
            strncpy(filename, buffer + 9, BUFFER_SIZE - 9);
            filename[BUFFER_SIZE - 9] = '\0'; // 确保字符串以空字符结尾

            // 构建响应字符串
            char response[BUFFER_SIZE + 50];
            snprintf(response, sizeof(response), "Ready to receive file: %s", filename);

            // 发送响应字符串回客户端
            send(client_socket, response, strlen(response), 0);

            // 接收文件
            write_file_from_client(client_socket);
            log_message("File received successfully.");
            continue;
        }

        log_message("Received from client: %s", buffer);

        // 回复客户端
        send(client_socket, buffer, strlen(buffer), 0);
    }
    // 关闭客户端套接字
    close(client_socket);
}

int main()
{
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(struct sockaddr_in);

    set_log_file(LOG_FILE);

    // 创建套接字
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
        handle_error("Socket creation failed", -1);

    // 设置套接字选项，允许重用本地地址
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        handle_error("setsockopt failed", server_socket);

    // 准备地址结构
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 绑定套接字到地址
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        handle_error("Bind failed", server_socket);

    // 监听连接
    if (listen(server_socket, 5) == -1)
        handle_error("Listen failed", server_socket);
    log_message("Server is listening on port %d...", PORT);

    // 接受客户端连接
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == -1)
        handle_error("Accept failed", server_socket);
    log_message("Client connected.");

    handle_client(client_socket);

    // 关闭服务器套接字
    close(server_socket);
    return 0;
}