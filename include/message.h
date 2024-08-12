#include <winsock2.h>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
 
class Message {
public:
    Message(int socket_fd);
    ~Message();

    void send_message(const json& message);
    json receive_message();

private:
    int socket_fd_;
};