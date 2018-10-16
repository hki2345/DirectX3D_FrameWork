#include "KDevice.h"
#include "KWindow.h"
#include "Core_Class.h"
#include "Pixel_Shader.h"
#include "Stl_Assistor.h"

#include "DirectXHeader_DE.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"
#include "Blend.h"

#include "KMath.h"
#include "KFont.h"



KDevice::KDevice(KWindow* _Window) : Mother_KWindow(_Window),
m_pDevice(nullptr),
m_pContext(nullptr),
m_pTargetView(nullptr),
m_pTexture2D(nullptr),
m_pDS_View(nullptr),
m_pSwapChain(nullptr),
m_bInit(false),
m_Color(.5f, .5f, .5f, 1.0f)
{
}

KDevice::~KDevice()
{
	Release();
}

void KDevice::Release()
{
	if (nullptr != m_pDS_View) { m_pDS_View->Release(); }
	if (nullptr != m_pTargetView) { m_pTargetView->Release(); }
	if (nullptr != m_pSwapChain) { m_pSwapChain->Release(); }
	if (nullptr != m_pBackBuffer) { m_pBackBuffer->Release(); }
	if (nullptr != m_pTexture2D) { m_pTexture2D->Release(); }
	if (nullptr != m_pDevice) { m_pDevice->Release(); }
	if (nullptr != m_pContext) { m_pContext->Release(); }
}




bool KDevice::Init()
{
	m_bInit = false;
	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 嬢恐 獄穿生稽 巨郊戚什研 姥失拝 依戚劃-- 悦汽 切疑生稽背爽艦猿 紺 益惟 蒸陥.
	// 遂亀亜 薄仙 獄穿戚 嬢恐依昔走 達澗 依戚陥.
	D3D_FEATURE_LEVEL eLv = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	// D3D_DRIVER_TYPE_HARDWARE 虞壱 梅陥檎 3D 須採 3D舌帖研 達澗陥. 
	// -> 越朝研 達澗陥澗 倶
	// 社覗闘析 井酔澗 CPU稽 尻至 坦軒 - 持唖幻 背亀 位蟹汗険 依 旭走 省劃 せせせせ

	//  
	if (S_OK != D3D11CreateDevice(
		/*働呪雌伐*/nullptr, /*須採舌帖*/D3D_DRIVER_TYPE_HARDWARE
		, /*須採乞汲*/nullptr, /*Flag*/ iFlag, 
		/*??*/nullptr,/*??*/ 0,/*SDK 獄鮮*/ D3D11_SDK_VERSION, 
		/*閤焼臣 舌帖*/&m_pDevice, /*閤焼臣 薄仙 獄鮮*/&eLv, 
		/*閤焼臣 Context - 竺誤精 蟹掻拭廃嬉澗汽 情賎昔走澗 奄鉦 蒸製*/&m_pContext))
	{
		Release();
		return false;
	}

	// 菰銅 児巴元 傾婚聖 端滴廃陥.
	// 護舘域稽 採球郡惟 拝 依戚劃 -> 護 舘域稽 溌企獣轍 戚耕走研 杯失拝 依戚劃
	// 食奄辞澗 4舘域稽 馬心陥.
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_iMSLv);

	// 鋼発葵戚 0戚檎 拭君 降持廃 依績 - 雁尻 照鞠醤臓
	if (0 == m_iMSLv) { Release(); return false; }

	if (false == Create_SwapChain()) { Release(); return false; }
	if (false == Create_View()) { Release(); return false; }
	if (false == Create_ViewPort()) { Release(); return false; }
	

	m_bInit = true;
	return true;
}


bool KDevice::Create_SwapChain()
{
	// 杭 聖 亜遭 姥繕端陥. -> 戚依級聖 乞砧 段奄鉢背爽嬢醤 廃願ぞぞぞぞ
	// 蝕獣 益撹 鞠澗 惟 焼艦壱... 嬢巨研 展壱 展壱 展壱 亜辞 段奄鉢 馬澗 縦
	/*DXGI_MODE_DESC BufferDesc;
	DXGI_SAMPLE_DESC SampleDesc;
	DXGI_USAGE BufferUsage;
	UINT BufferCount;
	HWND OutputWindow;
	BOOL Windowed;
	DXGI_SWAP_EFFECT SwapEffect;
	UINT Flags;*/
	DXGI_SWAP_CHAIN_DESC m_Decs = {};

	// 拷獄遁 滴奄
	m_Decs.BufferDesc.Width = (UINT)window()->width();
	m_Decs.BufferDesc.Height = (UINT)window()->heigth();

	// 拷獄遁 爽紫晴 + 拷獄遁 鯵呪
	m_Decs.BufferDesc.RefreshRate.Numerator = 60;
	m_Decs.BufferDesc.RefreshRate.Denominator = 1;

	// 獄遁税 波侍 - 嬢彊廃 事 増杯生稽 獄遁税 波侍聖 辰酔畏陥.
	m_Decs.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 什諜 虞昔 什追析 室特 - 爽源 因採
	m_Decs.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_Decs.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 獄遁税 遂亀 - 鉢檎 紫窒
	m_Decs.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;


	// 疾巴元 旋遂 - 戚闇 戚穿 舘域拭辞 護 舘域稽 走据馬澗走 閤焼紳 及戚陥.
	m_Decs.SampleDesc.Count = m_iMSLv;
	m_Decs.SampleDesc.Quality = m_iMSLv - 1;


	// 制亀酔 室特
	m_Decs.OutputWindow = window()->KWnd();

	// 什尋拝凶 戚薙闘- 酔鍵 走錘陥.
	m_Decs.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 獄遁 鯵呪
	m_Decs.BufferCount = 1;

	// 穿端鉢檎 食採 - 背雁 制亀酔税 酔識授是 __ 穿端鉢檎戚 置酔識
	m_Decs.Windowed = !window()->Is_Full();


	/*********** 蓄板 竺誤 *************/ // 陳梓端人 淫恵赤製しし
	IDXGIDevice* p_Device = nullptr;
	IDXGIAdapter* p_Adapter = nullptr;
	IDXGIFactory* p_Factory = nullptr;

	m_pDevice->QueryInterface(__uuidof(p_Device), (void**)&p_Device);
	if (nullptr == p_Device) { return false; }

	p_Device->GetParent(__uuidof(IDXGIAdapter), (void**)&p_Adapter);
	if (nullptr == p_Adapter) { return false; }

	p_Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&p_Factory);
	if (nullptr == p_Factory) { return false; }

	// 食奄辞 什尋端昔聖 閤焼神奄 是敗戚陥.
	if (nullptr != p_Factory && S_OK != p_Factory->CreateSwapChain(m_pDevice, &m_Decs, &m_pSwapChain))
	{
		return false;
	}

	// 廃 板虞檎 琶推亜 蒸奄 凶庚拭 背薦
	p_Factory->Release();
	p_Adapter->Release();
	p_Device->Release();

	return true;
}

// 什尋端昔拭 紫遂吃 努什坦研 食奄辞 啄情危陥.
// 亀鉢走研 幻窮陥澗 汗界戚陥.
bool KDevice::Create_View()
{
	m_pBackBuffer = nullptr;
	if(S_OK != m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer))
	{
		return false;
	}

	// 展為坂研 砧舘域研 杏団辞 亜閃紳陥.戚係惟...
	if (S_OK != m_pDevice->CreateRenderTargetView(m_pBackBuffer, 0, &m_pTargetView))
	{
		return false;
	}

	// 陥献 員拭辞亀 床戚奄 凶庚拭 害移 兜澗陥.
	// m_pBackBuffer->Release();



	// 努什団 舛左 室特
	D3D11_TEXTURE2D_DESC m_Decs = {};

	m_Decs.ArraySize = 1;

	// 食奄辞 鋼生稽 幻級檎 鉢檎拭 鋼鉱拭 照蟹紳陥壱..せせせせ
	m_Decs.Width = (UINT)window()->width();
	m_Decs.Height = (UINT)window()->heigth();
	m_Decs.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 24 搾闘澗 float搭背 燕戚 端滴
	// 及拭 8 搾闘澗 int莫 搾闘舘是 尻至生稽 什度叔 獄遁 端滴

	m_Decs.SampleDesc.Count = m_iMSLv;
	m_Decs.SampleDesc.Quality = m_iMSLv - 1;

	// 荒己 竺舛
	m_Decs.MipLevels = 1;

	m_Decs.Usage = D3D11_USAGE_DEFAULT;
	m_Decs.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	

	m_pDevice->CreateTexture2D(&m_Decs, 0, &m_pTexture2D);

	if (nullptr == m_pTexture2D)
	{
		return false;
	}

	if (S_OK != m_pDevice->CreateDepthStencilView(m_pTexture2D, 0, &m_pDS_View))
	{
		return false;
	}
	
	// 沓希元 督戚覗 虞昔引 淫恵赤陥澗 ... 因採 馬虞管
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDS_View);

	return true;
}

bool KDevice::Create_ViewPort()
{
	D3D11_VIEWPORT m_Decs = {};

	m_Decs.TopLeftX = 0;
	m_Decs.TopLeftY = 0;

	m_Decs.Width = (float)window()->width();
	m_Decs.Height = (float)window()->heigth();

	m_Decs.MinDepth = 0;
	m_Decs.MaxDepth = 1;

	m_pContext->RSSetViewports(1, &m_Decs);

	return true;
}





/***************** Raster ***************/
KPtr<KDevice::RasterState> KDevice::Find_RasterMode(const wchar_t* _Name)
{
	return Map_Find<KPtr<RasterState>>(m_RasterMap, _Name);
}

void KDevice::Create_RasterMode(const wchar_t* _Name, D3D11_FILL_MODE _Fill, D3D11_CULL_MODE _Cull)
{
	RasterState* TempRaster = new RasterState();
	TempRaster->Create(m_pDevice, m_pContext, _Fill, _Cull);
	m_RasterMap.insert(std::unordered_map<std::wstring, KPtr<RasterState>>::value_type(_Name, TempRaster));
}


void KDevice::Set_RasterMode(const wchar_t* _Name)
{
	KPtr<RasterState> PRS = Map_Find<KPtr<RasterState>>(m_RasterMap, _Name);
	if (nullptr == PRS)
	{
		KASSERT(true);
		return;
	}
	PRS->Update();
}

void KDevice::Set_Raster(const wchar_t* _Name)
{
	KPtr<RasterState> RSS = Map_Find<KPtr<RasterState>>(m_RasterMap, _Name);
	if (nullptr == RSS)
	{
		KASSERT(true);
		return;
	}

	if (nullptr == RSS->m_RS)
	{
		KASSERT(true);
		return;
	}

	m_RState_Def =  RSS;	
	m_RState_Def->Update();
	return;
}

void KDevice::ResetRasterState()
{
	m_RState_Def->Update();
}

void KDevice::RasterState::Update()
{
	m_Context->RSSetState(m_RS);
}

void KDevice::RasterState::Create(ID3D11Device* _Device, ID3D11DeviceContext* _Context
	, D3D11_FILL_MODE _Fill, D3D11_CULL_MODE _Cull)
{
	if (nullptr == _Context)
	{
		KASSERT(true);
		return;
	}

	m_Desc.FillMode = _Fill;
	m_Desc.CullMode = _Cull;
	m_Context = _Context;

	if (S_OK != _Device->CreateRasterizerState(&m_Desc, &m_RS))
	{
		return;
	}
}



/******************** Depth Stencil ********************/
// Raster 淫恵
KPtr<KDevice::DepthStencilState> KDevice::Find_DepthStencil(const wchar_t* _Name)
{
	return Map_Find<KPtr<DepthStencilState>>(m_DepthStencilMap, _Name);
}

void KDevice::Create_DepthSencil(const wchar_t* _Name, D3D11_DEPTH_STENCIL_DESC _Desc)
{
	DepthStencilState* TempDS = new DepthStencilState();
	TempDS->Create(m_pDevice, m_pContext, _Desc);
	m_DepthStencilMap.insert(std::unordered_map<std::wstring, KPtr<DepthStencilState>>::value_type(_Name, TempDS));
}



void KDevice::Set_DepthSencil(const wchar_t* _Name)
{
	KPtr<DepthStencilState> DSS = Map_Find<KPtr<DepthStencilState>>(m_DepthStencilMap, _Name);
	if (nullptr == DSS)
	{
		KASSERT(true);
		return;
	}

	if (nullptr == DSS->m_DSS)
	{
		KASSERT(true);
		return;
	}

	m_DState_Def = DSS;
	m_DState_Def->Update();
	return;
}

void KDevice::Set_DepthSencilMode(const wchar_t* _Name, const KUINT& _Ref)
{
	KPtr<DepthStencilState> DSS = Map_Find<KPtr<DepthStencilState>>(m_DepthStencilMap, _Name);
	if (nullptr == DSS)
	{
		KASSERT(true);
		return;
	}
	DSS->Update(_Ref);
}





void KDevice::Reset_DepthStencil()
{
	m_DState_Def->Update();
}

void KDevice::DepthStencilState::Update(const KUINT& _Ref /*= 0*/)
{
	m_Context->OMSetDepthStencilState(m_DSS, _Ref);
}

void KDevice::DepthStencilState::Create(ID3D11Device* _Device, ID3D11DeviceContext* _Context, D3D11_DEPTH_STENCIL_DESC _Desc)
{
	if (nullptr == _Context)
	{
		KASSERT(true);
		return;
	}
	m_Context = _Context;

	if (S_OK != _Device->CreateDepthStencilState(&m_Desc, &m_DSS))
	{
		return;
	}
}


// 凹拐戚 背爽澗 敗呪
void KDevice::Clear_Target()
{
	m_pContext->ClearRenderTargetView(m_pTargetView, m_Color.s);
	m_pContext->ClearDepthStencilView(m_pDS_View, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

// 啄嬢爽澗 敗呪
void KDevice::Present()
{
	m_pSwapChain->Present(0, 0);
}


void KDevice::Reset_Context()
{
	m_pContext->VSSetShader(nullptr, nullptr, 0);
	m_pContext->HSSetShader(nullptr, nullptr, 0);
	m_pContext->GSSetShader(nullptr, nullptr, 0);
	m_pContext->CSSetShader(nullptr, nullptr, 0);
	m_pContext->PSSetShader(nullptr, nullptr, 0);
}


void KDevice::SetOM()
{
	// 越切蟹 陥献 兄希君亜 什度叔聖 闇球険 井酔 陥獣 兄希授辞研 説奄 是倍
	// 舛溌備澗 沓希 授辞虞奄 左陥 及宋酵宋吉 燕戚葵聖 仙繕舛
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDS_View);
}


bool KDevice::Create_DeviceCB(Device_CB* _NewBuf)
{
	if (S_OK != Core_Class::Device()->CreateBuffer(&_NewBuf->tDesc, nullptr, &_NewBuf->p_ConstBuffer))
	{
		delete _NewBuf;
		return false;
	}
	return true;
}

KPtr<KDevice::Device_CB> KDevice::Find_DeviceCB(const wchar_t* _Name)
{
	return Map_Find<KPtr<KDevice::Device_CB>>(m_CB_Map, _Name);
}