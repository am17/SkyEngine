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
		SKYENGINEDLL void bindMouseCommand(EMOUSE_COMMAND mouseCommand, ICommand *command);
		SKYENGINEDLL DIMOUSESTATE& getMouseState() { return mouseLastState; }
	private:
		bool init(HINSTANCE hInstance);
		IDirectInputDevice8* DIKeyboard;
		IDirectInputDevice8* DIMouse;
		HWND _hwnd;

		DIMOUSESTATE mouseLastState;
		LPDIRECTINPUT8 DirectInput;
		ICommand *defaultCommand;

		ICommand *upArrowKeyPadCommand;
		ICommand *downArrowKeyPadCommand;
		ICommand *leftArrowKeyPadCommand;
		ICommand *rightArrowKeyPadCommand;
		ICommand *wKeyPadCommand;
		ICommand *sKeyPadCommand;
		ICommand *f12KeyPadCommand;

		ICommand *rightMouseCommand;
	};

	class DefaultCommand : public ICommand
	{
	public:
		void execute(EKEYBOARD_COMMAND command) override {}
		void execute(EMOUSE_COMMAND command) override {}
	};
}

