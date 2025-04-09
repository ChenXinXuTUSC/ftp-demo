#include "common.h"
#include "utils/logger.h"
#include "utils/sendrecv_msg.h"
#include "utils/tools.h"

#include "gate.h"

void echo(int sock);

init __init__; // global object constructor run before main
int main(int argc, char** argv)
{
    std::vector<std::thread> workers;
    gate gg;

    gg.start_listen();
    
    while (true)
    {
        int clnt_sock = gg.pick_conn();
        workers.emplace_back(std::thread(echo, clnt_sock));
    }
    
    gg.stop_listen();

    for (auto& t : workers)
        if (t.joinable())
            t.join();
    
    return 0;
}

void echo(int sock)
{
    while (true)
    {
        std::cout << "echo >>> ";
        std::string msg = recv_msg(sock);
        std::cout << msg << std::endl;
        if (string_tolower(msg) == "exit" || string_tolower(msg) == "quit")
            break;
        send_msg(sock, msg);
    }
    close(sock);
}
