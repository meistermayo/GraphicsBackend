#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <assert.h>

#include "ShaderBase.h"
#include "../GraphicsBackend.h"
#include "../d3dUtil.h"

ShaderBase::ShaderBase(std::string filename)
{
	mpShaderInterface = new ShaderInterface();
	mpShaderInterface->BuildShaders(filename);
}

ShaderBase::~ShaderBase()
{
	delete mpShaderInterface;
}

void ShaderBase::SetToContext_VS_PS_InputLayout()
{
	mpShaderInterface->SetToContext_VS_PS_InputLayout();
}
