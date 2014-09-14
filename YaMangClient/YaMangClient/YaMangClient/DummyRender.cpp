#include "stdafx.h"
#include "DummyRender.h"
#include "Renderer.h"

DummyRender::DummyRender()
{
	Renderer::GetInstance()->RenderBegin();
}

DummyRender::~DummyRender()
{
	Renderer::GetInstance()->RenderEnd();
}
