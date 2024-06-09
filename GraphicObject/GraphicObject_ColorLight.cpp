#include "GraphicObject_ColorLight.h"
#include "../Camera.h"
#include "../Model/Model.h"
#include "../Math/Vect.h"
#include "../Shader/ShaderColor.h"
#include <assert.h>

GraphicObject_ColorLight::GraphicObject_ColorLight(Model* inModel, ShaderColorLight* inShader, const Vect &inAmb, const Vect &inDif, const Vect& inSp)
{
	pModel = inModel;
	pShader = inShader;

	material = new ShaderColorLight::Material();
	this->material->Ambient = inAmb;
	this->material->Diffuse = inDif;
	this->material->Specular = inSp;

	pColor = new Vect[pModel->GetMeshCount()];

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pColor[i] = Vect::Zero;
	}

	mWorld = Matrix::Identity;
}

GraphicObject_ColorLight::~GraphicObject_ColorLight()
{

}

void GraphicObject_ColorLight::SetColor(const Vect& inCol, int inMeshNum) 
{
	assert(pModel->ValidMeshNum(inMeshNum));
	pColor[inMeshNum] = inCol;
}

void GraphicObject_ColorLight::Render(Camera* inCamera)
{
	pModel->BindVertexIndexBuffers();
	pShader->SendWorldAndMaterial(mWorld, material->Ambient, material->Diffuse, material->Specular);
	pShader->SendCamMatrices(inCamera->getViewMatrix(), inCamera->getProjMatrix());
	pShader->SetToContext();

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pModel->RenderMesh(i);
	}
}

void GraphicObject_ColorLight::SetLights(ShaderColorLight::DirectionalLight* inDirLight, ShaderColorLight::SpotLight* inSptLight, ShaderColorLight::PointLight* inPntLight)
{
	this->dirLight = inDirLight;
	this->sptLight = inSptLight;
	this->pntLight = inPntLight;
}

void GraphicObject_ColorLight::SetDirectionalLightParameters(const Vect& inDir, const Vect& inAmb, const Vect& inDif, const Vect& inSp)
{
	pShader->SetDirectionalLightParameters(inDir, inAmb, inDif, inSp);
	dirLight->Direction = inDir;
	dirLight->Light.Ambient = inAmb;
	dirLight->Light.Diffuse = inDif;
	dirLight->Light.Specular = inSp;
}

void GraphicObject_ColorLight::SetPointLightParameters1(const Vect& inPos, float inR, const Vect& inAtt, const Vect& inAmb, const Vect& inDif, const Vect& inSp)
{
	pShader->SetPointLightParameters1(inPos, inR, inAtt, inAmb, inDif, inSp);
	pntLight->Attenuation = inAtt;
	pntLight->Light.Ambient = inAmb;
	pntLight->Light.Diffuse = inDif;
	pntLight->Light.Specular = inSp;
}

void GraphicObject_ColorLight::SetPointLightParameters2(const Vect& inPos, float inR, const Vect& inAtt, const Vect& inAmb, const Vect& inDif, const Vect& inSp)
{
	pShader->SetPointLightParameters2(inPos, inR, inAtt, inAmb, inDif, inSp);
	pntLight->Attenuation = inAtt;
	pntLight->Light.Ambient = inAmb;
	pntLight->Light.Diffuse = inDif;
	pntLight->Light.Specular = inSp;
}

void GraphicObject_ColorLight::SetPointLightParameters3(const Vect& inPos, float inR, const Vect& inAtt, const Vect& inAmb, const Vect& inDif, const Vect& inSp)
{
	pShader->SetPointLightParameters3(inPos, inR, inAtt, inAmb, inDif, inSp);
	pntLight->Attenuation = inAtt;
	pntLight->Light.Ambient = inAmb;
	pntLight->Light.Diffuse = inDif;
	pntLight->Light.Specular = inSp;
}

void GraphicObject_ColorLight::SetSpotLightParameters(const Vect& inPos, float inR, const Vect& inAtt, const Vect& inDir, float inSpotExp, const Vect& inAmb, const Vect& inDif, const Vect& inSp)
{
	pShader->SetSpotLightParameters(inPos, inR, inAtt, inDir, inSpotExp, inAmb, inDif, inSp);
	sptLight->Attenuation = inAtt;
	sptLight->Light.Ambient = inAmb;
	sptLight->Light.Diffuse = inDif;
	sptLight->Light.Specular = inSp;
}

void GraphicObject_ColorLight::SetMaterial(ShaderColorLight::Material* inMaterial)
{
	this->material = inMaterial;
}