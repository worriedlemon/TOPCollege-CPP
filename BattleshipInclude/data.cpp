#include "data.h"

Data::Data(int playerId)
{
	player = playerId;
	for (int i = 0; i < fieldSize; ++i)
	{
		for (int j = 0; j < fieldSize; ++j)
		{
			data[i][j] = 0;
		}
	}
}

void Data::output()
{

}