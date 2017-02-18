#pragma once
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

#define ANTI_ALIAS_ENABLED true

class DirectXDevice
{
public:
	HRESULT	CreateD3DDevice(HWND Window);
	void CleanUp();
	void Render();
	virtual void RenderScene() {};
	int	GetScreenWidth() const;
	int	GetScreenHeight() const;
	//bool Allow2SidedShadows();
	void ClearBuffer();
	void SetClearBufferColour(int r, int g, int b);
	/*void GetClearBufferColour(int& r, int& g, int& b);*/
protected:
	LPDIRECT3D9 m_D3D;
	int m_screenWidth;
	int m_screenHeight;
	/*bool m_autoGenMips;
	bool m_allow2Sided;*/
	int m_ClearR;
	int m_ClearG;
	int m_ClearB;
};

