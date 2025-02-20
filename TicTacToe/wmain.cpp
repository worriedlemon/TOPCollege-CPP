#include <wapi_oop.h>

#define BUTTON_SIZE 100
#define BUTTON_GAP 10

class MainWindow : public Window
{
	std::unique_ptr<Button> buttons[3][3];
	HFONT font;
	int moves;

	enum Player : wchar_t
	{
		First = L'X',
		Second = L'O'
	} currentPlayer = Player::First;

	void ButtonUnhash(WORD index, int8_t& i, int8_t& j)
	{
		i = index / 10;
		j = index % 10;
	}

	WORD ButtonHash(int8_t i, int8_t j)
	{
		return static_cast<WORD>(i * 10 + j);
	}

	int CheckWin(int i, int j)
	{
		std::wstring const& player = buttons[i][j]->GetText();
		int hor = 0, vert = 0, diag1 = 0, diag2 = 0;

		// Горизонтально и вертикально
		for (int k = 0; k < 3; ++k)
		{
			if (k != i && player == buttons[k][j]->GetText()) ++hor;
			if (k != j && player == buttons[i][k]->GetText()) ++vert;
		}

		// Диагонали
		if (i == j || i == 2 - j)
		{
			for (int k = 0; k < 3; ++k)
			{
				if (player == buttons[k][k]->GetText()) ++diag1;
				if (player == buttons[k][2 - k]->GetText()) ++diag2;
			}
		}

		if (hor == 2 || vert == 2 || diag1 == 3 || diag2 == 3) return 1;

		return moves == 9 ? 0 : -1;
	}

	void Reset()
	{
		moves = 0;
		currentPlayer = Player::First;
		for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
		{
			buttons[i][j]->SetText(L"");
			buttons[i][j]->SetEnabled(true);
		}
	}

public:
	MainWindow(HINSTANCE hInstance) : Window(hInstance), moves(0)
	{
		InitComponent(nullptr);

		font = CreateFontW(54, 0, 0, 0,
			FW_BOLD, false, false, false,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				buttons[i][j] = std::make_unique<Button>(hInstance, ButtonHash(i, j) + 1, this);
				buttons[i][j]->SetPosition(i * (BUTTON_SIZE + BUTTON_GAP) + BUTTON_GAP, j * (BUTTON_SIZE + BUTTON_GAP) + BUTTON_GAP);
				buttons[i][j]->SetSize(BUTTON_SIZE, BUTTON_SIZE);
				buttons[i][j]->SetFont(font);
			}
		}

		SetText(L"Крестики-нолики");
		SetStyle(WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
		SetSize(3 * (BUTTON_SIZE + BUTTON_GAP) + 2 * BUTTON_GAP + 6, 3 * (BUTTON_SIZE + BUTTON_GAP) + 2 * BUTTON_GAP + 30);
	}
	~MainWindow()
	{
		DeleteObject(font);
	}

	LRESULT OnCommand(WPARAM wp, LPARAM lp) override
	{
		WORD btn = LOWORD(wp), code = HIWORD(wp);
		if (code == BN_CLICKED)
		{
			int8_t i, j;
			ButtonUnhash(btn - 1, i, j);
			buttons[i][j]->SetEnabled(false);
			WCHAR text[2] = { currentPlayer, 0 };
			buttons[i][j]->SetText(text);
			++moves;
			
			int result = -1;
			switch (CheckWin(i, j))
			{
			case 1:
				result = MessageBoxW(hwnd, (L"Игрок '" + std::wstring(text) + L"' выиграл!\nЗаново?").c_str(), L"Победа", MB_YESNO);
				break;
			case 0:
				result = MessageBoxW(hwnd, L"Ничья.\nЗаново?", L"Ничья", MB_YESNO);
				break;
			}

			if (result == IDYES) Reset();
			else if (result == IDNO)
			{
				for (int i = 0; i < 3; ++i)
				for (int j = 0; j < 3; ++j)
				{
					buttons[i][j]->SetEnabled(false);
				}
			}

			currentPlayer = currentPlayer == Player::First ? Player::Second : Player::First;
		}
		return 0;
	}

	LRESULT OnPaint(HDC& hdc, PAINTSTRUCT& ps, WPARAM wp, LPARAM lp) override
	{
		FillRect(hdc, &ps.rcPaint, reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1));
		return 0;
	}

	LRESULT OnClose(WPARAM wp, LPARAM lp) override
	{
		if (MessageBoxW(hwnd, L"Вы уверены, что хотите выйти?", L"Подтвердите действие", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			DestroyWindow(hwnd);
		}
		return 0;
	}

protected:
	DECLARE_CLASS_NAME(MainWindow)
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR cmdLine, int nCmdShow)
{
	RegisterControl(MainWindow, hInstance);

	MainWindow mw(hInstance);
	mw.Show();

	mw.ProcessMessages();

	return 0;
}