#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <iostream>
#include <winsock2.h>
#include <unordered_map>
#include "client_manager.h"
#include <atomic>
#include <thread>
#include <vector>
#include <mutex>

class Server {
public:
    Server(const std::string& server_address, unsigned short server_port);
    ~Server();

    void start();
    void stop();

private:
    void create_socket();
    void bind_socket();
    void socket_listen();
    
    SOCKET accept_client();
    void handle_client(SOCKET client_socket);
    void cleanup_client_resources(SOCKET client_socket);

    void control_loop();
    
    void broadcast_message(const std::string& message, SOCKET sender_socket);
    void broadcast_shutdown_message(SOCKET sender_socket);

    std::string server_address_;
    unsigned short server_port_;
    SOCKET server_socket_;
    ClientManager client_manager_;
    std::atomic<bool> is_running_;
    std::mutex mutex_;
};
#endif