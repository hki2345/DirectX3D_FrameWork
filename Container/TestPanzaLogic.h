#pragma once
#include <SomeOne.h>
#include <KCollision.h>

class HSpRenderer;
class HSpFrameAnimater;
class KCollider2D_DE;
class TestPanzaLogic : public SomeOne
{
private:
	size_t m_ImgIndex;
	KPtr<HSpRenderer> ChRender;
	KPtr<HSpRenderer> ChRender2;
	KPtr<KCollider2D_DE> m_Col;
	KPtr<HSpFrameAnimater> AniMater;

public:
	bool Init() override;
	void Update() override;
	void DebugRender() override;

public:
	void MyColTest(KCollision* _Left, KCollision* _Right);
	void MyColExitTest(KCollision* _Left, KCollision* _Right);

public:
	TestPanzaLogic();
	~TestPanzaLogic();
};

