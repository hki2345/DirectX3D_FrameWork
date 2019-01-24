#include "SUpdater.h"
#include <Renderer_Mesh.h>
#include <Renderer_UI.h>
#include <SoundPlayer.h>

#include <Core_Class.h>

#include <InputManager.h>
#include <ResourceManager.h>
#include <KFont.h>



SUpdater::SUpdater()
{
}


SUpdater::~SUpdater()
{
}


void SUpdater::Start_State()
{
	Init_Mesh();
	Init_Sound(); 
	Init_UI();
}


void SUpdater::Init_Mesh()
{
	if (MSphere != nullptr)
	{
		return;
	}

	MSphere = state()->Create_One(L"BackGround")->Add_Component<Renderer_Mesh>();
	MSphere->one()->Trans()->scale_local(KVector4(100.0f, 100.0f, 100.0f));
	MSphere->one()->Trans()->rotate_local(KVector4(10.0f, 10.0f, 10.0f));
	MSphere->one()->Trans()->pos_local(KVector4(150.0f, 0.0f, 100.0f));
	MSphere->Set_Material(L"DEFFERD3DMAT");
	MSphere->Set_RSState(L"SNONE");
	MSphere->ROpt.Defferd_orForward = 1;
	MSphere->Set_Mesh(L"SPHERE");

	MSphere->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"skyboxchar_planetdiffuse.dds");
	MSphere->material()->Insert_TexData(TEX_TYPE::TEX_BUMP, 1, L"skyboxchar_planetnormal.dds");
	MSphere->material()->Insert_TexData(TEX_TYPE::TEX_SPEC, 2, L"skyboxchar_planetspecular.dds");
	MSphere->material()->Insert_TexData(TEX_TYPE::TEX_EMIS, 3, L"skyboxchar_planetemissive.dds");
}

void SUpdater::Init_Sound()
{
	if (0 == m_SList.size())
	{
		KPtr<SoundPlayer> NS = new SoundPlayer();
		NS->Set_Sound(L"MainTitleSC1-01.ogg");
		m_SList.push_back(NS);

		KPtr<SoundPlayer> NS2 = new SoundPlayer();
		NS2->Set_Sound(L"nDLC01_Music_Cue03.ogg");
		m_SList.push_back(NS2);

		KPtr<SoundPlayer> NS3 = new SoundPlayer();
		NS3->Set_Sound(L"BattleNet_MusicwithEndFade.ogg");
		m_SList.push_back(NS3);

		m_SSI = m_SList.begin();
	}

	if (m_SSI == m_SList.end())
	{
		m_SSI = m_SList.begin();
	}

	(*m_SSI)->Play();
}

void SUpdater::Init_UI()
{
	m_Start = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_Start->one()->Trans()->scale_local(KVector(300.0f, 70.0f, 10.0f, .0f));
	m_Start->one()->Trans()->pos_local(KVector(0, -20.0f, 1.1f, .0f));
	m_Start->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"StartBtn.png");
	m_Start->cut_fade(1.0f);
	m_Start->cut_value(1.f);

	m_Quit = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_Quit->one()->Trans()->scale_local(KVector(300.0f, 70.0f, 10.0f, .0f));
	m_Quit->one()->Trans()->pos_local(KVector(0, -100.0f, 1.1f, .0f));
	m_Quit->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"StartBtn.png");
	m_Quit->cut_fade(1.0f);
	m_Quit->cut_value(1.f);

	m_Logo = state()->Create_One(L"TT")->Add_Component<Renderer_UI>();
	m_Logo->one()->Trans()->scale_local(KVector(500.0f, 280.0f, 10.0f, .0f));
	m_Logo->one()->Trans()->pos_local(KVector(-150.0f, 150.0f, 1.1f, .0f));
	m_Logo->material()->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, L"starcraft2-logo-300x165.png");
	m_Logo->cut_fade(1.0f);
	m_Logo->cut_value(1.f);

	m_GameLauncher = false;
	m_LSound = false;
	m_LTime = .0f;
}

void SUpdater::Update_State()
{
	MSphere->one()->Trans()->Rotating_Deg(KVector4(0.0f, .002f, .0f));

	if (true == m_GameLauncher)
	{
		m_LTime += DELTATIME;

		if (false == m_LSound)
		{
			(*m_SSI)->Stop();
			SoundPlayer SCount = SoundPlayer();
			SCount.Play(L"UI_BnetSelect01_1.wav");

			SoundPlayer SCF = SoundPlayer();
			SCF.Play(L"UI_BnetGameFound_1.wav");

			SoundPlayer STt = SoundPlayer();
			STt.Play(L"UI_AC_Countdown_5MxNone.ogg");

			m_LSound = true;
		}

		if (m_LTime > 5.0f)
		{
			Core_Class::MainSceneMgr().Change_State(L"InGame");
			++m_SSI;
		}

		return;
	}

	if (true == m_Start->Mouse_In())
	{
		if (true == KEY_DOWN(L"LB"))
		{
			m_GameLauncher = true;
		}
	}

	if (true == m_Quit->Mouse_In())
	{
		if (true == KEY_DOWN(L"LB"))
		{
			SoundPlayer SCount = SoundPlayer();
			SCount.Play(L"UI_BnetSelect01_1.wav");
			exit(1);
		}
	}
}

void SUpdater::UIRender()
{
	KPtr<KFont> TF = ResourceManager<KFont>::Find(L"Kostar");
	TF->Draw_Font(L"���� ����", KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f - m_Start->one()->Trans()->pos_local().y - 10.0f)
		, 20, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);

	TF->Draw_Font(L"���� ����", KVector2(kwindow()->size().x * .5f, kwindow()->size().y * .5f - m_Quit->one()->Trans()->pos_local().y - 10.0f)
		, 20, KColor::White.color_to_reverse255(), FW1_TEXT_FLAG::FW1_CENTER);
}