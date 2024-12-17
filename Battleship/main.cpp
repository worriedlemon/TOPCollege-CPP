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

	// Ввод данных сервера
	char ip[16];
	short port;
	std::cout << "Enter the server IP address: "; // Адрес в формате aaa.aaa.aaa.aaa
	std::cin >> ip;
	std::cout << "Enter the server port: "; // Порт в формате bbbbb (обычно 5150)
	std::cin >> port;

	// Создание сокета
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Создание структуры адреса сервера
	sockaddr_in server{};
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_port = htons(port);

	// Соединение с сервером
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

	// Ожидаем второго пользователя
	{
		std::cout << "Waiting for other player." << std::endl;
		int wait;
		recv(sock, BUFFER(wait), 0);
	}

	{
		system("cls"); // очищаем экран

		/*   Логика клиента
		* 1) Получить данные о расположении кораблей флота (4-х п. - 1, 3-х п. - 2, 2-х п. - 3, 4-х п. - 4)
		*    Не забываем, что необходимо проверить корректность расположения кораблей (как минимум одна пустая клетка между ними)
		* 2) Отправить данные на сервер в виде структуры Data с помощью функции send
		* 3) Ожидать второго пользователя
		* 4) Когда оба игрока готовы, нужно начать игру - ходы осуществляются по одному путем ввода координат
		* 5) При попадании игра текущим игроком продолжается, при промахе - ход передается второму игроку
		* 6) Необходимо отрисовывать данные, полученные от сервера - два поля (игрока и противника)
		* 7) Игра оканчивается в тот момент, когда у одного из игроков уничтожен весь флот
		*/



	}


	closesocket(sock);

	WSACleanup();

	return EXIT_SUCCESS;
}