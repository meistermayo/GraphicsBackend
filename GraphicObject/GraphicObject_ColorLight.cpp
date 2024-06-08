#include "GraphicObject_ColorLight.h"
#include "Model.h"
#include "ShaderColor.h"
#include "src/Graphics/CrazySpaceMeatLand/src/Camera.h"
#include <assert.h>

GraphicObject_ColorLight::GraphicObject_ColorLight(ShaderColorLight* shader, int meshCount, Model* mod, const Vect &amb, const Vect &dif, const Vect& sp)
{
	meshCount;
	material = new ShaderColorLight::Material();
	this->material->Ambient = amb;
	this->material->Diffuse = dif;
	this->material->Specular = sp;
	SetModel(mod );
	pShader = shader;

	Color = new Vect[mod->GetMeshCount()];

	for (int i = 0; i < mod->GetMeshCount(); i++)
	{
		Color[i] = Vect(0,0,0, 1);
	}
	pWorld = new Matrix(Matrix::Identity);
}
// NOTE can prob get rid of ptrs to lights...
GraphicObject_ColorLight::~GraphicObject_ColorLight()
{

}

void GraphicObject_ColorLight::SetColor(const Vect& col, int meshNum) 
{
	assert(pModel->ValidMeshNum(meshNum));
	Color[meshNum] = col;
}

void GraphicObject_ColorLight::Render(Camera* inCamera)
{
	//pShader->SendWorldAndMaterial(World,material->Ambient,material->Diffuse,material->Specular); 
	pModel->BindVertexIndexBuffers();
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldAndMaterial(*pWorld, material->Ambient, material->Diffuse, material->Specular);
		pShader->SendCamMatrices(inCamera->getViewMatrix(), inCamera->getProjMatrix());
		pShader->SetToContext();
		pModel->RenderMesh(i);
	}
}

void GraphicObject_ColorLight::SetLights(ShaderColorLight::DirectionalLight* _dirLight, ShaderColorLight::SpotLight* _sptLight, ShaderColorLight::PointLight* _pntLight)
{
	this->dirLight = _dirLight;
	this->sptLight = _sptLight;
	this->pntLight = _pntLight;
}

void GraphicObject_ColorLight::SetDirectionalLightParameters(const Vect& dir, const Vect& amb, const Vect& dif, const Vect& sp)
{
	pShader->SetDirectionalLightParameters(dir,amb,dif,sp);
	dirLight->Direction = dir;
	dirLight->Light.Ambient = amb;
	dirLight->Light.Diffuse = dif;
	dirLight->Light.Specular = sp;
}

void GraphicObject_ColorLight::SetPointLightParameters1(const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp)
{
	pShader->SetPointLightParameters1(pos,r,att,amb,dif,sp);
	pntLight->Attenuation = att;
	pntLight->Light.Ambient = amb;
	pntLight->Light.Diffuse = dif;
	pntLight->Light.Specular = sp;
}

void GraphicObject_ColorLight::SetPointLightParameters2(const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp)
{
	pShader->SetPointLightParameters2(pos, r, att, amb, dif, sp);
	pntLight->Attenuation = att;
	pntLight->Light.Ambient = amb;
	pntLight->Light.Diffuse = dif;
	pntLight->Light.Specular = sp;
}

void GraphicObject_ColorLight::SetPointLightParameters3(const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp)
{
	pShader->SetPointLightParameters3(pos, r, att, amb, dif, sp);
	pntLight->Attenuation = att;
	pntLight->Light.Ambient = amb;
	pntLight->Light.Diffuse = dif;
	pntLight->Light.Specular = sp;
}

void GraphicObject_ColorLight::SetSpotLightParameters(const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb, const Vect& dif, const Vect& sp)
{
	pShader->SetSpotLightParameters(pos,r,att,dir,spotExp,amb,dif,sp);
	sptLight->Attenuation = att;
	sptLight->Light.Ambient = amb;
	sptLight->Light.Diffuse = dif;
	sptLight->Light.Specular = sp;
}

void GraphicObject_ColorLight::SetMaterial(ShaderColorLight::Material* _material)
{
	this->material = _material;
}