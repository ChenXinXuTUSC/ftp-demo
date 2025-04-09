#include "common.h"
#include "utils/logger.h"
#include "utils/sendrecv_msg.h"
#include "utils/tools.h"


init __init__;
int main(int argc, char** argv)
{
    if (argc < 1 + 2)
    {
        WARN("usage: echo_client <ip> <port>");
        return -1;
    }
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        ERRO("failed to create client socket");
        throw std::runtime_error("invalid socket");
    }

    sockaddr_in srv_addr;
    srv_addr.sin_family = AF_INET; // IPv4
    srv_addr.sin_port = htons(atoi(argv[2])); // 服务器端口号 (需要转换为网络字节序)
    if (inet_pton(AF_INET, argv[1], &srv_addr.sin_addr) <= 0)
    {
        ERRO("failed to convert ip address");
        throw std::runtime_error("invalid socket");
    }

    if (connect(sock, (sockaddr*)&srv_addr, sizeof(srv_addr)) < 0)
    {
        ERRO("failed to connect to server", argv[1]);
        throw std::runtime_error("invalid server address");
    }

    std::string input;
    while (true)
    {
        std::cout << "echo >>> ";
        std::cin >> input;

        if (string_tolower(input) == "exit" || string_tolower(input) == "quit")
            break;

        send_msg(sock, input);
        std::cout << recv_msg(sock) << std::endl;
    }
    
    close(sock);
    return 0;
}
