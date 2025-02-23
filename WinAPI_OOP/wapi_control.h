#pragma once

#ifndef WINAPI_CONTROL_H
#define WINAPI_CONTROL_H

#include <windows.h>
#include <string>
#include <map>
#include <memory>
#include <stdexcept>
#include <set>

/// Макрос для преобразования имени класса в строку
#define CONTROL_NAME(class) TEXT(#class)

/// Макрос для переопределения метода ClassName
#define DECLARE_CLASS_NAME(class) \
std::wstring ClassName() const override \
{ return CONTROL_NAME(class); }

/// <summary>
/// Общий абстрактный класс для любых графических объектов
/// </summary>
class Control
{
protected:
	HWND hwnd;
	HINSTANCE hInstance;
	std::wstring text;

	int x, y, cx, cy;

	/// <summary>
	/// Инициализация компонента
	/// </summary>
	/// <param name="parent">Родительский объект</param>
	void InitComponent(Control* parent);

private:
	// Словарь с созданными объектами
	static std::map<HWND, Control*> controls;

	// Словарь с зарегистрированными классами
	static std::map<std::wstring, std::shared_ptr<WNDCLASS>> classes;

public:
	/// <summary>
	/// Регистрация класса (не использовать напрямую, лучше использовать макрос RegisterControl)
	/// </summary>
	/// <param name="name">Имя класса</param>
	/// <param name="hInstance">Дескриптор экземпляра файла</param>
	static void Register(std::wstring name, HINSTANCE hInstance);

	/// <summary>
	/// Проверка регистрации класса (используется для отладки)
	/// </summary>
	/// <param name="name">Имя класса</param>
	/// <returns>true, если класс зарегистрирован, false - если нет</returns>
	static inline bool IsRegistered(std::wstring name)
	{
		std::set<std::wstring> defaultClasses = {
			L"BUTTON", L"EDIT", L"COMBOBOX", L"LISTBOX", L"ScrollBar"
		};

		return defaultClasses.find(name) != defaultClasses.end() ||
			classes.find(name) != classes.end();
	}

	// Основной обработчик сообщений WINAPI
	static LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	// Конструктор
	Control(HINSTANCE hInstance, Control* parent);

	// Деструктор
	virtual ~Control();

	/// <summary>
	/// Установить текст объекта
	/// </summary>
	/// <param name="text">Отображаемый текст</param>
	void SetText(std::wstring text);

	/// <summary>
	/// Получить текст объекта
	/// </summary>
	/// <returns>Отображаемый текст</returns>
	std::wstring GetText() const;

	/// <summary>
	/// Установка стиля окна
	/// </summary>
	/// <param name="style">Стиль в виде WINAPI флагов</param>
	void SetStyle(DWORD style);

	/// <summary>
	/// Установка размером окна
	/// </summary>
	/// <param name="cx">Размер по горизонтали</param>
	/// <param name="cy">Размер по вертикали</param>
	void SetSize(int cx, int cy);

	/// <summary>
	/// Установки позиции окна (левого верхнего угла)
	/// </summary>
	/// <param name="x">Значение по горизонтали</param>
	/// <param name="y">Значение по вертикали</param>
	void SetPosition(int x, int y);

protected:
	// Обработчик события WM_PAINT
	virtual LRESULT OnPaint(HDC& hdc, PAINTSTRUCT& ps, WPARAM wp, LPARAM lp);
	
	// Обработчик события WM_COMMAND
	virtual LRESULT OnCommand(WPARAM wp, LPARAM lp);

	// Обработчик события WM_CLOSE
	virtual LRESULT OnClose(WPARAM wp, LPARAM lp);

	/// <summary>
	/// Получение имени класса (переопределяется макросом DECLARE_CLASS_NAME)
	/// </summary>
	/// <returns>Имя класса</returns>
	virtual std::wstring ClassName() const = 0;

	/// <summary>
	/// Стиль окна по умолчанию
	/// </summary>
	/// <returns>Стиль в виде флагов WinAPI</returns>
	virtual DWORD DefaultStyle() const = 0;

public:
	/// <summary>
	/// Уникальный идентификатор окна
	/// </summary>
	/// <returns>Идентификатор</returns>
	virtual WORD GetId() const = 0;
};

#endif // WINAPI_CONTROL_H