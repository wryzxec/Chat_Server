#include <winsock2.h>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
 
class Message {
public:
    Message(SOCKET socket_fd);
    ~Message();

    void send_message(const json& message);
    json receive_message();

private:
    SOCKET socket_fd_;
};