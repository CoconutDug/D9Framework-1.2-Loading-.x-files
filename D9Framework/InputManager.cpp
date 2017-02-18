#include "InputManager.h"
DefineSingleton(InputManager);

InputManager::InputManager() :
	m_directInput(0),
	m_keyboard(0)
{
	memset(m_keyStates, 0, sizeof(m_keyStates));
	memset(m_vkkeyStates, 0, sizeof(m_vkkeyStates));

}

InputManager::~InputManager()
{
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}
}

bool InputManager::Setup(HINSTANCE hInst, HWND hWindow)
{
	HRESULT hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);

	if (hr != S_OK)
	{
		return false;
	}

	if (CreateKeyboard(hWindow) == false)
	{
		return false;
	}
	for (int i = 0; i < 256; i++)
	{
		m_ASCII[i] = KeycodeToASCII(i);
	}

	if (CreateMouse(hWindow) == false)
	{
		return false;
	}

	return true;
}

bool InputManager::CreateKeyboard(HWND hWindow)
{
	HRESULT hr;

	//Setting of flags.
	bool bExclusive = false;
	bool bForeground = true;
	bool bDisableWindowsKey = true;

	DWORD dwCoopFlags;

	//DISCL_EXCLUSIVE allows only keyboard inputs for the program when active and in focus
	if (bExclusive)
	{
		dwCoopFlags = DISCL_EXCLUSIVE;
	}
	else
	{
		dwCoopFlags = DISCL_NONEXCLUSIVE;
	}

	//DISCL_FOREGROUND allows inputs to occor only when in focus
	if (bForeground)
	{
		dwCoopFlags |= DISCL_FOREGROUND;
	}
	else
	{
		dwCoopFlags |= DISCL_BACKGROUND;
	}
	//DISCL_NOWINKEY overrides the Windows key on your keyboard,
	if (bDisableWindowsKey && !bExclusive && bForeground)
	{
		dwCoopFlags |= DISCL_NOWINKEY;
	}
	//Create the keyboard device
	hr = m_directInput->CreateDevice(GUID_SysKeyboard, (LPDIRECTINPUTDEVICE8*)&m_keyboard, NULL);
	if (FAILED(hr))
	{
		return false;
	}
	//Set Data Format for keyboard
	hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		return false;
	}
	//set the Cooperative Level with the flags set above
	hr = m_keyboard->SetCooperativeLevel(hWindow, dwCoopFlags);
	if (FAILED(hr))
	{
		return false;
	}
	//aquire the new device
	m_keyboard->Acquire();
	return true;
}

/*****************************************************************************************/

bool InputManager::CreateMouse(HWND hWindow)
{
	HRESULT hr;
	bool bExclusive = false;
	bool bForeground = true;
	bool bDisableWindowsKey = true;

	DWORD dwCoopFlags;

	//DISCL_EXCLUSIVE allows only mouse inputs for the program when active and in focus
	if (bExclusive)
	{
		dwCoopFlags = DISCL_EXCLUSIVE;
	}
	else
	{
		dwCoopFlags = DISCL_NONEXCLUSIVE;
	}

	//DISCL_FOREGROUND allows inputs to occor only when in focus
	if (bForeground)
	{
		dwCoopFlags |= DISCL_FOREGROUND;
	}
	else
	{
		dwCoopFlags |= DISCL_BACKGROUND;
	}

	hr = m_directInput->CreateDevice(GUID_SysMouse, (LPDIRECTINPUTDEVICE8*)&m_mouse, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	//Set Data Format for keyboard
	hr = m_mouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr))
	{
		return false;
	}
	//set the Cooperative Level with the flags set above
	hr = m_keyboard->SetCooperativeLevel(hWindow, dwCoopFlags);
	if (FAILED(hr))
	{
		return false;
	}

	//aquire the new device
	m_mouse->Acquire();

	return true;
}

void InputManager::UpdateKeyboard()
{
	if (!m_keyboard)
	{
		return;
	}

	m_change = false;
	//Create a keys array
	HRESULT hr;
	char keys[256];

	//Set Memory size and get the state of the device
	memset(keys, 0, sizeof(keys));

	hr = m_keyboard->GetDeviceState(sizeof(keys), (LPVOID)&keys);
	if (hr != DI_OK)
	{
		hr = m_keyboard->Acquire();
		while (hr == DIERR_INPUTLOST)
		{
			hr = m_keyboard->Acquire();
		}
	}

	//If complete, Keys contains the current status of the Keys
	for (int i = 0; i < 256; i++)
	{
		//is the key is active
		if (keys[i] & 0x80)
		{
			//if in m_keyStates, the key is off, set to clicked
			if (m_keyStates[i] == ButtonState_Off)
			{
				m_keyStates[i] = ButtonState_Clicked;
				m_change = true;
			}
			else
			{
				//if the key is already clicked, set to on, as in held down
				m_keyStates[i] = ButtonState_On;
				m_change = true;
			}
		}
		else
		{
			//else the keys is off
			if (m_keyStates[i] != ButtonState_Off)
			{
				if (m_ASCII[i] != -1)
				{
					m_change = true;
				}
			}
			m_keyStates[i] = ButtonState_Off;
		}
	}
	for (int i = 0; i < 256; i++)
	{
		if (m_change == true)
		{
			if (m_ASCII[i] != -1)
			{
				m_vkkeyStates[m_ASCII[i]] = m_keyStates[i];
			}
		}
	}
}

void InputManager::UpdateMouse()
{
	HRESULT hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&m_mouseState);
	if (hr != S_OK)
	{
		// Mouse lost, zero out mouse data structure.
		ZeroMemory(&m_mouseState, sizeof(m_mouseState));

		// Try to acquire for next time we poll.
		hr = m_mouse->Acquire();
	}
}

unsigned int InputManager::KeycodeToASCII(int code) const
{
	static HKL layout = GetKeyboardLayout(0);
	static unsigned __int8 state[256];
	unsigned __int8  result = 0;

	GetKeyboardState(state);

	int VK = MapVirtualKeyEx(code, 1, layout);

	if (ToAsciiEx(VK, code, state, (LPWORD)&result, 0, layout) == 0)
	{
		return -1;
	}
	return result;
}

void InputManager::Update()
{
	UpdateKeyboard();
	UpdateMouse();
}

InputManager::ButtonState InputManager::GetKeyState(int key) const
{
	return (InputManager::ButtonState)m_keyStates[key];
}

bool InputManager::IsKeyDown(bool ascii, int key) const
{
	if (ascii)
	{
		if (m_vkkeyStates[key] == ButtonState_On)
		{
			return true;
		}
	}
	else
	{
		if (m_keyStates[key] == ButtonState_On)
		{
			return true;
		}
	}
	return false;
}

bool InputManager::IsKeyClicked(bool ascii, int key) const
{
	if (ascii)
	{
		if (m_vkkeyStates[key] == ButtonState_Clicked)
		{
			return true;
		}
	}
	else
	{
		if (m_keyStates[key] == ButtonState_Clicked)
		{
			return true;
		}
	}
	return false;
}

bool InputManager::IsAnyKeyClicked() const
{
	for (int i = 0; i < 256; i++)
	{
		if (m_keyStates[i] == ButtonState_Clicked)
		{
			return true;
		}
	}

	return false;
}

bool InputManager::IsMouseButtonDown(int button) const
{
	return (m_mouseState.rgbButtons[button] & 0x80) != 0;
}

float InputManager::GetMouseX() const
{
	return (float)m_mouseState.lX;
}

float InputManager::GetMouseY() const
{
	return (float)m_mouseState.lY;
}

float InputManager::GetMouseZ() const
{
	return (float)m_mouseState.lZ;
}
