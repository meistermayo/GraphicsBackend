#include "ShaderColorLight.h"
#include "d3dUtil.h"
#include <d3d11.h>
#include <assert.h>
#include "GraphicsBackend.h"

ShaderColorLight::ShaderColorLight(std::string filename)
	: ShaderBase(filename)
{
	mpShaderInterface->DefineInputLayoutNormal();

	mpBufferCamMatrices = new GenericBufferObject<CamMatrices>();
	mpBufferCamMatrices->CreateBuffer(0);

	mpBufferLightParams = new GenericBufferObject<Data_LightParams>();
	mpBufferLightParams->CreateBuffer(1);

	mpBuffWordAndMaterial = new GenericBufferObject<Data_WorldAndMaterial>();
	mpBuffWordAndMaterial->CreateBuffer(2);

	mpFog = new GenericBufferObject<FogData>();
	mpFog->CreateBuffer(3);

	// Zeroing the light data
	ZeroMemory(&DirLightData, sizeof(DirLightData));
	ZeroMemory(&PointLightData1, sizeof(PointLightData1));
	ZeroMemory(&PointLightData2, sizeof(PointLightData2));
	ZeroMemory(&PointLightData3, sizeof(PointLightData3));
	ZeroMemory(&SpotLightData, sizeof(SpotLightData));
}

ShaderColorLight::~ShaderColorLight()
{
	delete mpBuffWordAndMaterial;
	delete mpBufferLightParams;
	delete mpBufferCamMatrices;
	delete mpFog;
}

void ShaderColorLight::SendCamMatrices(const Matrix& view, const Matrix& proj)
{
	CamMatrices	mCamMatrices;
	mCamMatrices.View = view;
	mCamMatrices.Projection = proj;

	mpBufferCamMatrices->UpdateBuffer(&mCamMatrices);
}

void ShaderColorLight::SetDirectionalLightParameters(const Vect& dir, const Vect& amb, const Vect& dif, const Vect& sp)
{
	DirLightData.Light.Ambient = amb;
	DirLightData.Light.Diffuse = dif;
	DirLightData.Light.Specular = sp;
	DirLightData.Direction = dir;
}

void ShaderColorLight::SetPointLightParameters1(const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp)
{
	PointLightData1.Light.Ambient = amb;
	PointLightData1.Light.Diffuse = dif;
	PointLightData1.Light.Specular = sp;
	PointLightData1.Position = pos;
	PointLightData1.Attenuation = att;
	PointLightData1.Range = r;
}
void ShaderColorLight::SetPointLightParameters2(const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp)
{
	PointLightData2.Light.Ambient = amb;
	PointLightData2.Light.Diffuse = dif;
	PointLightData2.Light.Specular = sp;
	PointLightData2.Position = pos;
	PointLightData2.Attenuation = att;
	PointLightData2.Range = r;
}
void ShaderColorLight::SetPointLightParameters3(const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp)
{
	PointLightData3.Light.Ambient = amb;
	PointLightData3.Light.Diffuse = dif;
	PointLightData3.Light.Specular = sp;
	PointLightData3.Position = pos;
	PointLightData3.Attenuation = att;
	PointLightData3.Range = r;
}

void ShaderColorLight::SetSpotLightParameters(const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb, const Vect& dif, const Vect& sp)
{
	SpotLightData.Light.Ambient = amb;
	SpotLightData.Light.Diffuse = dif;
	SpotLightData.Light.Specular = sp;
	SpotLightData.Position = pos;
	SpotLightData.Direction = dir;
	SpotLightData.Attenuation = att;
	SpotLightData.Range = r;
	SpotLightData.SpotExp = spotExp;
}

void ShaderColorLight::SendLightParameters( const Vect& eyepos)
{
	Data_LightParams dl;
	dl.DirLight = DirLightData;
	dl.PntLight1 = PointLightData1;
	dl.PntLight2 = PointLightData2;
	dl.PntLight3 = PointLightData3;
	dl.SptLight = SpotLightData;
	dl.EyePosWorld = eyepos;

	mpBufferLightParams->UpdateBuffer(&dl);
}

void ShaderColorLight::SendWorldAndMaterial(const Matrix& world, const Vect& amb, const Vect& dif, const Vect& sp)
{
	Data_WorldAndMaterial wm;
	wm.World = world;
	wm.WorlInv = world.GetWorldInverse();
	wm.Mat.Ambient = amb;
	wm.Mat.Diffuse = dif;
	wm.Mat.Specular = sp;

	mpBuffWordAndMaterial->UpdateBuffer(&wm);
}

void ShaderColorLight::SetToContext()
{
	ShaderBase::SetToContext_VS_PS_InputLayout();

	mpBufferCamMatrices->Bind();
	mpBufferLightParams->Bind();
	mpBuffWordAndMaterial->Bind();
	mpFog->Bind();
}

void ShaderColorLight::SendFogData(const float& fogMin, const float& fogMax, const Vect& fogCol)
{
	FogData fogData;
	fogData.fogMin = fogMin;
	fogData.fogMax = fogMax;
	fogData.fogCol = fogCol;

	mpFog->UpdateBuffer(&fogData);
}