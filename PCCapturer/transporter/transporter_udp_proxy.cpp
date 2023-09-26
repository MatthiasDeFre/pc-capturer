#include "transporter_udp_proxy.h"

// 172.22.107.250
//#define SERVER "193.190.127.246"	//ip address of udp server NO LONGER USED
#define BUFLEN 1500	//Max length of buffer
#define PORT 8001
void TransporterUDPProxy::Init() {
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("WSA ERROR");
		exit(EXIT_FAILURE);
	}

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("SOCKET ERROR");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	ULONG buf_size = 524288000;
	setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char*)&buf_size, sizeof(ULONG));
}

// Connection setup performed by sending / receiving a simple hello message
void TransporterUDPProxy::SetupConnection(std::string ip, uint32_t port) {
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &si_other.sin_addr.S_un.S_addr);

	char buf_connect[1500] = { 0 };

	struct ConnectionData td = { 0, 0 };
	memcpy(buf_connect, &td, sizeof(td));

	int size = 0;
	if ((size = sendto(s, buf_connect, 1500, 0, (struct sockaddr*)&si_other, slen)) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	pollfds[0].fd = s;
	pollfds[0].events = POLLIN;

	InputPacket p = PollNextPacket(1500);

	if (!std::string(p.data, strlen(p.data) + 1).compare("Hello!")) {
		exit(EXIT_FAILURE);
	}
	isClientConnected = true;
}

void TransporterUDPProxy::SendEncodedData(size_t size, const char* encoded_pc) {
	uint32_t current_offset = 0;
	int remaining = size;
	size_t data_size = (1300 - sizeof(PacketHeader));
	size_t next_size = data_size;
	int size_send = 0;
	int size_full = 0;
	// Output every 100 frames
	if (frame_counter % 100 == 0)
	{
		std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		);
		std::cout << "Frame Complete " << frame_counter << " TS=" << ms.count() << std::endl;
		// Remove newly added frame to make sure that memory doesn't get full
	}

	// Send in small packets instead of complete frame
	// Prevent IP layer fragmentation
	while (remaining > 0) {
		char buf_msg[1300];
		struct PacketHeader p { 2, frame_counter, size, current_offset, next_size };
		memcpy(buf_msg, &p, sizeof(p));
		memcpy(buf_msg + sizeof(p), encoded_pc + current_offset, next_size);
		remaining -= data_size;
		if (remaining < data_size)
			next_size = remaining;
		current_offset += data_size;
		if ((size_send = sendto(s, buf_msg, 1300, 0, (struct sockaddr*)&si_other, slen)) == SOCKET_ERROR)
		{
			return;
		}
		size_full += size_send;
	}
	//ResultWriter::addSendTimestamp(frame_counter, size_full);
	//ResultWriter::setFrameReadyToSave(frame_counter);
	frame_counter++;
	return;
}

InputPacket TransporterUDPProxy::PollNextPacket(uint32_t buf_size)
{	
	if (buf_size > input_buffer.capacity()) {
		input_buffer.reserve(buf_size);
	}
	int result = WSAPoll(pollfds, 1, 5000);
	if (result == SOCKET_ERROR) {
		// Error handling
		return InputPacket(1);
	}
	else if (result == 0) {
		// Timeout
		return InputPacket(2);
	}
	size_t packet_len;
	if ((packet_len = recvfrom(s, input_buffer.data(), buf_size, 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR)
	{
		return InputPacket(3);
	}
	return InputPacket(0, packet_len, input_buffer.data());
}
