#include "KDevice.h"
#include "DXContainer_DE.h"
#include "KWindow.h"
#include "Stl_AID.h"
#include "KMesh.h"
#include "Core_Class.h"
#include "Shader_Vertex.h"
#include "Shader_Pixel.h"
#include "ResourceManager.h"
#include "KMaterial.h"
#include "KBlend.h"
#include "KFont.h"
#include "KMath.h"
#include "Light.h"
#include "Renderer.h"
#include "Texture.h"
#include "Sampler.h"
#include "RenderTarget_Multi.h"



bool KDevice::DefRenderTaget() 
{


	// Defferd��
	ResourceManager<RenderTarget>::Create(L"COLOR_DIFFUSE" , Core_Class::MainWindow().width_u() , Core_Class::MainWindow().height_u()	, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE	, DXGI_FORMAT_R32G32B32A32_FLOAT);
	ResourceManager<RenderTarget>::Create(L"POSTION", Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	ResourceManager<RenderTarget>::Create(L"NORMAL", Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	ResourceManager<RenderTarget>::Create(L"DEPTH", Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(), D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	KPtr<RenderTarget_Multi> DefMRT = ResourceManager<RenderTarget_Multi>::Create(L"DEFFERD", L"COLOR_DIFFUSE", L"POSTION", L"NORMAL", L"DEPTH");

	ResourceManager<RenderTarget>::Create(L"LIGHT_DIFFUSE", Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(), KVector4::Black, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	ResourceManager<RenderTarget>::Create(L"LIGHT_SPECULAR", Core_Class::MainWindow().width_u(), Core_Class::MainWindow().height_u(),KVector4::Black,  D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

	KPtr<RenderTarget_Multi> LightMRT = ResourceManager<RenderTarget_Multi>::Create(L"LIGHT", L"LIGHT_DIFFUSE", L"LIGHT_SPECULAR");

	return true;
}

bool KDevice::Def3DCreate() 
{
	ResourceManager<Sampler>::Create(L"DefaultSmp");
	ResourceManager<KBlend>::Create(L"ALPHA");

	Core_Class::MainDevice().CreateCB<MatrixContainer>(L"MATCON", D3D11_USAGE_DYNAMIC, 10);
	Core_Class::MainDevice().CreateCB<RenderOption>(L"RENDEROPTION", D3D11_USAGE_DYNAMIC, 11);
	Core_Class::MainDevice().CreateCB<Light::LightCBDATA>(L"LIGHTDATA", D3D11_USAGE_DYNAMIC, 12);
	Core_Class::MainDevice().CreateCB<Light::LightData>(L"DEFFERDLIGHTDATA", D3D11_USAGE_DYNAMIC, 12);

	Core_Class::MainDevice().Create_RS(L"SNONE", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_NONE);
	Core_Class::MainDevice().Create_RS(L"SBACK", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK);
	Core_Class::MainDevice().Create_RS(L"SFRONT", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_FRONT);

	Core_Class::MainDevice().Create_RS(L"WNONE", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_NONE);
	Core_Class::MainDevice().Create_RS(L"WBACK", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_BACK);
	Core_Class::MainDevice().Create_RS(L"WFRONT", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_FRONT);
	Core_Class::MainDevice().Set_RSDef(L"SBACK");

	// �X���� �ѳ���
	D3D11_DEPTH_STENCIL_DESC DepthState;

	DepthState.DepthEnable = TRUE;
	// D3D11_DEPTH_WRITE_MASK_ALL �X���񱳸� �ϰڴٴ� ���̴�.
	// D3D11_DEPTH_WRITE_MASK_ZERO ���� �ʰڴ�.
	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
	// ���ٽǿ� ���õ� ��.
	// 0x000000ff ���� �ڿ� ����ϰڴ�.
	DepthState.StencilEnable = FALSE;
	DepthState.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	DepthState.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	D3D11_DEPTH_STENCILOP_DESC defaultStencil =
	{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };
	DepthState.FrontFace = defaultStencil;
	DepthState.BackFace = defaultStencil;
	Core_Class::MainDevice().Create_DSS(L"DEBUG", DepthState);

	// �������ΰ�.
	DepthState.DepthFunc = D3D11_COMPARISON_LESS;
	Core_Class::MainDevice().Create_DSS(L"BASIC", DepthState);

	DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
	Core_Class::MainDevice().Create_DSS(L"ALWAYS", DepthState);

	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
	Core_Class::MainDevice().Create_DSS(L"LIGHTDEPTH", DepthState);


	//////////////// VOLUMBACK
	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthState.DepthFunc = D3D11_COMPARISON_GREATER;
	DepthState.StencilEnable = TRUE;

	D3D11_DEPTH_STENCILOP_DESC LightStencil;
	LightStencil.StencilFunc = D3D11_COMPARISON_ALWAYS;
	LightStencil.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	LightStencil.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	LightStencil.StencilPassOp = D3D11_STENCIL_OP_REPLACE;

	DepthState.FrontFace = LightStencil;
	DepthState.BackFace = LightStencil;
	Core_Class::MainDevice().Create_DSS(L"BACK_ST", DepthState);

	//////////////// VOLUMFORONT
	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthState.DepthFunc = D3D11_COMPARISON_LESS;
	DepthState.StencilEnable = TRUE;

	LightStencil.StencilFunc = D3D11_COMPARISON_EQUAL;
	LightStencil.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	LightStencil.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	LightStencil.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	DepthState.FrontFace = LightStencil;
	DepthState.BackFace = LightStencil;
	Core_Class::MainDevice().Create_DSS(L"FRONT_ST", DepthState);

	//////////////// PASS

	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DepthState.DepthFunc = D3D11_COMPARISON_ALWAYS;
	DepthState.StencilEnable = TRUE;

	LightStencil.StencilFunc = D3D11_COMPARISON_EQUAL;
	LightStencil.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	LightStencil.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	LightStencil.StencilPassOp = D3D11_STENCIL_OP_ZERO;

	DepthState.FrontFace = LightStencil;
	DepthState.BackFace = LightStencil;

	Core_Class::MainDevice().Create_DSS(L"PASS_ST", DepthState);

	// �ϳ��� �� ������ �Ѵ�.

	Core_Class::MainDevice().Set_DSSDef(L"BASIC");



	///////////////////////////////////////////// BS
	D3D11_BLEND_DESC m_Desc;
	m_Desc.AlphaToCoverageEnable = false;

	// �ٸ� ���� Ÿ�ٵ� ���ε��� ���ڴ� Ȥ�� �ƴϴ�.
	// ������ ������ false�� �ϸ� 0������ �ʱ�ȭ �ȴ�.
	m_Desc.IndependentBlendEnable = true;


	// ��ǻ�� -> ��ǻ��� ���� ���� ���� ����� ���� ����
	m_Desc.RenderTarget[0].BlendEnable = true;
	m_Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	m_Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	m_Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	m_Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	m_Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	m_Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;


	// ���� ŧ�� - ���� ������ ���� ������ ���� -> ���� ���ķ����� ���� ������� ����ȴ�.
	// ���� ���� ����� -> ���Ŀ� ���İ� ���̴� ����
	m_Desc.RenderTarget[1].BlendEnable = true;
	m_Desc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_Desc.RenderTarget[1].BlendOp = D3D11_BLEND_OP_ADD;
	m_Desc.RenderTarget[1].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	m_Desc.RenderTarget[1].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	m_Desc.RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	m_Desc.RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ONE;
	m_Desc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ZERO;


	for (size_t i = 2; i < 8; i++)
	{
		m_Desc.RenderTarget[i] = D3D11_RENDER_TARGET_BLEND_DESC{};
	}

	ResourceManager<KBlend>::Create(L"LIGHTONE", m_Desc);

	m_Desc.RenderTarget[0] = D3D11_RENDER_TARGET_BLEND_DESC{};
	m_Desc.RenderTarget[0].RenderTargetWriteMask = 0;


	ResourceManager<KBlend>::Create(L"VOLUME", m_Desc);

	return true;
}
bool KDevice::Mesh3DCreate() {

	////////////////////////////////// RECT
#pragma region RectMeshCreate
	VTX3D ArrVTX3D[4];
	ArrVTX3D[0].Pos = KVector4(-0.5f, 0.5f, 0.0f, 1.0f);
	ArrVTX3D[1].Pos = KVector4(0.5f, 0.5f, 0.0f, 1.0f);
	ArrVTX3D[2].Pos = KVector4(-0.5f, -0.5f, 0.0f, 1.0f);
	ArrVTX3D[3].Pos = KVector4(0.5f, -0.5f, 0.0f, 1.0f);

	ArrVTX3D[0].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	ArrVTX3D[1].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	ArrVTX3D[2].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	ArrVTX3D[3].Color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);

	ArrVTX3D[0].Uv = KVector2(0.0f, 0.0f);
	ArrVTX3D[1].Uv = KVector2(1.0f, 0.0f);
	ArrVTX3D[2].Uv = KVector2(0.0f, 1.0f);
	ArrVTX3D[3].Uv = KVector2(1.0f, 1.0f);

	ArrVTX3D[0].Normal = KVector4(0.0f, 0.0f, -1.0f, 1.0f);
	ArrVTX3D[1].Normal = KVector4(0.0f, 0.0f, -1.0f, 1.0f);
	ArrVTX3D[2].Normal = KVector4(0.0f, 0.0f, -1.0f, 1.0f);
	ArrVTX3D[3].Normal = KVector4(0.0f, 0.0f, -1.0f, 1.0f);

	IDX16 ArrColorIDX[2] = {};

	ArrColorIDX[0] = IDX16(0, 3, 2);
	ArrColorIDX[1] = IDX16(0, 1, 3);

	ResourceManager<KMesh>::Create(L"RECT"
		, 4, (UINT)VTX3D::TypeSize(), D3D11_USAGE_DYNAMIC, ArrVTX3D
		, 6, (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, ArrColorIDX
		, IDX16::FM());
#pragma endregion

#pragma region BoxMeshCreate

	// ������ü �Ž����� ����� ����.
	std::vector<VTX3D> OverCube;
	std::vector<WORD> OverIdx;

	VTX3D V;
	V.Pos = KVector4{ -0.5f, 0.5f, 0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, 1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ 0.5f, 0.5f, 0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, 1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ 0.5f, 0.5f, -0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, 1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ -0.5f, 0.5f, -0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, 1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	// �Ʒ���
	V.Pos = KVector4{ -0.5f, -0.5f, 0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, -1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ 0.5f, -0.5f, 0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, -1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ 0.5f, -0.5f, -0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, -1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ -0.5f, -0.5f, -0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, -1.0f, 0.0f, 0.0f };
	OverCube.push_back(V);


	// ������
	V.Pos = KVector4{ 0.5f, 0.5f, -0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ 0.5f, 0.5f, 0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ 0.5f, -0.5f, 0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ 0.5f, -0.5f, -0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	// ����
	V.Pos = KVector4{ -0.5f, 0.5f, -0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ -1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ -0.5f, 0.5f, 0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ -1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ -0.5f, -0.5f, 0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ -1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ -0.5f, -0.5f, -0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ -1.0f, 0.0f, 0.0f, 0.0f };
	OverCube.push_back(V);

	// ��
	V.Pos = KVector4{ -0.5f, 0.5f, 0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, 0.0f, -1.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ 0.5f, 0.5f, 0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, 0.0f, -1.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ 0.5f, -0.5f, 0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, 0.0f, -1.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ -0.5f, -0.5f, 0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, 0.0f, -1.0f, 0.0f };
	OverCube.push_back(V);

	// ��
	V.Pos = KVector4{ -0.5f, 0.5f, -0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, 0.0f, 1.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ 0.5f, 0.5f, -0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, 0.0f, 1.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ 0.5f, -0.5f, -0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, 0.0f, 1.0f, 0.0f };
	OverCube.push_back(V);

	V.Pos = KVector4{ -0.5f, -0.5f, -0.5f, 1.0f };
	V.Uv = KVector2{ 0.0f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = KVector4{ 0.0f, 0.0f, 1.0f, 0.0f };
	OverCube.push_back(V);

	// ���� �ε���
	OverIdx.push_back(0); OverIdx.push_back(1); OverIdx.push_back(2);
	OverIdx.push_back(0); OverIdx.push_back(2); OverIdx.push_back(3);

	// �Ʒ���
	OverIdx.push_back(6); OverIdx.push_back(5); OverIdx.push_back(4);
	OverIdx.push_back(6); OverIdx.push_back(4); OverIdx.push_back(7);

	// ������
	OverIdx.push_back(8); OverIdx.push_back(9); OverIdx.push_back(10);
	OverIdx.push_back(8); OverIdx.push_back(10); OverIdx.push_back(11);

	// ���� ��
	OverIdx.push_back(13); OverIdx.push_back(12); OverIdx.push_back(15);
	OverIdx.push_back(13); OverIdx.push_back(15); OverIdx.push_back(14);

	// �ո�
	OverIdx.push_back(17); OverIdx.push_back(16); OverIdx.push_back(19);
	OverIdx.push_back(17); OverIdx.push_back(19); OverIdx.push_back(18);

	// �޸�
	OverIdx.push_back(20); OverIdx.push_back(21); OverIdx.push_back(22);
	OverIdx.push_back(20); OverIdx.push_back(22); OverIdx.push_back(23);

	ResourceManager<KMesh>::Create(L"CUBE"
		, (UINT)OverCube.size(), (UINT)VTX3D::TypeSize(), D3D11_USAGE_DYNAMIC, &OverCube[0]
		, (UINT)OverIdx.size(), (UINT)IDX16::MemberSize(), D3D11_USAGE_DEFAULT, &OverIdx[0]
		, IDX16::FM());

#pragma endregion

#pragma region SphereMeshCreate
	std::vector<VTX3D> SphereVtx;
	std::vector<UINT> SphereIdx;

	float fRadius = 0.5f;

	// �ϱ���
	V.Pos = KVector4{ 0.0f, fRadius, 0.0f, 1.0f };
	V.Uv = KVector2{ 0.5f, 0.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = V.Pos;
	V.Normal.NormalizeVec3();
	V.Normal.w = 0.0f;
	V.Tangent = KVector4{ 1.0f, 0.0f, 0.0f, 0.0f };
	V.BNormal = KVector4{ 0.0f, 0.0f, 1.0f, 0.0f };

	SphereVtx.push_back(V);

	UINT iStackCount = 40; // ���� ���� ����
	UINT iSliceCount = 40; // ���� ���� ����

	float yRotAngle = DirectX::XM_PI / (float)iStackCount;
	float zRotAngle = DirectX::XM_2PI / (float)iSliceCount;

	float yUvRatio = 1.0f / (float)iSliceCount;
	float zUvRatio = 1.0f / (float)iStackCount;

	// PointToDir

	for (UINT y = 1; y < iStackCount; ++y)
	{
		float phi = y * yRotAngle;

		for (UINT z = 0; z < iSliceCount + 1; ++z)
		{
			float theta = z * zRotAngle;

			V.Pos = KVector4
			{
				fRadius * sinf(y * yRotAngle) * cosf(z * zRotAngle),
				fRadius * cosf(y * yRotAngle),
				fRadius * sinf(y * yRotAngle) * sinf(z * zRotAngle),
				1.0f
			};

			V.Pos.ElipseCut();

			V.Pos.w = 1.0f;
			V.Uv = KVector2{ yUvRatio * z, zUvRatio * y };
			V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
			V.Normal = V.Pos;
			V.Normal.NormalizeVec3();
			V.Normal.w = 0.0f;

			V.Tangent.x = -fRadius * sinf(phi) * sinf(theta);
			V.Tangent.y = 0.0f;
			V.Tangent.z = fRadius * sinf(phi) * cosf(theta);
			V.Tangent.NormalizeVec3();
			V.Tangent.w = 0.0f;

			V.BNormal = KVector4::cross3D(V.Tangent, V.Normal);
			V.BNormal.NormalizeVec3();
			V.BNormal.w = 0.0f;

			SphereVtx.push_back(V);
		}
	}

	// �ϱ���
	V.Pos = KVector4{ 0.0f, -fRadius, 0.0f, 1.0f };
	V.Uv = KVector2{ 0.5f, 1.0f };
	V.Color = KVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
	V.Normal = V.Pos;
	V.Normal.NormalizeVec3();
	V.Normal.w = 0.0f;

	SphereVtx.push_back(V);

	SphereIdx.clear();

	// �ϱ��� �ε���
	for (UINT i = 0; i < iSliceCount; i++)
	{
		SphereIdx.push_back(0);
		SphereIdx.push_back(i + 2);
		SphereIdx.push_back(i + 1);
	}


	for (UINT y = 0; y < iStackCount - 2; ++y)
	{
		for (UINT z = 0; z < iSliceCount; ++z)
		{
			SphereIdx.push_back((iSliceCount + 1) * y + z + 1);
			SphereIdx.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
			SphereIdx.push_back((iSliceCount + 1) * (y + 1) + z + 1);

			SphereIdx.push_back((iSliceCount + 1) * y + z + 1);
			SphereIdx.push_back((iSliceCount + 1) * y + (z + 1) + 1);
			SphereIdx.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
		}
	}

	// �ϱ��� �ε���

	UINT iBotStart = (UINT)SphereVtx.size() - 1;

	for (UINT i = 0; i < iSliceCount; ++i)
	{
		SphereIdx.push_back(iBotStart);
		SphereIdx.push_back(iBotStart - (i + 2));
		SphereIdx.push_back(iBotStart - (i + 1));
	}

	ResourceManager<KMesh>::Create(L"SPHERE"
		, (UINT)SphereVtx.size(), (UINT)VTX3D::TypeSize(), D3D11_USAGE_DYNAMIC, &SphereVtx[0]
		, (UINT)SphereIdx.size(), (UINT)IDX32::MemberSize(), D3D11_USAGE_DEFAULT, &SphereIdx[0]
		, IDX32::FM());

#pragma endregion
	return true;
}
bool KDevice::Mat3DCreate() {

	// NONE
	KPtr<Shader_Vertex> NONE3DVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"NONEVTX", L"Shader", L"NONE.fx", "VTXNONE3D");
	NONE3DVTX->Add_LayoutFin("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<Shader_Pixel> NONE3DPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"NONEPIX", L"Shader", L"NONE.fx", "PIXNONE3D");

	KPtr<KMaterial> NONEMAT = ResourceManager<KMaterial>::Create(L"NONEMAT");
	NONEMAT->Set_VTShader(L"NONEVTX");
	NONEMAT->Set_PXShader(L"NONEPIX");
	NONEMAT->Set_Blend(L"ALPHA");

	KPtr<Shader_Vertex> TAGETDEBUGVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"TAGETDEBUGVTX", L"Shader", L"TagetDebug.fx", "VS_TagetTex");
	TAGETDEBUGVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	TAGETDEBUGVTX->Add_LayoutFin("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	KPtr<Shader_Pixel> TAGETDEBUGPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"TAGETDEBUGPIX", L"Shader", L"TagetDebug.fx", "PS_TagetTex");

	KPtr<KMaterial> TAGETDEBUGMAT = ResourceManager<KMaterial>::Create(L"TAGETDEBUGMAT");
	TAGETDEBUGMAT->Set_VTShader(L"TAGETDEBUGVTX");
	TAGETDEBUGMAT->Set_PXShader(L"TAGETDEBUGPIX");
	TAGETDEBUGMAT->Set_Blend(L"ALPHA");

	///////////////////////////////////////// GRID
	KPtr<Shader_Vertex> GRID3DVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"GRID3DVTX", L"Shader", L"GRID3D.fx", "VS_GRID3D");
	GRID3DVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	GRID3DVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	GRID3DVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	GRID3DVTX->Add_LayoutFin("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

	KPtr<Shader_Pixel> GRID3DPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"GRID3DPIX", L"Shader", L"GRID3D.fx", "PS_GRID3D");
	GRID3DPIX->CreateCB<KVector4>(L"GRIDDATA", D3D11_USAGE_DYNAMIC, 0);

	KPtr<KMaterial> GRID3DMAT = ResourceManager<KMaterial>::Create(L"GRID3DMAT");
	GRID3DMAT->Set_VTShader(L"GRID3DVTX");
	GRID3DMAT->Set_PXShader(L"GRID3DPIX");
	GRID3DMAT->Set_Blend(L"ALPHA");

	// RECT
	KPtr<Shader_Vertex> RECT3DVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"RECT3DVTX", L"Shader", L"RECT3D.fx", "VS_RECT3D");
	RECT3DVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	RECT3DVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	RECT3DVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	RECT3DVTX->Add_LayoutFin("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<Shader_Pixel> RECT3DPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"RECT3DPIX", L"Shader", L"RECT3D.fx", "PS_RECT3D");

	KPtr<KMaterial> RECT3DMAT = ResourceManager<KMaterial>::Create(L"RECT3DMAT");
	RECT3DMAT->Set_VTShader(L"RECT3DVTX");
	RECT3DMAT->Set_PXShader(L"RECT3DPIX");
	RECT3DMAT->Set_Blend(L"ALPHA");

	// SKY
	KPtr<Shader_Vertex> SKY3DVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"SKY3DVTX", L"Shader", L"SkyBox.fx", "VS_SKY3D");
	SKY3DVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	SKY3DVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	SKY3DVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	SKY3DVTX->Add_LayoutFin("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<Shader_Pixel> SKY3DPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"SKY3DPIX", L"Shader", L"SkyBox.fx", "PS_SKY3D");

	KPtr<KMaterial> SKY3DMAT = ResourceManager<KMaterial>::Create(L"SKY3DMAT");
	SKY3DMAT->Set_VTShader(L"SKY3DVTX");
	SKY3DMAT->Set_PXShader(L"SKY3DPIX");
	SKY3DMAT->Set_Blend(L"ALPHA");
	// SKY3DMAT->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"Sky01.png");

	KPtr<Shader_Vertex> MESH3DVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"MESH3DVTX", L"Shader", L"MeshMat.fx", "VS_MESH3D");
	MESH3DVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	MESH3DVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	MESH3DVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	MESH3DVTX->Add_Layout("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	MESH3DVTX->Add_Layout("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	MESH3DVTX->Add_LayoutFin("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<Shader_Pixel> MESH3DPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"MESH3DPIX", L"Shader", L"MeshMat.fx", "PS_MESH3D");

	KPtr<KMaterial> MESH3DMAT = ResourceManager<KMaterial>::Create(L"MESH3DMAT");
	MESH3DMAT->Set_VTShader(L"MESH3DVTX");
	MESH3DMAT->Set_PXShader(L"MESH3DPIX");
	MESH3DMAT->Set_Blend(L"ALPHA");

	KPtr<Shader_Vertex> DEFFERD3DVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"DEFFERD3DVTX", L"Shader", L"Defferd.fx", "VS_DEFFERD");
	DEFFERD3DVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DVTX->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	DEFFERD3DVTX->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DVTX->Add_Layout("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DVTX->Add_Layout("TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERD3DVTX->Add_LayoutFin("BINORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<Shader_Pixel> DEFFERD3DPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"DEFFERD3DPIX", L"Shader", L"Defferd.fx", "PS_DEFFERD");

	KPtr<KMaterial> DEFFERD3DMAT = ResourceManager<KMaterial>::Create(L"DEFFERD3DMAT");
	DEFFERD3DMAT->Set_VTShader(L"DEFFERD3DVTX");
	DEFFERD3DMAT->Set_PXShader(L"DEFFERD3DPIX");
	DEFFERD3DMAT->Set_Blend(L"ALPHA");

	// DefferdDirLight
	KPtr<Shader_Vertex> DEFFERDLIGHTVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"DEFFERDLIGHTVTX", L"Shader", L"Defferd.fx", "VS_DEFFERDLIGHT");
	DEFFERDLIGHTVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERDLIGHTVTX->Add_LayoutFin("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	KPtr<Shader_Pixel> DEFFERDLIGHTPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"DEFFERDLIGHTPIX", L"Shader", L"Defferd.fx", "PS_DEFFERDLIGHT");

	KPtr<KMaterial> DEFFERDMAT = ResourceManager<KMaterial>::Create(L"DEFFERDLIGHTMAT");
	DEFFERDMAT->Set_VTShader(L"DEFFERDLIGHTVTX");
	DEFFERDMAT->Set_PXShader(L"DEFFERDLIGHTPIX");
	DEFFERDMAT->Set_Blend(L"LIGHTONE");
	DEFFERDMAT->Insert_TexData(TEX_TYPE::TEX_TAGET, 0, L"POSTION");
	DEFFERDMAT->Insert_TexData(TEX_TYPE::TEX_TAGET, 1, L"NORMAL");
	DEFFERDMAT->Insert_TexData(TEX_TYPE::TEX_TAGET, 2, L"DEPTH");

	// MERGE
	KPtr<Shader_Vertex> DEFFERDMERGEVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"DEFFERDMERGEVTX", L"Shader", L"Defferd.fx", "VS_DEFFERDMERGE");
	DEFFERDMERGEVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	DEFFERDMERGEVTX->Add_LayoutFin("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	KPtr<Shader_Pixel> DEFFERDMERGEPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"DEFFERDMERGEPIX", L"Shader", L"Defferd.fx", "PS_DEFFERDMERGE");

	KPtr<KMaterial> DEFFERDMERGEMAT = ResourceManager<KMaterial>::Create(L"DEFFERDMERGEMAT");
	DEFFERDMERGEMAT->Set_VTShader(L"DEFFERDMERGEVTX");
	DEFFERDMERGEMAT->Set_PXShader(L"DEFFERDMERGEPIX");
	DEFFERDMERGEMAT->Set_Blend(L"ALPHA");
	DEFFERDMERGEMAT->Insert_TexData(TEX_TYPE::TEX_TAGET, 0, L"COLOR_DIFFUSE");
	DEFFERDMERGEMAT->Insert_TexData(TEX_TYPE::TEX_TAGET, 1, L"LIGHT_DIFFUSE");
	DEFFERDMERGEMAT->Insert_TexData(TEX_TYPE::TEX_TAGET, 2, L"LIGHT_SPECULAR");

	KPtr<Shader_Vertex> SCREENMERGEVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"SCREENMERGEVTX", L"Shader", L"ScreenMerge.fx", "VS_SCREENMERGE");
	SCREENMERGEVTX->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	SCREENMERGEVTX->Add_LayoutFin("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	KPtr<Shader_Pixel> SCREENMERGEPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"SCREENMERGEPIX", L"Shader", L"ScreenMerge.fx", "PS_SCREENMERGE");

	KPtr<KMaterial> SCREENMERGEMAT = ResourceManager<KMaterial>::Create(L"SCREENMERGEMAT");
	SCREENMERGEMAT->Set_VTShader(L"SCREENMERGEVTX");
	SCREENMERGEMAT->Set_PXShader(L"SCREENMERGEPIX");

	KPtr<Shader_Vertex> VOLUMEVTX = ResourceManager<Shader_Vertex>::Load_FromKey(L"VOLUMEVTX", L"Shader", L"VolumeMesh.fx", "VS_VOLUME");
	VOLUMEVTX->Add_LayoutFin("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	KPtr<Shader_Pixel> VOLUMEPIX = ResourceManager<Shader_Pixel>::Load_FromKey(L"VOLUMEPIX", L"Shader", L"VolumeMesh.fx", "PS_VOLUME");

	KPtr<KMaterial> VOLUMEMAT = ResourceManager<KMaterial>::Create(L"VOLUMEMAT");
	VOLUMEMAT->Set_Blend(L"VOLUME");
	VOLUMEMAT->Set_VTShader(L"VOLUMEVTX");
	VOLUMEMAT->Set_PXShader(L"VOLUMEPIX");

	return true;
}

bool KDevice::Init_BasicFigure3D() 
{

	DefRenderTaget();
	Def3DCreate();
	Mesh3DCreate();
	Mat3DCreate();

	return true;
}