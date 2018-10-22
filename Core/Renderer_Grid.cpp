#include "Renderer_Grid.h"
#include "KMacro.h"
#include "KWindow.h"
#include "TransPosition.h"
#include "DebugManager.h"


Renderer_Grid::Renderer_Grid() : m_BlockSizeInter(10.0f)
{
	ROpt.Defferd_orForward = 0;
}


Renderer_Grid::~Renderer_Grid()
{
}

void Renderer_Grid::Set_Data(KPtr<Camera> _Camera) 
{
	float Y = fabsf(_Camera->Trans()->pos_world().y);
	float CheckUper = m_BlockSizeInter;
	float MaxSize = m_BlockSizeInter;

	// ������ ��ȯ
	while (MaxSize <= Y)	{
		MaxSize *= m_BlockSizeInter;
	}

	MaxSize *= 0.1f;

	m_GD.x = MaxSize;

	// 0 ~ m_Size ũ�� ����
	if (m_BlockSizeInter >= m_GD.x)
	{
		MaxSize = m_BlockSizeInter;
		m_GD.x = m_BlockSizeInter;
	}

	CheckUper = MaxSize * m_BlockSizeInter;

	m_GD.y = Y * 0.01f;

	m_GD.z = fabsf((Y - CheckUper) / (CheckUper - MaxSize));


	DRAW_LOG(L"Alpha : %f", m_GD.z);
	
	// ���Ĵ� ������
	// m_GD.m_GD.z = 1.0f - (CamY / (MaxSize * m_Size));
}

void Renderer_Grid::Update_Trans(KPtr<Camera> _Camera)
{
	KASSERT(nullptr == m_Trans);
	if (nullptr == m_Trans)
	{
		return;
	}

	Set_Data(_Camera);

	// �θ����� ������� ���Ǿ�� �Ѵ�.
	SubTransUpdate();

	m_MD.m_W = m_Trans->worldmat_const();

	m_MD.m_W.v4.x = state()->Camera()->Trans()->pos_world().x;
	m_MD.m_W.v4.y = state()->Camera()->Trans()->pos_world().y;

	m_MD.m_V = _Camera->View();
	m_MD.m_P = _Camera->Proj();
	m_MD.m_WVP = (CSWMat() * _Camera->ViewProj()).RTranspose();

	Renderer::Update_TransCB();
}

void Renderer_Grid::Render(KPtr<Camera> _Camera)
{
	m_Mat->PShader()->SettingCB<KVector4>(L"GRIDDATA", m_GD);
}

bool Renderer_Grid::Init(int _Order /*= 0*/)
{
	Renderer::Init(_Order);

	if (false == SetMat(L"GRID3DMAT"))
	{
		return false;
	}
	if (false == SetMesh(L"RECT"))
	{
		return false;
	}

	if (nullptr == Window()->Device().FindCB(L"GRIDDATA"))
	{
		Window()->Device().CreateCB<KVector4>(L"GRIDDATA", D3D11_USAGE_DYNAMIC, 0);
	}

	SetRSState(L"SNONE");

	return true;
}


void Renderer_Grid::DebugRender() 
{


}