#include "Controll_Nova.h"
#include "Force_Unit.h"
#include "SC2_Force.h"
#include "Con_Class.h"

#include <ResourceManager.h>
#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>
#include <InputManager.h>


Controll_Nova::Controll_Nova()
{
}


Controll_Nova::~Controll_Nova()
{
}



void Controll_Nova::Update_MIDLE()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::STAND02);
	m_UTime += DELTATIME;

	if (true == m_pUnit->Check_AniDone())
	{
		m_pUnit->force()->Delete_Unit(m_pUnit);
		one()->Set_Death();
	}
}
void Controll_Nova::Update_ATTACK()
{
	m_pUnit->Set_Animation(Force_Unit::ANI_TYPE::ATTACK02);
	m_UTime += DELTATIME;


	if (m_UTime > .5f)
	{
		m_UTime = .0f;

		std::list<KPtr<Force_Unit>>::iterator S = m_pEnemyList.begin();

		(*S)->Damage(1000.0f);
		m_pEnemyList.erase(S);
	}

	if (0 == m_pEnemyList.size())
	{
		m_MType = MOVE_TYPE::MT_IDLE;
	}
}


bool Controll_Nova::Init(const KVector& _InitPos, const KVector& _RotPos, KPtr<Renderer_Terrain> _Ter)
{
	m_pUnit = Con_Class::s2_manager()->Find_Force(L"BAAM")->Create_Unit(L"NOVA", _Ter, state());

	m_TPos = _InitPos + KVector(1.0f, .0f, .0f);
	m_pUnit->one()->Trans()->pos_local(m_TPos);


	(*m_pUnit->list_renderer().begin())->rot_pivot(_RotPos);
	m_MType = MOVE_TYPE::MT_ATTACK;



	int Limit = 15;
	int Cnt = 0;

	std::list<KPtr<Force_Unit>> TList = *Con_Class::force_enemy()->unit_list();
	std::list<KPtr<Force_Unit>>::iterator S = TList.begin();
	std::list<KPtr<Force_Unit>>::iterator E = TList.end();

	int THp = 300;


	for (size_t i = 0; i < 4; i++)
	{
		std::list<KPtr<Force_Unit>>::iterator S = TList.begin();
		std::list<KPtr<Force_Unit>>::iterator E = TList.end();

		for (; S != E; ++S)
		{
			if (THp < (*S)->hp() && m_pEnemyList.size() <= Limit)
			{
				m_pEnemyList.push_back((*S));
			}
			else
			{
				m_UTime = .0f;
				return true;
			}
		}

		THp -= 100;
	}

	m_UTime = .0f;
	return true;
}


void Controll_Nova::Update()
{
	switch (m_MType)
	{
	case Controll_Nova::MT_IDLE:
		Update_MIDLE();
		break;
	case Controll_Nova::MT_ATTACK:
		Update_ATTACK();
		break;
	default:
		break;
	}
}