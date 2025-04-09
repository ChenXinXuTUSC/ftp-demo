#ifndef __COMMON__
#define __COMMON__

#include <stdexcept> // standard error

#include <iostream>
#include <cstring> // memset
#include <string>

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <arpa/inet.h> // inet_pton
#include <netinet/in.h>
#include <sys/socket.h>

#include <thread>
#include <mutex>

#include <random>
#include <ctime>


void my_terminate_handler();

class init
{
public:
    init()
    {
        // random seed initialization
        srand((unsigned int)time(NULL));

        // handle uncatched exceptions
        std::set_terminate(my_terminate_handler);
    }
};

#endif
