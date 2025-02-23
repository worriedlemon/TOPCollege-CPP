#include <wapi_oop.h>
#include <cmath>

// Класс нового окна наследуется от абстракного окна
class MainWindow : public Window
{
	// Для использования внутри окна других графических
	// объектов используется std::unique_ptr
	std::unique_ptr<PaintBox> pb;

public:
	MainWindow(HINSTANCE hInstance) : Window(hInstance)
	{
		// Основное окно не имеет родителя
		// Этот метод вызывается первым
		InitComponent(nullptr);

		// Инициализация вложенных полей
		pb = std::make_unique<PaintBox>(hInstance, 1, this);
		
		// Установка свойств полей
		pb->SetPosition(10, 10);
		pb->SetSize(512, 512);

		// Установка свойств окна
		SetSize(550, 570);
		SetText(L"Painting in C++");
		SetStyle(WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
	}
	~MainWindow() = default;

	// Функция для отрисовки синуса
	void DrawSin()
	{
		double offX = 10, offY = 200;
		double scX = 40, scY = 40;
		double h = 2.5e-1;
		double maxt = 10;
		int xprev = offX, yprev = offY;

		// График
		pb->SetPen(PaintBox::Solid, 3, { 255, 0, 0 }); // Устанавливаем красный цвет
		for (double t = 0; t <= maxt; t += h)
		{
			int x = offX + t * scX;
			int y = offY - sin(t) * scY; // Минус для переворачивания
			pb->Draw<Figure::LINE>({ xprev, yprev, x, y });
			xprev = x;
			yprev = y;
		}

		// Оси
		pb->SetPen(PaintBox::Solid, 1, { 0, 0, 0 }); // Устанавливаем черный цвет
		pb->Draw<Figure::LINE>({ (int)offX, (int)offY, (int)(offX + maxt * scX), (int)offY});
		pb->Draw<Figure::LINE>({ (int)offX, (int)(offY - scY - 10), (int)offX, (int)(offY + scY + 10) });
		
		// Подписи
		pb->DrawString(offX, offY - scY - 30, L"sin(t)");
		pb->DrawString(offX + maxt * scX + 10, offY, L"t, s");

		pb->Update(); // Отправляем все данные на отрисовку
	}

protected:
	// Обязательно определяем имя класса
	DECLARE_CLASS_NAME(MainWindow)
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR cmdLine, int nCmdShow)
{
	// Регистрация класса PaintBox
	REGISTER_DEFAULT(hInstance);

	// Регистрация основного окна
	RegisterControl(MainWindow, hInstance);

	MainWindow mw(hInstance);
	mw.Show();
	mw.DrawSin();

	mw.ProcessMessages();

	return 0;
}