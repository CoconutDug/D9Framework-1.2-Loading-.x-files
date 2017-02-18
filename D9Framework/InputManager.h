#pragma once
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include "Singleton.h"
#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class InputManager : public Singleton<InputManager>
{
public:
	enum ButtonState
	{
		ButtonState_Off = 0,
		ButtonState_Clicked = 1,
		ButtonState_On = 2,
	};

	enum MouseButtons
	{
		LeftMouse = 0,
		RightMouse = 1,
	};
	InputManager();
	~InputManager();
	bool Setup(HINSTANCE hInst, HWND hWindow);
	unsigned int KeycodeToASCII(int code) const;
	void Update();
	ButtonState	GetKeyState(int key) const;
	bool IsKeyDown(bool ascii, int key)const;
	bool IsKeyClicked(bool ascii, int key)const;
	bool IsAnyKeyClicked()const;
	bool IsMouseButtonDown(int button) const;
	float GetMouseX() const;
	float GetMouseY() const;
	float GetMouseZ() const;
private:
	bool CreateKeyboard(HWND hWindow);
	bool CreateMouse(HWND hWindow);
	void UpdateKeyboard();
	void UpdateMouse();
	__int8 m_keyStates[256];
	__int8 m_vkkeyStates[256];
	LPDIRECTINPUT8 m_directInput;
	LPDIRECTINPUTDEVICE8 m_keyboard;
	LPDIRECTINPUTDEVICE8 m_mouse;
	DIMOUSESTATE2        m_mouseState;
	bool m_change;
	unsigned int m_ASCII[256];

};