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

	// Создаем сокет
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Could not create socket!" << std::endl;
		return WSAGetLastError();
	}

	// Получаем IP-адреса хоста
	hostent* localHost = gethostbyname("");
	char* localIP = inet_ntoa(*(in_addr*)(*localHost->h_addr_list));

	// Создаем структуру адреса сервера
	sockaddr_in server{};
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(localIP);
	server.sin_port = htons(5150);

	std::cout << "Your server address: " << localIP << ":" << 5150 << std::endl;

	int result;

	// Соединяем сокет с адресом - теперь пакеты по адресу сервера будут приходить на сокет
	result = bind(sock, (sockaddr*)&server, sizeof(server));
	if (result != 0)
	{
		std::cerr << "Failed to bind!" << std::endl;
		closesocket(sock);
		return WSAGetLastError();
	}

	std::cout << "Waiting for first player connection...\n";
	
	// Прослушиваем два входящих соединения
	result = listen(sock, 2);
	if (result != 0)
	{
		std::cerr << "Failed to bind!" << std::endl;
		closesocket(sock);
		return WSAGetLastError();
	}

	// Получаем первое подключение
	SOCKET firstClient = accept(sock, nullptr, nullptr);
	if (firstClient == INVALID_SOCKET)
	{
		std::cerr << "Connection refused." << std::endl;
		closesocket(sock);
		return WSAGetLastError();
	}

	std::cout << "First player connected.\n";
	{
		// Отправляем информацию клиенту, о том, что он - первый игрок
		int first = 1;
		send(firstClient, BUFFER(first), 0);
	}
	std::cout << "Waiting for second player connection...\n";

	// Получаем второе подключение
	SOCKET secondClient = accept(sock, nullptr, nullptr);
	if (secondClient == INVALID_SOCKET)
	{
		std::cerr << "Connection refused." << std::endl;
		closesocket(sock);
		return WSAGetLastError();
	}

	std::cout << "Second player connected.\n";
	{
		// Отправляем информацию клиенту, о том, что он - второй игрок
		int second = 2;
		send(secondClient, BUFFER(second), 0);
	}

	// Отправляем обоим игрокам флаг, что они готовы к игре
	{
		int wait = 0;
		send(firstClient, BUFFER(wait), 0);
		send(secondClient, BUFFER(wait), 0);
	}

	{
		/*   Логика работы сервера
		* 1) Обработка входящих данных в виде структуры Data
		* 2) Получение данных о ходе игрока (кто куда выстрелил) и отправление обратно результатов
		* 3) Данные от обоих игроков желательно ожидать в двух потоках, хотя можно воспользоваться функциями select и poll
		*/
	}


	closesocket(sock);

	WSACleanup();

	return EXIT_SUCCESS;
}