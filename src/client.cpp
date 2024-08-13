#include "client.h"
#include "message.h"
#include <iostream>
#include <winsock2.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Client::Client(const std::string& server_address, unsigned short server_port)
    : server_address_(server_address), server_port_(server_port), client_socket_(INVALID_SOCKET), is_connected_(false) {
    WSADATA wsa_data;
    int wserr = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (wserr != 0) {
        std::cerr << "WSAStartup failed: " << wserr << std::endl;
        exit(1);
    }
}

Client::~Client() {
    WSACleanup();
}

void Client::connect_to_server(){
    SOCKET client_socket = INVALID_SOCKET;
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(client_socket == INVALID_SOCKET){
        std::cerr << "Socket creation failed with error: "<< WSAGetLastError() << std::endl;
        WSACleanup();
        exit(1);
    }

    sockaddr_in server_address; 
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_address_.c_str());
    server_address.sin_port = htons(server_port_);

    if(connect(client_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == SOCKET_ERROR){
        std::cerr << "Client connection failed with error: " << WSAGetLastError() << std::endl;
        closesocket(client_socket);
        WSACleanup();
        exit(1);
    }else{
        client_socket_ = client_socket;
        is_connected_ = true;
        std::cout << "Client successfully connected to server" << std::endl;
    }
}

void Client::disconnect_from_server(){
    if(is_connected_){
        closesocket(client_socket_);
        is_connected_ = false;
        std::cout << "Client disconnected from server" << std::endl;
    }
}

void Client::send_message(json& message) {
    Message message_handler(client_socket_);
    message_handler.send_message(message);
}
