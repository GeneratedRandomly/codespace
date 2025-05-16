// 多线程服务器端代码
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include "logger.h"

#define PORT 8888
#define BUFFER_SIZE 1024
#define EOF_MARKER "\xFF\xFF\xFF\xFF"
#define LOG_FILE "multiprocess_server.log"
#define MAX_CLIENTS 5

int running = 1; // 服务器运行状态
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int clients[MAX_CLIENTS];
int num_clients = 0;

// 错误处理函数
void handle_error(const char *message, int socket_fd)
{
    log_message("Error on socket %d: %s", socket_fd, message);
    if (socket_fd != -1)
        close(socket_fd);
    exit(EXIT_FAILURE);
}

void write_file_from_client(int client_socket)
{
    int n;
    FILE *fp;
    char *filename = "received_file.txt";
    char buffer[BUFFER_SIZE];

    fp = fopen(filename, "w");
    if (fp == NULL)
        handle_error("File open failed", client_socket);

    log_message("prepare to receive file from client %d", client_socket);

    while ((n = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0)
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
        handle_error("Receive file failed", client_socket);

    log_message("File received successfully from client %d.", client_socket);
    log_message("File saved to %s", filename);

    send(client_socket, "File received successfully.", strlen("File received successfully."), 0);

    fclose(fp);
}

// 线程处理函数，处理单个客户端连接
void *handle_client(void *client_socket_ptr)
{
    int client_socket = *((int *)client_socket_ptr);
    free(client_socket_ptr);
    char buffer[BUFFER_SIZE];

    while (1)
    {
        // 接收客户端消息
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received == -1)
        {
            log_message("Receive failed from client %d", client_socket);
            break;
        }
        else if (bytes_received == 0) // 客户端关闭连接
            break;

        // 检查是否为退出命令
        if (strcmp(buffer, "exit") == 0)
        {
            send(client_socket, "Bye.", strlen("Bye."), 0);
            log_message("Client %d shutting down.", client_socket);
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
            log_message("File received successfully from client %d.", client_socket);
            continue;
        }

        log_message("Received from client %d: %s", client_socket, buffer);

        // 回复客户端
        send(client_socket, buffer, strlen(buffer), 0);
    }
    // 关闭客户端套接字
    close(client_socket);

    // 从客户端列表中移除
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < num_clients; i++)
    {
        if (clients[i] == client_socket)
        {
            clients[i] = clients[num_clients - 1];
            num_clients--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    pthread_exit(NULL);
}

// 监听用户输入的线程函数
void *input_listener(void *arg)
{
    int server_socket = *((int *)arg);
    char input[BUFFER_SIZE];
    while (running)
    {
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            log_message("Input failed");
            running = 0;
        }
        if (strncmp(input, "exit", 4) == 0)
        {
            log_message("Server shutting down.");
            running = 0;
        }
        if (running == 0)
        {
            close(server_socket);

            // 关闭所有客户端连接
            pthread_mutex_lock(&clients_mutex);
            for (int i = 0; i < num_clients; i++)
                close(clients[i]);
            pthread_mutex_unlock(&clients_mutex);

            break;
        }
    }
    pthread_exit(NULL);
}

int main()
{
    struct sockaddr_in server_addr;
    socklen_t client_addr_len = sizeof(struct sockaddr_in);

    set_log_file(LOG_FILE);

    // 创建套接字
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
        handle_error("Socket creation failed", -1);

    // 设置套接字为非阻塞模式
    int flags = fcntl(server_socket, F_GETFL, 0);
    fcntl(server_socket, F_SETFL, flags | O_NONBLOCK);

    // 准备地址结构
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 绑定套接字到地址
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        handle_error("Bind failed", server_socket);

    // 监听连接
    if (listen(server_socket, MAX_CLIENTS) == -1)
        handle_error("Listen failed", server_socket);
    log_message("Server is listening on port %d...", PORT);

    // 创建输入监听线程
    pthread_t input_thread;
    if (pthread_create(&input_thread, NULL, input_listener, (void *)&server_socket) != 0)
        handle_error("Input listener thread creation failed", server_socket);

    while (running)
    {
        int *client_socket = (int *)malloc(sizeof(int));
        // 接受客户端连接
        *client_socket = accept(server_socket, (struct sockaddr *)&server_addr, &client_addr_len);
        if (*client_socket == -1)
        {
            if (!running) // 如果服务器正在关闭，跳出循环
            {
                free(client_socket);
                break;
            }
            if (errno != EAGAIN && errno != EWOULDBLOCK)
                perror("Accept failed");
            free(client_socket);
            usleep(100000); // 等待0.1秒，避免忙等待
            continue;
        }
        log_message("Client %d connected.", *client_socket);

        if (*client_socket != -1) // 将客户端套接字添加到列表中
        {
            pthread_mutex_lock(&clients_mutex);
            if (num_clients < MAX_CLIENTS)
                clients[num_clients++] = *client_socket;
            else
            {
                log_message("Max clients reached. Rejecting client %d.", *client_socket);
                close(*client_socket);
                free(client_socket);
                pthread_mutex_unlock(&clients_mutex);
                continue;
            }
            pthread_mutex_unlock(&clients_mutex);
        }

        pthread_t thread;
        // 创建新线程处理客户端连接
        if (pthread_create(&thread, NULL, handle_client, (void *)client_socket) != 0)
        {
            perror("Thread creation failed");
            close(*client_socket);
            free(client_socket);
        }
        else // 分离线程，使其在后台运行
            pthread_detach(thread);
    }

    // 等待输入监听线程结束
    pthread_join(input_thread, NULL);

    // 关闭服务器套接字
    close(server_socket);
    return 0;
}