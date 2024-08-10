#include "server.h"
#include <iostream>

const std::string SERVER_IP = "127.0.0.1";
const unsigned short SERVER_PORT = 12345;

int main(){
    Server server(SERVER_IP, SERVER_PORT);
    server.start();

    return 0;
}