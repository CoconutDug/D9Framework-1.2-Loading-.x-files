#include "Types.h"
#include "DirectXDevice.h"
#include "Error.h"

LPDIRECT3DDEVICE9 m_D3DDevice = NULL;
D3DPRESENT_PARAMETERS d3dpp;

HRESULT DirectXDevice::CreateD3DDevice(HWND Window)
{
	//The first step is to set generate the LPDIRECT3D9 structure with information
	//regarding the version we wish to use, this is stored in the variable D3D_SDK_VERSION
	if (NULL == (m_D3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		ErrorMessage("Failed, Step 1 Direct3DCreate9");
		return E_FAIL;
	}

	//Create a variable to control the display mode of the DirectX App.
	D3DDISPLAYMODE d3ddm;

	//Next we retreve the adapter display from the system
	if (FAILED(m_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		ErrorMessage("Failed, Step 2 GetAdapterDisplayMode");
		return E_FAIL;
	}

	//Then test the device to see if it compatible with the parameters we want the display to be
	//the parameters are, the adapter, the Device type (Hardware), the Display Format (32bit) and the back buffer format
	if (FAILED(m_D3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, FALSE)))
	{
		ErrorMessage("Failed, Step 3 CheckDeviceType");
		return E_FAIL;
	}

	//we check that the adapter has a particular surface type available for it to use, 
	//here we check for a depth-stencil format
	if (FAILED(m_D3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16)))
	{
		ErrorMessage("Failed, Step 4 CheckDeviceFormat");
		return E_FAIL;
	}

	//next we create a Caps variable that will store the capabilities of the hardware exposed through D3D
	D3DCAPS9 d3dcaps;
	//then we populate that variable using the GetDeviceCaps command
	if (FAILED(m_D3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps)))
	{
		ErrorMessage("Failed, Step 5 GetDeviceCaps");
		return E_FAIL;
	}

	if (FAILED(m_D3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8,
		D3DFMT_X8R8G8B8, D3DFMT_D24X8)))
		return false;

	if (FAILED(m_D3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8,
		D3DFMT_A8R8G8B8, D3DFMT_D24S8)))
		return false;

	if (d3dcaps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP)
	{
		HRESULT hr = D3D_OK;

		hr = m_D3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_AUTOGENMIPMAP,
			D3DRTYPE_TEXTURE, D3DFMT_X8R8G8B8);
		/*if(hr == D3D_OK)
		m_autoGenMips = true;
		else
		m_autoGenMips = false;*/
	}

	DWORD flags = 0;
	//we now test is the adapter can support hardware or software vertex processing
	if (d3dcaps.VertexProcessingCaps != 0)
	{
		flags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		flags |= D3DCREATE_PUREDEVICE;
	}
	else
	{
		flags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	//We when set our parameters for the display, i.e. windowed
	memset(&d3dpp, 0, sizeof(d3dpp));

	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//Finally we create the Device, attaching it to the focus window, with the behavior we require.
	//the result is stored within the m_D3DDevice variable

	//switcher for anti alias enable
	if (ANTI_ALIAS_ENABLED)
	{
		//SetUp for Anti Alias using a D3DMULTISAMPLE_NONMASKABLE parameter,
		//the quality is determined by if the device can support anti aliasing
		DWORD total;
		if (SUCCEEDED(m_D3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3ddm.Format, true, D3DMULTISAMPLE_NONMASKABLE, &total)))
		{
			d3dpp.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
			d3dpp.MultiSampleQuality = total - 1;
		}
	}

	if (FAILED(m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, flags, &d3dpp, &m_D3DDevice)))
	{
		ErrorMessage("Failed, Step 6 CreateDevice");
		return E_FAIL;
	}

	if (FAILED(m_D3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8)))
	{
		ErrorMessage("Depth Stencil not allowed");
		return E_FAIL;
	}

	// Turn on the z buffer
	m_D3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	//Check for Shader Support
	D3DCAPS9 shaCaps;
	m_D3DDevice->GetDeviceCaps(&shaCaps);

	if( shaCaps.StencilCaps & D3DSTENCILCAPS_TWOSIDED )
	{
	//m_allow2Sided = true;
	}
	else
	{
	//m_allow2Sided = false;
	}

	if(shaCaps.VertexShaderVersion < D3DVS_VERSION(2, 0))
	{
	ErrorMessage("You do not have shader 2.0 hardware, check your graphic settings");
	}

	if(shaCaps.PixelShaderVersion < D3DPS_VERSION(2, 0))
	{
	ErrorMessage("You do not have shader 2.0 hardware, check your graphic settings");
	}

	return S_OK;
}

void DirectXDevice::CleanUp()
{
	if (m_D3DDevice != NULL)
	{
		m_D3DDevice->Release();
	}

	if (m_D3D != NULL)
	{
		m_D3D->Release();
	}
}

void DirectXDevice::Render()
{
	if (m_D3DDevice == NULL)
	{
		return;
	}

	//firstly we clear the screen
	m_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(m_ClearR, m_ClearG, m_ClearB), 1.0f, 0);

	//We then begin the scene (BeginScene must be called before any rendering can take place)
	if (SUCCEEDED(m_D3DDevice->BeginScene()))
	{
		//we call RenderScene, which will only render the objects from the game itself (keeping this core element separate)
		RenderScene();
		//we end the scene when rendering is complete. (like BeginScene, EndScene must also be called)
		m_D3DDevice->EndScene();
	}

	//we then present the scene.
	m_D3DDevice->Present(NULL, NULL, NULL, NULL);
}

int DirectXDevice::GetScreenWidth() const
{
	return m_screenWidth;
}

int DirectXDevice::GetScreenHeight() const
{
	return m_screenHeight;
}

//bool DirectXDevice::Allow2SidedShadows()
//{
//	return m_allow2Sided;
//}


void DirectXDevice::ClearBuffer()
{
	m_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(m_ClearR, m_ClearG, m_ClearB), 1.0f, 0);
}

void DirectXDevice::SetClearBufferColour(int r, int g, int b)
{
	m_ClearR = r;
	m_ClearG = g;
	m_ClearB = b;
}

//void DirectXDevice::GetClearBufferColour(int& r, int& g, int& b)
//{
//	r = m_ClearR;
//	g = m_ClearG;
//	b = m_ClearB;
//}

