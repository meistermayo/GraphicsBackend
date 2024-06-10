#include <d3d11.h>
#include <assert.h>

#include "ShaderWireframe.h"

#include "../d3dUtil.h"
#include "../GraphicsBackend.h"


ShaderWireframe::ShaderWireframe(std::string filename)
	: ShaderBase(filename)
{
	mpShaderInterface->DefineInputLayout();

	mpBufferCamMatrices = new GenericBufferObject<CamMatrices>();
	mpBufferCamMatrices->CreateBuffer(0);

	mpBuffWordColor = new GenericBufferObject<Data_WorldColor>();
	mpBuffWordColor->CreateBuffer(1);

	mpFog = new GenericBufferObject<FogData>();
	mpFog->CreateBuffer(2);
}

ShaderWireframe::~ShaderWireframe()
{
	delete mpBuffWordColor;
	delete mpBufferCamMatrices;
	delete mpFog;
}

void ShaderWireframe::SendCamMatrices( const Matrix& view, const Matrix& proj)
{
	CamMatrices	mCamMatrices;
	mCamMatrices.View = view;  
	mCamMatrices.Projection = proj;

	mpBufferCamMatrices->UpdateBuffer(&mCamMatrices);
}

void ShaderWireframe::SendWorldColor(const Matrix& world, const Vect& col)
{
	Data_WorldColor wc;
	wc.World = world;
	wc.WorldInv = world.GetWorldInverse();
	wc.Color = col;

	mpBuffWordColor->UpdateBuffer(&wc);
}

void ShaderWireframe::SetToContext()
{
	ShaderBase::SetToContext_VS_PS_InputLayout();

	mpBufferCamMatrices->Bind();
	mpBuffWordColor->Bind();
	mpFog->Bind();
}

void ShaderWireframe::SendFogData(const float& fogMin, const float& fogMax, const Vect& fogCol)
{
	FogData fogData;
	fogData.fogMin = fogMin;
	fogData.fogMax = fogMax;
	fogData.fogCol = fogCol;
	fogData.eyePos = eyepos;
	mpFog->UpdateBuffer(&fogData);
}

void ShaderWireframe::GetEyePos(const Vect& _eyepos)
{
	this->eyepos = _eyepos;
}


