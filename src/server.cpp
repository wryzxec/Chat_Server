#include "server.h"
#include <iostream>
#include <winsock.h>

Server::Server(const std::string& server_address, unsigned short server_port)
    : server_address_(server_address), server_port_(server_port), server_socket_(INVALID_SOCKET), is_connected_(false) {
    WSADATA wsa_data;
    int wserr = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (wserr != 0) {
        std::cerr << "WSAStartup failed: " << wserr << std::endl;
        exit(1);
    }
}

Server::~Server() {
    // stop();
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

void Server::accept_client(){
    SOCKET accept_socket = INVALID_SOCKET;
    accept_socket = accept(server_socket_, nullptr, nullptr);

    if(accept_socket == INVALID_SOCKET){
        std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_socket_);
        WSACleanup();
        exit(1);
    }
}

void Server::start(){
    create_socket();
    bind_socket();
    socket_listen();
    accept_client();
}