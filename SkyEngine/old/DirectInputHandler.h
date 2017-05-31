#pragma once
#include "IInputHandler.h"
#include <dinput.h>

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

namespace sky
{
	class DirectInputHandler :
		public IInputHandler
	{
	public:
		SKYENGINEDLL DirectInputHandler(HINSTANCE hInstance, HWND hwnd);
		SKYENGINEDLL ~DirectInputHandler();
		SKYENGINEDLL void handleInput();
		SKYENGINEDLL void bindKeyCommand(EKEYBOARD_COMMAND keyboardCommand, ICommand *command);
	private:
		bool init(HINSTANCE hInstance, HWND hwnd);
		IDirectInputDevice8* DIKeyboard;
		IDirectInputDevice8* DIMouse;

		DIMOUSESTATE mouseLastState;
		LPDIRECTINPUT8 DirectInput;
		ICommand *defaultCommand;

		ICommand *upArrowKeyPadCommand;
		ICommand *downArrowKeyPadCommand;
		ICommand *leftArrowKeyPadCommand;
		ICommand *rightArrowKeyPadCommand;
		ICommand *wKeyPadCommand;
		ICommand *sKeyPadCommand;
		ICommand *aKeyPadCommand;
		ICommand *dKeyPadCommand;
	};

	class DefaultCommand : public ICommand
	{
	public:
		void execute() override {}
	};
}

