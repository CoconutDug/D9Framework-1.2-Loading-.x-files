#pragma once
#include "DirectXDevice.h"

class Program : public Singleton<Program>,
	public DirectXDevice
{
public:
	Program();
	~Program();
	int	GenerateProgram(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow);
	/*int GenerateProgram(HWND hWnd, HWND hWndParent, HINSTANCE hInstance);*/
	bool GetRunning() const;
	void SetRunning(bool running);
	//HWND GetWindow() const;
	virtual void RenderScene();
	virtual LRESULT	WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
	void Update(float elapsedTime);
	//const char* GetDirectory();
	//bool	GetCanAutoGenerateMips() const;
private:
	bool m_running;
	HWND hWindow;
	int m_ClearR;
	int m_ClearG;
	int m_ClearB;
	//char* m_buffer;
};
