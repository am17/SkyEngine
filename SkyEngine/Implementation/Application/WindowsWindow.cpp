#include "Application\WindowsWindow.h"

WindowsWindow::WindowsWindow()
	:hInstance(nullptr),
	hWnd(nullptr)
{
}

WindowsWindow::~WindowsWindow()
{
}

bool WindowsWindow::Create(int Width, int Height)
{
	hInstance = GetModuleHandle(NULL);

	applicationName = L"Engine";

	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	int posX = (GetSystemMetrics(SM_CXSCREEN) - Width) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - Height) / 2;

	// Create the window with the screen settings and get the handle to it.
	hWnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, posX, posY, Width, Height, NULL, NULL, hInstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	// Hide the mouse cursor.
	ShowCursor(true);

	return true;
}

HWND WindowsWindow::GetHWND() const
{
	return hWnd;
}

LRESULT WindowsWindow::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	//PAINTSTRUCT ps;
	//HDC hdc;

	switch (umessage)
	{
		case WM_PAINT:
			//hdc = BeginPaint(hWnd, &ps);
			//EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_ACTIVATEAPP:
			//Keyboard::ProcessMessage(message, wParam, lParam);
			//Mouse::ProcessMessage(message, wParam, lParam);
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			//Keyboard::ProcessMessage(message, wParam, lParam);
			break;
		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			//Mouse::ProcessMessage(message, wParam, lParam);
			break;
		default:
			break;
	}

	return DefWindowProc(hwnd, umessage, wparam, lparam);
}
