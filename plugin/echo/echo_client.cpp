#include "common.h"
#include "utils/tools.h"
#include "utils/sendrecv_msg.h"

extern "C"
{

// 0000000000004c59 T echo_client_handler
int echo_client_handler(int sock)
{
    std::string input;
    while (true)
    {
        std::cout << "echo >>> ";
        std::cin >> input;
        send_msg(sock, input);
        std::cout << recv_msg(sock) << std::endl;
        
        if (string_tolower(input) == "exit" || string_tolower(input) == "quit")
            break;
    }
    close(sock);

    return 0;
}

}
