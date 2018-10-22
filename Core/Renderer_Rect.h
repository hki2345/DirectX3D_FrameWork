#pragma once
#include "Renderer.h"

class Renderer_Rect : public Renderer
{
public:
	MatrixContainer m_MD;

public:
	void Render(KPtr<Camera> _Camera) override;

public:
	bool Init(int _Order = 0) override;
	// bool Init(const wchar_t* _ImgName, int _Order = 0);


public:
	Renderer_Rect();
	~Renderer_Rect();
};
