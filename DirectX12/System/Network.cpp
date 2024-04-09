#include "Main.h"
#include "Network.h"

#include"System\Debug.h"

namespace Network
{

	namespace Data
	{

		Interface::Interface()
		{
			WSADATA data;
			WSAStartup(MAKEWORD(2, 0), &data);
			mSock = static_cast<int>(socket(AF_INET, SOCK_DGRAM, 0));
			if (mSock = -1) {
				Debug::Device::Log("ソケットの作成に失敗しました");
				return;
			}
		}

		Interface::~Interface()
		{
			closesocket(mSock);
			WSACleanup();
		}

		void Interface::Recv(crstring buff, int len, Address& addr)
		{
			int addrLen = sizeof(SOCKADDR_IN);
			recvfrom(
				mSock, 
				const_cast<char*>(buff.c_str()), 
				len, 
				0, 
				reinterpret_cast<SOCKADDR*>(&addr), 
				&addrLen);
		}

		void Interface::Send(crstring buff, int len, Address& addr)
		{
			sendto(
				mSock, 
				const_cast<char*>(buff.c_str()), 
				len, 
				0, 
				reinterpret_cast<SOCKADDR*>(&addr), 
				sizeof(SOCKADDR_IN));
		}

		cint Interface::Select(const FD& read, const FD& write, const FD& error, crint sec, crint min)
		{
			int max = read.mMaxSock;
			if (max < write.mMaxSock)max = write.mMaxSock;
			if (max < error.mMaxSock)max = error.mMaxSock;
			timeval tv;
			tv.tv_sec = sec;
			tv.tv_usec = min;
			return select(
				max + 1,
				reinterpret_cast<fd_set*>(const_cast<FD*>(&read)),
				reinterpret_cast<fd_set*>(const_cast<FD*>(&write)),
				reinterpret_cast<fd_set*>(const_cast<FD*>(&error)),
				&tv);
		}

	}

	namespace UDP
	{

		void Client::Init()
		{
		}

		void Client::Update()
		{
		}

		void Client::Uninit()
		{
		}

		void Server::Init()
		{
		}

		void Server::Update()
		{
		}

		void Server::Uninit()
		{
		}

		void Server::OpenTerminal(crint port)
		{
			SOCKADDR_IN addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = htonl(INADDR_ANY);

			int result = bind(
				mSock,
				reinterpret_cast<SOCKADDR*>(&addr),
				sizeof(SOCKADDR_IN));
			

		}

	}

}
