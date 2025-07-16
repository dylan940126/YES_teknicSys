
#include "../include/DeltaClient.h"
#include <cstring>

DeltaClient::DeltaClient(bool isOnline, bool useRail){ 
    this->isOnline = isOnline;
    this->useRail = useRail;
    
    if (!this->useRail) return;
    
    // Create UDP socket
    this->udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (this->udpSocket < 0) {
        cout << "Failed to create socket" << endl;
        return;
    }

    // Set server address
    memset(&this->serverAddr, 0, sizeof(this->serverAddr));
    this->serverAddr.sin_family = AF_INET;
    this->serverAddr.sin_port = htons(12345); // Use appropriate port number
    this->serverAddr.sin_addr.s_addr = inet_addr("192.168.0.1"); // Use RPi's IP address

    this->isConnected = false;
}

void DeltaClient::Connect(int motorPortNumber, int railNumber){
    if(!this->useRail) return;
    this->railNumber = railNumber;
    this->bArry = new bool[railNumber];
    
    const char* request = "connect";
    
    if (sendto(this->udpSocket, request, strlen(request), 0, 
               (struct sockaddr*)&this->serverAddr, sizeof(this->serverAddr)) < 0) {
        cout << "Failed to send request to Delta Server" << endl;
        close(this->udpSocket);
        return;
    }

    // Receive response
    char buffer[1024];
    socklen_t serverAddrLen = sizeof(this->serverAddr);
    int bytesReceived = recvfrom(this->udpSocket, buffer, sizeof(buffer), 0, 
                                (struct sockaddr*)&this->serverAddr, &serverAddrLen);
    
    if (bytesReceived < 0) {
        cout << "Failed to receive response from Delta Server" << endl;
        close(this->udpSocket);
        this->isConnected = false;
        return;
    }

    cout << "Rail Controller Online." << endl;
    this->isConnected = true;
}

void DeltaClient::Disconnect(){
    if(!this->useRail) return;
    if (this->udpSocket >= 0) {
        close(this->udpSocket);
    }
    this->isConnected = false;
}

bool DeltaClient::IsConnected(){ return this->isConnected; }

void DeltaClient::SelectWorkingMotor(int index){
    if(!this->useRail) return;
    this->bArry[workingMotor] = false;
    this->bArry[index] = true;
    workingMotor = index;
}

void DeltaClient::MoveSelectedMotorCmd(double cmd, bool absulote){
    if(!this->useRail || !absulote) return;
    
    const char* request = "move";
    if (sendto(this->udpSocket, request, strlen(request), 0, 
               (struct sockaddr*)&this->serverAddr, sizeof(this->serverAddr)) < 0) {
        cout << "Failed to send request to Delta Server" << endl;
        close(this->udpSocket);
        return;
    }
}

void DeltaClient::CalibrationMotor(int index, double currentCmdPos){
    cout << "Caution !!!!!! CalibrationMotor not implemented" << endl;
    return;
}

vector<int> DeltaClient::GetMotorPosMeasured(){
    cout << "Caution !!!!!! GetMotorPosMeasured not implemented" << endl;
    return vector<int>();
}