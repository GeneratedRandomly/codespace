// 客户端代码
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include "logger.h"

#define SERVER_IP "127.0.0.1"
#define PORT 8888
#define BUFFER_SIZE 1024
#define EOF_MARKER "\xFF\xFF\xFF\xFF"
#define LOG_FILE "client.log"

void send_file(FILE *fp, int sockfd)
{
    char data[BUFFER_SIZE];
    size_t n;

    while ((n = fread(data, sizeof(char), BUFFER_SIZE, fp)) > 0)
    {
        if (send(sockfd, data, n, 0) == -1)
        {
            log_message("Send file failed");
            exit(EXIT_FAILURE);
        }
    }
    // 发送结束标志
    send(sockfd, EOF_MARKER, strlen(EOF_MARKER), 0);
}

ssize_t receive_response(int sockfd, char *buffer, size_t buffer_size)
{
    memset(buffer, 0, buffer_size);
    ssize_t bytes_received = recv(sockfd, buffer, buffer_size - 1, 0);
    if (bytes_received == -1)
        log_message("Receive failed");
    else if (bytes_received == 0)
        log_message("Server closed connection");
    else
    {
        char log_buffer[BUFFER_SIZE + 50];
        snprintf(log_buffer, sizeof(log_buffer), "Received from server: %s", buffer);
        log_message(log_buffer);
    }
    return bytes_received;
}

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int exit_flag = 0;

    set_log_file(LOG_FILE);

    // 创建套接字
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        log_message("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 准备地址结构
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // 连接到服务器
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        log_message("Connect failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    log_message("Connected to server.");

    while (1)
    {
        // 输入消息
        printf("Enter message to send (type 'exit' to quit or 'sendfile <filename>' to send a file): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            log_message("Input failed");
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        // 检查是否为发送文件命令
        if (strncmp(buffer, "sendfile ", 9) == 0)
        {
            char *filename = buffer + 9;
            FILE *fp = fopen(filename, "r");
            if (fp == NULL)
            {
                log_message("File open failed");
                continue;
            }

            // 发送文件命令和文件名
            send(client_socket, buffer, strlen(buffer), 0);

            // 接收服务器响应
            if (receive_response(client_socket, buffer, sizeof(buffer)) <= 0)
            {
                fclose(fp);
                break;
            }

            // 发送文件数据
            send_file(fp, client_socket);
            fclose(fp);
            log_message("File sent successfully.");

            if (receive_response(client_socket, buffer, sizeof(buffer)) <= 0)
                break;

            continue;
        }
        else if (strcmp(buffer, "exit") == 0) // 检查是否为退出命令
            exit_flag = 1;                    // 不立即退出，先告知对方

        // 发送消息到服务器
        send(client_socket, buffer, strlen(buffer), 0);

        // 接收服务器响应
        if (receive_response(client_socket, buffer, sizeof(buffer)) <= 0)
            break;

        // 退出
        if (exit_flag)
        {
            log_message("Client shutting down.");
            break;
        }
    }

    // 关闭套接字
    close(client_socket);
    return 0;
}