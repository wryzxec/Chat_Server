#include "client.h"
#include <iostream>

const std::string CLIENT_IP = "127.0.0.1";
const unsigned short CLIENT_PORT = 12345;

int main(){
    Client client(CLIENT_IP, CLIENT_PORT);
    client.connect_to_server();

    return 0;
}