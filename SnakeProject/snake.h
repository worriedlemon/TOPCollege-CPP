#pragma once

#include <list>
#include <windows.h>

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

enum Key
{
    K_UP = VK_UP,
    K_LEFT = VK_LEFT,
    K_DOWN = VK_DOWN,
    K_RIGHT = VK_RIGHT,
    ESC = VK_ESCAPE,
    BREAK = VK_CANCEL
};

enum Direction
{
    LEFT = -1, RIGHT = 1, UP = -2, DOWN = 2, NONE = 0
};

struct Snake
{
    std::list<COORD> snake;
    Direction direction;

    Snake()
    {
        snake = { { 20, 8 }, { 20, 9 }, { 20, 10 } };
        direction = Direction::UP;
    }

    void drawSnake(int color)
    {
        for (COORD value : snake)
        {
            SetConsoleCursorPosition(console, value);
            printf("\033[%dm%c\033[0m", color, ' ');
        }
    }

    void moveSnake()
    {
        const COORD prev = snake.front();
        COORD next;

        switch (direction)
        {
        case Direction::LEFT:
            next.X = prev.X - 1;
            next.Y = prev.Y;
            break;
        case Direction::RIGHT:
            next.X = prev.X + 1;
            next.Y = prev.Y;
            break;
        case Direction::UP:
            next.X = prev.X;
            next.Y = prev.Y - 1;
            break;
        case Direction::DOWN:
            next.X = prev.X;
            next.Y = prev.Y + 1;
            break;
        }

        snake.push_front(next);
        snake.pop_back();
    }

    void grow()
    {
        snake.push_back(snake.back());
    }

    bool checkApple(COORD* apple)
    {
        COORD head = snake.front();
        return head.X == apple->X && head.Y == apple->Y;
    }

    bool checkBounds(COORD* size)
    {
        int w = size->X, h = size->Y;
        COORD head = snake.front();
        return (head.Y > 0 && head.Y < h - 1 && head.X > 0 && head.X < w - 1);
    }
};