///	----------------------------------
///	@file	socket.h
///	@data	2021/06/13
///	@brief
///	----------------------------------
#pragma once

const int USER_ID[2] = { 0, 1 }; ///< 각 클라이언트에 매길 식별자

///
///	@struct	SOCKET_COMPONENT
///	@brief	소켓 통신을 위한 멤버들을 명시해놓은 구조체.
///	
struct SOCKET_COMPONENT
{
	WSADATA wsaData; ///< 윈속 초기화를 위한 변수
	SOCKET server_socket; ///< server의 소켓을 저장
	const int server_port;  ///< 서버의 포트를 저장
	SOCKADDR_IN server_address; ///< 서버의 주소를 저장

	SOCKADDR_IN client_address[2]; ///< 클라이언트의 주소를 저장
	SOCKET client_socket[2]; ///< 클라이언트의 소켓들을 저장

	SOCKET_COMPONENT() : server_port(9876)
	{
		///Winsock2를 초기화
		std::cout << "Init Winsock...";
		if (WSAStartup(MAKEWORD(2, 2), &wsaData)) err_quit("Init Winsock2");
		std::cout << " Complete!\n";

		///소켓 통신의 과정 - socket
		std::cout << "Init server's socket... ";
		server_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (server_socket == INVALID_SOCKET) err_quit("socket()");
		std::cout << " Complete!\n";

		///소켓 통신의 과정 - bind
		std::cout << "Binding server address...";
		memset(&server_address, 0, sizeof(server_address));
		server_address.sin_family = AF_INET; //AF_INET? : IPv4 주소 체계
		server_address.sin_addr.s_addr = htonl(INADDR_ANY);
		server_address.sin_port = htons(server_port);

		if (bind(server_socket, (SOCKADDR*)&server_address, sizeof(server_address))
			== SOCKET_ERROR) err_quit("bind()");
		std::cout << " Complete!\n";

		///소켓 통신의 과정 - listen
		if (listen(server_socket, 2) == SOCKET_ERROR) err_quit("listen()");
	}

	///에러가 생길 시, 에러문을 출력하고 종료
	void err_quit(const char* msg)
	{
		std::cout << msg << '\n';
		exit(-1);
	}

	///윈속과 소켓을 닫는다.
	void CleanSocket(void)
	{
		closesocket(server_socket);
		WSACleanup();
	}
};

///	클라이언트와 연결한다
bool ConnectClient(SOCKET_COMPONENT& sc, int id);

///	클라이언트들로부터 수신받은 정보를 서로 교환하여 송신함.
void ExchangeData(SOCKET_COMPONENT& sc, int id);