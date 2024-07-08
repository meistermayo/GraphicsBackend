#include <assert.h>

#include "GraphicsObject_TextureLightTransparent.h"
#include "../Camera.h"
#include "../Model/Model.h"
#include "../Shader/ShaderColorLightTextureTransparent.h"
#include "../Texture/Texture.h"


GraphicsObject_TextureLightTransparent::GraphicsObject_TextureLightTransparent(ShaderColorLightTextureTransparent* inShader, Model* inModel, float inAlpha)
	: alpha(inAlpha)
{
	mAmbColor = Vect::One;
	mDifColor = Vect::One;
	pShader = inShader;
	pModel = inModel;
	pTex = new Texture * [inModel->GetMeshCount()];
	mWorld = Matrix::Identity;
}

GraphicsObject_TextureLightTransparent::GraphicsObject_TextureLightTransparent(Model* inModel, ShaderColorLightTextureTransparent* inShader, Texture* inTexture, const Vect& inAmb, const Vect& inDif, float inAlpha)
	: alpha(inAlpha)
{
	mAmbColor = inAmb;
	mDifColor = inDif;
	pShader = inShader;
	pModel = inModel;
	pTex = new Texture * [inModel->GetMeshCount()];
	pTex[0] = inTexture;
	mWorld = Matrix::Identity;
}

GraphicsObject_TextureLightTransparent::~GraphicsObject_TextureLightTransparent() {
	delete[] pTex;
}

void GraphicsObject_TextureLightTransparent::SetTexture(Texture* inTex, int i)
{
	assert(pModel->ValidMeshNum(i));
	this->pTex[i] = inTex;
}

void GraphicsObject_TextureLightTransparent::Render(Camera* inCamera)
{
	pModel->BindVertexIndexBuffers();
	pShader->SendWorldAndMaterial(mWorld, mAmbColor, mDifColor, Vect::One);
	pShader->SendCamMatrices(inCamera->getViewMatrix(), inCamera->getProjMatrix());
	pShader->SendAlphaData(alpha);
	pShader->SetToContext();
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pTex[i]->SetToContext();
		pModel->RenderMesh(i);
	}
}
