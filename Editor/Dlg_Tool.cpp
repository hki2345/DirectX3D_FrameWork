// Dlg_Tool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Editor.h"
#include "Dlg_Tool.h"
#include "afxdialogex.h"

#include <Renderer_Grid3D.h>
#include <Renderer_Rect3D.h>
#include <Renderer_Mesh.h>
#include <FreeCam.h>

#include <ResourceManager.h>
#include <KImage.h>
#include <KLight.h>




#include "View_Component.h"
#include "Edit_Class.h"

// Dlg_Tool 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_Tool, TabState)

Dlg_Tool::Dlg_Tool(CWnd* pParent /*=NULL*/)
	: TabState(IDD_DLG_TOOL, pParent)
{

}

Dlg_Tool::~Dlg_Tool()
{
}

void Dlg_Tool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Dlg_Tool, CDialogEx)
END_MESSAGE_MAP()


BOOL Dlg_Tool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	KPtr<State> CurState = Core_Class::Main_StateManager().find_state(StateName.GetString());

	if (nullptr == CurState)
	{
		KASSERT(true);
	}

	CurState->camera()->Add_Component<FreeCam>();
	CurState->camera()->Far(100000.0f);
	CurState->camera()->one()->Local_Pos(.0f, .0f, -40.0f);
	CurState->camera()->one()->Local_Rotate(90.0f, .0f, 0.0f);

	KPtr<TheOne> UICam = CurState->Create_TheOne(L"UI_Cam");
	UICam->Local_Pos(.0f, .0f, -40.0f);

	KPtr<Camera> UICom = UICam->Add_Component<Camera>();
	UICom->Insert_LayerData(6);
	UICom->Mode(Camera::PROJ_MODE::PM_ORTHGRAPHICS);

	// KPtr<TheOne> CUI = CurState->Create_TheOne(L"UI");
	// CUI->Local_Pos(350.0f, 100.0f, 0.0f);
	// CUI->Local_Rotate(0.0f, 0.0f, .0f);
	// CUI->Local_Scale(100.0f, 50.0f, 1.0f);
	// KPtr<Renderer_Rect3D> CUIR = CUI->Add_Component<Renderer_Rect3D>(6);
	// CUIR->material()->insert_TD(Texture_Type::TEX_COLOR, 0, L"다운로드.png");
	// CUIR->Create_RasterState(L"SBACK");
	// CUIR->billboard(true);




	KPtr<TheOne> NewLight = CurState->Create_TheOne(L"Light");
	NewLight->World_Pos(.0f, 0.0f, .0f);
	NewLight->World_Rotate(.0f, 0.0f, .0f);
	
	KPtr<KLight> NewPLight = NewLight->Add_Component<KLight>();
	NewPLight->Insert_Layer(0, 1, 2, 3, 4, 5);
	NewPLight->type(KLight::LightType::Point);
	NewPLight->color(KColor::White);
	NewPLight->range(100.0f);



	// KPtr<TheOne> NewLight2 = CurState->Create_TheOne(L"Light");
	// NewLight2->World_Pos(KVector::Zero);
	// NewLight2->World_Rotate(.0f, 30.0f, .0f);
	// 
	// KPtr<KLight> NewPLight2 = NewLight2->Add_Component<KLight>();
	// NewPLight2->Insert_Layer(0, 1, 2, 3, 4, 5);
	// NewPLight2->color(KColor::White);
	// NewPLight2->type(KLight::LightType::Direction);
	
	


	CCreateContext         m_context;

	m_context.m_pNewViewClass = RUNTIME_CLASS(View_Component);
	View_Component* pView = (View_Component*)((CFrameWnd*)this)->CreateView(&m_context);

	RECT ViewSize = { 200, 10, 700, 530 };
	pView->ShowWindow(SW_SHOW);
	pView->MoveWindow(&ViewSize);

	Edit_Class::m_View_Component->Dlg_Release();
	Edit_Class::m_View_Component->set_one(NewLight);


	KPtr<TheOne> NewSky = CurState->Create_TheOne(L"Sky");
	NewSky->World_Rotate(KVector(0.0f, 0.0f, 0.0f));
	NewSky->World_Scale(KVector(10000.0f, 10000.0f, 10000.0f));

	KPtr<Renderer_Mesh> SkyRender = NewSky->Add_Component<Renderer_Mesh>();
	SkyRender->Set_Material(L"IMG_MAT");
	SkyRender->Set_Mesh(L"SPHERE_MESH");
	SkyRender->Create_RasterState(L"SFRONT");
	SkyRender->material()->insert_TD(Texture_Type::TEX_COLOR, 0, L"Sky044.png");
	
	KPtr<TheOne> Grid = CurState->Create_TheOne(L"Grid");
	Grid->World_Rotate(KVector(90.0f, 0.0f, 0.0f));
	Grid->World_Scale(KVector(100000.0f, 100000.0f, 100000.0f));
	KPtr<Renderer_Grid3D> GridRender = Grid->Add_Component<Renderer_Grid3D>();
	
	
	
	KPtr<TheOne> LS = CurState->Create_TheOne(L"Sphere");
	LS->Local_Pos(-10.0f, 0.0f, 0.0f);
	LS->Local_Scale(10.0f, 10.0f, 10.0f);
	KPtr<Renderer_Mesh> LSR = LS->Add_Component<Renderer_Mesh>();
	LSR->Set_Material(L"MESH_MAT");
	LSR->Set_Mesh(L"SPHERE_MESH");
	LSR->billboard(true);
	LSR->material()->insert_TD(Texture_Type::TEX_COLOR, 0, L"MoonDiff.jpg");
	LSR->material()->insert_TD(Texture_Type::TEX_BUMP, 7, L"MoonBump.jpg");
	
	KPtr<TheOne> RS = CurState->Create_TheOne(L"Sphere");
	RS->Local_Pos(10.0f, 0.0f, 0.0f);
	RS->Local_Scale(10.0f, 10.0f, 10.0f);
	KPtr<Renderer_Mesh> RSR = RS->Add_Component<Renderer_Mesh>();
	RSR->Set_Material(L"MESH_MAT");
	RSR->Set_Mesh(L"SPHERE_MESH");
	RSR->material()->insert_TD(Texture_Type::TEX_COLOR, 0, L"MoonDiff.jpg");
	RSR->material()->insert_TD(Texture_Type::TEX_BUMP, 1, L"MoonBump.jpg");
	
	KPtr<TheOne> TUI = CurState->Create_TheOne(L"UI");
	TUI->Local_Pos(10.0f, 10.0f, 0.0f);
	TUI->Local_Rotate(0.0f, 0.0f, 90.0f);
	TUI->Local_Scale(10.0f, 5.0f, 10.0f);
	KPtr<Renderer_Rect3D> TUIR = TUI->Add_Component<Renderer_Rect3D>();
	TUIR->material()->insert_TD(Texture_Type::TEX_COLOR, 0, L"Health-armor-shields.png");
	TUIR->Create_RasterState(L"SBACK");
	TUIR->billboard(true);

	KPtr<TheOne> MC = CurState->Create_TheOne(L"Cube");
	MC->Local_Pos(0.0f, 20.0f, 0.0f);
	MC->Local_Scale(10.0f, 10.0f, 10.0f);
	KPtr<Renderer_Mesh> MCR = MC->Add_Component<Renderer_Mesh>();
	MCR->Set_Material(L"MESH_MAT");
	MCR->Set_Mesh(L"CUBE_MESH");
	// MCR->image(L"Sky01.png");

	KPtr<TheOne> NewOne2 = CurState->Create_TheOne(L"Cube");
	NewOne2->Local_Pos(0.0f, 40.0f, 0.0f);
	NewOne2->Local_Scale(10.0f, 10.0f, 10.0f);
	NewOne2->Add_Component<Renderer_Mesh>();


	return TRUE;

}