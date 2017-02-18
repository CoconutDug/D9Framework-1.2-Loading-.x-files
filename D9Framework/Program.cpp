#include "Types.h"
#include "GameStateManager.h"
#include "Program.h"
#include "Error.h"
#include "InputManager.h"

DefineSingleton(Program);

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return Program::GetInstance()->WndProc(hwnd, msg, wParam, lParam);
}

Program::Program()
{
}

Program::~Program()
{
	CleanUp();

	/*if (m_buffer)
	{
	delete [] m_buffer;
	m_buffer = NULL;
	}*/
}

int Program::GenerateProgram(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR ApplicationName[] = TEXT("DirectX");

	WNDCLASSEX wndclass;

	DWORD dwExStyle;
	DWORD dwStyle;
	RECT WindowRect;
	m_screenWidth = 800;
	m_screenHeight = 600;
	m_running = true;

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.lpfnWndProc = MainWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = ApplicationName;
	wndclass.hIconSm = NULL;

	if (!RegisterClassEx(&wndclass))
	{
		ErrorMessage("Failed RegisterClassEx");
		return 0;
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	hWindow = CreateWindowEx(dwExStyle, ApplicationName, ApplicationName, dwStyle,
		0, 0, m_screenWidth, m_screenHeight, NULL, NULL, hInstance, NULL);



	if (SUCCEEDED(CreateD3DDevice(hWindow)))
	{
		ShowWindow(hWindow, iCmdShow);
		UpdateWindow(hWindow);
	}
	else
	{
		ErrorMessage("Failed CreateD3DDevice");
		return 0;
	}

	InputManager::Create();
	InputManager::GetInstance()->Setup(hInstance,hWindow);
	return 1;
}

/*****************************************************************************************/

//int
//Program::GenerateProgram(HWND hWnd, HWND hWndParent, HINSTANCE hInstance)
//{
//	hWindow = hWnd;
//
//	m_screenWidth = 800;
//	m_screenHeight = 600;
//	m_running = true;
//
//	if(SUCCEEDED(CreateD3DDevice(hWindow)))
//	{
//		UpdateWindow(hWindow);
//	}
//	else
//	{
//		ErrorMessage("Failed CreateD3DDevice");
//		return 0;
//	}
//
//	InputManager::Create();
//	InputManager::GetInstance()->Setup(hInstance,hWndParent);
//	return 1;
//
//}

/*****************************************************************************************/

LRESULT
Program::WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SYSCOMMAND:						
	{
		switch (wParam)						
		{
		case SC_KEYMENU:
		case SC_SCREENSAVE:					
		case SC_MONITORPOWER:				
			return 0;						
		}
		break;								
	}
	case WM_PAINT:
		ValidateRect(hWindow, NULL);
		return 0;
		break;
	case WM_DESTROY:
		SetRunning(false);
		DestroyWindow(hWindow);
		//Cleanup();
		PostQuitMessage(0);
		return 0;
		break;
	}
	return DefWindowProc(hWindow, msg, wParam, lParam);
}

void Program::SetRunning(bool running)
{
	m_running = running;
}

bool Program::GetRunning() const
{
	return m_running;
}

//HWND Program::GetWindow() const
//{
//	return hWindow;
//}

void Program::RenderScene()
{
	GameStateManager::GetInstance()->Render();
}

void Program::Update(float elapsedTime)
{
	GameStateManager::GetInstance()->Update(elapsedTime);
}

//const char* Program::GetDirectory()
//{
//	if (!m_buffer)
//	{
//		int bufferSize=GetCurrentDirectory(0,NULL);
//		m_buffer = new char[bufferSize];
//		GetCurrentDirectory(bufferSize, m_buffer);
//	}
//
//	return m_buffer;
//}

//bool Program::GetCanAutoGenerateMips() const
//{
//	return m_autoGenMips;
//}
