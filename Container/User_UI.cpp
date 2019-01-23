#include "Controll_User.h"
#include "Force_Unit.h"
#include "Container.h"

#include <Core_Class.h>
#include <Renderer_UI.h>


void Controll_User::Init_UI()
{
	m_uCFire = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_uCFire->one()->Trans()->scale_local(KVector(50.0f, 50.0f, 10.0f, .0f));
	m_uCFire->one()->Trans()->pos_local(KVector(-165.0f, -250.0f, 1.1f, .0f));
	m_uCFire->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"TemCover.png");
	m_uCFire->cut_fade(1.0f);
	m_uCFire->cut_value(1.f);

	m_uCMedic = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_uCMedic->one()->Trans()->scale_local(KVector(50.0f, 50.0f, 10.0f, .0f));
	m_uCMedic->one()->Trans()->pos_local(KVector(-225.0f, -250.0f, 1.1f, .0f));
	m_uCMedic->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"TemCover.png");
	m_uCMedic->cut_fade(1.0f);
	m_uCMedic->cut_value(1.f);

	m_uCOpti = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_uCOpti->one()->Trans()->scale_local(KVector(70.0f, 70.0f, 10.0f, .0f));
	m_uCOpti->one()->Trans()->pos_local(KVector(-300.0f, -250.0f, 1.1f, .0f));
	m_uCOpti->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"TemCover.png");
	m_uCOpti->cut_fade(1.0f);
	m_uCOpti->cut_value(1.f);




	m_uFire = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_uFire->one()->Trans()->scale_local(KVector(50.0f, 50.0f, 10.0f, .0f));
	m_uFire->one()->Trans()->pos_local(KVector(-165.0f, -250.0f, 1.1f, .0f));
	m_uFire->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"btn-ability-tychus-tychus-shreddergrenade.dds");
	m_uFire->cut_fade(1.0f);

	m_uMedic = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_uMedic->one()->Trans()->scale_local(KVector(50.0f, 50.0f, 10.0f, .0f));
	m_uMedic->one()->Trans()->pos_local(KVector(-225.0f, -250.0f, 1.1f, .0f));
	m_uMedic->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"btn-ability-tychus-medivac.dds");
	m_uMedic->cut_fade(1.0f);

	m_uOpti = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_uOpti->one()->Trans()->scale_local(KVector(70.0f, 70.0f, 10.0f, .0f));
	m_uOpti->one()->Trans()->pos_local(KVector(-300.0f, -250.0f, 1.1f, .0f));
	m_uOpti->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"btn-ability-tychus-odin.dds");
	m_uOpti->cut_fade(1.0f);




	m_uFBackHP = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_uFBackHP->one()->Trans()->scale_local(KVector(300.0f, 50.0f, 10.0f, .0f));
	m_uFBackHP->one()->Trans()->pos_local(KVector(0.0f, 200.0f, 1.1f, .0f));
	m_uFBackHP->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"HPBack.png");
	m_uFBackHP->cut_fade(.0f);
	m_uFBackHP->cut_value(1.0f);

	m_uFocusHP = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_uFocusHP->one()->Trans()->scale_local(KVector(300.0f, 50.0f, 10.0f, .0f));
	m_uFocusHP->one()->Trans()->pos_local(KVector(0.0f, 200.0f, 1.1f, .0f));
	m_uFocusHP->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"FHP.png");
	m_uFocusHP->cut_fade(.0f);
	m_uFocusHP->cut_value(.3f);



	m_uBHP = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_uBHP->one()->Trans()->scale_local(KVector(100.0f, 20.0f, 10.0f, .0f));
	m_uBHP->one()->Trans()->pos_local(KVector(300.0f, -250.0f, 1.1f, .0f));
	m_uBHP->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"HPBack.png");
	m_uBHP->cut_fade(.0f);
	m_uBHP->cut_value(1.0f);

	m_uHp = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_uHp->one()->Trans()->scale_local(KVector(100.0f, 20.0f, 10.0f, .0f));
	m_uHp->one()->Trans()->pos_local(KVector(300.0f, -250.0f, 1.1f, .0f));
	m_uHp->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"HP.png");
	m_uHp->cut_fade(.0f);
	m_uHp->cut_value(1.0f);
}

void Controll_User::Update_UI()
{
	if (nullptr == m_pFocusUnit || PLAYABLE_TYPE::PBT_ENEMY != m_pFocusUnit->playable_type())
	{
		m_uFocusHP->one()->Active_Off();
		m_uFBackHP->one()->Active_Off();
		return;
	}

	m_uFocusHP->one()->Active_On();
	m_uFBackHP->one()->Active_On();
	m_uFocusHP->cut_value(m_pFocusUnit->hp() / m_pFocusUnit->maxhp());
}


void Controll_User::Update_AUI()
{
	m_uFire->cut_value(m_FiATime / m_FiTime);
	m_uMedic->cut_value(m_MeATime / m_MeTime);
	m_uOpti->cut_value(m_OpATime / m_OpTime);

	switch (m_OType)
	{
	case Controll_User::OT_ODIN:
		m_uOpti->material()->Set_Tex(0, L"btn-ability-tychus-odin.dds");
		break;
	case Controll_User::OT_NOVA:
		m_uOpti->material()->Set_Tex(0, L"btn-ability-terran-snipe-color.dds");
		break;
	case Controll_User::OT_HYPERION:
		m_uOpti->material()->Set_Tex(0, L"btn-ability-terran-warpjump.dds");
		break;
	default:
		break;
	}
}