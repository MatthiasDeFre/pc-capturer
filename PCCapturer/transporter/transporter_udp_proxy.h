#pragma once
#include "transporter.h"
#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <chrono>
#include <memory.h>
#include <map>
#include <vector>
#include <queue>
#include <thread>
#include <windows.h>

class TransporterUDPProxy : public Transporter
{
public:
    void SetFragmentSize(int _fragmentSize);
    void Init();
    void SetupConnection(std::string ip, uint32_t port);
    void SendEncodedData(size_t size, const char* data, bool indi, uint64_t clientID);
    std::map<uint64_t, uint32_t> GetClientBitrates();
    InputPacket PollNextPacket(uint32_t buf_size);
private:
    WSADATA wsa;
    WSAPOLLFD pollfds[1];
    struct sockaddr_in si_other;
    int s, slen = sizeof(si_other);
};

