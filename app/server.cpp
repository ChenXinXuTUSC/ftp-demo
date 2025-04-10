#include "common.h"
#include "utils/logger.h"
#include "utils/sendrecv_msg.h"
#include "utils/tools.h"

#include "gate.h"

// void echo(int sock);

init __init__; // global object constructor run before main
int main(int argc, char** argv)
{
    if (argc < 1 + 2)
    {
        fprintf(stderr, "usage: %s <dll_path> <symbol_name>\n", argv[0]);
        return -1;
    }

    std::vector<std::thread> workers;
    gate gg; // listening on 9092 by default

    gg.start_listen();
    while (true)
    {
        int clnt_sock = gg.pick_conn();
        if (clnt_sock < 0) break;
        workers.emplace_back(socket_handler(argv[1], argv[2]), clnt_sock);
    }
    gg.stop_listen();

    INFO("echo service exit 1");
    for (auto& t : workers)
        if (t.joinable())
            t.join();
    INFO("echo service exit 2");
    return 0;
}

// void echo(int sock)
// {
//     while (true)
//     {
//         printf("echo[%d] >>> ", sock);
//         std::string msg = recv_msg(sock);
//         send_msg(sock, "[server echo] " + msg);
//         std::cout << string_tolower(msg) << std::endl;
//         if (string_tolower(msg) == "exit" || string_tolower(msg) == "quit")
//         {
//             INFO("client", sock, "disconnected...");
//             break;
//         }
//     }
//     close(sock);
// }
