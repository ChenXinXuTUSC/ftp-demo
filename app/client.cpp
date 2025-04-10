#include "common.h"
#include "utils/logger.h"
#include "utils/sendrecv_msg.h"
#include "utils/tools.h"

int connect_to_host(const char* ip, int port);


init __init__;
int main(int argc, char** argv)
{
    if (argc < 1 + 4)
    {
        WARN("usage: echo_client <ip> <port> <plugin_dll> <plugin_method>");
        return -1;
    }

    int sock = connect_to_host(argv[1], atoi(argv[2]));
    if (sock < 0)
    {
        ERRO("failed to connect to", argv[1], "on port", argv[2]);
        return -2;
    }

    std::thread thd(socket_handler(argv[3], argv[4]), sock);
    if (thd.joinable())
        thd.join();
    return 0;
}


int connect_to_host(const char* ip, int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        ERRO("failed to create client socket");
        throw std::runtime_error("invalid socket");
    }

    sockaddr_in srv_addr;
    srv_addr.sin_family = AF_INET; // IPv4
    srv_addr.sin_port = htons(port); // 服务器端口号 (需要转换为网络字节序)
    if (inet_pton(AF_INET, ip, &srv_addr.sin_addr) <= 0)
    {
        ERRO("failed to convert ip address");
        // throw std::runtime_error("invalid socket");
        return -1;
    }

    if (connect(sock, (sockaddr*)&srv_addr, sizeof(srv_addr)) < 0)
    {
        ERRO("failed to connect to server", ip);
        // throw std::runtime_error("invalid server address");
        return -1;
    }

    return sock;
}
