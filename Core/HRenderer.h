#pragma once
#include "HSubTransCom.h"
// #include "HVtxShader.h"
#include "HMaterial.h"
#include "HMesh.h"
#include "HBlend.h"
#include "HDevice.h"

class RenderOption
{
private:
	friend class HRenderer;

public:
	int IsLight;
	int IsDefferdAndForward; // 디퍼느냐 포워드냐?
	int LightVtxToPix; // 버텍스냐 픽셀이냐? // 0 이면 버텍스 1이면 픽셀
	int TexCount; // 버텍스냐 픽셀이냐? // 8

private:
	TexData ArrTex[8]; // 20 216

public:
	RenderOption() : IsLight(1), TexCount(0)
		, LightVtxToPix(1), IsDefferdAndForward(1)	{	}
};


class HCamera;
class HMesh;
class HMaterial;
class HRenderer : public HSubTransCom
{
	friend class HRenderMgr;
protected:
	MATDATA		m_MatData;
	int         m_bLight;
	int		    m_Order;
	HPTR<HMesh> m_Mesh;
	HPTR<HMaterial> m_Mat;
	HPTR<HDevice::RSState> m_RsState;

public:
	RenderOption ROpt;

public:
	void SetRSState(const wchar_t* _Name);

public:
	int Order() {
		return m_Order;
	}

public:
	bool SetMesh(const wchar_t* _Res);
	bool SetMat(const wchar_t* _Res);

public:
	HPTR<HMaterial> Mat();

private:
	void RenderUpdate();
	void RenderEndUpdate();

protected:
	virtual void TransUpdate(HPTR<HCamera> _Camera);
	virtual void TransCBUpdate();

protected:
	virtual void MatUpdate();
	virtual void MeshUpdate();

public:
	virtual bool Init(int _Order = 0);
	virtual void Render(HPTR<HCamera> _Camera) = 0;

public:
	HRenderer();
	~HRenderer();
};

