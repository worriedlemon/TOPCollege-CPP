#pragma once

constexpr int fieldSize = 10;

struct Data
{
	char data[fieldSize][fieldSize];
	int player;

	// Конструктор структуры
	Data(int playerId);

	// Метод для вывода данных на экран
	void output();
};