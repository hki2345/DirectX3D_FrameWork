#include "Controll_Abrr.h"
#include "Force_Unit.h"
#include "SC2_Force.h"

#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>

#include <InputManager.h>
#include <TimeManager.h>


Controll_Abrr::Controll_Abrr()
{
}
Controll_Abrr::~Controll_Abrr()
{
}

void Controll_Abrr::Update_MIDLE()
{
	Controll_AI::Update_MIDLE();
}
void Controll_Abrr::Update_MOVE()
{
	Controll_AI::Update_MOVE();
}



void Controll_Abrr::Update_AIDLE()
{
	Controll_AI::Update_AIDLE();
}


void Controll_Abrr::Update_BURROW()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::FIDGET02);
}
void Controll_Abrr::Update_HIDE()
{
	Controll_AI::Update_HIDE();
}
void Controll_Abrr::Update_UNBURROW()
{
	Controll_AI::Update_UNBURROW();
}


void Controll_Abrr::Update_ATTACK01()
{
	Controll_AI::Update_ATTACK01();
}
void Controll_Abrr::Update_ATTACK02()
{
	m_RenderRot.y = m_AIRot.y + KPI;
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK02);
}
void Controll_Abrr::Update_ATTACK03()
{
	m_RenderRot.y = m_AIRot.y + KPI;
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK03);
}
void Controll_Abrr::Update_DEATH()
{
	m_pUnit->Reset_Renderer();
	if (nullptr == DeathRender)
	{
		DeathRender = one()->Add_Component<Renderer_BonAni>();
		DeathRender->Set_Fbx(L"Aberration_Death.KM3");
		DeathRender->Create_Animation(false);
		DeathRender->Set_Clip(0);
	}

	if (true == DeathRender->Check_AniDone())
	{
		m_pUnit->force()->Delete_Unit(m_pUnit);
		one()->Set_Death();
	}
}