#include "ShaderBase.h"
#include "GraphicsBackend.h"
#include <d3d11.h>
#include "d3dUtil.h"
#include <d3dcompiler.h>
#include <string>
#include <assert.h>

ShaderBase::ShaderBase(std::string filename)
{
	mpShaderInterface = new ShaderInterface();
	mpShaderInterface->BuildShaders(filename);
}

ShaderBase::~ShaderBase()
{
	delete mpShaderInterface;
}
/*
void ShaderBase::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layoutdesc, UINT size)
{
	mpShaderInterface->CreateInputLayout(layoutdesc, size); // todo - refactor
}
*/
// Sets the DX context to use these VS, PS and input layout
void ShaderBase::SetToContext_VS_PS_InputLayout()
{
	mpShaderInterface->SetToContext_VS_PS_InputLayout(); // todo - refactor
}
