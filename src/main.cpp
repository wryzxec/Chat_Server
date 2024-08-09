#include "server.h"
#include "client.h"
#include <thread>

const std::string SERVER_IP = "127.0.0.1";
const unsigned short SERVER_PORT = 12345;

const std::string CLIENT_IP = "127.0.0.1";
const unsigned short CLIENT_PORT = 12345;

void start_server(){
    Server server(SERVER_IP, SERVER_PORT);
    server.start();
}
void start_client(){
    Client client(CLIENT_IP, CLIENT_PORT);
    client.connect_to_server();
}

int main(){   
    std::thread server_thread(start_server);
    std::thread client_thread(start_client);
    server_thread.join();
    client_thread.join();

    return 0;
}