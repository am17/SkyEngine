#include "stdafx.h"
#include "DirectInputHadler.h"
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
		f12KeyPadCommand(nullptr),
		_hwnd(hwnd)
	{
		init(hInstance);
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

	bool DirectInputHandler::init(HINSTANCE hInstance)
	{
		HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput, nullptr);

		hr = DirectInput->CreateDevice(GUID_SysKeyboard, &DIKeyboard, nullptr);

		hr = DirectInput->CreateDevice(GUID_SysMouse, &DIMouse, nullptr);

		hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
		hr = DIKeyboard->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		hr = DIMouse->SetDataFormat(&c_dfDIMouse);
		hr = DIMouse->SetCooperativeLevel(_hwnd, DISCL_NONEXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

		defaultCommand = new DefaultCommand();

		upArrowKeyPadCommand = defaultCommand;
		downArrowKeyPadCommand = defaultCommand;
		leftArrowKeyPadCommand = defaultCommand;
		rightArrowKeyPadCommand = defaultCommand;
		wKeyPadCommand = defaultCommand;
		sKeyPadCommand = defaultCommand;
		f12KeyPadCommand = defaultCommand;

		rightMouseCommand = defaultCommand;

		return true;
	}

	void DirectInputHandler::handleInput()
	{
		BYTE keyboardState[256];

		DIKeyboard->Acquire();

		HRESULT HR = DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

		if (keyboardState[DIK_ESCAPE] & 0x80)
		{
			PostMessage(_hwnd, WM_DESTROY, 0, 0);
		}

		if (keyboardState[DIK_LEFT] & 0x80)
		{
			leftArrowKeyPadCommand->execute(EKEYBOARD_COMMAND::EKC_ARROWLEFT);
		}
		if (keyboardState[DIK_RIGHT] & 0x80)
		{
			rightArrowKeyPadCommand->execute(EKEYBOARD_COMMAND::EKC_ARROWRIGHT);
		}
		if (keyboardState[DIK_UP] & 0x80)
		{
			upArrowKeyPadCommand->execute(EKEYBOARD_COMMAND::EKC_ARROWUP);
		}
		if (keyboardState[DIK_DOWN] & 0x80)
		{
			downArrowKeyPadCommand->execute(EKEYBOARD_COMMAND::EKC_ARROWDOWN);
		}

		if (keyboardState[DIK_W] & 0x80)
		{
			wKeyPadCommand->execute(EKEYBOARD_COMMAND::EKC_W);
		}

		if (keyboardState[DIK_S] & 0x80)
		{
			sKeyPadCommand->execute(EKEYBOARD_COMMAND::EKC_S);
		}

		if (keyboardState[DIK_F12] & 0x80)
		{
			f12KeyPadCommand->execute(EKEYBOARD_COMMAND::EKC_F12);
		}

		DIMOUSESTATE mouseCurrState;
		//mouse
		DIMouse->Acquire();

		DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

		if (mouseCurrState.rgbButtons[0] & 0x80)
		{
			//printf("left mouse\n");
		}

		if (mouseCurrState.rgbButtons[1] & 0x80)
		{
			rightMouseCommand->execute(EMOUSE_COMMAND::EMC_RIGHTBNT);
		}

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
		case sky::EKEYBOARD_COMMAND::EKC_F12:
			f12KeyPadCommand = command;
			break;
		default:
			break;
		}
	}

	void DirectInputHandler::bindMouseCommand(EMOUSE_COMMAND mouseCommand, ICommand *command)
	{
		assert(command != nullptr);

		switch (mouseCommand)
		{
		case sky::EMOUSE_COMMAND::EMC_LEFTBNT:
			break;
		case sky::EMOUSE_COMMAND::EMC_RIGHTBNT:
			rightMouseCommand = command;
			break;
		default:
			break;
		}
	}
}
