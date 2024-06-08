#include "ShaderColor.h"
#include "d3dUtil.h"
#include <d3d11.h>
#include <assert.h>
#include "GraphicsBackend.h"

ShaderColor::ShaderColor(std::string filename)
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

ShaderColor::~ShaderColor()
{
	delete mpBuffWordColor;
	delete mpBufferCamMatrices;
	delete mpFog;
}

void ShaderColor::SendCamMatrices( const Matrix& view, const Matrix& proj)
{
	CamMatrices	mCamMatrices;
	mCamMatrices.View = view;  
	mCamMatrices.Projection = proj;

	mpBufferCamMatrices->UpdateBuffer(&mCamMatrices);
}

void ShaderColor::SendWorldColor(const Matrix& world, const Vect& col)
{
	Data_WorldColor wc;
	wc.World = world;
	wc.WorldInv = world.GetWorldInverse();
	wc.Color = col;

	mpBuffWordColor->UpdateBuffer(&wc);
}

void ShaderColor::SetToContext()
{
	ShaderBase::SetToContext_VS_PS_InputLayout();

	mpBufferCamMatrices->Bind();
	mpBuffWordColor->Bind();
	mpFog->Bind();
}


void ShaderColor::SendFogData(const float& fogMin, const float& fogMax, const Vect& fogCol)
{
	FogData fogData;
	fogData.fogMin = fogMin;
	fogData.fogMax = fogMax;
	fogData.fogCol = fogCol;
	fogData.eyePos = eyepos;
	mpFog->UpdateBuffer(&fogData);
}
void ShaderColor::GetEyePos(const Vect& _eyepos)
{
	this->eyepos = _eyepos;
}


