#include "GraphicsBackend.h"


void OGL_GraphicsBackend::SetData()
{
}

int OGL_GraphicsBackend::privInitialize()
{
	return 0;
}

void OGL_GraphicsBackend::privInitApp()
{
}

bool OGL_GraphicsBackend::privStillOpen()
{
	return false;
}

int OGL_GraphicsBackend::privPoll()
{
	return 0;
}

void OGL_GraphicsBackend::privPrepare()
{
}

void OGL_GraphicsBackend::privPresent()
{
}

void OGL_GraphicsBackend::privCleanupApp()
{
}

void OGL_GraphicsBackend::privSetClearColor(float r, float g, float b, float a)
{
}

void OGL_GraphicsBackend::privSetPrimitiveTopologyAsTriList() const
{
}

void OGL_GraphicsBackend::privDrawIndexed(int indexCount, int startIndex, int baseVertex) const
{
}

void OGL_GraphicsBackend::privSetDrawModeFill() const
{
}

void OGL_GraphicsBackend::privSetDrawModeWireframe() const
{
}

void OGL_GraphicsBackend::privSetBlendMode(bool inBlendEnabled) const
{
}

const std::string& OGL_GraphicsBackend::privGetBackendFolder() const
{
	return "error";
}

const std::string& OGL_GraphicsBackend::privGetVertexShaderExt() const
{
	return "error";
}

const std::string& OGL_GraphicsBackend::privGetFragmentShaderExt() const
{
	return "error";
}

const float OGL_GraphicsBackend::privGetTime() const
{
	return 0.0f;
}

const bool OGL_GraphicsBackend::privGetKeyState(KEY_CODE key) const
{
	return false;
}

const int OGL_GraphicsBackend::privGetWindowWidth() const
{
	return 0;
}

const int OGL_GraphicsBackend::privGetWindowHeight() const
{
	return 0;
}

void OGL_GraphicsBackend::OnKey(KEY_CODE key, bool down)
{
}

void OGL_GraphicsBackend::OnMouseMove(int x, int y)
{
}

void OGL_GraphicsBackend::OnMouseWheel(int w)
{
}
