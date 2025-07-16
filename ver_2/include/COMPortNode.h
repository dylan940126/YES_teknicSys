#ifndef COMPortNode_H
#define COMPortNode_H

#include <string>
#include <boost/asio.hpp>

// Use Boost.Asio for cross-platform serial port support
class COMPortNode{
private:
    bool isOnline;
    boost::asio::io_context ioContext;
    boost::asio::serial_port serial;
    int readTimeout = 1000;

    // set communication port
    bool SetSerialParams();
public:
    COMPortNode(bool isOnline=false);

    bool Connect(std::string portName);  
    void Disconnect();

    std::string Read();
    std::string Send(std::string Ard_char);
};
#endif