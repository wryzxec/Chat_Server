#include "client_manager.h"
#include <iostream>
#include <winsock2.h>

ClientManager::ClientManager() : id_counter_(1) {}
ClientManager::~ClientManager() {}

void ClientManager::add_client_id(SOCKET client_socket){
    socket_id_map_[client_socket] = id_counter_;
    id_counter_++;
    std::cout << "Assigned client ID: " << socket_id_map_[client_socket] << std::endl;
}

void ClientManager::remove_client_id(SOCKET client_socket){
    socket_id_map_.erase(client_socket);
}

void ClientManager::add_client_thread(unsigned short client_id, std::thread client_thread){
    id_thread_map_[client_id] = std::move(client_thread);
}

void ClientManager::remove_client_thread(unsigned short client_id){
    id_thread_map_.erase(client_id);
}

unsigned short ClientManager::get_client_id(SOCKET client_socket) const{
    if(socket_id_map_.find(client_socket) == socket_id_map_.end()){
        std::cerr << "Error: Client not found" << std::endl;
        return 0;
    }
    return socket_id_map_.at(client_socket);
}

const std::unordered_map<SOCKET, unsigned short>& ClientManager::get_socket_id_map() const{
    return socket_id_map_;
}

std::unordered_map<unsigned short, std::thread>& ClientManager::get_id_thread_map(){
    return id_thread_map_;
}

void ClientManager::cleanup(){
    for(auto& client : id_thread_map_){
        if(client.second.joinable()){
            client.second.join();
        }
    }
    id_thread_map_.clear();
    socket_id_map_.clear();
}

