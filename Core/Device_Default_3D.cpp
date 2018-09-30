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
#include "KLight.h"
#include "Renderer_Grid3D.h"


bool KDevice::Init_DefaultData_3D()
{
	ResourceManager<Blend>::Create(L"AlphaBlend3D");
	Core_Class::MainDevice().Create_DeviceCB<DATA_3D>(L"DATA3D", D3D11_USAGE_DYNAMIC, 1);
	Core_Class::MainDevice().Create_DeviceCB<KLight::LightCB>(L"LIGHT_DATA", D3D11_USAGE_DYNAMIC, 10);


	Init_RectMesh();
	Init_CubeMesh();
	Init_SphereMesh();
	

	Init_ColorMat();
	Init_GridMat();
	Init_ImageMat();
	Init_LightMat();

	Core_Class::MainDevice().Create_RasterMode(L"SOLID_NONE", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_NONE);
	Core_Class::MainDevice().Create_RasterMode(L"SBACK", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK);
	Core_Class::MainDevice().Create_RasterMode(L"SFRONT", D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_FRONT);
	Core_Class::MainDevice().Create_RasterMode(L"WNONE", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_NONE);
	Core_Class::MainDevice().Create_RasterMode(L"WBACK", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_BACK);
	Core_Class::MainDevice().Create_RasterMode(L"WFRONT", D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_FRONT);

	Core_Class::MainDevice().Create_RasterMode(L"SBACK");

	return true;
}



/********************* Mesh ********************/
void KDevice::Init_RectMesh()
{
	Vertex_3D Rect3D_Vert[4] = {};
	Index_16 Rect3D_Idx[2] = {};

	Rect3D_Vert[0].Pos = KVector4(-0.5f, 0.5f, 0.0f, 1.0f);
	Rect3D_Vert[1].Pos = KVector4(0.5f, 0.5f, 0.0f, 1.0f);
	Rect3D_Vert[2].Pos = KVector4(-0.5f, -0.5f, 0.0f, 1.0f);
	Rect3D_Vert[3].Pos = KVector4(0.5f, -0.5f, 0.0f, 1.0f);

	Rect3D_Vert[0].Uv = KVector2(0.0f, 0.0f);
	Rect3D_Vert[1].Uv = KVector2(1.0f, 0.0f);
	Rect3D_Vert[2].Uv = KVector2(0.0f, 1.0f);
	Rect3D_Vert[3].Uv = KVector2(1.0f, 1.0f);


	Rect3D_Vert[0].Color = KColor::White;
	Rect3D_Vert[1].Color = KColor::White;
	Rect3D_Vert[2].Color = KColor::White;
	Rect3D_Vert[3].Color = KColor::White;

	Rect3D_Idx[0] = Index_16(0, 3, 2);
	Rect3D_Idx[1] = Index_16(0, 1, 3);

	ResourceManager<Mesh>::Create(L"RECT3D_MESH",
		4, (UINT)sizeof(Vertex_3D), D3D11_USAGE_DYNAMIC, Rect3D_Vert,
		6, (UINT)Index_16::MemberSize(), D3D11_USAGE_DEFAULT, Rect3D_Idx,
		Index_16::FM());
}

void KDevice::Init_CubeMesh()
{
	Vertex_3D Cube3D_Vert[8] = {};
	Index_16 Cube3D_Idx[12] = {};

	Cube3D_Vert[0].Pos = KVector4(-0.5f, 0.5f, -0.5f, 1.0f);
	Cube3D_Vert[1].Pos = KVector4(0.5f, 0.5f, -0.5f, 1.0f);
	Cube3D_Vert[2].Pos = KVector4(-0.5f, -0.5f, -0.5f, 1.0f);
	Cube3D_Vert[3].Pos = KVector4(0.5f, -0.5f, -0.5f, 1.0f);
	Cube3D_Vert[4].Pos = KVector4(-0.5f, 0.5f, 0.5f, 1.0f);
	Cube3D_Vert[5].Pos = KVector4(0.5f, 0.5f, 0.5f, 1.0f);
	Cube3D_Vert[6].Pos = KVector4(-0.5f, -0.5f, 0.5f, 1.0f);
	Cube3D_Vert[7].Pos = KVector4(0.5f, -0.5f, 0.5f, 1.0f);

	Cube3D_Vert[0].Normal = KVector4(-0.5f, 0.5f, -0.5f, .0f);
	Cube3D_Vert[1].Normal = KVector4(0.5f, 0.5f, -0.5f, .0f);
	Cube3D_Vert[2].Normal = KVector4(-0.5f, -0.5f, -0.5f, .0f);
	Cube3D_Vert[3].Normal = KVector4(0.5f, -0.5f, -0.5f, .0f);
	Cube3D_Vert[4].Normal = KVector4(-0.5f, 0.5f, 0.5f, .0f);
	Cube3D_Vert[5].Normal = KVector4(0.5f, 0.5f, 0.5f, .0f);
	Cube3D_Vert[6].Normal = KVector4(-0.5f, -0.5f, 0.5f, .0f);
	Cube3D_Vert[7].Normal = KVector4(0.5f, -0.5f, 0.5f, .0f);

	for (size_t i = 0; i < 8; i++)
	{
		Cube3D_Vert[i].Normal.NormalizeVec3();
	}

	Cube3D_Vert[0].Uv = KVector2(0.0f, 0.0f);
	Cube3D_Vert[1].Uv = KVector2(1.0f, 0.0f);
	Cube3D_Vert[2].Uv = KVector2(0.0f, 1.0f);
	Cube3D_Vert[3].Uv = KVector2(1.0f, 1.0f);
	Cube3D_Vert[4].Uv = KVector2(0.0f, 0.0f);
	Cube3D_Vert[5].Uv = KVector2(1.0f, 0.0f);
	Cube3D_Vert[6].Uv = KVector2(1.0f, 1.0f);
	Cube3D_Vert[7].Uv = KVector2(0.0f, 1.0f);

	Cube3D_Vert[0].Color = KColor::White;
	Cube3D_Vert[1].Color = KColor::White;
	Cube3D_Vert[2].Color = KColor::White;
	Cube3D_Vert[3].Color = KColor::White;
	Cube3D_Vert[4].Color = KColor::White;
	Cube3D_Vert[5].Color = KColor::White;
	Cube3D_Vert[6].Color = KColor::White;
	Cube3D_Vert[7].Color = KColor::White;



	Cube3D_Idx[0] = Index_16(0, 3, 2);
	Cube3D_Idx[1] = Index_16(0, 1, 3);
	Cube3D_Idx[2] = Index_16(0, 2, 6);
	Cube3D_Idx[3] = Index_16(0, 6, 4);
	Cube3D_Idx[4] = Index_16(4, 1, 0);
	Cube3D_Idx[5] = Index_16(4, 5, 1);
	Cube3D_Idx[6] = Index_16(4, 6, 5);
	Cube3D_Idx[7] = Index_16(5, 6, 7);
	Cube3D_Idx[8] = Index_16(1, 7, 3);
	Cube3D_Idx[9] = Index_16(1, 5, 7);
	Cube3D_Idx[10] = Index_16(7, 6, 3);
	Cube3D_Idx[11] = Index_16(2, 3, 6);


	ResourceManager<Mesh>::Create(L"CUBE3D_MESH",
		8, (UINT)sizeof(Vertex_3D), D3D11_USAGE_DYNAMIC, Cube3D_Vert,
		36, (UINT)Index_16::MemberSize(), D3D11_USAGE_DEFAULT, Cube3D_Idx,
		Index_16::FM()/*, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP*/);
}

void KDevice::Init_SphereMesh()
{

	std::vector<Vertex_3D> SphereVtx;
	std::vector<UINT> SphereIdx;

	float fRadius = 0.5f;

	Vertex_3D V;
	// �ϱ���
	V.Pos = KVector{ 0.0f, fRadius, 0.0f, 1.0f };
	V.Uv = KVector2{ 0.5f, 0.0f };
	V.Color = KColor::White;
	V.Normal = V.Pos;
	V.Normal.NormalizeVec3();

	SphereVtx.push_back(V);

	UINT iStackCount = 40; // ���� ���� ����
	UINT iSliceCount = 40; // ���� ���� ����

	float yRotAngle = KPI / (float)iStackCount;
	float zRotAngle = KPI * 2.0f / (float)iSliceCount;

	float yUvRatio = 1.0f / (float)iSliceCount;
	float zUvRatio = 1.0f / (float)iStackCount;

	// PointToDir

	// ļ
	// ���� ȸ�����Ѽ� ���� ����� ���� stack���� ȸ������ ���� �ϴµ���

	for (size_t y = 1; y < iStackCount; ++y)
	{
		for (size_t z = 0; z < iSliceCount + 1; ++z)
		{
			V.Pos = KVector
			{
				fRadius * sinf(y * yRotAngle) * cosf(z * zRotAngle),
				fRadius * cosf(y * yRotAngle),
				fRadius * sinf(y * yRotAngle) * sinf(z * zRotAngle),
				1.0f
			};
			V.Pos.ElipseCut();

			V.Pos.w = 1.0f;
			V.Uv = KVector2{ yUvRatio * z, zUvRatio * y };
			V.Color = KColor::White;
			V.Normal = V.Pos;
			V.Normal.NormalizeVec3();

			SphereVtx.push_back(V);
		}
	}

	// �ϱ���
	V.Pos = KVector{ 0.0f, -fRadius, 0.0f, 1.0f };
	V.Uv = KVector2{ 0.5f, 1.0f };
	V.Color = KColor::White;
	V.Normal = V.Pos;
	V.Normal.NormalizeVec3();

	SphereVtx.push_back(V);

	SphereIdx.clear();

	// �ϱ��� �ε���
	for (UINT i = 0; i < iSliceCount; i++)
	{
		SphereIdx.push_back(0);
		SphereIdx.push_back(i + 2);
		SphereIdx.push_back(i + 1);
	}


	for (UINT y = 0; y < iStackCount - 2; y++)
	{
		for (UINT z = 0; z < iSliceCount; ++z)
		{
			// *
			// **

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

	ResourceManager<Mesh>::Create(L"SPHERE_MESH"
		, (UINT)SphereVtx.size(), (UINT)Vertex_3D::TypeSize(), D3D11_USAGE_DYNAMIC, &SphereVtx[0]
		, (UINT)SphereIdx.size(), (UINT)Index_32::MemberSize(), D3D11_USAGE_DEFAULT, &SphereIdx[0]
		, Index_32::FM());
}




/********************* Material ********************/
void KDevice::Init_GridMat()
{
	KPtr<Vertex_Shader> NewVert =
		ResourceManager<Vertex_Shader>::Load_FromKey(L"GRID3D_VERT", L"Shader", L"Grid3DShader.fx", "Grid3D_VT");

	NewVert->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	NewVert->Add_Layout("UV", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	NewVert->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	NewVert->Add_LayoutFin("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

	KPtr<Pixel_Shader> NewPix =
		ResourceManager<Pixel_Shader>::Load_FromKey(L"GRID3D_PIX", L"Shader", L"Grid3DShader.fx", "Grid3D_PX");

	NewPix->Create_ConstBuf<Renderer_Grid3D::GRID_DATA>(L"GRIDDATA", D3D11_USAGE_DYNAMIC, 2);

	KPtr<Material> NewMat = ResourceManager<Material>::Create(L"GRID3D_MAT");
	NewMat->Set_VShader(L"GRID3D_VERT");
	NewMat->Set_PShader(L"GRID3D_PIX");
	NewMat->Set_Blend(L"AlphaBlend3D");
}



void KDevice::Init_ColorMat()
{
	KPtr<Vertex_Shader> NewVert =
		ResourceManager<Vertex_Shader>::Load_FromKey(L"COLOR_VERT", L"Shader", L"Rect3DShader.fx", "Rect3D_VT");

	NewVert->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	NewVert->Add_Layout("UV", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	NewVert->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	NewVert->Add_LayoutFin("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

	KPtr<Pixel_Shader> NewPix =
		ResourceManager<Pixel_Shader>::Load_FromKey(L"COLOR_PIX", L"Shader", L"Rect3DShader.fx", "Rect3D_PX");


	KPtr<Material> NewMat = ResourceManager<Material>::Create(L"COLOR_MAT");
	NewMat->Set_VShader(L"COLOR_VERT");
	NewMat->Set_PShader(L"COLOR_PIX");
	NewMat->Set_Blend(L"AlphaBlend3D");
}

void KDevice::Init_ImageMat()
{
	KPtr<Vertex_Shader> NewVert =
		ResourceManager<Vertex_Shader>::Load_FromKey(L"IMG_VERT", L"Shader", L"ImageShader.fx", "Img_VT");

	NewVert->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	NewVert->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	NewVert->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	NewVert->Add_LayoutFin("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	
	KPtr<Pixel_Shader> NewPix =
		ResourceManager<Pixel_Shader>::Load_FromKey(L"IMG_PIX", L"Shader", L"ImageShader.fx", "Img_PX");

	KPtr<Material> NewMat = ResourceManager<Material>::Create(L"IMG_MAT");
	NewMat->Set_VShader(L"IMG_VERT");
	NewMat->Set_PShader(L"IMG_PIX");
	NewMat->Set_Blend(L"AlphaBlend3D");
}

void KDevice::Init_LightMat()
{
	KPtr<Vertex_Shader> NewVVert = ResourceManager<Vertex_Shader>::Load_FromKey(
		L"VLIGHT_VERT", L"Shader", L"VLight.fx", "VS_MESH3D");
	NewVVert->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	NewVVert->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	NewVVert->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	NewVVert->Add_LayoutFin("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	
	KPtr<Pixel_Shader> NewVPix = ResourceManager<Pixel_Shader>::Load_FromKey(
		L"VLIGHT_PIX", L"Shader", L"VLight.fx", "PS_MESH3D");


	KPtr<Material> NewVMat = ResourceManager<Material>::Create(L"VLIGHT_MAT");
	NewVMat->Set_VShader(L"VLIGHT_VERT");
	NewVMat->Set_PShader(L"VLIGHT_PIX");
	NewVMat->Set_Blend(L"AlphaBlend3D");



	KPtr<Vertex_Shader> NewPVert = ResourceManager<Vertex_Shader>::Load_FromKey(
		L"PLIGHT_VERT", L"Shader", L"PLight.fx", "VS_MESH3D");
	NewPVert->Add_Layout("POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	NewPVert->Add_Layout("TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0);
	NewPVert->Add_Layout("COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	NewPVert->Add_LayoutFin("NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

	KPtr<Pixel_Shader> NewPPix = ResourceManager<Pixel_Shader>::Load_FromKey(
		L"PLIGHT_PIX", L"Shader", L"PLight.fx", "PS_MESH3D");

	KPtr<Material> NewPMat = ResourceManager<Material>::Create(L"PLIGHT_MAT");
	NewPMat->Set_VShader(L"PLIGHT_VERT");
	NewPMat->Set_PShader(L"PLIGHT_PIX");
	NewPMat->Set_Blend(L"AlphaBlend3D");
}