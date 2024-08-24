#include "server.h"
#include "message.h"
#include <iostream>
#include <winsock2.h>
#include <nlohmann/json.hpp>
#include <thread>
#include <string>

using json = nlohmann::json;

Server::Server(const std::string& server_address, unsigned short server_port)
    : server_address_(server_address), server_port_(server_port), server_socket_(INVALID_SOCKET), client_manager_(), is_running_(true) {
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
    {
        SOCKET accept_socket = INVALID_SOCKET;
        accept_socket = accept(server_socket_, nullptr, nullptr);

        if(accept_socket == INVALID_SOCKET){
            std::lock_guard<std::mutex> lock(mutex_);
            if(is_running_){
                std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            }
            return INVALID_SOCKET;
        }
        return accept_socket;
    }
}

void Server::handle_client(SOCKET client_socket){
    Message message_handler(client_socket);
    while(true){
        // mutex
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if(!is_running_){
                break;
            }
        }
        json received_message;
        try {
            received_message = message_handler.receive_message();
        } catch (const std::exception& e) {
            std::cerr << "Error receiving message: " << e.what() << std::endl;
            break;
        }

        if (received_message.is_null()) {
            std::cerr << "Failed to receive message" << std::endl;
            break;
        }
        if(received_message["type"] == "Ping"){
            message_handler.send_message({{"type", "Pong"}});
            continue;
        }
        std::cout << "Received message: " << received_message.dump() << std::endl;
    }
    closesocket(client_socket);
}

void Server::broadcast_message(const std::string& message, SOCKET server_socket){
    for(auto const& client : client_manager_.get_socket_id_map()){
       if (client.first != server_socket) {
            Message message_handler(client.first);
            message_handler.send_message(json::parse(message));
        }
    }
}

void Server::broadcast_shutdown_message(SOCKET server_socket){
    for(auto const& client : client_manager_.get_socket_id_map()){
        if (client.first != server_socket) {
            Message message_handler(client.first);
            message_handler.send_message({{"type", "Shutdown"}});
        }
    }
}

void Server::control_loop() {
    std::string command;
    while (std::getline(std::cin, command)) {
        std::cout << "Command received: " << command << std::endl;  // Log command input
        if (command == "/stop") {
            std::cout << "Stopping server..." << std::endl;  // Log stop command
            stop();
            break;
        }
    }
}

void Server::start(){

    std::thread control_thread(&Server::control_loop, this);
    control_thread.detach();

    std::cout << "Server starting..." << std::endl;
    create_socket();
    bind_socket();
    socket_listen();
    std::cout << "Server started, listening for connections" << std::endl;

    while (true) {
        // mutex
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if(!is_running_){
                break;
            }
        }
        SOCKET client_socket = accept_client();
        if(client_socket != INVALID_SOCKET){
            client_manager_.add_client_id(client_socket);
            client_manager_.add_client_thread(client_manager_.get_client_id(client_socket), std::thread(&Server::handle_client, this, client_socket));
        }
    }
}    

void Server::stop() {

    // mutex
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(!is_running_) {
            return;
        }
        is_running_ = false;
    }

    std::cout << "Broadcasting shutdown message..." << std::endl;
    broadcast_shutdown_message(server_socket_);
    
    std::cout << "Cleaning up client resources..." << std::endl;
    client_manager_.cleanup();
    
    std::cout << "Server has stopped." << std::endl;
    closesocket(server_socket_);
}