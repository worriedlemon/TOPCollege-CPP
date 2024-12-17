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

	// ���� ������ �������
	char ip[16];
	short port;
	std::cout << "Enter the server IP address: "; // ����� � ������� aaa.aaa.aaa.aaa
	std::cin >> ip;
	std::cout << "Enter the server port: "; // ���� � ������� bbbbb (������ 5150)
	std::cin >> port;

	// �������� ������
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// �������� ��������� ������ �������
	sockaddr_in server{};
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_port = htons(port);

	// ���������� � ��������
	int result = connect(sock, (sockaddr*)&server, sizeof(server));
	if (result != 0)
	{
		std::cerr << "Connection failed." << std::endl;
		closesocket(sock);
		return WSAGetLastError();
	}

	std::cout << "Connection successfull!\n";

	int player;
	recv(sock, BUFFER(player), 0);

	std::cout << "You are Player #" << player << std::endl;

	Data data(player);

	// ������� ������� ������������
	{
		std::cout << "Waiting for other player." << std::endl;
		int wait;
		recv(sock, BUFFER(wait), 0);
	}

	{
		system("cls"); // ������� �����

		/*   ������ �������
		* 1) �������� ������ � ������������ �������� ����� (4-� �. - 1, 3-� �. - 2, 2-� �. - 3, 4-� �. - 4)
		*    �� ��������, ��� ���������� ��������� ������������ ������������ �������� (��� ������� ���� ������ ������ ����� ����)
		* 2) ��������� ������ �� ������ � ���� ��������� Data � ������� ������� send
		* 3) ������� ������� ������������
		* 4) ����� ��� ������ ������, ����� ������ ���� - ���� �������������� �� ������ ����� ����� ���������
		* 5) ��� ��������� ���� ������� ������� ������������, ��� ������� - ��� ���������� ������� ������
		* 6) ���������� ������������ ������, ���������� �� ������� - ��� ���� (������ � ����������)
		* 7) ���� ������������ � ��� ������, ����� � ������ �� ������� ��������� ���� ����
		*/



	}


	closesocket(sock);

	WSACleanup();

	return EXIT_SUCCESS;
}