#include "utils/sendrecv_msg.h"
#include "utils/logger.h"

#include "common.h"

#include <vector>

// Function to send a message with length prefix
uint32_t send_msg(int sock_fd, const std::string& msg)
{
    if (sock_fd < 0)
    {
        ERRO("trying to send msg on invalid socket fd");
        return 0;
    }

    ssize_t bytes_tx_t;
    ssize_t bytes_tx_c;
    const char* data_ptr;
    
    // 1. send message length
    bytes_tx_t = 0;
    bytes_tx_c = 0;

    uint32_t msg_len = htonl(msg.length()); // Convert to network byte order
    data_ptr = reinterpret_cast<const char*>(&msg_len);
    do {
        bytes_tx_c = send(sock_fd, data_ptr + bytes_tx_t, sizeof(msg_len) - bytes_tx_t, 0);
        bytes_tx_t += bytes_tx_c;
    } while (bytes_tx_c > 0 && bytes_tx_t < sizeof(msg_len));
    
    if (bytes_tx_t != sizeof(msg_len))
    {
        ERRO("failed to send msg len", std::string(strerror(errno)));
        return -1;
    }

    // 2. send actual message data bytes
    bytes_tx_t = 0;
    bytes_tx_c = 0;
    data_ptr = reinterpret_cast<const char*>(msg.c_str());
    do {
        bytes_tx_c = send(sock_fd, data_ptr + bytes_tx_t, msg.length() - bytes_tx_t, 0);
        bytes_tx_t += bytes_tx_c;
    } while (bytes_tx_c > 0 && bytes_tx_t < msg.length());
    if (bytes_tx_t != msg.length()) {
        ERRO("failed to send msg data", std::string(strerror(errno)));
        return -1;
    }

    return bytes_tx_t;
}

// Function to receive a message with length prefix
std::string recv_msg(int sock_fd)
{
    if (sock_fd < 0)
    {
        ERRO("trying to recv msg on invalid socket fd");
        return "";
    }

    ssize_t bytes_rx_t;
    ssize_t bytes_rx_c;
    char* data_ptr;

    // 1. recv message length
    uint32_t msg_len;

    bytes_rx_t = 0;
    bytes_rx_c = 0;
    data_ptr = reinterpret_cast<char*>(&msg_len);

    do {
        bytes_rx_c = recv(sock_fd, data_ptr + bytes_rx_t, sizeof(msg_len) - bytes_rx_t, 0);
        bytes_rx_t += bytes_rx_c;
    } while (bytes_rx_c > 0 && bytes_rx_t < sizeof(msg_len));

    if (bytes_rx_t != sizeof(msg_len))
    {
        ERRO("failed to recv msg len", std::string(strerror(errno)));
        return "";
    }

    // 2. recv message data
    msg_len = ntohl(msg_len); // Convert from network byte order
    std::vector<char> msg_buffer(msg_len, '\0');
    
    bytes_rx_t = 0;
    bytes_rx_c = 0;
    data_ptr = reinterpret_cast<char*>(msg_buffer.data());

    do {
        bytes_rx_c = recv(sock_fd, data_ptr + bytes_rx_t, msg_len - bytes_rx_t, 0);
        bytes_rx_t += bytes_rx_c;
    } while (bytes_rx_c > 0 && bytes_rx_t < msg_len);

    if (bytes_rx_t != msg_len)
    {
        ERRO("failed to recv msg data", std::string(strerror(errno)));
        return "";
    }

    return std::string(msg_buffer.data());
}

