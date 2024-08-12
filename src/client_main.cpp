#include "client.h"
#include <iostream>
#include <nlohmann/json.hpp>

const std::string CLIENT_IP = "127.0.0.1";
const unsigned short CLIENT_PORT = 12345;

int main(){
    Client client(CLIENT_IP, CLIENT_PORT);
    client.connect_to_server();
    nlohmann::json j;
    j["type"] = "Test Message";
    j["content"] = "This is a test.";
    client.send_message(j);

    return 0;
}