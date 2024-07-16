#pragma once
#include "transporter.h"
class TransporterMock : public Transporter
{
    public:
        void Init();
        void SetupConnection(std::string ip, uint32_t port);
        void SendEncodedData(size_t size, const char* data, bool indi, uint64_t clientID);
        InputPacket PollNextPacket(uint32_t buf_size);
        std::map<uint64_t, uint32_t> GetClientBitrates();
};

