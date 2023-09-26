#pragma once
#include <vector>
#include <string>
struct MessageType {
	uint8_t packet_type;
};
struct ConnectionData {
	MessageType msg_type;
	uint32_t extra_data_len;
};
struct PacketHeader {
	uint8_t packet_type;
	uint32_t frame_nr;
	uint32_t frame_len;
	uint32_t frame_offset;
	uint32_t packet_len;
};
struct InputPacket {
	uint32_t status;
	size_t packet_len;
	char* data;
};
class Transporter
{
public:
	virtual void Init() = 0;
	virtual void SetupConnection(std::string ip, uint32_t port) = 0;
	virtual void SendEncodedData(size_t size, const char*) = 0;
	virtual InputPacket PollNextPacket(uint32_t buf_size) = 0;
	bool GetIsClientConnected();
protected:
	bool isClientConnected;
	uint32_t frame_counter = 0;
	std::vector<char> input_buffer;
	std::vector<char> output_buffer;
};

