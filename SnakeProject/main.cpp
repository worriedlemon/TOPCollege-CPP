#include "snake.h"
#include <ctime>
#include <conio.h>

// ‘ункци€ не работает в общем случае - нужно еще провер€ть, чтобы €блоко не попало на змею
void generateApple(COORD* apple, COORD* size, int color)
{
    int w = size->X, h = size->Y;
    apple->X = (rand() % (w - 2)) + 1;
    apple->Y = (rand() % (h - 2)) + 1;

    SetConsoleCursorPosition(console, *apple);
    printf("\033[%dm%c\033[0m", color, '@');
}

void drawFrame(COORD* size)
{
    int w = size->X, h = size->Y;

    for (int i = 0; i < w; i++)
    for (int j = 0; j < h; j++)
    {
        if (i == 0 || i == w - 1 || j == 0 || j == h - 1)
        {
            COORD c = { i, j };
            SetConsoleCursorPosition(console, c);
            printf("\033[107m%c\033[0m", ' ');
        }
    }
}

bool stop = false;

void getKey(Snake* snake)
{
    Direction newDir = Direction::NONE;

    if (GetAsyncKeyState(Key::K_UP))
        newDir = Direction::UP;
    else if (GetAsyncKeyState(Key::K_LEFT))
        newDir = Direction::LEFT;
    else if (GetAsyncKeyState(Key::K_DOWN))
        newDir = Direction::DOWN;
    else if (GetAsyncKeyState(Key::K_RIGHT))
        newDir = Direction::RIGHT;
    
    if (GetAsyncKeyState(Key::ESC) || GetAsyncKeyState(Key::BREAK))
    {
        stop = true;
        return;
    }

    if (newDir == Direction::NONE) return;

    if (snake->direction != -newDir) snake->direction = newDir;
}


int main()
{
    {
        printf("%s\n%s\n", "SNAKE", "Press any key to start!");
        auto _ = _getch();
        system("cls");
    }

    COORD size = { 50, 22 };
    drawFrame(&size);

    srand(time(0));
    COORD apple;
    generateApple(&apple, &size, 92);

    Snake s;

    while (!stop)
    {
        getKey(&s);
        s.drawSnake(0);

        if (s.checkApple(&apple))
        {
            generateApple(&apple, &size, 92);
            s.grow();
        }

        s.moveSnake();

        stop = !s.checkBounds(&size);

        s.drawSnake(41);

        Sleep(200);
    }

    system("cls"); // очищаем экран
    printf("%s", "Game over!");

    return 0;
}

