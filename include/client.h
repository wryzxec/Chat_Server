#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <iostream>
#include <winsock2.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Client {
public:
    Client(const std::string& server_address, unsigned short server_port);
    ~Client();

    void connect_to_server();
    void disconnect_from_server();
    void send_message(json& message);
    // void receive_message();
    
private:
    std::string server_address_;
    unsigned short server_port_;
    SOCKET client_socket_;
    bool is_connected_;
};
#endif