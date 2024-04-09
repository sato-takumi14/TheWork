#pragma once

#include<winsock.h>
#include<conio.h>

#pragma comment (lib, "ws2_32.lib")

namespace Network
{

	namespace Data
	{

		class Address
		{
		private:

			SOCKADDR_IN mAddr{};

		public:

			inline Address() {}
			inline Address(crint port, const u_long& addr) {
				this->mAddr.sin_family = AF_INET;
				this->mAddr.sin_port = htons(port);
				this->mAddr.sin_addr.s_addr = htonl(addr);
			}
			inline Address(crint port, crstring addr) {
				this->mAddr.sin_family = AF_INET;
				this->mAddr.sin_port = htons(port);
				this->mAddr.sin_addr.s_addr = inet_addr(addr.c_str());
			}
			operator SOCKADDR_IN& () { return mAddr; }

		};

		class FD
		{
		public:

			int mMaxSock = -1;
			fd_set mFD{};

		public:

			void Set(crint sock) {
				FD_SET(sock, &mFD);
				if (mMaxSock < sock)mMaxSock = sock;
			}

			void Remove(crint sock) {
				FD_CLR(sock, &mFD);
				if (mMaxSock == sock) {
					mMaxSock = -1;
					for (uint i = 0; i < mFD.fd_count; i++) {
						if (mMaxSock < mFD.fd_array[i])mMaxSock = static_cast<int>(mFD.fd_array[i]);
					}
				}
			}

		};

		class Data
		{
		public:

		};

		class Interface
		{
		protected:
			
			int mSock = 0;

		public:

			Interface();
			~Interface();

			void Recv(crstring buff, int len, Address& addr);
			void Send(crstring buff, int len, Address& addr);
			cint Select(const FD& read, const FD& write, const FD& error, crint sec, crint min);

		};

	}

	namespace UDP
	{
		class Client : public Data::Interface
		{
		private:

			

		public:

			void Init();
			void Update();
			void Uninit();

		};

		class Server : public Data::Interface
		{
		private:



		public:

			void Init();
			void Update();
			void Uninit();

			void OpenTerminal(crint port);

		};
	}

}