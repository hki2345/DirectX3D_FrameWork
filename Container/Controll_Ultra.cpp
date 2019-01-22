#include "Controll_Ultra.h"
#include "Force_Unit.h"
#include "SC2_Force.h"

#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>

#include <InputManager.h>
#include <TimeManager.h>



#include <SoundPlayer.h>



Controll_Ultra::Controll_Ultra()
{
}
Controll_Ultra::~Controll_Ultra()
{
}

void Controll_Ultra::Update_MIDLE()
{
	Controll_AI::Update_MIDLE();
}
void Controll_Ultra::Update_MOVE()
{
	Controll_AI::Update_MOVE();
}



void Controll_Ultra::Update_AIDLE()
{
	Controll_AI::Update_AIDLE();
}


void Controll_Ultra::Update_BURROW()
{
	m_AType = Controll_AI::AT_IDLE;
}
void Controll_Ultra::Update_HIDE()
{
	m_AType = Controll_AI::AT_IDLE;
}
void Controll_Ultra::Update_UNBURROW()
{
	m_AType = Controll_AI::AT_IDLE;
}


void Controll_Ultra::Update_ATTACK01()
{
	Controll_AI::Update_ATTACK01();
}
void Controll_Ultra::Update_ATTACK02()
{
	m_RenderRot.y = m_AIRot.y + KPI;
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK02);
}
void Controll_Ultra::Update_ATTACK03()
{
	m_RenderRot.y = m_AIRot.y + KPI;
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK03);
}
void Controll_Ultra::Update_DEATH()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::DEATH);

	if (true == m_pUnit->Check_AniDone())
	{
		m_pUnit->force()->Delete_Unit(m_pUnit);
		one()->Set_Death();
	}
}