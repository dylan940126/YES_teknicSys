#include "../include/COMPortNode.h"
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

COMPortNode::COMPortNode(bool isOnline)
    : isOnline(isOnline), serial(ioContext) {}

bool COMPortNode::SetSerialParams(){
    if (!isOnline) return true;
    try {
        serial.set_option(boost::asio::serial_port_base::baud_rate(57600));
        serial.set_option(boost::asio::serial_port_base::character_size(8));
        serial.set_option(boost::asio::serial_port_base::stop_bits(
            boost::asio::serial_port_base::stop_bits::one));
        serial.set_option(boost::asio::serial_port_base::parity(
            boost::asio::serial_port_base::parity::none));
    } catch(const boost::system::system_error& e) {
        std::cerr << "Error setting serial params: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool COMPortNode::Connect(std::string portName){
    if(!this->isOnline) return true;
    if (!isOnline) return true;
    try {
        serial.open(portName);
    } catch(const boost::system::system_error& e) {
        std::cerr << "Error opening port " << portName << ": " << e.what() << std::endl;
        return false;
    }
    if (!SetSerialParams()) return false;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return true;
}

void COMPortNode::Disconnect(){
    if(!this->isOnline) return;
    if (!isOnline) return;
    serial.close();
}

std::string COMPortNode::Send(std::string Ard_char){
    if(!this->isOnline) return "";
    if (!isOnline) return "";
    std::array<char,8> buffer{};
    std::copy_n(Ard_char.data(), std::min<size_t>(8, Ard_char.size()), buffer.data());
    try {
        boost::asio::write(serial, boost::asio::buffer(buffer));
    } catch(const boost::system::system_error& e) {
        std::cerr << "Error writing to serial: " << e.what() << std::endl;
        return "False";
    }
    return "Success";
    // return Read();
    // Read(); // need this?
}

std::string COMPortNode::Read(){
    if(!this->isOnline) return "";
    if (!isOnline) return "";
    std::string msg;
    char tmp;
    boost::system::error_code ec;
    // Read at least one character
    size_t len = serial.read_some(boost::asio::buffer(&tmp,1), ec);
    if (!ec && len > 0) {
        msg.push_back(tmp);
        // Continue reading remaining data
        while ((len = serial.read_some(boost::asio::buffer(&tmp,1), ec)) && !ec) {
            msg.push_back(tmp);
        }
    }
    return msg;

}