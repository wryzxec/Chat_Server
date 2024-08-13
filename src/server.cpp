#include "server.h"
#include "message.h"
#include <iostream>
#include <winsock2.h>
#include <nlohmann/json.hpp>
#include <thread>
#include <string>

using json = nlohmann::json;

Server::Server(const std::string& server_address, unsigned short server_port)
    : server_address_(server_address), server_port_(server_port), server_socket_(INVALID_SOCKET), id_counter_(1){
    WSADATA wsa_data;
    int wserr = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (wserr != 0) {
        std::cerr << "WSAStartup failed: " << wserr << std::endl;
        exit(1);
    }
}

Server::~Server() {
    WSACleanup();
}

void Server::create_socket(){
    server_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(server_socket_ == INVALID_SOCKET){
        std::cerr << "Socket creation failed with error: "<< WSAGetLastError() << std::endl;
        WSACleanup();
        exit(1);
    }
}

void Server::bind_socket(){
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(server_address_.c_str());
    service.sin_port = htons(server_port_);
    
    if(bind(server_socket_, reinterpret_cast<sockaddr*>(&service), sizeof(service)) == SOCKET_ERROR){
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_socket_);
        WSACleanup();
        exit(1);
    }
}

void Server::socket_listen(){
    if(listen(server_socket_, 1) == SOCKET_ERROR){
        std::cerr << "Socket Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_socket_);
        WSACleanup();
        exit(1);
    }
}

SOCKET Server::accept_client(){
    SOCKET accept_socket = INVALID_SOCKET;
    accept_socket = accept(server_socket_, nullptr, nullptr);

    if(accept_socket == INVALID_SOCKET){
        std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_socket_);
        WSACleanup();
        exit(1);
    }
    return accept_socket;
}

void Server::assign_client_id(SOCKET client_socket){
    std::string client_id = std::to_string(id_counter_);
    client_ids_[client_socket] = client_id;
    id_counter_++;
    std::cout << "Assigned client ID: " << client_id << std::endl;
}

void Server::handle_client(SOCKET client_socket){
    assign_client_id(client_socket);
    Message message_handler(client_socket);
    while(true){
        json received_message = message_handler.receive_message();
        if(received_message == nullptr){
            std::cerr << "Failed to receive message" << std::endl;
            break;
        }
        std::cout << "Received message: " << received_message.dump() << std::endl;
    }
    closesocket(client_socket);
}

void Server::start(){
    std::cout << "Server starting..." << std::endl;
    create_socket();
    bind_socket();
    socket_listen();
    std::cout << "Server started, listening for connections" << std::endl;

    while (true) {
        SOCKET client_socket = accept_client();
        std::thread client_thread(&Server::handle_client, this, client_socket);
        client_thread.detach();
    }
}    

void Server::stop(){
    std::cout << "Server stopping..." << std::endl;
    closesocket(server_socket_);
    std::cout << "Server stopped" << std::endl;
    WSACleanup();
}