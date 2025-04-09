#ifndef __TCP_SERVER__
#define __TCP_SERVER__

#include <common.h>

#include <queue>

class gate
{
public:
    gate();
    gate(int p, std::string a);
    ~gate();

    int pick_conn();
    void start_listen();
    void stop_listen();

private:
    void init();
    void loop();

private:
    int         port;
    std::string addr;

    int gate_sock;
    struct sockaddr_in gate_addr;

    std::mutex mu_q;
    std::condition_variable cv_q;
    std::queue<int> q_sock;

    std::thread thd;
};

#endif
