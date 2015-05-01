#ifndef __MYUN2_GITHUB_COM__CAPSULE__SERVER_HPP__
#define __MYUN2_GITHUB_COM__CAPSULE__SERVER_HPP__

#include "myun2/responder/listener.hpp"
#include "myun2/cornelius/http/request.hpp"
#include "myun2/cornelius/http/response.hpp"

#include <stdio.h>
#ifdef WIN32
	#include <winsock2.h>
	#include <io.h>
	#include <fcntl.h>
	#define SO_SYNCHRONOUS_NONALERT 0x20
	#define SO_OPENTYPE     0x7008
#endif

namespace myun2
{
	namespace capsule
	{
		class server_initializer
		{
#ifdef WIN32
			::myun2::responder::wsa_cleaner _wsacl;
		public:
			server_initializer() {
				int sockopt = SO_SYNCHRONOUS_NONALERT;
				setsockopt(INVALID_SOCKET, SOL_SOCKET, SO_OPENTYPE, (char *)&sockopt, sizeof(sockopt));
			}
#endif
		};
		template <typename _ContentsProcessor>
		class server
		{
			static server_initializer _initializer;
			responder::listener listener;
			_ContentsProcessor _proc;

			int proccess_request(FILE* in, FILE* out)
			{
				char buffer[2048];
				fgets(buffer, sizeof(buffer), in);
				cornelius::http::request r = cornelius::http::request::parse(buffer);
				return _proc.process(r, out, in);
			}

			int server_callback(struct sockaddr_in addr, responder::socket_desc::sock_type sock)
			{
				FILE *fp;
			#ifdef WIN32
				fp = fdopen(_open_osfhandle(sock, _O_RDWR || _O_TEXT), "r+");
			#else
				fp = fdopen(sock, "r+");
			#endif
				setvbuf(fp, NULL, _IONBF, 0);
				return proccess_request(fp, fp);
			}
		public:
			server(unsigned short port, int max_connect=10) : listener(port, max_connect, server_callback) {}
		};
	}
}

#endif//__MYUN2_GITHUB_COM__CAPSULE__SERVER_HPP__
