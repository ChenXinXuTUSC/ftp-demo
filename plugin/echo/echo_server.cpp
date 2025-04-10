#include "common.h"
#include "utils/logger.h"
#include "utils/tools.h"
#include "utils/sendrecv_msg.h"

extern "C"
{

// 0000000000005012 T echo_server_handler
void echo_server_handler(int sock)
{
    while (true)
    {
        printf("echo[%d] >>> ", sock);
        std::string msg = recv_msg(sock);
        send_msg(sock, "[server echo] " + msg);
        std::cout << string_tolower(msg) << std::endl;
        if (string_tolower(msg) == "exit" || string_tolower(msg) == "quit")
        {
            INFO("client", sock, "disconnected...");
            break;
        } else {
            DBUG("not exit branch");
        }
    }
    close(sock);
}

}
