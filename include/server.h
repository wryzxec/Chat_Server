#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <iostream>
#include <winsock2.h>

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

    // void broadcast_message(const std::string& message, SOCKET sender_socket);

    std::string server_address_;
    unsigned short server_port_;
    SOCKET server_socket_;
    bool is_connected_;
};
#endif