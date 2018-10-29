#include "Renderer_BonAni.h"
#include "Texture.h"
#include "TimeManager.h"
#include "DXContainer_DE.h"
#include "ResourceManager.h"
#include "DebugManager.h"




Renderer_BonAni::Renderer_BonAni() :
m_pLoader(nullptr), 
m_ClipInx(0), 
m_FrameCnt(30), 
m_UpdateSpd(.0f), 
m_UpdateTime(.0f), 
SFrame(0), 
EFrame(0)
{
	ROpt.Render_DT = RENDER_DATATYPE::RDT_DATA;
	ROpt.IsBoneAni = 1;
}


Renderer_BonAni::~Renderer_BonAni()
{
	if (nullptr != m_pLoader)
	{
		delete m_pLoader;
	}
}



// 析舘, 居酔澗 暗拭 段繊聖 限秩陥.
void Renderer_BonAni::Load_FbxTest(const wchar_t* _Path)
{
	m_pLoader = new FBXLoader();
	m_pLoader->Load_FBX(_Path);

	KFBX* pFbx = m_pLoader->m_pNewFbx;


	for (size_t MeshInx = 0; MeshInx < pFbx->MeshData_Vec.size(); MeshInx++)
	{
		Mesh_FbxData* pMD = pFbx->MeshData_Vec[MeshInx];

		if (nullptr == pMD)
		{
			BBY;
		}

		KPtr<KMesh> NMesh = new KMesh();
		NMesh->draw_mode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 獄努什 獄遁
		if (false == NMesh->Create_Vertex((KUINT)pMD->VertVec.size()
		, sizeof(Vertex_FbxData)
		, D3D11_USAGE_DYNAMIC
		, &pMD->VertVec[0]))
		{
			BBY;
		}

		for (size_t SubInx = 0; SubInx < pMD->IdxVec.size(); SubInx++)
		{
			NMesh->Create_Index(
				(KUINT)pMD->IdxVec[SubInx].size()
				, IDX32::MemberSize()
				, D3D11_USAGE_DEFAULT
				, IDX32::FM()
				, &pMD->IdxVec[SubInx][0]
			);
		}


		// 背雁 五習研 淳生稽 隔澗 引舛 しし
		Set_Mesh(NMesh, (int)MeshInx);



		// 五斗軒杖 -> 五斗軒杖 鯵呪幻鏑 暁 昇趨爽嬢醤 廃陥.
		for (KUINT MtlIdx = 0; MtlIdx < (KUINT)pMD->m_MtlVec.size(); MtlIdx++)
		{
			switch (ROpt.Defferd_orForward)
			{
			case 0:
				Set_Material(L"MESH3DVTX", MtlIdx);
				break;

			case 1:
				Set_Material(L"DEFFERD3DANIMAT", MtlIdx);
				break;

			default:
				break;
			}

			KPtr<KMaterial> CMat = material(MtlIdx);

			Material_FbxData* pData = pMD->m_MtlVec[MtlIdx];

			// 巨濃綜 閤焼神奄
			if (pData->Diff != L"")
			{
				KPtr<Texture> Tex = ResourceManager<Texture>::Load(pData->Diff.c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, Tex->FileForder());
			}


			// 骨覗 閤焼神奄
			if (pData->Bump != L"")
			{
				KPtr<Texture> Tex = ResourceManager<Texture>::Load(pData->Bump.c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_BUMP, 1, Tex->FileForder());
			}


			// 什薙 閤焼神奄
			if (pData->Spec != L"")
			{
				KPtr<Texture> Tex = ResourceManager<Texture>::Load(pData->Spec.c_str());
				CMat->Insert_TexData(TEX_TYPE::TEX_SPEC, 2, Tex->FileForder());
			}

			// 神覗実 鯵呪亜 五斗軒杖 鯵呪陥. -> 神覗実精 奄沙 五習虞 坐亀 巷号
			Insert_RenderData((KUINT)MeshInx, 0, MtlIdx, MtlIdx);
		}
	}

	if (0 >=  m_pLoader->m_pNewFbx->Ani_Vec.size())
	{
		return;
	}

	m_pBoneTex = new Texture();
	m_pBoneTex->Create(
		m_pLoader->m_BoneCnt * 4, 1
		, D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R32G32B32A32_FLOAT
		, D3D11_USAGE_DYNAMIC
	);

	m_MXData_CurAni.resize(m_pLoader->m_BoneCnt);
}

void Renderer_BonAni::EndUpdate()
{
	if (0 >= m_pLoader->m_pNewFbx->Ani_Vec.size())
	{
		return;
	}

	m_CurTime = .0f;

	KFBX* pFbx = m_pLoader->m_pNewFbx;

	m_UpdateTime += DELTATIME;

	if (m_UpdateTime >= pFbx->Ani_Vec[m_ClipInx]->Length_Time)
	{
		m_UpdateTime = .0f;
	}

	// 薄仙 獣拙 獣娃拭 置重鉢 獣娃聖 希廃 獣娃戚 薄仙 遭楳掻昔 獣娃 - 薄仙 覗傾績戚 鞠畏陥.
	m_CurTime = (float)(pFbx->Ani_Vec[m_ClipInx]->Stime.GetSecondDouble() + m_UpdateTime);

	int iFrameInx = (int)(m_CurTime* m_FrameCnt);
	int iNextFrameInx = 0;

	// 薄仙 覗傾績戚 覗傾績税 魁左陥 滴檎 0生稽 段奄鉢
	if (iFrameInx >= pFbx->Ani_Vec[m_ClipInx]->Length_Time - 1)
	{
		m_UpdateTime = .0f;
		iFrameInx = 0;
	}

	// 雁尻馬走幻 陥製 舌檎精 + 1 覗傾績戚 鞠畏陥.
	iNextFrameInx = iFrameInx + 1;

	for (size_t i = 0; i < m_pLoader->m_BoneCnt; i++)
	{
		if (pFbx->Bone_Vec[i]->KFVec.empty())
		{
			m_MXData_CurAni[i] = FBXLoader::FMXtoKMX(pFbx->Bone_Vec[i]->Bone_MX);
			continue;
		}

		KeyFrame& CFrame = pFbx->Bone_Vec[i]->KFVec[iFrameInx];
		KeyFrame& NFrame = pFbx->Bone_Vec[i]->KFVec[iNextFrameInx];

		float fCFTime = (float)CFrame.dTime;
		float fNFTime = (float)NFrame.dTime;

		// 亜掻帖拭 企廃 搾晴
		float fPercent = (m_CurTime - fCFTime) / (1.0f / m_FrameCnt);

		// 益凶税 滴奄 楳慶 薄仙 - 陥製
		DirectX::XMVECTOR vSC = FBXLoader::FVectoKVec(CFrame.MX_Frame.GetS());
		DirectX::XMVECTOR vSN = FBXLoader::FVectoKVec(NFrame.MX_Frame.GetS());

		// 益凶税 是帖 楳慶 薄仙 - 陥製
		DirectX::XMVECTOR vTC = FBXLoader::FVectoKVec(CFrame.MX_Frame.GetT());
		DirectX::XMVECTOR vTN = FBXLoader::FVectoKVec(NFrame.MX_Frame.GetT());

		// 益凶税 噺穿 楳慶 薄仙 - 陥製
		// 舘 噺穿精 汀斗艦情生稽 閤焼身 
		// 刃失吉 杉球 楳慶拭辞 噺穿楳慶聖 皐紳陥澗 闇 災亜管拭 亜猿崇
		// 呪俳拭辞, 紫据呪(淕螽皃, 慎嬢: quaternion 汀斗艦情[*]) 暁澗 背腔渡 呪(慎嬢: Hamilton number)澗
		// 差社呪研 溌舌背 幻窮 呪 端域戚陥.革 鯵税 叔呪 失歳聖 亜走悟,
		// 機疾引 咽疾税 衣杯狛帳 貢 機疾税 嘘発狛帳聖 幻膳獣徹走幻 咽疾税 嘘発狛帳精 失験馬走 省澗陥.
		DirectX::XMVECTOR vQC = FBXLoader::FQTtoKVec(CFrame.MX_Frame.GetQ());
		DirectX::XMVECTOR vQN = FBXLoader::FQTtoKVec(NFrame.MX_Frame.GetQ());


		// 切 戚採歳戚 左娃 背爽澗 姥娃 -> 薄仙 陥製 益軒壱 益 搾晴 隔嬢爽檎 切疑生稽 左娃背層陥.
		// けず 陥戚刑闘 X戚陥 照拭辞 嬢恐 析戚 析嬢劾猿 せせせ -> 識莫左娃 しし
		DirectX::XMVECTOR vS = DirectX::XMVectorLerp(vSC, vSN, fPercent);
		DirectX::XMVECTOR vT = DirectX::XMVectorLerp(vTC, vTN, fPercent);
		DirectX::XMVECTOR vQ = DirectX::XMQuaternionSlerp(vQC, vQN, fPercent);

		DirectX::XMVECTOR vZero = DirectX::XMVectorSet(.0f, .0f, .0f, .0f);

		// 奄沙 神覗実拭 姥背遭 左娃 楳慶聖 姥敗 -疑拙 姥薄
		// 舘 益 咽馬澗 引舛亀 滴切戚 因採研 咽背爽澗 橿汐戚 陥戚刑闘拭 赤陥.
		KMatrix OffMat = FBXLoader::FMXtoKMX(pFbx->Bone_Vec[i]->Offset_MX);
		m_MXData_CurAni[i] = OffMat * DirectX::XMMatrixAffineTransformation(vS, vZero, vQ, vT);
	}

	// 戚薦 益 努什団拭 汽戚斗 号縦生稽 隔澗陥. -姥背遭 滴切戚 楳慶聖 源戚陥.
	m_pBoneTex->Set_Pixel(&m_MXData_CurAni[0], sizeof(KMatrix) * m_MXData_CurAni.size());
}

void Renderer_BonAni::Render(KPtr<Camera> _Cam)
{
	if (nullptr != m_pBoneTex)
	{
		// 汽戚斗 努什団澗 10腰拭 拝雁廃陥.
		m_pBoneTex->Update(10);
	}
}