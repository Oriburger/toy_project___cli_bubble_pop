///	----------------------------------
///	@file	socket.h
///	@data	2021/06/13
///	@brief
///	----------------------------------
#pragma once

const int USER_ID[2] = { 0, 1 }; ///< �� Ŭ���̾�Ʈ�� �ű� �ĺ���

///
///	@struct	SOCKET_COMPONENT
///	@brief	���� ����� ���� ������� ����س��� ����ü.
///	
struct SOCKET_COMPONENT
{
	WSADATA wsaData; ///< ���� �ʱ�ȭ�� ���� ����
	SOCKET server_socket; ///< server�� ������ ����
	const int server_port;  ///< ������ ��Ʈ�� ����
	SOCKADDR_IN server_address; ///< ������ �ּҸ� ����

	SOCKADDR_IN client_address[2]; ///< Ŭ���̾�Ʈ�� �ּҸ� ����
	SOCKET client_socket[2]; ///< Ŭ���̾�Ʈ�� ���ϵ��� ����

	SOCKET_COMPONENT() : server_port(9876)
	{
		///Winsock2�� �ʱ�ȭ
		std::cout << "Init Winsock...";
		if (WSAStartup(MAKEWORD(2, 2), &wsaData)) err_quit("Init Winsock2");
		std::cout << " Complete!\n";

		///���� ����� ���� - socket
		std::cout << "Init server's socket... ";
		server_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (server_socket == INVALID_SOCKET) err_quit("socket()");
		std::cout << " Complete!\n";

		///���� ����� ���� - bind
		std::cout << "Binding server address...";
		memset(&server_address, 0, sizeof(server_address));
		server_address.sin_family = AF_INET; //AF_INET? : IPv4 �ּ� ü��
		server_address.sin_addr.s_addr = htonl(INADDR_ANY);
		server_address.sin_port = htons(server_port);

		if (bind(server_socket, (SOCKADDR*)&server_address, sizeof(server_address))
			== SOCKET_ERROR) err_quit("bind()");
		std::cout << " Complete!\n";

		///���� ����� ���� - listen
		if (listen(server_socket, 2) == SOCKET_ERROR) err_quit("listen()");
	}

	///������ ���� ��, �������� ����ϰ� ����
	void err_quit(const char* msg)
	{
		std::cout << msg << '\n';
		exit(-1);
	}

	///���Ӱ� ������ �ݴ´�.
	void CleanSocket(void)
	{
		closesocket(server_socket);
		WSACleanup();
	}
};

///	Ŭ���̾�Ʈ�� �����Ѵ�
bool ConnectClient(SOCKET_COMPONENT& sc, int id);

///	Ŭ���̾�Ʈ��κ��� ���Ź��� ������ ���� ��ȯ�Ͽ� �۽���.
void ExchangeData(SOCKET_COMPONENT& sc, int id);