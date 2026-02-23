#include <iostream>

int main()
{
	setlocale(LC_ALL, "russian_Russia.1251");
	std::cout << "Загадайте любое число, и программа попробует отгадать его.\n";
	std::cout << "Сначала задайте диапазон двумя целыми числами:\n";

	int a, b;
	std::cin >> a >> b;
	
	// Если концы диапазона совпадают, то можно ничего не считать
	if (a == b)
	{
		std::cout << "Хмм, наверно ответ " << a << std::endl;
		return 0;
	}

	if (a > b)
	{
		// Меняю числа местами
		int temp = a;
		a = b;
		b = temp;
	}

	int guess;
	bool found = false;
	while (!found)
	{
		// Предполагаем, что число посередине интервала
		guess = (a + b) / 2;

		// Пользователь сообщает, число больше, меньше или равно предложенному
		std::cout << "Ваше число " << guess << "? [>/</=]" << std::endl;
		
		char opt;
		std::cin >> opt;
		switch (opt)
		{
		case '>':
			a = guess; // Смещаем левую границу, если меньше
			break;
		case '<':
			b = guess; // Смещаем правую границу, если больше
			break;
		case '=':
			found = true; // Нашли, если равно
			break;
		default:
			// Если введенный символ плохой
			std::cout << "Я вас не понял. Попробуйте еще раз." << std::endl;
		}
	}
	
	// Вывод результата
	std::cout << "Ваше число это " << guess << ". Я молодец." << std::endl;
	return 0;
}