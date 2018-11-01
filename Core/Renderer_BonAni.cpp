#include "Renderer_BonAni.h"
#include "Texture.h"
#include "TimeManager.h"
#include "DXContainer_DE.h"
#include "ResourceManager.h"
#include "DebugManager.h"




Renderer_BonAni::Renderer_BonAni() :
m_ClipInx(1), 
m_FrameCnt(30), 
m_UpdateSpd(.0f), 
m_UpdateTime(.0f)
{
	ROpt.Render_DT = RENDER_DATATYPE::RDT_DATA;
	ROpt.IsBoneAni = 1;
}


Renderer_BonAni::~Renderer_BonAni()
{
}


void Renderer_BonAni::Set_Fbx(const wchar_t* _Name)
{
	MCon = ResourceManager<MeshContainer>::Find(_Name);
	Init_Mesh();
}


void Renderer_BonAni::Init_Mesh()
{
	for (size_t MeshInx = 0; MeshInx < MCon->m_Data.MeshVec.size(); MeshInx++)
	{
		KM3Mesh* pMD = MCon->m_Data.MeshVec[MeshInx];

		if (nullptr == pMD)
		{
			BBY;
		}

		KPtr<KMesh> NMesh = new KMesh();
		NMesh->draw_mode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 獄努什 獄遁
		if (false == NMesh->Create_Vertex(
			pMD->VxCnt
			, pMD->VxSize
			, pMD->Usage
			, pMD->m_VertBD))
		{
			BBY;
		}

		for (size_t SubInx = 0; SubInx < pMD->IxVec.size(); SubInx++)
		{
			KM3Index* pInx = &pMD->IxVec[SubInx];

			NMesh->Create_Index(
				pInx->IxCnt
				, pInx->IxSize
				, pInx->Usage
				, pInx->Fm
				, pInx->m_InxBD
			);
		}


		// 背雁 五習研 淳生稽 隔澗 引舛 しし
		Set_Mesh(NMesh, (int)MeshInx);



		// 五斗軒杖 -> 五斗軒杖 鯵呪幻鏑 暁 昇趨爽嬢醤 廃陥.
		for (KUINT MtlIdx = 0; MtlIdx < (KUINT)pMD->MtlVec.size(); MtlIdx++)
		{
			switch (ROpt.Defferd_orForward)
			{
			case 0:
				Set_Material(L"MESH3DMAT", MtlIdx);
				break;

			case 1:
				Set_Material(L"DEFFERD3DANIMAT", MtlIdx);
				break;

			default:
				break;
			}

			KPtr<KMaterial> CMat = material(MtlIdx);

			Material_FbxData* pData = &pMD->MtlVec[MtlIdx];

			// 巨濃綜 閤焼神奄
			if (pData->Diff[0] != 0)
			{
				KPtr<Texture> Tex = ResourceManager<Texture>::Load(pData->Diff);
				CMat->Insert_TexData(TEX_TYPE::TEX_COLOR, 0, Tex->FileForder());
			}


			// 骨覗 閤焼神奄
			if (pData->Bump[0] != 0)
			{
				KPtr<Texture> Tex = ResourceManager<Texture>::Load(pData->Bump);
				CMat->Insert_TexData(TEX_TYPE::TEX_BUMP, 1, Tex->FileForder());
			}


			// 什薙 閤焼神奄
			if (pData->Spec[0] != 0)
			{
				KPtr<Texture> Tex = ResourceManager<Texture>::Load(pData->Spec);
				CMat->Insert_TexData(TEX_TYPE::TEX_SPEC, 2, Tex->FileForder());
			}

			// 神覗実 鯵呪亜 五斗軒杖 鯵呪陥. -> 神覗実精 奄沙 五習虞 坐亀 巷号
			Insert_RenderData((KUINT)MeshInx, 0, MtlIdx, MtlIdx);
		}
	}

	if (0 >= MCon->m_Data.AniVec.size())
	{
		return;
	}

	m_pBoneTex = new Texture();
	m_pBoneTex->Create(
		(KUINT)MCon->m_Data.BoneVec.size() * 4, 1
		, D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R32G32B32A32_FLOAT
		, D3D11_USAGE_DYNAMIC
	);

	m_MXData_CurAni.resize(MCon->m_Data.BoneVec.size());

}

// 析舘, 居酔澗 暗拭 段繊聖 限秩陥.
void Renderer_BonAni::Load_FbxTest(const wchar_t* _Path)
{
	MCon = ResourceManager<MeshContainer>::Load(_Path);
	Init_Mesh();
}

void Renderer_BonAni::EndUpdate()
{
	if (0 >= MCon->m_Data.BoneVec.size())
	{
		return;
	}

	m_CurTime = .0f;
	m_UpdateTime += DELTATIME;

	if (m_UpdateTime >= MCon->m_Data.AniVec[m_ClipInx].Length_Time)
	{
		m_UpdateTime = .0f;
	}

	// 薄仙 獣拙 獣娃拭 置重鉢 獣娃聖 希廃 獣娃戚 薄仙 遭楳掻昔 獣娃 - 薄仙 覗傾績戚 鞠畏陥.
	m_CurTime = (float)(MCon->m_Data.AniVec[m_ClipInx].Stime.GetSecondDouble() + m_UpdateTime);

	int iFrameInx = (int)(m_CurTime* m_FrameCnt);
	int iNextFrameInx = 0;

	// 薄仙 覗傾績戚 覗傾績税 魁左陥 滴檎 0生稽 段奄鉢
	if (iFrameInx >= MCon->m_Data.AniVec[m_ClipInx].Length_Time - 1)
	{
		m_UpdateTime = .0f;
		iFrameInx = 0;
	}

	// 雁尻馬走幻 陥製 舌檎精 + 1 覗傾績戚 鞠畏陥.
	iNextFrameInx = iFrameInx + 1;

	for (size_t i = 0; i < MCon->m_Data.BoneVec.size(); i++)
	{
		if (MCon->m_Data.BoneVec[i]->KFrameVec.empty())
		{
			m_MXData_CurAni[i] = MCon->m_Data.BoneVec[i]->BoneMX;
			continue;
		}

		KeyFrame& CFrame = MCon->m_Data.BoneVec[i]->KFrameVec[iFrameInx];
		KeyFrame& NFrame = MCon->m_Data.BoneVec[i]->KFrameVec[iNextFrameInx];

		float fCFTime = (float)CFrame.dTime;
		float fNFTime = (float)NFrame.dTime;

		// 亜掻帖拭 企廃 搾晴
		float fPercent = (m_CurTime - fCFTime) / (1.0f / m_FrameCnt);


		// 背雁 引舛戚 KM3 適掘什稽 匙然奄 凶庚拭 爽汐 坦軒
		//// 益凶税 滴奄 楳慶 薄仙 - 陥製
		//DirectX::XMVECTOR vSC = FBXLoader::FVectoKVec(CFrame.MX_Frame.GetS());
		//DirectX::XMVECTOR vSN = FBXLoader::FVectoKVec(NFrame.MX_Frame.GetS());

		//// 益凶税 是帖 楳慶 薄仙 - 陥製
		//DirectX::XMVECTOR vTC = FBXLoader::FVectoKVec(CFrame.MX_Frame.GetT());
		//DirectX::XMVECTOR vTN = FBXLoader::FVectoKVec(NFrame.MX_Frame.GetT());

		//// 益凶税 噺穿 楳慶 薄仙 - 陥製
		//// 舘 噺穿精 汀斗艦情生稽 閤焼身 
		//// 刃失吉 杉球 楳慶拭辞 噺穿楳慶聖 皐紳陥澗 闇 災亜管拭 亜猿崇
		//// 呪俳拭辞, 紫据呪(淕螽皃, 慎嬢: quaternion 汀斗艦情[*]) 暁澗 背腔渡 呪(慎嬢: Hamilton number)澗
		//// 差社呪研 溌舌背 幻窮 呪 端域戚陥.革 鯵税 叔呪 失歳聖 亜走悟,
		//// 機疾引 咽疾税 衣杯狛帳 貢 機疾税 嘘発狛帳聖 幻膳獣徹走幻 咽疾税 嘘発狛帳精 失験馬走 省澗陥.
		//DirectX::XMVECTOR vQC = FBXLoader::FQTtoKVec(CFrame.MX_Frame.GetQ());
		//DirectX::XMVECTOR vQN = FBXLoader::FQTtoKVec(NFrame.MX_Frame.GetQ());


		//// 切 戚採歳戚 左娃 背爽澗 姥娃 -> 薄仙 陥製 益軒壱 益 搾晴 隔嬢爽檎 切疑生稽 左娃背層陥.
		//// けず 陥戚刑闘 X戚陥 照拭辞 嬢恐 析戚 析嬢劾猿 せせせ -> 識莫左娃 しし
		DirectX::XMVECTOR vS = DirectX::XMVectorLerp(CFrame.Scale, NFrame.Scale, fPercent);
		DirectX::XMVECTOR vT = DirectX::XMVectorLerp(CFrame.Pos, NFrame.Pos, fPercent);
		DirectX::XMVECTOR vQ = DirectX::XMQuaternionSlerp(CFrame.Rotate, NFrame.Rotate, fPercent);

		DirectX::XMVECTOR vZero = DirectX::XMVectorSet(.0f, .0f, .0f, 1.0f);

		// 奄沙 神覗実拭 姥背遭 左娃 楳慶聖 姥敗 -疑拙 姥薄
		// 舘 益 咽馬澗 引舛亀 滴切戚 因採研 咽背爽澗 橿汐戚 陥戚刑闘拭 赤陥.
		// KMatrix OffMat = FBXLoader::FMXtoKMX(pFbx->Bone_Vec[i]->Offset_FMX);
		m_MXData_CurAni[i] = MCon->m_Data.BoneVec[i]->OffsetMX * DirectX::XMMatrixAffineTransformation(vS, vZero, vQ, vT);
	}

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