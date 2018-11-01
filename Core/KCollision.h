#pragma once
#include "Component_DE.h"
#include "KMath.h"
#include <set>
#include <functional>

// 도형을 표현할때.
// 도형은 방정식이다.
// 위치를 표현하는 X Y
// +X +Y
// 0 == X(+Y)
// 원
// 사각형
// 점

// 상대 점이
// 방정식은 항상 X Y == 0
// KVector4 하나만 있다면 뭘 표현할수 있는가?

enum COLTYPE
{
	CT_POINT2D, // 8
	CT_RECT2D, // 7
	CT_CIRCLE2D, // 6
	CT_POLY2D, // 4
	CT_PLANE3D,
	CT_POINT3D,
	CT_SPHERE3D, // 3
	CT_RAY3D, // 2
	CT_AABB3D, // 1// 회전 하지 않는 큐브
	CT_OBB3D,// 회전 하지 않는 큐브
	CT_MAX
};

class KCollision;
class Funtion_Col;
class Figure_Col 
{
public:
	friend KCollision;
	friend Funtion_Col;

public:
	COLTYPE m_ColType;

public:
	Figure_Col() : m_ColType(CT_MAX) {}
	virtual ~Figure_Col() = 0 {	}
};

class Figure2D_DE : public Figure_Col
{
public:
	KVector4 m_Vec;

public:
	Figure2D_DE() {}
	virtual ~Figure2D_DE() {}
};



class KSphereCon : public Figure_Col
{
public:
	DirectX::BoundingSphere m_Sphere;
public:
	KSphereCon() {}
	virtual ~KSphereCon() {}
};

class KRayCon : public Figure_Col
{
public:
	KVector Ori;
	KVector Dir;
	float Dist;

public:
	KRayCon() {}
	virtual ~KRayCon() {}
};




class Funtion_Col
{

#pragma region Funtion 2D
public:
	static bool RectToRectFi(const Figure_Col* _Left, const Figure_Col* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->m_ColType != COLTYPE::CT_RECT2D || _Right->m_ColType != COLTYPE::CT_RECT2D)
		{
			return false;
		}
#endif
		return KMath::RectToRect(((Figure2D_DE*)_Left)->m_Vec, ((Figure2D_DE*)_Right)->m_Vec);
	}
	static bool CirCleToCirCleFi(const Figure_Col* _Left, const Figure_Col* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->m_ColType != COLTYPE::CT_CIRCLE2D || _Right->m_ColType != COLTYPE::CT_CIRCLE2D)
		{
			return false;
		}
#endif
		return KMath::CirCleToCirCle(((Figure2D_DE*)_Left)->m_Vec.m_Vec3, ((Figure2D_DE*)_Right)->m_Vec.m_Vec3);
	}

	static bool PointToCirCleFi(const Figure_Col* _Left, const  Figure_Col* _Right) {
		return CirCleToPointFi(_Right, _Left);
	}
	static bool CirCleToPointFi(const Figure_Col* _Left, const  Figure_Col* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->m_ColType != COLTYPE::CT_CIRCLE2D || _Right->m_ColType != COLTYPE::CT_POINT2D)
		{
			return false;
		}
#endif
		return KMath::CirCleToPoint(((Figure2D_DE*)_Left)->m_Vec.m_Vec3, ((Figure2D_DE*)_Right)->m_Vec.m_Pos);
	}

	static bool PointToRectFi(const Figure_Col* _Left, const  Figure_Col* _Right) {
		return RectToPointFi(_Right, _Left);
	}
	static bool RectToPointFi(const Figure_Col* _Left, const Figure_Col* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->m_ColType != COLTYPE::CT_RECT2D || _Right->m_ColType != COLTYPE::CT_POINT2D)
		{
			return false;
		}
#endif
		return KMath::RectToPoint(((Figure2D_DE*)_Left)->m_Vec, ((Figure2D_DE*)_Right)->m_Vec.m_Pos);
	}

	static bool CirCleToRectFi(const Figure_Col* _Left, const Figure_Col* _Right) {
		return RectToCirCleFi(_Right, _Left);
	}

	static bool RectToCirCleFi(const Figure_Col* _Left, const  Figure_Col* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->m_ColType != COLTYPE::CT_RECT2D || _Right->m_ColType != COLTYPE::CT_CIRCLE2D)
		{
			return false;
		}
#endif
		return KMath::RectToCirCle(((Figure2D_DE*)_Left)->m_Vec, ((Figure2D_DE*)_Right)->m_Vec.m_Vec3);
	}

#pragma endregion

	static bool SphereToSphereFunc(const Figure_Col* _Left, const Figure_Col* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->m_ColType != COLTYPE::CT_SPHERE3D || _Right->m_ColType != COLTYPE::CT_SPHERE3D)
		{
			return false;
		}
#endif
		return KMath::SphereToSphere(((KSphereCon*)_Left)->m_Sphere, ((KSphereCon*)_Right)->m_Sphere);
	}

	static bool SphereToRayFunc(const Figure_Col* _Left, const Figure_Col* _Right)
	{
#ifdef _DEBUG
		if (nullptr == _Left || nullptr == _Right)
		{
			return false;
		}
		if (_Left->m_ColType != COLTYPE::CT_SPHERE3D || _Right->m_ColType != COLTYPE::CT_RAY3D)
		{
			return false;
		}
#endif
		return KMath::SphereToRay(((KSphereCon*)_Left)->m_Sphere, ((KRayCon*)_Right)->Ori, ((KRayCon*)_Right)->Dir, ((KRayCon*)_Right)->Dist);

	}

	static bool RayToSphereFunc(const Figure_Col* _Left, const Figure_Col* _Right)
	{
		return SphereToRayFunc(_Right, _Left);
	}

};


class KCore;
class CollisionManager;
class KCollision : public Component_DE
{
public:
	friend KCore;
	friend CollisionManager;

private:
	static void ColInit();
	static bool(*ColFunc[CT_MAX][CT_MAX])(const Figure_Col* _Left, const  Figure_Col* _Right);

private:
	Figure_Col* m_Fi;

protected:
	int	    m_Order;

private:
	std::set<KCollision*>::iterator ColFindIter;
	std::set<KCollision*> m_ColSet;

public:
	void DeathRelease();

public:
	virtual bool Init();
	virtual void Update_Figure();

protected:
	template<typename T>
	T* CreateColFi(COLTYPE _ColType)
	{
		T* Fi = new T();
		Fi->m_ColType = _ColType;
		m_Fi = Fi;
		return Fi;
	}

public:
	void ColCheck(KCollision* _Col);
	bool FiColCheck(const Figure_Col* _Col);

private:
	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>>::iterator m_EnterStartIter;
	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>>::iterator m_EnterEndIter;
	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>> m_EnterFuncList;

	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>>::iterator m_StayStartIter;
	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>>::iterator m_StayEndIter;
	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>> m_StayFuncList;

	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>>::iterator m_ExitStartIter;
	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>>::iterator m_ExitEndIter;
	std::list<std::function<void(KCollision* _Left, KCollision* _Right)>> m_ExitFuncList;

public:
	// 함수를 호출할때 그 오브젝트가 지워진다면 터지게 된다.
	template<typename T>
	void EnterFunc(T* _Obj, void(T::*_PTR)(KCollision*, KCollision*))
	{
		if (nullptr == _PTR)		{			return;		}
		if (nullptr == _Obj)		{			return;		}
		m_EnterFuncList.push_back(std::bind(_PTR, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

	template<typename T>
	void StayFunc(T* _Obj, void(T::*_PTR)(KCollision*, KCollision*))
	{
		if (nullptr == _PTR) { return; }
		if (nullptr == _Obj) { return; }
		m_StayFuncList.push_back(std::bind(_PTR, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

	template<typename T>
	void ExitFunc(T* _Obj, void(T::*_PTR)(KCollision*, KCollision*))
	{
		if (nullptr == _PTR) { return; }
		if (nullptr == _Obj) { return; }
		m_ExitFuncList.push_back(std::bind(_PTR, _Obj, std::placeholders::_1, std::placeholders::_2));
	}

private:
	void CallEnterList(KCollision* _Right);
	void CallStayList(KCollision* _Right);
	void CallExitList(KCollision* _Right);

public:
	KCollision();
	virtual ~KCollision();
};

