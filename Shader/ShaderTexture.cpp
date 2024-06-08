#include "ShaderTexture.h"
#include <assert.h>
#include "DirectXTex.h"
#include "d3dUtil.h"
#include "GraphicsBackend.h"

ShaderTexture::ShaderTexture(std::string filename) :
	ShaderBase(filename)
{
	mpShaderInterface->DefineInputLayoutTex();

	mpBufferCamMatrices = new GenericBufferObject<CamMatrices>();
	mpBufferCamMatrices->CreateBuffer(0);

	mpBuffWordColor = new GenericBufferObject<InstanceData>();
	mpBuffWordColor->CreateBuffer(1);

	mpFog = new GenericBufferObject<FogData>();
	mpFog->CreateBuffer(2);
	
	eyepos = Vect::Zero;
	SendFogData(1000.0f, 10000.0f, Vect(1.f, 0.f, 1.f));
}

ShaderTexture::~ShaderTexture()
{
	delete mpBufferCamMatrices;
	delete mpBuffWordColor;
	delete mpFog;
}

void ShaderTexture::SetToContext()
{
	ShaderBase::SetToContext_VS_PS_InputLayout();

	mpBufferCamMatrices->Bind();
	mpBuffWordColor->Bind();
	mpFog->Bind();

	// mTex->SetToContext(); // todo
}

void ShaderTexture::SendCamMatrices(const Matrix& view, const Matrix& proj)
{

	CamMatrices	mCamMatrices;
	mCamMatrices.View = view;
	mCamMatrices.Projection = proj;

	mpBufferCamMatrices->UpdateBuffer(&mCamMatrices);
}

void ShaderTexture::SendFogData(const float& fogMin, const float& fogMax, const Vect& fogCol)
{
	FogData fogData;
	fogData.fogMin = fogMin;
	fogData.fogMax = fogMax;
	fogData.fogCol = fogCol;
	fogData.eyePos = eyepos;
	mpFog->UpdateBuffer(&fogData);
}
void ShaderTexture::GetEyePos(const Vect& _eyepos)
{
	this->eyepos = _eyepos;
}
void ShaderTexture::SendWorld(const Matrix& world)
{
	InstanceData wc;
	wc.World = world;
	wc.WorldInv = world.GetWorldInverse();
	mpBuffWordColor->UpdateBuffer(&wc);
}