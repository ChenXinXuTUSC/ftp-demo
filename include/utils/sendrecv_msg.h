#ifndef __SENDRECV_MSG__
#define __SENDRECV_MSG__

#include "common.h"

uint32_t send_msg(int sock_fd, const std::string& msg);
std::string recv_msg(int sock);

#endif
