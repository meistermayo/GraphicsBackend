#include <assert.h>

#include "GraphicObject_TextureLight.h"
#include "../Camera.h"
#include "../Model/Model.h"
#include "../Shader/ShaderColorLightTexture.h"
#include "../Texture/Texture.h"


GraphicObject_TextureLight::GraphicObject_TextureLight(ShaderColorLightTexture* inShader, Model* inModel)
{
	mAmbColor = Vect::One;
	mDifColor = Vect::One;
	pShader = inShader;
	pModel = inModel;
	pTex = new Texture* [inModel->GetMeshCount()];
	mWorld = Matrix::Identity;
}

GraphicObject_TextureLight::GraphicObject_TextureLight(Model* inModel, ShaderColorLightTexture* inShader, Texture* inTexture, const Vect& inAmb, const Vect& inDif)
{
	mAmbColor = inAmb;
	mDifColor = inDif;
	pShader = inShader;
	pModel = inModel;
	pTex = new Texture* [inModel->GetMeshCount()];
	pTex[0] = inTexture;
	mWorld = Matrix::Identity;
}

GraphicObject_TextureLight::~GraphicObject_TextureLight() {
	delete[] pTex;
}

void GraphicObject_TextureLight::SetTexture(Texture* inTex, int i)
{
	assert(pModel->ValidMeshNum(i));
	this->pTex[i] = inTex;
}

void GraphicObject_TextureLight::Render(Camera* inCamera)
{
	pModel->BindVertexIndexBuffers();
	pShader->SendWorldAndMaterial(mWorld, mAmbColor, mDifColor, Vect::One);
	pShader->SendCamMatrices(inCamera->getViewMatrix(), inCamera->getProjMatrix());
	pShader->SetToContext();
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pTex[i]->SetToContext();
		pModel->RenderMesh(i);
	}
}
