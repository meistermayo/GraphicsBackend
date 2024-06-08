#include "src/Graphics/CrazySpaceMeatLand/src/GraphicObject_TextureLight.h"
#include "Model.h"
#include <assert.h>
#include "src/Graphics/CrazySpaceMeatLand/src/Camera.h"


GraphicObject_TextureLight::GraphicObject_TextureLight(ShaderColorLightTexture* shader, Model* mod)
{
	ambColor = Vect::One;
	difColor = Vect::One;
	pShader = shader;
	SetModel(mod);
	tex = new Texture * [mod->GetMeshCount()];
	pWorld = new Matrix(Matrix::Identity);
}

GraphicObject_TextureLight::GraphicObject_TextureLight(Model* mod, ShaderBase* shader, Texture* inTexture, const Vect& inAmb, const Vect& inDif)
{
	ambColor = inAmb;
	ambColor = inDif;

	pShader = (ShaderColorLightTexture*)shader;
	SetModel(mod);

	tex = new Texture * [mod->GetMeshCount()];
	tex[0] = inTexture;

	pWorld = new Matrix(Matrix::Identity);
}

GraphicObject_TextureLight::~GraphicObject_TextureLight() {
	delete[] tex;
}

void GraphicObject_TextureLight::SetTexture(Texture* _tex, int i)
{
	assert(pModel->ValidMeshNum(i));
	this->tex[i] = _tex;
}

void GraphicObject_TextureLight::Render(Camera* inCamera)
{
	pModel->BindVertexIndexBuffers();
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		tex[i]->SetToContext();
		pShader->SendWorldAndMaterial(*pWorld, ambColor, difColor, Vect::One);
		pShader->SendCamMatrices(inCamera->getViewMatrix(), inCamera->getProjMatrix());
		pShader->SetToContext();
		pModel->RenderMesh(i);
	}
}
