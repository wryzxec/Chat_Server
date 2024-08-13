#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <iostream>
#include <winsock2.h>
#include <unordered_map>

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
    void assign_client_id(SOCKET client_socket);
    void handle_client(SOCKET client_socket);

    // void broadcast_message(const std::string& message, SOCKET sender_socket);

    std::string server_address_;
    unsigned short server_port_;
    SOCKET server_socket_;
    unsigned short id_counter_;
    std::unordered_map<SOCKET, std::string> client_ids_;
};
#endif