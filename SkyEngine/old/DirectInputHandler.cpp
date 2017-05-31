#include "stdafx.h"
#include "DirectInputHandler.h"
#include "Log.h"

namespace sky
{
	DirectInputHandler::DirectInputHandler(HINSTANCE hInstance, HWND hwnd)
		:defaultCommand(nullptr),
		upArrowKeyPadCommand(nullptr),
		downArrowKeyPadCommand(nullptr),
		leftArrowKeyPadCommand(nullptr),
		rightArrowKeyPadCommand(nullptr),
		wKeyPadCommand(nullptr),
		sKeyPadCommand(nullptr),
		aKeyPadCommand(nullptr),
		dKeyPadCommand(nullptr)
	{
		init(hInstance, hwnd);
	}


	DirectInputHandler::~DirectInputHandler()
	{
		if (defaultCommand)
		{
			delete defaultCommand;
			defaultCommand = nullptr;
		}

		DIKeyboard->Unacquire();
		DIMouse->Unacquire();
		DirectInput->Release();
	}

	bool DirectInputHandler::init(HINSTANCE hInstance, HWND hwnd)
	{
		HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&DirectInput), nullptr);

		assert(SUCCEEDED(hr));

		hr = DirectInput->CreateDevice(GUID_SysKeyboard, &DIKeyboard, nullptr);

		assert(SUCCEEDED(hr));

		hr = DirectInput->CreateDevice(GUID_SysMouse, &DIMouse, nullptr);

		assert(SUCCEEDED(hr));

		hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);

		assert(SUCCEEDED(hr));

		hr = DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		assert(SUCCEEDED(hr));

		hr = DIMouse->SetDataFormat(&c_dfDIMouse);

		assert(SUCCEEDED(hr));

		hr = DIMouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

		assert(SUCCEEDED(hr));

		defaultCommand = new DefaultCommand();

		upArrowKeyPadCommand = defaultCommand;
		downArrowKeyPadCommand = defaultCommand;
		leftArrowKeyPadCommand = defaultCommand;
		rightArrowKeyPadCommand = defaultCommand;
		wKeyPadCommand = defaultCommand;
		sKeyPadCommand = defaultCommand;
		aKeyPadCommand = defaultCommand;
		dKeyPadCommand = defaultCommand;

		return true;
	}

	void DirectInputHandler::handleInput()
	{
		BYTE keyboardState[256];

		DIKeyboard->Acquire();

		DIKeyboard->GetDeviceState(sizeof(keyboardState), static_cast<LPVOID>(&keyboardState));

		//assert(SUCCEEDED(hr));

		if (keyboardState[DIK_ESCAPE] & 0x80)
		{
			//PostMessage(hwnd, WM_DESTROY, 0, 0);
		}

		if (keyboardState[DIK_LEFT] & 0x80)
		{
			leftArrowKeyPadCommand->execute();
		}
		if (keyboardState[DIK_RIGHT] & 0x80)
		{
			rightArrowKeyPadCommand->execute();
		}
		if (keyboardState[DIK_UP] & 0x80)
		{
			upArrowKeyPadCommand->execute();
		}
		if (keyboardState[DIK_DOWN] & 0x80)
		{
			downArrowKeyPadCommand->execute();
		}

		if (keyboardState[DIK_W] & 0x80)
		{
			wKeyPadCommand->execute();
		}

		if (keyboardState[DIK_S] & 0x80)
		{
			sKeyPadCommand->execute();
		}

		if (keyboardState[DIK_A] & 0x80)
		{
			aKeyPadCommand->execute();
		}

		if (keyboardState[DIK_D] & 0x80)
		{
			dKeyPadCommand->execute();
		}

		DIMOUSESTATE mouseCurrState;
		//mouse
		DIMouse->Acquire();

		DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);


		if (mouseCurrState.lX != mouseLastState.lX)
		{
			//Log::write("MOUSEX");
		}
		if (mouseCurrState.lY != mouseLastState.lY)
		{
			//Log::write("MOUSEY");
		}

		mouseLastState = mouseCurrState;
	}

	void DirectInputHandler::bindKeyCommand(EKEYBOARD_COMMAND keyboardCommand, ICommand *command)
	{
		assert(command != nullptr);

		switch (keyboardCommand)
		{
		case sky::EKEYBOARD_COMMAND::EKC_ARROWUP:
			upArrowKeyPadCommand = command;
			break;
		case sky::EKEYBOARD_COMMAND::EKC_ARROWDOWN:
			downArrowKeyPadCommand = command;
			break;
		case sky::EKEYBOARD_COMMAND::EKC_ARROWLEFT:
			leftArrowKeyPadCommand = command;
			break;
		case sky::EKEYBOARD_COMMAND::EKC_ARROWRIGHT:
			rightArrowKeyPadCommand = command;
			break;
		case sky::EKEYBOARD_COMMAND::EKC_W:
			wKeyPadCommand = command;
			break;
		case sky::EKEYBOARD_COMMAND::EKC_S:
			sKeyPadCommand = command;
			break;
		case sky::EKEYBOARD_COMMAND::EKC_A:
			aKeyPadCommand = command;
			break;
		case sky::EKEYBOARD_COMMAND::EKC_D:
			dKeyPadCommand = command;
			break;
		default:
			break;
		}
	}
}
