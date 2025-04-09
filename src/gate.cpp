#include "gate.h"
#include "utils/logger.h"

// public

gate::gate() : port(9092), addr("0.0.0.0")
{
    // exit if init failed with exception throwed
    init();
}

gate::gate(int p, std::string a) : port(p), addr(a)
{
    // exit if init failed with exception throwed
    init();
}

gate::~gate()
{
    mu_q.unlock();
    // 如果文件描述符未释放
    if (gate_sock >= 0)
        close(gate_sock);
    // 如果线程未结束
    if (thd.joinable())
        thd.join();
}

int gate::pick_conn()
{
    std::lock_guard<std::mutex> lock(mu_q);

    if (q_sock.size() == 0)
    {
        WARN("empty accepted socket queue");
        return -1;
    }
    int clnt_sock = q_sock.front(); q_sock.pop();
    return clnt_sock;
}

void gate::start_listen()
{
    thd = std::thread([this] { this->loop(); });
}

void gate::stop_listen()
{
    // 关闭套接字使得 accept 直接结束阻塞返回
    shutdown(gate_sock, SHUT_RDWR); // 关闭 socket 读写
    close(gate_sock); // 释放文件描述符
    gate_sock = -1;
    if (thd.joinable())
        thd.join();
}


// private

void gate::init()
{
    // 1. 创建套接字
    if ((gate_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERRO("socket failed to create", std::string(strerror(errno)));
        throw std::runtime_error("socket failed to create: "+std::string(strerror(errno)));
    }

    struct sockaddr_in host_addr;
    // 2. 设置服务器地址结构
    std::memset(&host_addr, 0, sizeof(host_addr)); // 清空地址
    host_addr.sin_family = AF_INET; // IPv4
    // host_addr.sin_addr.s_addr = htonl(inet_addr(addr.c_str())); // 地址, 0.0.0.0 等价于 INADDR_ANY
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 地址, 0.0.0.0 等价于 INADDR_ANY
    host_addr.sin_port = htons(port); // 端口（网络字节序）

    // 3. 绑定监听地址
    if (bind(gate_sock, (struct sockaddr*)&host_addr, sizeof(host_addr)) < 0)
    {
        close(gate_sock);
        ERRO("socket failed to bind", std::string(strerror(errno)));
        throw std::runtime_error("socket failed to bind: "+std::string(strerror(errno)));
    }
    
    // 4. 监听连接请求
    if (listen(gate_sock, 5) < 0)
    { // 可排队等候的连接队列是 5，也即 TCP 三次握手未完成的那些连接
        close(gate_sock);
        throw std::runtime_error("socket failed to listen: "+std::string(strerror(errno)));
    }
}

void gate::loop()
{
    INFO("start listening loop at", addr);

    if (gate_sock < 0) init();

    // ip 地址最多 15 个字节，加一个终止符 '\0'
    char ipv4_addr[INET_ADDRSTRLEN]; // 16

    // loop to accept connection
    int clnt_sock;
    struct sockaddr_in clnt_addr;
    while (gate_sock >= 0)
    {
        socklen_t addr_sz = sizeof(clnt_addr);
        clnt_sock = accept(gate_sock, (struct sockaddr*)&clnt_addr, &addr_sz);
        inet_ntop(AF_INET, &(clnt_addr.sin_addr), ipv4_addr, INET_ADDRSTRLEN);

        if (clnt_sock < 0)
        {
            if (gate_sock >= 0)
                ERRO("failed to accept:"+std::string(strerror(errno)));
            break;
        }
        INFO("accept clnt_sock", clnt_sock, "from", std::string(ipv4_addr));

        {
            std::lock_guard<std::mutex> lock(mu_q);
            q_sock.push(clnt_sock);
        }
    }
}

