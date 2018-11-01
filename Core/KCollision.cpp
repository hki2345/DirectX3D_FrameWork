#include "KCollision.h"
#include "KMath.h"

bool(*KCollision::ColFunc[CT_MAX][CT_MAX])(const Figure_Col* _Left, const Figure_Col* _Right);

void KCollision::ColInit() {

	for (size_t y = 0; y < CT_MAX; ++y)
	{
		for (size_t x = 0; x < CT_MAX; ++x)
		{
			ColFunc[y][x] = nullptr;
		}
	}

	ColFunc[CT_RECT2D][CT_RECT2D] = &Funtion_Col::RectToRectFi;
	ColFunc[CT_CIRCLE2D][CT_CIRCLE2D] = &Funtion_Col::CirCleToCirCleFi;
	ColFunc[CT_CIRCLE2D][CT_POINT2D] = &Funtion_Col::CirCleToPointFi;
	ColFunc[CT_POINT2D][CT_CIRCLE2D] = &Funtion_Col::PointToCirCleFi;
	ColFunc[CT_RECT2D][CT_POINT2D] = &Funtion_Col::RectToPointFi;
	ColFunc[CT_POINT2D][CT_RECT2D] = &Funtion_Col::PointToRectFi;
	ColFunc[CT_RECT2D][CT_CIRCLE2D] = &Funtion_Col::RectToCirCleFi;
	ColFunc[CT_CIRCLE2D][CT_RECT2D] = &Funtion_Col::CirCleToRectFi;

	ColFunc[CT_SPHERE3D][CT_SPHERE3D] = &Funtion_Col::SphereToSphereFunc;
	ColFunc[CT_SPHERE3D][CT_RAY3D] = &Funtion_Col::SphereToRayFunc;
	ColFunc[CT_RAY3D][CT_SPHERE3D] = &Funtion_Col::RayToSphereFunc;
}

KCollision::KCollision()
{
}


KCollision::~KCollision()
{
	if (nullptr != m_Fi)
	{
		delete m_Fi;
	}
}

void KCollision::Update_Figure() {

}

void KCollision::ColCheck(KCollision* _Col) {

	if (nullptr == ColFunc)
	{
		return;
	}

 	bool Check = FiColCheck(_Col->m_Fi);

	if (true == Check)
	{
		ColFindIter = m_ColSet.find(_Col);
		if (ColFindIter == m_ColSet.end())
		{
			m_ColSet.insert(_Col);
			_Col->m_ColSet.insert(this);
			CallEnterList(_Col);
		}
		else {
			CallStayList(_Col);
		}
	}
	else 
	{
		ColFindIter = m_ColSet.find(_Col);
		if (ColFindIter != m_ColSet.end())
		{
			m_ColSet.erase(_Col);
			_Col->m_ColSet.erase(this);
			CallExitList(_Col);
		}
	}
	return;
}

bool KCollision::FiColCheck(const Figure_Col* _Col)
{
	if (nullptr == _Col)
	{
		return false;
	}

	if (_Col->m_ColType == CT_MAX || m_Fi->m_ColType == CT_MAX)
	{
		return false;
	}

	return ColFunc[m_Fi->m_ColType][_Col->m_ColType](m_Fi, _Col);
}

bool KCollision::Init()
{
	return true;
}

void KCollision::CallEnterList(KCollision* _Right)
{
	m_EnterStartIter = m_EnterFuncList.begin();
	m_EnterEndIter = m_EnterFuncList.end();

	for (; m_EnterStartIter != m_EnterEndIter; ++m_EnterStartIter)
	{
		(*m_EnterStartIter)(this, _Right);
	}
}
void KCollision::CallStayList(KCollision* _Right) 
{
	m_StayStartIter = m_StayFuncList.begin();
	m_StayEndIter = m_StayFuncList.end();

	for (; m_StayStartIter != m_StayEndIter; ++m_StayStartIter)
	{
		(*m_StayStartIter)(this, _Right);
	}
}

void KCollision::CallExitList(KCollision* _Right) {
	m_ExitStartIter = m_ExitFuncList.begin();
	m_ExitEndIter = m_ExitFuncList.end();

	for (; m_ExitStartIter != m_ExitEndIter; ++m_ExitStartIter)
	{
		(*m_ExitStartIter)(this, _Right);
	}
}

void KCollision::DeathRelease() 
{
	std::set<KCollision*>::iterator StartIter = m_ColSet.begin();
	std::set<KCollision*>::iterator EndIter = m_ColSet.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		(*StartIter)->m_ColSet.erase(this);
	}

}