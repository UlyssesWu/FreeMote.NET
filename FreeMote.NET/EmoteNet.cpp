// 这是主 DLL 文件。

#include "Stdafx.h"
#include "iemote.h"
#include "EmoteNet.h"
#include <vector>
#include <map>
#include <d3dx9.h>
#include <vcclr.h>
#include <Windows.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"user32.lib")

#pragma comment(lib,"emotedriver.lib")

using namespace System::IO;

#define SAFE_RELEASE(p) { if(p){ (p)->Release(); (p)=NULL; } }
#define FVF_LVERTEX     (D3DFVF_XYZ    | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct LVERTEX {
	float    x, y, z;
	D3DCOLOR color;
	float    tu, tv;
};

namespace FreeMote {

	//------------------------------------------------
	// 定数定義
	//------------------------------------------------

	//------------------------------------------------


	//------------------------------------------------
	// 画面サイズ
	//static const int SCREEN_WIDTH = 1280;
	//static const int SCREEN_HEIGHT = 720;

	//int sScreenWidth = SCREEN_WIDTH;
	//int sScreenHeight = SCREEN_HEIGHT;
		
	//------------------------------------------------
	// Window関連
	HWND sHwnd;
	int sPrevMouseX, sPrevMouseY;
	int sLeftMouseDragging;
	int sRightMouseDragging;

	//------------------------------------------------
	// D3D関連
	LPDIRECT3D9 sD3D;
	D3DCAPS9 sD3DCaps;
	D3DPRESENT_PARAMETERS sD3Dpp;
	//LPDIRECT3DDEVICE9 sD3DDevice;
	//LPDIRECT3DTEXTURE9 sCanvasTexture;
	//int sCanvasTextureWidth, sCanvasTextureHeight;
	LPDIRECT3DSURFACE9 sBackBufferSurface;

	//ADDED:TODO:因为E-moteDX不支持Ex，只能考虑共享Surface
	typedef HRESULT(WINAPI *DIRECT3DCREATE9EX)(UINT SDKVersion, IDirect3D9Ex**);

	DIRECT3DCREATE9EX       g_pfnCreate9Ex = NULL;
	//BOOL                    g_is9Ex = FALSE;  //useD3DEx
	LPDIRECT3D9EX           sD3DEx = NULL;
	//LPDIRECT3DDEVICE9EX     sD3DDeviceEx = NULL;

	//Experimental
	LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}


	WNDCLASSEX g_wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"Foo", NULL };

	//ADDED:
	//LPDIRECT3DSURFACE9   g_pd3dSurface = NULL;
	LPDIRECT3DSURFACE9   g_psysSurface = NULL;

	//------------------------------------------------
	// Emote関連
	//IEmoteDevice *sEmoteDevice;
	//IEmotePlayer *sEmotePlayer;
	//std::vector<IEmotePlayer *> sClonePlayerList; //MARK: Removed


	//IEmotePlayer *ssEmotePlayer;
	//------------------------------------------------
	// 関数宣言
	//------------------------------------------------
	/*------------------------------
	* 2の累乗への切り上げ 取得大于该数的最小的2的幂
	------------------------------*/
	static DWORD clp(DWORD x) {
		x = x - 1;
		x = x | (x >> 1);
		x = x | (x >> 2);
		x = x | (x >> 4);
		x = x | (x >> 8);
		x = x | (x >> 16);
		return x + 1;
	}

	//struct VariableTable
	//{
	//	const char *key;
	//	float value;
	//};

	////------------------------------------------------
	//// 変数設定のセット
	////------------------------------------------------
	//static const char *sPoseTimelineList[] = {
	//	"test00",
	//	"test01",
	//	"test02",
	//	"test03"
	//};

	//static const char *sMouthTimelineList[] = {
	//	"mouth_test00",
	//	"mouth_test01"
	//};

	//static const VariableTable
	//	sFaceTable0[] =
	//{
	//	{ "face_eye_UD", 0.0 },
	//	{ "face_eye_LR", 0.0 },
	//	{ "face_eye_open", 0.0 },
	//	{ "face_eyebrow", 0.0 },
	//	{ "face_mouth", 0.0 },
	//	{ "face_talk", 0.0 },
	//	{ "face_tears", 0.0 },
	//	{ "face_cheek", 0.0 },
	//	{}
	//};

	//static const VariableTable
	//	sFaceTable1[] =
	//{
	//	{ "face_eye_UD", 0.0 },
	//	{ "face_eye_LR", -30 },
	//	{ "face_eye_open", 0.0 },
	//	{ "face_eyebrow", 40 },
	//	{ "face_mouth", 30 },
	//	{ "face_talk", 0.0 },
	//	{ "face_tears", 0.0 },
	//	{ "face_cheek", 1 },
	//	{},
	//};

	//static const VariableTable
	//	sFaceTable2[] =
	//{
	//	{ "face_eye_UD", 0.0 },
	//	{ "face_eye_LR", -30.0 },
	//	{ "face_eye_open", 10 },
	//	{ "face_eyebrow", 30 },
	//	{ "face_mouth", 20 },
	//	{ "face_talk", 0.0 },
	//	{ "face_tears", 1 },
	//	{ "face_cheek", 0.0 },
	//	{},
	//};

	//static const VariableTable
	//	sFaceTable3[] =
	//{
	//	{ "face_eye_UD", -30 },
	//	{ "face_eye_LR", 0.0 },
	//	{ "face_eye_open", 5 },
	//	{ "face_eyebrow", 20 },
	//	{ "face_mouth", 20 },
	//	{ "face_talk", 0.5 },
	//	{ "face_tears", 0.0 },
	//	{ "face_cheek", 0.0 },
	//	{},
	//};

	void GrayscaleFilter(BYTE *image, ULONG imageSize)
	{
		while (imageSize) {
			BYTE gray = int(0.298912f * image[2] + 0.586611f * image[1] + 0.114478f * image[0]);
			image[0] = image[1] = image[2] = gray;
			image += 4;
			imageSize -= 4;
		}
	}

	//-------------------------------------------------
	void Emote::D3DInitRenderState()
	{
		if (sD3DDevice == NULL)
			return;

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);

		D3DXMATRIX matProj;
		D3DXMatrixIdentity(&matProj);

		float aspect = 1.0f * sScreenWidth / sScreenHeight;
		matProj._11 = 1.0f / aspect;
		matProj._34 = 1.0f;
		matProj._44 = 0.0f;
		matProj._41 = 0.0f;
		matProj._42 = 0.0f;

		float scale = 1.0f;
		float fw = (float)sScreenWidth;
		float fh = (float)sScreenHeight;

		D3DXMATRIX matView;
		D3DXMatrixLookAtLH(&matView,
			&D3DXVECTOR3(-0.5f, 0.5f, -fh / 2),
			&D3DXVECTOR3(-0.5f, 0.5f, 0),
			&D3DXVECTOR3(0, -1, 0));

		matView._11 = -matView._11;

		// E-moteドライバは、以下3つの行列値に応じて射影を行う。
		sD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
		sD3DDevice->SetTransform(D3DTS_VIEW, &matView);
		sD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

		// レンダリングターゲット描画用に最低限のパラメータ初期化
		sD3DDevice->SetFVF(FVF_LVERTEX);
		sD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		sD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		sD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		sD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		sD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		sD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		sD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		sD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		sD3DDevice->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
		sD3DDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
		sD3DDevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
		sD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		sD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		sD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
		sD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		sD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		sD3DDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
		sD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		sD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		sD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		sD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		sD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		sD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		sD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		sD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	}

	//void D3DInitRenderState(int sScreenWidth, int sScreenHeight, LPDIRECT3DDEVICE9 sD3DDevice)
	void Emote::D3DInitRenderState(int screenWidth, int screenHeight, IntPtr handle)
	{
		LPDIRECT3DDEVICE9 device;
		sScreenWidth = screenWidth;
		sScreenHeight = screenHeight;
		if (handle != IntPtr::Zero && handle.ToInt64() > 0)
		{
			device = (LPDIRECT3DDEVICE9)(handle.ToPointer());
		}
		else
		{
			device = sD3DDevice;
		}

		if (device == NULL)
			return;

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);

		D3DXMATRIX matProj;
		D3DXMatrixIdentity(&matProj);

		float aspect = 1.0f * screenWidth / screenHeight;
		matProj._11 = 1.0f / aspect;
		matProj._34 = 1.0f;
		matProj._44 = 0.0f;
		matProj._41 = 0.0f;
		matProj._42 = 0.0f;

		float scale = 1.0f;
		float fw = (float)screenWidth;
		float fh = (float)screenHeight;

		D3DXMATRIX matView;
		D3DXMatrixLookAtLH(&matView,
			&D3DXVECTOR3(-0.5f, 0.5f, -fh / 2),
			&D3DXVECTOR3(-0.5f, 0.5f, 0),
			&D3DXVECTOR3(0, -1, 0));

		matView._11 = -matView._11;

		// E-moteドライバは、以下3つの行列値に応じて射影を行う。
		device->SetTransform(D3DTS_WORLD, &matWorld);
		device->SetTransform(D3DTS_VIEW, &matView);
		device->SetTransform(D3DTS_PROJECTION, &matProj);

		// レンダリングターゲット描画用に最低限のパラメータ初期化
		device->SetFVF(FVF_LVERTEX);
		device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
		device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
		device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
		device->SetRenderState(D3DRS_LIGHTING, FALSE);
		device->SetRenderState(D3DRS_ZENABLE, FALSE);
		device->SetRenderState(D3DRS_ALPHAREF, 0x00);
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	}

	//ADDED:D3DEx
	DWORD GetVertexProcessingCaps()
	{
		D3DCAPS9 caps;
		DWORD dwVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		if (SUCCEEDED(sD3DEx->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
		{
			if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == D3DDEVCAPS_HWTRANSFORMANDLIGHT)
			{
				dwVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			}
		}
		return dwVertexProcessing;
	}

	void InitializeD3DEx(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp, LPDIRECT3DDEVICE9EX* sD3DDeviceEx, LPDIRECT3DDEVICE9* sD3DDevice, bool rewrite = true)
	{
		//ADDED
		d3dpp.BackBufferCount = 2;
		d3dpp.SwapEffect = D3DSWAPEFFECT_FLIPEX;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		//ADDED END

		// initialize Direct3D using the Ex function
		if (FAILED((*g_pfnCreate9Ex)(D3D_SDK_VERSION, &sD3DEx)))
		{
			return;
		}

		if (rewrite)
		{
			// obtain the standard D3D interface
			if (FAILED(sD3DEx->QueryInterface(__uuidof(IDirect3D9), reinterpret_cast<void **>(&sD3D))))
			{
				return;
			}
		}

		// determine what type of vertex processing to use based on the device capabilities
		DWORD dwVertexProcessing = GetVertexProcessingCaps();

		// create the D3D device using the Ex function
		if (FAILED(sD3DEx->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			dwVertexProcessing | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE,
			&d3dpp, NULL, sD3DDeviceEx)))
		{
			return;
		}

		if (rewrite)
		{
			// obtain the standard D3D device interface
			if (FAILED((*sD3DDeviceEx)->QueryInterface(__uuidof(IDirect3DDevice9), reinterpret_cast<void **>(sD3DDevice))))
			{
				return;
			}
		}
	}

	void InitializeD3D(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp, LPDIRECT3DDEVICE9* sD3DDevice)
	{
		// initialize Direct3D
		if (NULL == (sD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		{
			return;
		}

		// determine what type of vertex processing to use based on the device capabilities
		//DWORD dwVertexProcessing = GetVertexProcessingCaps();
		DWORD dwVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		// create the D3D device
		if (FAILED(sD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			dwVertexProcessing | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE,
			&d3dpp, sD3DDevice)))
		{
			return;
		}
	}

	//初始化D3D
	void Emote::D3DInit(IntPtr formHandle, int width, int height, bool useSurface)
	{
		if (formHandle != IntPtr::Zero)
		{
			sHwnd = (HWND)formHandle.ToInt32();
		}

		sScreenHeight = height;
		sScreenWidth = width;
		useD3DSurface = useSurface;

		if (width < 0 || height < 0)
		{
			RECT rect;
			GetClientRect(sHwnd, &rect);
			sScreenWidth = abs(rect.right - rect.left);
			sScreenHeight = abs(rect.bottom - rect.top);
		}

		HMODULE hD3D9 = LoadLibrary(TEXT("d3d9.dll"));
		g_pfnCreate9Ex = (DIRECT3DCREATE9EX)GetProcAddress(hD3D9, "Direct3DCreate9Ex");
		useD3DEx = (g_pfnCreate9Ex != NULL);
		FreeLibrary(hD3D9);

		useD3DEx = false; //Unsupport!

		//ADDED
		//sD3Dpp.BackBufferCount = 2;
		//sD3Dpp.SwapEffect = D3DSWAPEFFECT_FLIPEX;
		sD3Dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		//ADDED END

		// D3D 
		//sD3D = Direct3DCreate9(D3D_SDK_VERSION);
		//
		//HRESULT hr;
		//hr = sD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &sD3DCaps);

		memset(&sD3Dpp, 0, sizeof(sD3Dpp));
		sD3Dpp.BackBufferWidth = sScreenWidth;
		sD3Dpp.BackBufferHeight = sScreenHeight;
		sD3Dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		sD3Dpp.Windowed = true;
		sD3Dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

		sD3Dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		if (useD3DEx)
		{
			// create the device's window
			//RegisterClassEx(&g_wc);
			//HWND hWnd = CreateWindow(L"Foo", L"Foo", WS_OVERLAPPEDWINDOW,
			//	0, 0, 0, 0, NULL, NULL, g_wc.hInstance, NULL);
			pin_ptr<LPDIRECT3DDEVICE9> ptr = &sD3DDevice;
			pin_ptr<LPDIRECT3DDEVICE9EX> ptrEx = &sD3DDeviceEx;

			InitializeD3DEx(sHwnd, sD3Dpp, ptrEx, ptr, true);

			//InitializeD3D(hWnd, sD3Dpp);
		}
		else
		{
			//InitializeD3DTest(sHwnd, sD3Dpp);
			pin_ptr<LPDIRECT3DDEVICE9> ptr = &sD3DDevice; //pin_ptr不需要delete?
			InitializeD3D(sHwnd, sD3Dpp, ptr);
		}

		//hr = sD3D->CreateDevice(D3DADAPTER_DEFAULT,
		//	D3DDEVTYPE_HAL,
		//	sHwnd,
		//	D3DCREATE_FPU_PRESERVE | D3DCREATE_MULTITHREADED | D3DCREATE_HARDWARE_VERTEXPROCESSING,
		//	&sD3Dpp,
		//	&sD3DDevice);

		//ADDED
		D3Device = (int)sD3DDevice;
		if (useD3DEx)
		{
			D3Device = (int)sD3DDeviceEx;
		}

		if (useD3DSurface)
		{
			pin_ptr<LPDIRECT3DSURFACE9> nativePtr = &sSurface;
			if (useD3DEx)
			{
				if (FAILED(sD3DDeviceEx->CreateRenderTarget(sScreenWidth, sScreenHeight,
					D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0,
					!useD3DEx, // lockable
					&sBackBufferSurface, NULL)))
				{
					return;
				}
				//surface = (int)sBackBufferSurface;

				//sD3DDeviceEx->SetRenderTarget(0, g_pd3dSurface);
				//用DX9绘图到Surface
				if (FAILED(sD3DDevice->CreateOffscreenPlainSurface(sScreenWidth, sScreenHeight, D3DFMT_UNKNOWN, D3DPOOL_SYSTEMMEM, nativePtr, NULL)))
				{
					return;
				}
				//if (FAILED(sD3DDevice->CreateRenderTarget(sScreenWidth, sScreenHeight,
				//	D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0,
				//	!useD3DEx, // lockable
				//	nativePtr, NULL)))
				//{
				//	return;
				//}
				if (useD3DSurface)
				{
					sD3DDevice->SetRenderTarget(0, sSurface);
				}
			}
			else
			{
				if (FAILED(sD3DDevice->CreateRenderTarget(sScreenWidth, sScreenHeight,
					D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0,
					!useD3DEx, // lockable
					nativePtr, NULL)))
				{
					return;
				}
				//surface = (int)sSurface;
				sD3DDevice->SetRenderTarget(0, sSurface);
				sD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				// turn off D3D lighting to use vertex colors
				sD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
				// Enable alpha blending.
				sD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			}

		}

		D3DInitRenderState();
	}

	EmoteDevice^ Emote::EmoteInit()
	{
		// E-moteデバイス作成
		IEmoteDevice::InitParam param;
		memset(&param, 0, sizeof(param));
		param.d3dDevice = sD3DDevice;

		// メモリ確保・解放関数の指定はNULLのままならデフォルトのmalloc/freeが使われる
		// param.objAllocator.alloc = &ObjAlloc;
		// param.objAllocator.free =  &ObjFree;

		sEmoteDevice = EmoteCreate(param);

		device = gcnew EmoteDevice(sEmoteDevice);
		PlayerCollection = gcnew Dictionary<String^, FreeMote::EmotePlayer^>();

		return device;
	}

	EmoteDevice^ Emote::EmoteInit(IntPtr dxHandle)
	{
		// E-moteデバイス作成
		IEmoteDevice::InitParam param;
		memset(&param, 0, sizeof(param));

		sD3DDevice = (LPDIRECT3DDEVICE9)(dxHandle.ToPointer());
		param.d3dDevice = sD3DDevice;

		// メモリ確保・解放関数の指定はNULLのままならデフォルトのmalloc/freeが使われる
		// param.objAllocator.alloc = &ObjAlloc;
		// param.objAllocator.free =  &ObjFree;

		sEmoteDevice = EmoteCreate(param);

		device = gcnew EmoteDevice(sEmoteDevice);
		PlayerCollection = gcnew Dictionary<String^, FreeMote::EmotePlayer^>();

		return device;
	}

	//Emote初始化
//		void Emote::EmoteInit(String^ path)
//		{
//			if (!System::IO::File::Exists(path))
//			{
//				throw gcnew FileNotFoundException("File not exist.");
//			}
//
//			// E-moteデバイス作成
//			IEmoteDevice::InitParam param;
//			memset(&param, 0, sizeof(param));
//			param.d3dDevice = sD3DDevice;
//			
//			// メモリ確保・解放関数の指定はNULLのままならデフォルトのmalloc/freeが使われる
//			// param.objAllocator.alloc = &ObjAlloc;
//			// param.objAllocator.free =  &ObjFree;
//
//			sEmoteDevice = EmoteCreate(param);
//			//MARK
//			//sEmoteDevice->SetMaskRegionClipping(true);
//			sEmoteDevice->SetShaderModel(IEmoteDevice::SHADER_MODEL_2);
//
//			pin_ptr<const wchar_t> nativePath = PtrToStringChars(path);
//			// E-moteデータファイル読み込み
//			//读取Emote PSB文件
//			HANDLE handle = CreateFile(nativePath,
//				GENERIC_READ,
//				0,
//				NULL,
//				OPEN_EXISTING,
//				FILE_ATTRIBUTE_NORMAL,
//				NULL);
//
//			unsigned int size = GetFileSize(handle, NULL);
//			BYTE *buf = new BYTE[size];
//			DWORD dummy;
//			ReadFile(handle, buf, size, &dummy, NULL);
//			CloseHandle(handle);
//
//#if 0
//			// E-moteファイルイメージに事前にフィルタ処理を行う
//			EmoteFilterTexture(buf, size, GrayscaleFilter);
//#endif
//
//			// E-moteプレイヤ作成
//			//ADDED:
//			pin_ptr<IEmotePlayer*> nativePtr = &sEmotePlayer;
//			sEmoteDevice->CreatePlayer(buf, size, nativePtr);
//			sPlayer = gcnew AZUSA::EmoteNet::EmotePlayer(sEmotePlayer);
//
//			// 処理の終わったファイルイメージを破棄
//			delete[] buf;
//
//			// 表示開始
//			sEmotePlayer->Show();
//		}

		// Emote更新
	void Emote::Update(float frameCount)
	{
		// E-moteは1/60秒を1単位で駆動するので時間単位を変換。

		for each (auto player in PlayerCollection->Values)
		{
			if (player != nullptr)
			{
				player->Progress(frameCount * MSTOF60THS);
			}
		}
	}

	// 渲染
	void Emote::Draw(void)
	{
		Lock lock(PlayerCollection);
		for each (auto player in PlayerCollection->Values)
		{
			if (player != nullptr)
			{
				sEmoteDevice->OnRenderTarget(sCanvasTexture);
				player->Render();
			}
		}
		//sEmoteDevice->OnRenderTarget(sCanvasTexture);
		//sEmotePlayer->Render();
		//for (emote_uint32_t i = 0; i < sClonePlayerList.size(); i++) {
		//	sEmoteDevice->OnRenderTarget(sCanvasTexture);
		//	sClonePlayerList[i]->Render();
		//}
		//if (useD3DEx && UseD3DSurface)
		//{
		//	sD3DDeviceEx->GetRenderTargetData(g_pd3dSurface, sBackBufferSurface);
		//	//sD3DDeviceEx->UpdateSurface(g_pd3dSurface, NULL, sBackBufferSurface, NULL);
		//}
	}

	//------------------------------------------------
	// メイン Main
	//------------------------------------------------
	//void
	//	RequireCanvasTexture(void)
	//{
	//	if (sCanvasTexture)
	//		return;
	//	
	//	sCanvasTextureWidth = clp(SCREEN_WIDTH);
	//	sCanvasTextureHeight = clp(SCREEN_HEIGHT);
	//	HRESULT hr;
	//	hr = sD3DDevice->CreateTexture(sCanvasTextureWidth, sCanvasTextureHeight,
	//		1,
	//		D3DUSAGE_RENDERTARGET,
	//		D3DFMT_A8R8G8B8,
	//		D3DPOOL_DEFAULT,
	//		&sCanvasTexture,
	//		NULL);
	//}

	unsigned long Emote::D3DTestCooperativeLevel()
	{
		return sD3DDevice->TestCooperativeLevel();
	}

	void Emote::RequireCanvasTexture(void)
	{
		if (sCanvasTexture)
			return;

		sCanvasTextureWidth = clp(sScreenWidth);
		sCanvasTextureHeight = clp(sScreenWidth); //clp(sScreenHeight);

		//sCanvasTextureWidth = clp(1024);
		//sCanvasTextureHeight = clp(1024);

		pin_ptr<LPDIRECT3DTEXTURE9> nativePtr = &sCanvasTexture;

		HRESULT hr;
		hr = sD3DDevice->CreateTexture(sCanvasTextureWidth, sCanvasTextureHeight,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			nativePtr,
			NULL);
	}

	void Emote::AttachCanvasTexture(void)
	{
		Emote::RequireCanvasTexture();

		if (!sCanvasTexture)
			return;

		// バックバッファサーフェース退避 保存BackBuffers
		sD3DDevice->GetRenderTarget(0, &sBackBufferSurface);
		// レンダターゲットを設定 设定RenderTarget
		LPDIRECT3DSURFACE9 surface;
		sCanvasTexture->GetSurfaceLevel(0, &surface);
		sD3DDevice->SetRenderTarget(0, surface);
		surface->Release();
		// ビューポート設定 Viewport设定
		D3DVIEWPORT9 vp = { 0, 0, sScreenWidth, sScreenHeight, 0.0f, 1.0f };
		sD3DDevice->SetViewport(&vp);
		sD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);
	}

	void Emote::DetachCanvasTexture(void)
	{
		if (!sCanvasTexture)
			return;

		// バックバッファサーフェース復帰
		//ADDED
		useD3DSurface = false;
		sD3DDevice->SetRenderTarget(0, sBackBufferSurface);
		sBackBufferSurface->Release();
	}

	void Emote::DrawCanvasTexture(void)
	{
		if (!sCanvasTexture)
			return;

		float vl = -sScreenWidth / 2;
		float vt = -sScreenHeight / 2;
		float vr = sScreenWidth / 2;
		float vb = sScreenHeight / 2;
		float tl = 0.0f;
		float tt = 0.0f;
		float tr = float(sScreenWidth) / sCanvasTextureWidth;
		float tb = float(sScreenHeight) / sCanvasTextureHeight;

		LVERTEX vtx[4] = {
				{ vl, vt, 0, D3DCOLOR_ARGB(255, 255, 255, 255), tl, tt },
				{ vr, vt, 0, D3DCOLOR_ARGB(255, 255, 255, 255), tr, tt },
				{ vl, vb, 0, D3DCOLOR_ARGB(255, 255, 255, 255), tl, tb },
				{ vr, vb, 0, D3DCOLOR_ARGB(255, 255, 255, 255), tr, tb }
		};

		sD3DDevice->SetTexture(0, sCanvasTexture);
		sD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx, sizeof(LVERTEX));
		sD3DDevice->SetTexture(0, NULL);
	}

	//释放D3D资源
	void Emote::D3DRelease(void)
	{
		SAFE_RELEASE(sCanvasTexture);

		SAFE_RELEASE(sBackBufferSurface);

		SAFE_RELEASE(sD3DDevice);

		SAFE_RELEASE(sD3DDeviceEx);

		SAFE_RELEASE(sD3D);

		SAFE_RELEASE(sD3DEx);

		SAFE_RELEASE(sSurface);

		//if (g_pd3dDevice != NULL)
		//	g_pd3dDevice->Release();

		//if (g_pd3dDeviceEx != NULL)
		//	g_pd3dDeviceEx->Release();

		//if (g_pD3D != NULL)
		//	g_pD3D->Release();

		//if (g_pD3DEx != NULL)
		//	g_pD3DEx->Release();

		//if (g_pd3dSurface != NULL)
		//	g_pd3dSurface->Release();
	}

	void Emote::OnDeviceLost(void)
	{
		SAFE_RELEASE(sCanvasTexture);
		SAFE_RELEASE(sBackBufferSurface);
		SAFE_RELEASE(sSurface);
		device->OnDeviceLost();
	}

	void Emote::OnDeviceReset(void)
	{
		if (useD3DSurface)
		{
			pin_ptr<LPDIRECT3DSURFACE9> nativePtr = &sSurface;
			if (useD3DEx)
			{
				if (FAILED(sD3DDeviceEx->CreateRenderTarget(sScreenWidth, sScreenHeight,
					D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0,
					!useD3DEx, // lockable
					&sBackBufferSurface, NULL)))
				{
					return;
				}
				//surface = (int)sBackBufferSurface;
				//sD3DDeviceEx->SetRenderTarget(0, g_pd3dSurface);
				//用DX9绘图到Surface
				if (FAILED(sD3DDevice->CreateOffscreenPlainSurface(sScreenWidth, sScreenHeight, D3DFMT_UNKNOWN, D3DPOOL_SYSTEMMEM, nativePtr, NULL)))
				{
					return;
				}
				//if (FAILED(sD3DDevice->CreateRenderTarget(sScreenWidth, sScreenHeight,
				//	D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0,
				//	!useD3DEx, // lockable
				//	nativePtr, NULL)))
				//{
				//	return;
				//}
				if (useD3DSurface)
				{
					sD3DDevice->SetRenderTarget(0, sSurface);
				}
			}
			else
			{
				if (FAILED(sD3DDevice->CreateRenderTarget(sScreenWidth, sScreenHeight,
					D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0,
					!useD3DEx, // lockable
					nativePtr, NULL)))
				{
					return;
				}
				//surface = (int)g_pd3dSurface;
				sD3DDevice->SetRenderTarget(0, sSurface);
				sD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				// turn off D3D lighting to use vertex colors
				sD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
				// Enable alpha blending.
				sD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			}
		}
		D3DInitRenderState();
	}

	//D3D
	void Emote::D3DBeginScene(void)
	{
		// clear the surface to transparent
		sD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

		// render the scene
		if (!SUCCEEDED(sD3DDevice->BeginScene()))
		{
			throw gcnew Exception("D3D Begin Scene Failed.");
		}

		if (useD3DEx)
		{
			sD3DDeviceEx->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
			if (!SUCCEEDED(sD3DDeviceEx->BeginScene()))
			{
				throw gcnew Exception("D3DEx Begin Scene Failed.");
			}
		}
	}

	void Emote::D3DEndScene(void)
	{
		sD3DDevice->EndScene();
		if (useD3DEx | 0)
		{
			sD3DDevice->Present(NULL, NULL, NULL, NULL);
			if (useD3DEx && UseD3DSurface)
			{
				HRESULT hr = sD3DDeviceEx->StretchRect(sSurface, NULL, sBackBufferSurface, NULL, D3DTEXF_NONE);
				if (FAILED(hr))
				{
					return;
				}
				//sD3DDeviceEx->UpdateSurface(g_pd3dSurface, NULL, sBackBufferSurface, NULL);
			}
			sD3DDeviceEx->EndScene();
			//sD3DDevice->Present(NULL, NULL, NULL, NULL);
		}
		//HRESULT hr = sD3DDevice->Present(NULL, NULL, NULL, NULL);
	}

	//------------------------------------------------
	// スキップ処理
	void Emote::Skip(void)
	{
		for each (auto player in PlayerCollection->Values)
		{
			if (player != nullptr)
			{
				player->Skip();
			}
		}
		//OutputDebugString(L"emote skip.\n");
	}

	void Emote::Show(void)
	{
		for each (auto player in PlayerCollection->Values)
		{
			if (player != nullptr)
			{
				player->Show();
			}
		}
	}

	EmotePlayer ^ EmoteDevice::CreatePlayer(String^ path)
	{
		if (!System::IO::File::Exists(path))
		{
			throw gcnew FileNotFoundException("File not exist.");
		}
		//MARK
		//sEmoteDevice->SetMaskRegionClipping(true);

		pin_ptr<const wchar_t> nativePath = PtrToStringChars(path);
		// E-mtoeデータファイル読み込み
		//读取Emote PSB文件
		HANDLE handle = CreateFile(nativePath,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		unsigned int size = GetFileSize(handle, NULL);
		BYTE *buf = new BYTE[size];
		DWORD dummy;
		ReadFile(handle, buf, size, &dummy, NULL);
		CloseHandle(handle);

		//// E-moteファイルイメージに事前にフィルタ処理を行う
		//EmoteFilterTexture(buf, size, GrayscaleFilter);

		if (UseTextureFilter && TextureFilter != nullptr)
		{
			EmoteFilterTexture(buf, size, (FP_EMOTE_FILTER_FUNC)((Marshal::GetFunctionPointerForDelegate(TextureFilter)).ToPointer()));
		}

		// E-moteプレイヤ作成
		//ADDED:
		IEmotePlayer *p;
		pin_ptr<IEmotePlayer*> nativePtr = &p;
		device->CreatePlayer(buf, size, nativePtr);
		FreeMote::EmotePlayer^ player = gcnew FreeMote::EmotePlayer(p);

		// 処理の終わったファイルイメージを破棄
		delete[] buf;
		//sClonePlayerList.push_back(p);
		// 表示開始
		//sEmotePlayer->Show();
		return player;
	}

	EmotePlayer ^ EmoteDevice::CreatePlayer(array<String^>^ paths)
	{
		emote_uint8_t **bufs = new emote_uint8_t*[paths->Length];
		unsigned long *sizes = new unsigned long[paths->Length];

		for (int i = 0; i < paths->Length; i++)
		{
			auto path = paths[i];
			if (!System::IO::File::Exists(path))
			{
				throw gcnew FileNotFoundException("File not exist.");
			}

			pin_ptr<const wchar_t> nativePath = PtrToStringChars(path);
			// E-mtoeデータファイル読み込み
			//读取Emote PSB文件
			HANDLE handle = CreateFile(nativePath,
				GENERIC_READ,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			unsigned long size = GetFileSize(handle, NULL);
			BYTE *buf = new BYTE[size];
			DWORD dummy;
			ReadFile(handle, buf, size, &dummy, NULL);
			CloseHandle(handle);

			if (UseTextureFilter && TextureFilter != nullptr)
			{
				EmoteFilterTexture(buf, size, (FP_EMOTE_FILTER_FUNC)((Marshal::GetFunctionPointerForDelegate(TextureFilter)).ToPointer()));
			}
			//ADDED:
			bufs[i] = buf;
			sizes[i] = size;
		}
		//MARK
		//sEmoteDevice->SetMaskRegionClipping(true);

		// E-moteプレイヤ作成
		//SEE: http://my.oschina.net/xiangxw/blog/123279
		//SEE: http://blog.csdn.net/zzxiang1985/article/details/6155153
		emote_uint8_t const ** eBuf = const_cast<const emote_uint8_t**>(bufs);
		//const ULONG* eSize = sizes;
		IEmotePlayer *p;
		pin_ptr<IEmotePlayer*> nativePtr = &p;
		device->CreatePlayer(paths->Length, eBuf, sizes, nativePtr);

		FreeMote::EmotePlayer^ player = gcnew FreeMote::EmotePlayer(p);

		// 処理の終わったファイルイメージを破棄
		delete[] bufs;
		delete[] sizes;
		//sClonePlayerList.push_back(p);
		// 表示開始
		//sEmotePlayer->Show();
		return player;
	}

	EmotePlayer^ Emote::CreatePlayer(String^ name, String^ path)
	{
		Lock lock(PlayerCollection);
		if (PlayerCollection->ContainsKey(name))
		{
			return PlayerCollection[name];
		}
		//sClonePlayerList.push_back(sEmotePlayer->Clone());
		auto player = device->CreatePlayer(path);
		PlayerCollection->Add(name, player);
		return player;
	}

	EmotePlayer^ Emote::CreatePlayer(String^ name, array<String^>^ paths)
	{
		Lock lock(PlayerCollection);
		if (PlayerCollection->ContainsKey(name))
		{
			return PlayerCollection[name];
		}
		//sClonePlayerList.push_back(sEmotePlayer->Clone());
		auto player = device->CreatePlayer(paths);
		PlayerCollection->Add(name, player);
		return player;
	}


	//void EmoteDevice::FilterProcess(BYTE *image, ULONG imageSize)
	//{
	//	if (ApplyTextureFilter != nullptr)
	//	{
	//		ApplyTextureFilter->Invoke(image, imageSize);
	//	}
	//}

	//void
	//	Emote::EmoteDelete(void)
	//{
	//	if (sClonePlayerList.size()) {
	//		sClonePlayerList[0]->Release();
	//		sClonePlayerList.erase(sClonePlayerList.begin());
	//	}
	//}

	//static Emote^ CreateFromDevice(IntPtr^ nativePointer)
	//{
	//	sD3DDevice = nativePointer->operator int;
	//	Emote^ e = gcnew Emote();
	//	sD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &sBackBufferSurface);
	//	
	//	D3DDEVICE_CREATION_PARAMETERS * sD3DCpp;
	//	sD3DDevice->GetDirect3D(&sD3D);
	//	
	//	
	//	sScreenHeight = sD3DDevice->;
	//	sScreenWidth = width;
	//	useD3DSurface = useSurface;

	//	if (width < 0 || height < 0)
	//	{
	//		RECT rect;
	//		GetClientRect(sHwnd, &rect);
	//		sScreenWidth = abs(rect.right - rect.left);
	//		sScreenHeight = abs(rect.bottom - rect.top);
	//	}
}
