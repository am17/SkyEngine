#pragma once
#include <Windows.h>

class WindowsWindow
{
public:
	WindowsWindow();
	~WindowsWindow();
	bool Create(int Width, int Height);
	HWND GetHWND() const;
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

	HWND hWnd;
	HINSTANCE hInstance;
	LPCWSTR applicationName;
};