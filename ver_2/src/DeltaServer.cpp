#include "../include/RailController.h"
#include "../tools/json.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <cstring>
#include <stdexcept>
#include <boost/asio.hpp>

using boost::asio::ip::udp;
using json = nlohmann::json;

class DeltaServer {
private:
    boost::asio::io_context io_context_;
    udp::socket socket_;
    enum { max_length = 65507 }; // Maximum UDP packet size
    char data_[max_length];
    RailController rail;
    udp::endpoint remote_endpoint_;

public:
    DeltaServer(unsigned short port) 
        : socket_(io_context_, udp::endpoint(udp::v4(), port)) {
        std::cout << "Server initialized on port " << port << std::endl;
    }

    void start() {
        std::cout << "Connecting to Rail Controller..." << std::endl;
        
        // Init Rail Motor Nodes
        this->rail = RailController(false, true);
        this->rail.Connect(851, 1);
        if(!this->rail.IsConnected()){
            std::cout << "Error: Rail motors not connected." << std::endl;
            std::cout << "Exiting ..." << std::endl;
            exit(-1);
        }

        std::cout << "Server is listening..." << std::endl;
        
        do_receive();
        io_context_.run();
    }

private:
    void do_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(data_, max_length),
            remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0) {
                    try {
                        std::string json_str(data_, bytes_recvd);
                        json j = json::parse(json_str);
                        handle_json_message(j);
                    }
                    catch (const json::parse_error& e) {
                        std::cerr << "JSON parsing error: " << e.what() << std::endl;
                    }
                }
                else if (ec) {
                    std::cerr << "Receive error: " << ec.message() << std::endl;
                }
                
                // Setup to receive next message
                do_receive();
            });
    }

    void handle_json_message(const json& j) {
        // Example of processing the JSON message
        std::cout << "Received JSON message: " << j.dump(2) << std::endl;
        try {
            int index = j["index"];
            int command = j["command"];
            switch(command) {
                case 0: {  // Add braces to create a new scope
                    double position = j["position"];
                    this->rail.SelectWorkingMotor(index);
                    this->rail.MoveSelectedMotorCmd(position);
                    break;
                }
                default:
                    std::cout << "Invalid command" << std::endl;
                    break;
            }
        } catch(const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
};

int main() {
    try {
        DeltaServer server(12345); // Listen on port 12345
        server.start();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
