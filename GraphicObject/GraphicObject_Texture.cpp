#include "GraphicObject_Texture.h"
#include "../Model/Model.h"
#include "../Camera.h"
#include <assert.h>

GraphicObject_Texture::GraphicObject_Texture(Model* inModel, ShaderTexture* inShader)
{
	pModel = inModel;
	pShader = inShader;
	pTex = new Texture*[pModel->GetMeshCount()];
	mWorld = Matrix::Identity;
}

GraphicObject_Texture::GraphicObject_Texture(Model* inModel, ShaderTexture* inShader, Texture* inTexture)
	: GraphicObject_Texture(inModel, inShader)
{
	pTex[0] = inTexture;
}

GraphicObject_Texture::~GraphicObject_Texture() {
	delete[] pTex;
}

void GraphicObject_Texture::SetTexture(Texture* _tex, int i)
{
	assert(pModel->ValidMeshNum(i));
	this->pTex[i] = _tex;
}

void GraphicObject_Texture::Render(Camera* inCamera)
{
	pModel->BindVertexIndexBuffers();
	pShader->SetToContext();
	pShader->SendCamMatrices(inCamera->getViewMatrix(), inCamera->getProjMatrix());
	pShader->SendWorld(mWorld);
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pTex[i]->SetToContext();
		pModel->RenderMesh(i);
	}
}