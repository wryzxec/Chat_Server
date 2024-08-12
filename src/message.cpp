#include "message.h"
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>

using json = nlohmann::json;

Message::Message(int socket_fd) : socket_fd_(socket_fd) {}
Message::~Message() {}

void Message::send_message(const json& message) {
    std::string serialized_message = message.dump();
    int bytes = send(socket_fd_, serialized_message.c_str(), serialized_message.size(), 0);
    if(bytes < 0){
        std::cerr << "Failed to send message" << std::endl;
    }
}

json Message::receive_message() {
    char buffer[4096] = {0};
    int bytes = recv(socket_fd_, buffer, sizeof(buffer), 0);
    if(bytes > 0){
        try {
            std::string received_data(buffer, bytes);
            return json::parse(received_data);
        }catch(const std::exception& e){
            std::cerr << "Failed to parse received JSON: " << e.what() << std::endl;
            return nullptr;
        }
    }else if(bytes == 0){
        std::cerr << "Connection closed by peer" << std::endl;
    }else {
        std::cerr << "Failed to receive message" << std::endl;
    }
    return nullptr;
}