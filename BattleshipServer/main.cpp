#include <sockets.h>
#include <data.h>

#include <iostream>

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

int main()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		std::cerr << "Error initializing application!" << std::endl;
		return WSAGetLastError();
	}

	// ������� �����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Could not create socket!" << std::endl;
		return WSAGetLastError();
	}

	// �������� IP-������ �����
	hostent* localHost = gethostbyname("");
	char* localIP = inet_ntoa(*(in_addr*)(*localHost->h_addr_list));

	// ������� ��������� ������ �������
	sockaddr_in server{};
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(localIP);
	server.sin_port = htons(5150);

	std::cout << "Your server address: " << localIP << ":" << 5150 << std::endl;

	int result;

	// ��������� ����� � ������� - ������ ������ �� ������ ������� ����� ��������� �� �����
	result = bind(sock, (sockaddr*)&server, sizeof(server));
	if (result != 0)
	{
		std::cerr << "Failed to bind!" << std::endl;
		closesocket(sock);
		return WSAGetLastError();
	}

	std::cout << "Waiting for first player connection...\n";
	
	// ������������ ��� �������� ����������
	result = listen(sock, 2);
	if (result != 0)
	{
		std::cerr << "Failed to bind!" << std::endl;
		closesocket(sock);
		return WSAGetLastError();
	}

	// �������� ������ �����������
	SOCKET firstClient = accept(sock, nullptr, nullptr);
	if (firstClient == INVALID_SOCKET)
	{
		std::cerr << "Connection refused." << std::endl;
		closesocket(sock);
		return WSAGetLastError();
	}

	std::cout << "First player connected.\n";
	{
		// ���������� ���������� �������, � ���, ��� �� - ������ �����
		int first = 1;
		send(firstClient, BUFFER(first), 0);
	}
	std::cout << "Waiting for second player connection...\n";

	// �������� ������ �����������
	SOCKET secondClient = accept(sock, nullptr, nullptr);
	if (secondClient == INVALID_SOCKET)
	{
		std::cerr << "Connection refused." << std::endl;
		closesocket(sock);
		return WSAGetLastError();
	}

	std::cout << "Second player connected.\n";
	{
		// ���������� ���������� �������, � ���, ��� �� - ������ �����
		int second = 2;
		send(secondClient, BUFFER(second), 0);
	}

	// ���������� ����� ������� ����, ��� ��� ������ � ����
	{
		int wait = 0;
		send(firstClient, BUFFER(wait), 0);
		send(secondClient, BUFFER(wait), 0);
	}

	{
		/*   ������ ������ �������
		* 1) ��������� �������� ������ � ���� ��������� Data
		* 2) ��������� ������ � ���� ������ (��� ���� ���������) � ����������� ������� �����������
		* 3) ������ �� ����� ������� ���������� ������� � ���� �������, ���� ����� ��������������� ��������� select � poll
		*/
	}


	closesocket(sock);

	WSACleanup();

	return EXIT_SUCCESS;
}