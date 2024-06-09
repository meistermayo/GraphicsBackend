#include "GraphicsObject_Texture.h"
#include "../Model/Model.h"
#include "../Camera.h"
#include <assert.h>

GraphicsObject_Texture::GraphicsObject_Texture(Model* inModel, ShaderTexture* inShader)
{
	pModel = inModel;
	pShader = inShader;
	pTex = new Texture*[pModel->GetMeshCount()];
	mWorld = Matrix::Identity;
}

GraphicsObject_Texture::GraphicsObject_Texture(Model* inModel, ShaderTexture* inShader, Texture* inTexture)
	: GraphicsObject_Texture(inModel, inShader)
{
	pTex[0] = inTexture;
}

GraphicsObject_Texture::~GraphicsObject_Texture() {
	delete[] pTex;
}

void GraphicsObject_Texture::SetTexture(Texture* _tex, int i)
{
	assert(pModel->ValidMeshNum(i));
	this->pTex[i] = _tex;
}

void GraphicsObject_Texture::Render(Camera* inCamera)
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