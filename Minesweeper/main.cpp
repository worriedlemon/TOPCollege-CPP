#include <wapi_oop.h>
#include <vector>

const int FIELD_SIZE = 10; // Размер игрового поля
const int MINE_COUNT = 10; // Количество мин

class MainWindow : public Window
{
    std::vector<std::vector<std::unique_ptr<Button>>> buttons; // Двумерный массив кнопок
    std::vector<std::vector<bool>> mines; // Двумерный массив мин
    std::vector<std::vector<int>> numbers; // Двумерный массив чисел

public:
    MainWindow(HINSTANCE h) : Window(h)
    {
        InitComponent(nullptr);

        // Инициализация игрового поля
        InitializeGameField();

        // Создание кнопок
        for (int i = 0; i < FIELD_SIZE; ++i)
        {
            for (int j = 0; j < FIELD_SIZE; ++j)
            {
                buttons[i][j] = std::make_unique<Button>(h, i * FIELD_SIZE + j, this);
                buttons[i][j]->SetSize(50, 50);
                buttons[i][j]->SetPosition(j * 50 + 20, i * 50 + 20);
                buttons[i][j]->SetText(L"");
            }
        }

        SetSize(640, 640);
        SetStyle(WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX);
        SetText(L"Сапер");
    }

    ~MainWindow()
    {
        // Освобождение ресурсов
        for (int i = 0; i < FIELD_SIZE; ++i)
        {
            for (int j = 0; j < FIELD_SIZE; ++j)
            {
                buttons[i][j].reset();
            }
        }
    }

    LRESULT OnCommand(WPARAM wp, LPARAM lp) override
    {
        WORD id = LOWORD(wp), code = HIWORD(wp);

        // Обработка нажатия на кнопку
        if (code == BN_CLICKED)
        {
            int row = id / FIELD_SIZE;
            int col = id % FIELD_SIZE;

            if (mines[row][col])
            {
                // Если нажата мина, игра окончена
                GameOver();
            }
            else
            {
                // Если не мина, открываем клетку
                OpenCell(row, col);
            }
        }
        return 0;
    }

    void InitializeGameField()
    {
        buttons.resize(FIELD_SIZE);
        mines.resize(FIELD_SIZE);
        numbers.resize(FIELD_SIZE);

        for (int i = 0; i < FIELD_SIZE; ++i)
        {
            buttons[i].resize(FIELD_SIZE);
            mines[i].resize(FIELD_SIZE);
            numbers[i].resize(FIELD_SIZE);
        }

        // Размещение мин
        for (int i = 0; i < MINE_COUNT; ++i)
        {
            int row, col;
            do
            {
                row = rand() % FIELD_SIZE;
                col = rand() % FIELD_SIZE;
            } while (mines[row][col]);

            mines[row][col] = true;
        }

        // Расчет чисел
        for (int i = 0; i < FIELD_SIZE; ++i)
        {
            for (int j = 0; j < FIELD_SIZE; ++j)
            {
                if (!mines[i][j])
                {
                    numbers[i][j] = CountAdjacentMines(i, j);
                }
            }
        }
    }

    int CountAdjacentMines(int row, int col)
    {
        int count = 0;
        for (int i = -1; i <= 1; ++i)
        {
            for (int j = -1; j <= 1; ++j)
            {
                int r = row + i;
                int c = col + j;
                if (r >= 0 && r < FIELD_SIZE && c >= 0 && c < FIELD_SIZE && mines[r][c])
                {
                    count++;
                }
            }
        }
        return count;
    }

    void OpenCell(int row, int col)
    {
        if (numbers[row][col] > 0)
        {
            buttons[row][col]->SetText(std::to_wstring(numbers[row][col]).c_str());
            buttons[row][col]->SetEnabled(false);
        }
        else
        {
            buttons[row][col]->SetText(L"");
            buttons[row][col]->SetEnabled(false);
            // Рекурсивное открытие соседних клеток
            for (int i = -1; i <= 1; ++i)
            {
                for (int j = -1; j <= 1; ++j)
                {
                    int r = row + i;
                    int c = col + j;
                    if (r >= 0 && r < FIELD_SIZE && c >= 0 && c < FIELD_SIZE && buttons[r][c]->IsEnabled())
                    {
                        OpenCell(r, c);
                    }
                }
            }
        }
    }

    void GameOver()
    {
        MessageBoxW(hwnd, L"Вы проиграли!", L"Игра окончена", MB_OK);
        for (int i = 0; i < FIELD_SIZE; ++i)
        {
            for (int j = 0; j < FIELD_SIZE; ++j)
            {
                buttons[i][j]->SetEnabled(false);
                if (mines[i][j])
                {
                    buttons[i][j]->SetText(L"X");
                }
            }
        }
    }

protected:
    DECLARE_CLASS_NAME(MainWindow);
};

int WINAPI WinMain(HINSTANCE h, HINSTANCE hprev, LPSTR cmdLine, int nShow)
{
    RegisterControl(MainWindow, h);

    MainWindow wnd(h);
    wnd.Show();

    wnd.ProcessMessages();
    return EXIT_SUCCESS;
}
