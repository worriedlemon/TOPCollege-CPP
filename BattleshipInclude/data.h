#pragma once

constexpr int fieldSize = 10;

struct Data
{
	char data[fieldSize][fieldSize];
	int player;

	// ����������� ���������
	Data(int playerId);

	// ����� ��� ������ ������ �� �����
	void output();
};