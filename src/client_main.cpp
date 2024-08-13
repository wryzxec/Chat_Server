#include "client.h"
#include <iostream>
#include <nlohmann/json.hpp>

const std::string CLIENT_IP = "127.0.0.1";
const unsigned short CLIENT_PORT = 12345;

int main(){
    Client client(CLIENT_IP, CLIENT_PORT);
    client.connect_to_server();
    while(true){
        nlohmann::json j;
        std::string type = "Message";
        std::string content;

        std::cout << "Enter Message: ";
        std::getline(std::cin, content);

        j["type"] = type;
        j["content"] = content;
        client.send_message(j);
    }
    client.disconnect_from_server();
    return 0;
}