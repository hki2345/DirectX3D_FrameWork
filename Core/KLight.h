#pragma once
#include "Component_DE.h"
#include <set>



class Camera;
class Mesh;
class Material;
class KLight : public Component_DE
{
public:
	KLight();
	~KLight();

public:
	friend class RenderManager;

	enum LightType
	{
		Direction = 0,
		Point,
		Spot,
	};

	class LightColor
	{
	public:
		KColor	Color;
		KVector Diff; // Ȯ�걤
		KVector Spec; // �ݻ籤
		KVector Ambi; // ȯ�汤
	};

	class LightData
	{
	public:
		LightColor LC;
		KVector m_Pos;
		KVector m_Dir;
		KVector CamPos;
		float Range;
		float Angle;
		int Type;
		int Dump;
	};

	class LightCB
	{
	public:
		LightData ArrLight[10];
		int LCnt;

		// 16����Ʈ ��.��;;
		int Temp1;
		int Temp2;
		int Temp3;
	};

private:
	int m_Order;
	std::set<int> m_LayerSet;
	LightData m_LD;
	LightType m_eType;


public:
	// ���� ���� �޽�
	KPtr<Mesh> m_LMesh;
	KPtr<Material> m_LMat;


	// �������� ó���� �� - ����Ʈ ���� ��
	DATA_3D		m_MD;
	KPtr<Mesh>	m_VolumeMesh;
	KPtr<Material>	m_VolumeMat;

public:
	void range(const float& _Value)
	{
		m_LD.Range = _Value;
	}

	void color(const KColor& _Value)
	{
		m_LD.LC.Color = _Value;
	}



	virtual bool Init();
	void FinalUpdate() override;


	bool Is_Focus(const int& _Layer) const
	{
		std::set<int>::iterator FI = m_LayerSet.find(_Layer);

		if (m_LayerSet.end() == FI)
		{
			return false;
		}

		return true;
	}


private:
	// �������� �ִ� ����
	template<typename ...Rest>
	void Insert_LayerTemp(Rest ...Arg)
	{
		Insert_Layer(Arg...);
	}

public:
	template<typename ...Rest>
	void Insert_Layer(int _Data, Rest ...Arg)
	{
		std::set<int>::iterator FI = m_LayerSet.find(_Data);

		if (m_LayerSet.end() == FI)
		{
			m_LayerSet.insert(_Data);
		}
		Insert_Layer(Arg...);
	}


public:
	void Insert_Layer() {}
	void type(const LightType& _Type);

private:
	void Render(KPtr<Camera> _Cam);
};

