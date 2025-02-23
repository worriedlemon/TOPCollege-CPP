#pragma once

#ifndef WINAPI_CONTROL_H
#define WINAPI_CONTROL_H

#include <windows.h>
#include <string>
#include <map>
#include <memory>
#include <stdexcept>
#include <set>

/// ������ ��� �������������� ����� ������ � ������
#define CONTROL_NAME(class) TEXT(#class)

/// ������ ��� ��������������� ������ ClassName
#define DECLARE_CLASS_NAME(class) \
std::wstring ClassName() const override \
{ return CONTROL_NAME(class); }

/// <summary>
/// ����� ����������� ����� ��� ����� ����������� ��������
/// </summary>
class Control
{
protected:
	HWND hwnd;
	HINSTANCE hInstance;
	std::wstring text;

	int x, y, cx, cy;

	/// <summary>
	/// ������������� ����������
	/// </summary>
	/// <param name="parent">������������ ������</param>
	void InitComponent(Control* parent);

private:
	// ������� � ���������� ���������
	static std::map<HWND, Control*> controls;

	// ������� � ������������������� ��������
	static std::map<std::wstring, std::shared_ptr<WNDCLASS>> classes;

public:
	/// <summary>
	/// ����������� ������ (�� ������������ ��������, ����� ������������ ������ RegisterControl)
	/// </summary>
	/// <param name="name">��� ������</param>
	/// <param name="hInstance">���������� ���������� �����</param>
	static void Register(std::wstring name, HINSTANCE hInstance);

	/// <summary>
	/// �������� ����������� ������ (������������ ��� �������)
	/// </summary>
	/// <param name="name">��� ������</param>
	/// <returns>true, ���� ����� ���������������, false - ���� ���</returns>
	static inline bool IsRegistered(std::wstring name)
	{
		std::set<std::wstring> defaultClasses = {
			L"BUTTON", L"EDIT", L"COMBOBOX", L"LISTBOX", L"ScrollBar"
		};

		return defaultClasses.find(name) != defaultClasses.end() ||
			classes.find(name) != classes.end();
	}

	// �������� ���������� ��������� WINAPI
	static LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	// �����������
	Control(HINSTANCE hInstance, Control* parent);

	// ����������
	virtual ~Control();

	/// <summary>
	/// ���������� ����� �������
	/// </summary>
	/// <param name="text">������������ �����</param>
	void SetText(std::wstring text);

	/// <summary>
	/// �������� ����� �������
	/// </summary>
	/// <returns>������������ �����</returns>
	std::wstring GetText() const;

	/// <summary>
	/// ��������� ����� ����
	/// </summary>
	/// <param name="style">����� � ���� WINAPI ������</param>
	void SetStyle(DWORD style);

	/// <summary>
	/// ��������� �������� ����
	/// </summary>
	/// <param name="cx">������ �� �����������</param>
	/// <param name="cy">������ �� ���������</param>
	void SetSize(int cx, int cy);

	/// <summary>
	/// ��������� ������� ���� (������ �������� ����)
	/// </summary>
	/// <param name="x">�������� �� �����������</param>
	/// <param name="y">�������� �� ���������</param>
	void SetPosition(int x, int y);

protected:
	// ���������� ������� WM_PAINT
	virtual LRESULT OnPaint(HDC& hdc, PAINTSTRUCT& ps, WPARAM wp, LPARAM lp);
	
	// ���������� ������� WM_COMMAND
	virtual LRESULT OnCommand(WPARAM wp, LPARAM lp);

	// ���������� ������� WM_CLOSE
	virtual LRESULT OnClose(WPARAM wp, LPARAM lp);

	/// <summary>
	/// ��������� ����� ������ (���������������� �������� DECLARE_CLASS_NAME)
	/// </summary>
	/// <returns>��� ������</returns>
	virtual std::wstring ClassName() const = 0;

	/// <summary>
	/// ����� ���� �� ���������
	/// </summary>
	/// <returns>����� � ���� ������ WinAPI</returns>
	virtual DWORD DefaultStyle() const = 0;

public:
	/// <summary>
	/// ���������� ������������� ����
	/// </summary>
	/// <returns>�������������</returns>
	virtual WORD GetId() const = 0;
};

#endif // WINAPI_CONTROL_H