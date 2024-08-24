#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <winsock2.h>
#include <string>
#include <unordered_map>
#include <thread>

class ClientManager{
    public:
        ClientManager();
        ~ClientManager();

        void add_client_id(SOCKET client_socket);
        void remove_client_id(SOCKET client_socket);
        void add_client_thread(unsigned short client_id, std::thread client_thread);
        void remove_client_thread(unsigned short client_id);

        void cleanup();

        unsigned short get_client_id(SOCKET client_socket) const;

        const std::unordered_map<SOCKET, unsigned short>& get_socket_id_map() const;
        std::unordered_map<unsigned short, std::thread>& get_id_thread_map();

    private:
        std::unordered_map<SOCKET, unsigned short> socket_id_map_;
        std::unordered_map<unsigned short, std::thread> id_thread_map_;
        unsigned short id_counter_;
};
#endif