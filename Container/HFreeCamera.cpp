#include "HFreeCamera.h"
#include <InputManager.h>
#include <TimeManager.h>
#include <DebugManager.h>


HFreeCamera::HFreeCamera() : m_Speed(10.0f)
{
}


HFreeCamera::~HFreeCamera()
{
}

bool HFreeCamera::Init() 
{
	m_Camera = Get_Component<Camera>();
	m_Camera->ProjectionMode(Camera::PROJ_MODE::PM_PERS);

#pragma region KEYCREATE


	if (false == InputManager::IsKey(L"FREELEFT"))
	{
		InputManager::Create_Command(L"FREELEFT", 'A');
	}

	if (false == InputManager::IsKey(L"FREERIGHT"))
	{
		InputManager::Create_Command(L"FREERIGHT", 'D');
	}

	if (false == InputManager::IsKey(L"FREEUP"))
	{
		InputManager::Create_Command(L"FREEUP", 'Q');
	}

	if (false == InputManager::IsKey(L"FREEDOWN"))
	{
		InputManager::Create_Command(L"FREEDOWN", 'E');
	}

	if (false == InputManager::IsKey(L"FREEFORWARD"))
	{
		InputManager::Create_Command(L"FREEFORWARD", 'W');
	}

	if (false == InputManager::IsKey(L"FREEBACK"))
	{
		InputManager::Create_Command(L"FREEBACK", 'S');
	}

	if (false == InputManager::IsKey(L"ROTLOCK"))
	{
		InputManager::Create_Command(L"ROTLOCK", VK_RBUTTON);
	}

	if (false == InputManager::IsKey(L"Boost"))
	{
		InputManager::Create_Command(L"Boost", VK_SHIFT);
	}

	if (false == InputManager::IsKey(L"MODECHANAGE"))
	{
		InputManager::Create_Command(L"MODECHANAGE", 'R');
	}

	if (false == InputManager::IsKey(L"Z"))
	{
		InputManager::Create_Command(L"Z", 'Z');
	}

	if (false == InputManager::IsKey(L"X"))
	{
		InputManager::Create_Command(L"X", 'X');
	}

	if (false == InputManager::IsKey(L"C"))
	{
		InputManager::Create_Command(L"C", 'C');
	}

	if (false == InputManager::IsKey(L"F"))
	{
		InputManager::Create_Command(L"F", 'F');
	}

#pragma endregion
	return true;
}
void HFreeCamera::Update() 
{


	if (true == InputManager::Press(L"Boost"))
	{
		m_RotSpeed = 360.0f;
		m_Speed = 100.0f;
	}
	else {
		m_RotSpeed = 90.0f;
		m_Speed = 10.0f;
	}

	if (true == InputManager::Down(L"MODECHANAGE"))
	{
		m_Camera->Change_Mode();
	}


	if (true == InputManager::Down(L"Z"))
	{
		KVector4 Rot = m_Trans->rotate_local();
		Rot.z = 0.0f;
		m_Trans->rotate_local(Rot);
	}

	if (true == InputManager::Down(L"Z"))
	{
		KVector4 Rot = m_Trans->rotate_local();
		Rot.z = 0.0f;
		m_Trans->rotate_local(Rot);
	}

	if (true == InputManager::Down(L"X"))
	{
		KVector4 Rot = m_Trans->rotate_local();
		Rot.x = 0.0f;
		m_Trans->rotate_local(Rot);
	}

	if (true == InputManager::Down(L"F"))
	{
		m_Trans->Reset();
		m_Trans->Moving(KVector4(0.0f, 0.0f, -10.0f));
	}

	if (true == InputManager::Press(L"FREELEFT"))
	{
		m_Trans->Moving(m_Trans->left_local() * TimeManager::DeltaTime() * m_Speed);
	}

	if (true == InputManager::Press(L"FREERIGHT"))
	{
		m_Trans->Moving(m_Trans->right_local() * TimeManager::DeltaTime() * m_Speed);
	}

	if (true == InputManager::Press(L"FREEUP"))
	{
		m_Trans->Moving(m_Trans->up_local() * TimeManager::DeltaTime() * m_Speed);
	}

	if (true == InputManager::Press(L"FREEDOWN"))
	{
		m_Trans->Moving(m_Trans->down_local() * TimeManager::DeltaTime() * m_Speed);
	}

	if (true == InputManager::Press(L"FREEFORWARD"))
	{
		m_Trans->Moving(m_Trans->forward_local() * TimeManager::DeltaTime() * m_Speed);
	}

	if (true == InputManager::Press(L"FREEBACK"))
	{
		m_Trans->Moving(m_Trans->back_local() * TimeManager::DeltaTime() * m_Speed);
	}

	if (true == InputManager::Press(L"ROTLOCK"))
	{
		m_Trans->Rotating_Deg(KVector4(InputManager::MouseDir().y * m_RotSpeed * TimeManager::DeltaTime(), InputManager::MouseDir().x * m_RotSpeed * TimeManager::DeltaTime()));
	}


	DRAW_LOG(L"CameraPos : %f, %f, %f", Trans()->pos_local().x, Trans()->pos_local().y, Trans()->pos_local().z);
}
void HFreeCamera::DebugRender() 
{
	//wchar_t Arr[256];

	//swprintf_s(Arr, L"RotScale : %f, %f", RotPos.x, RotPos.y);
	//DebugManager::Draw_Font(Arr, { 10.0f, 20.0f }, 20.0f);

	//HVEC2 CurMouse = InputManager::MousePos();

	//swprintf_s(Arr, L"MousePos : %f, %f", CurMouse.x, CurMouse.y);
	//DebugManager::Draw_Font(Arr, { 10.0f, 40.0f }, 20.0f);

	//swprintf_s(Arr, L"CameraRot : %f, %f", m_Trans->rotate_local().x, m_Trans->rotate_local().y);
	//DebugManager::Draw_Font(Arr, { 10.0f, 60.0f }, 20.0f);

	//swprintf_s(Arr, L"CameraF : %f, %f, %f", m_Trans->forward_local().x, m_Trans->forward_local().y, m_Trans->forward_local().z);
	//DebugManager::Draw_Font(Arr, { 10.0f, 80.0f }, 20.0f);

	//swprintf_s(Arr, L"CameraU : %f, %f, %f", m_Trans->up_local().x, m_Trans->up_local().y, m_Trans->up_local().z);
	//DebugManager::Draw_Font(Arr, { 10.0f, 100.0f }, 20.0f);

	//swprintf_s(Arr, L"CameraPos : %f, %f, %f", m_Trans->LPos().x, m_Trans->LPos().y, m_Trans->LPos().z);
	//DebugManager::Draw_Font(Arr, { 10.0f, 120.0f }, 20.0f);
}