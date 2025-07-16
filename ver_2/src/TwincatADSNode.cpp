#include "../include/TwincatADSNode.h"
#include <iostream>
#include <cstring>
TwincatADSNode::TwincatADSNode(bool isOnline){ this->isOnline = isOnline; }

bool TwincatADSNode::Connect(int port){
    if(!this->isOnline) return true;
    long nErr, nPort;
    unsigned long lHdlVar;
    bool TRUE_FLAG = true;

    cout << "Connecting to TwinCAT ADS..." << endl;
    // Open ADS port
    port_ = AdsPortOpenEx();
    // Get local address
    nErr = AdsGetLocalAddressEx(port_, &Addr);
    if (nErr) { cout << "Error: AdsGetLocalAddressEx: " << nErr << '\n'; return false; }
    // Set target port (PLC ADS port)
    Addr.port = port;
    // Enable power for motors, start state machine
    // Get handle of the request
    uint32_t bytesRead = 0;
    nErr = AdsSyncReadWriteReqEx2(port_, &Addr, ADSIGRP_SYM_HNDBYNAME, 0,
        sizeof(lHdlVar), &lHdlVar, strlen("MAIN.power"), "MAIN.power", &bytesRead);
    if (nErr){ cout << "Error: AdsSyncReadWriteReqEx2: " << nErr << '\n'; return false; }
    // Use the handle to write data
    cout << "MAIN.power handler: " << lHdlVar << endl;
    cout << "Enabling power for motors" << endl;
    nErr = AdsSyncWriteReqEx(port_, &Addr, ADSIGRP_SYM_VALBYHND,
        lHdlVar, sizeof(TRUE_FLAG), &TRUE_FLAG);
    if (nErr){ cout << "Error: AdsSyncWriteReq: " << nErr << '\n'; return false; }
    else { cout << "Linear rail motors enabled.\n"; }
    // Release the handle
    nErr = AdsSyncWriteReqEx(port_, &Addr, ADSIGRP_SYM_RELEASEHND,
        0, sizeof(lHdlVar), &lHdlVar);
    cout<<"1"<<endl;

    // Create list of handler by name

    for(int i = 0; i < 17; i++){
        uint32_t rd = 0;
        nErr = AdsSyncReadWriteReqEx2(port_, &Addr, ADSIGRP_SYM_HNDBYNAME, 0,
            sizeof(lHdlVar), &lHdlVar,
            static_cast<uint32_t>(adsVarNames[i].length()),
            adsVarNames[i].c_str(), &rd);
        if (nErr) {
            cout << "Error: AdsSyncReadWriteReqEx2: " << nErr
                 << ", handler for " << adsVarNames[i] << '\n';
            return false;
        }
        handlers[adsVarNames[i]] = lHdlVar;
    }
    cout << "Completed creating handler list of " << handlers.size() << endl;
    return true;
}

void TwincatADSNode::Disconnect(){
    if(!isOnline) return;
    long nErr;
    unsigned long lHdlVar;
    bool FALSE_FLAG = false;
    uint32_t bytesRead = 0;
    // Get handler for MAIN.power
    nErr = AdsSyncReadWriteReqEx2(port_, &Addr, ADSIGRP_SYM_HNDBYNAME, 0,
        sizeof(lHdlVar), &lHdlVar, strlen("MAIN.power"), "MAIN.power", &bytesRead);
    if(nErr){ cout << "Error: AdsSyncReadWriteReqEx2: " << nErr << '\n'; }
    // Disable power
    nErr = AdsSyncWriteReqEx(port_, &Addr, ADSIGRP_SYM_VALBYHND,
        lHdlVar, sizeof(FALSE_FLAG), &FALSE_FLAG);
    if(nErr){ cout << "Error: AdsSyncWriteReqEx: " << nErr << '\n'; }
    else { cout << "Linear rail motors disabled.\n"; }
    // Release handle
    nErr = AdsSyncWriteReqEx(port_, &Addr, ADSIGRP_SYM_RELEASEHND,
        0, sizeof(lHdlVar), &lHdlVar);
    // Close ADS port
    AdsPortCloseEx(port_);
}

void TwincatADSNode::WriteReq(string handle, double Data){
    if(!isOnline) return;
    long nErr = AdsSyncWriteReqEx(port_, &Addr, ADSIGRP_SYM_VALBYHND,
        handlers[handle], sizeof(double), &Data);
    if(nErr){ cout << "Error: AdsSyncWriteReqEx: " << nErr << '\n'; }
}


void TwincatADSNode::WriteReq(string handle, bool Data){
    if(!isOnline) return;
    long nErr = AdsSyncWriteReqEx(port_, &Addr, ADSIGRP_SYM_VALBYHND,
        handlers[handle], sizeof(bool), &Data);
    if(nErr){ cout << "Error: AdsSyncWriteReqEx: " << nErr << '\n'; }
}

void TwincatADSNode::ReadReq(string handle, bool& Data){
    if(!isOnline) return;
    uint32_t bytesRead = 0;
    long nErr = AdsSyncReadReqEx2(port_, &Addr, ADSIGRP_SYM_VALBYHND,
        handlers[handle], sizeof(bool), &Data, &bytesRead);
    if(nErr){ cout << "Error: AdsSyncReadReqEx2: " << nErr << '\n'; }
}

void TwincatADSNode::ReadReq(string handle, double& Data){
    if(!isOnline) return;
    uint32_t bytesRead = 0;
    long nErr = AdsSyncReadReqEx2(port_, &Addr, ADSIGRP_SYM_VALBYHND,
        handlers[handle], sizeof(double), &Data, &bytesRead);
    if(nErr){ cout << "Error: AdsSyncReadReqEx2: " << nErr << '\n'; }
}

void TwincatADSNode::ReadReq(string handle, int& Data){
    if(!isOnline) return;
    uint32_t bytesRead = 0;
    long nErr = AdsSyncReadReqEx2(port_, &Addr, ADSIGRP_SYM_VALBYHND,
        handlers[handle], sizeof(int), &Data, &bytesRead);
    if(nErr){ cout << "Error: AdsSyncReadReqEx2: " << nErr << '\n'; }
}

void TwincatADSNode::ReadReq(string handle, float& Data){
    if(!isOnline) return;
    uint32_t bytesRead = 0;
    long nErr = AdsSyncReadReqEx2(port_, &Addr, ADSIGRP_SYM_VALBYHND,
        handlers[handle], sizeof(float), &Data, &bytesRead);
    if(nErr){ cout << "Error: AdsSyncReadReqEx2: " << nErr << '\n'; }
}
